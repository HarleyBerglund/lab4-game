#include "goodFuncs.h"
#include <nrfx_uarte.h>
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

volatile int resign = 0;

// Create a driver instance for UARTE:
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);
uint8_t uarte_enabled = 0;

// Skapar en funktion för att skicka data:
void uarte_write(uint8_t *data, uint8_t length)
{
    nrfx_uarte_tx(&instance, data, length, 0);
}

// Function to read a string via UART
bool read_string(char *str, size_t max_len)
{
    // Checking if the is enough room to read inputs
    if (str == NULL || max_len < 2)
    {
        return false;
    }

    size_t index = 0;
    char received_char;
    nrfx_err_t err_code;

    while (index < max_len - 1)
    {
        // Read a character with nrfx_uarte_rx
        err_code = nrfx_uarte_rx(&instance, &received_char, sizeof(received_char));
        if (err_code != NRFX_SUCCESS)
        {
            return false;
        }

        // Check if the character is carriage return
        if (received_char == '\r')
        {
            break;
        }

        // Save the character in the string
        str[index++] = received_char;
    }

    // Add null-terminator at the end of the string
    str[index] = '\0';
    return true;
}

void init_uart(void)
{
    // Use default settings and specify our TX and RX pins:
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);

    // Initialize UARTE:
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
    if (errr != NRFX_SUCCESS)
    {
        // Handle it!
    }
    else
    {
        uarte_enabled = 1;
    }
}

void init_gpio(void)
{
    nrfx_systick_init();
    nrf_gpio_cfg_output(LED1);
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_cfg_output(LED3);
    nrf_gpio_cfg_output(LED4);
    nrf_gpio_pin_write(LED1, LED_OFF);
    nrf_gpio_pin_write(LED2, LED_OFF);
    nrf_gpio_pin_write(LED3, LED_OFF);
    nrf_gpio_pin_write(LED4, LED_OFF);
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);
}

void init_rtc(void)
{
    // NRF_RTC_DEFAULT_CONFIG sätter RTC'n till 32768 Hz
    nrfx_rtc_config_t config = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_err_t err = nrfx_rtc_init(&rtc_instance, &config, NULL);
    nrfx_rtc_enable(&rtc_instance);
}

void init_stuff(void)
{
    init_uart();
    init_gpio();
    init_rtc();
}

void GPIOTE0_IRQHandler(void)
{
    if (NRF_GPIOTE0->EVENTS_IN[0])
    {                                  // Check if event triggered
        NRF_GPIOTE0->EVENTS_IN[0] = 0; // Clear event
        resign = 1;                    // Resigns the game for the player whos current turn it is
    }
}

void init_button_interrupt(void)
{
    // Configure button pin as input with pull-up resistor
    NRF_P0->PIN_CNF[BUTTON1] =
        (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
        (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
        (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);

    // Configure GPIOTE0 channel 0 for pin change interrupt
    NRF_GPIOTE0->CONFIG[0] =
        (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos) |
        (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos) |
        (BUTTON1 << GPIOTE_CONFIG_PSEL_Pos);

    // Enable interrupt for GPIOTE0 channel 0
    NRF_GPIOTE0->INTENSET = GPIOTE_INTENSET_IN0_Msk;

    // Enable GPIOTE interrupt in NVIC
    NVIC_EnableIRQ(GPIOTE0_IRQn);
    NVIC_SetPriority(GPIOTE0_IRQn, 1); // Set priority
}
