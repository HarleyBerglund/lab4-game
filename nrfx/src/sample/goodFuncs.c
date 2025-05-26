#include "goodFuncs.h"
#include <nrfx_uarte.h>
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

// Function to read a string via UART
int read_int(void)
{
    // Buffer to store the received string
    char buffer[100];
    // Read a string via UART
    read_string(buffer, sizeof(buffer));
    // Converting the string to an int and returning it
    return atoi(buffer);
}

// Function to send an int via UART
bool send_int(uint64_t num)
{
    char transmitString[100];
    int intLength = sprintf(transmitString, "%d", num);

    if (intLength <= 0 || intLength >= sizeof(transmitString))
    {
        return false;
    }

    // Send the received string and in case of any error return false otherwise return true
    nrfx_err_t err_code = nrfx_uarte_tx(&instance, transmitString, strlen(transmitString), 0);
    return (err_code == NRFX_SUCCESS);
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

int wait_for_any_button(void)
{
    const uint32_t buttons[BUTTON_COUNT] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4};
    int i = 0;
    while (1)
    {
        i = (i + 1) % 4;
        if (nrf_gpio_pin_read(buttons[i]) == 0)
        {
            return 1;
        }
    }
}

void toggle_leds(void)
{
    static uint8_t button1_state = 0;
    static uint8_t button2_state = 0;
    static uint8_t button3_state = 0;
    static uint8_t button4_state = 0;

    uint8_t new_button1_state = nrf_gpio_pin_read(BUTTON1);
    uint8_t new_button2_state = nrf_gpio_pin_read(BUTTON2);
    uint8_t new_button3_state = nrf_gpio_pin_read(BUTTON3);
    uint8_t new_button4_state = nrf_gpio_pin_read(BUTTON4);

    if (new_button1_state != button1_state)
    {
        button1_state = new_button1_state;
        if (button1_state == 0)
        {
            nrf_gpio_pin_toggle(LED1);
        }
    }
    if (new_button2_state != button2_state)
    {
        button2_state = new_button2_state;
        if (button2_state == 0)
        {
            nrf_gpio_pin_toggle(LED2);
        }
    }
    if (new_button3_state != button3_state)
    {
        button3_state = new_button3_state;
        if (button3_state == 0)
        {
            nrf_gpio_pin_toggle(LED3);
        }
    }
    if (new_button4_state != button4_state)
    {
        button4_state = new_button4_state;
        if (button4_state == 0)
        {
            nrf_gpio_pin_toggle(LED4);
        }
    }
}

void set_random_seed(void) {
    int ticks;
    // Sparar ett tick
    ticks = nrfx_rtc_counter_get(&rtc_instance);
    // använder tick för att randomisera 
    srand(ticks);
}

uint8_t get_buttton_press() {
    
    static uint8_t button1_state = 0;
    static uint8_t button2_state = 0;
    static uint8_t button3_state = 0;
    static uint8_t button4_state = 0;
    wait_for_any_button();
    set_random_seed();
    uint8_t new_button1_state = nrf_gpio_pin_read(BUTTON1);
    uint8_t new_button2_state = nrf_gpio_pin_read(BUTTON2);
    uint8_t new_button3_state = nrf_gpio_pin_read(BUTTON3);
    uint8_t new_button4_state = nrf_gpio_pin_read(BUTTON4);
    if (new_button1_state == button1_state){ return 1; }
    if (new_button2_state == button2_state){ return 2; }
    if (new_button3_state == button3_state){ return 3; } 
    if (new_button4_state == button4_state){ return 4; } 
}

int get_random_number(int lower, int upper) {
    return((rand() % (upper - lower + 1)) + lower);
    nrfx_rtc_counter_clear(&rtc_instance);
}

int is_even(int num){
    if (num%2) {
        return 0;
    } else {
        return 1;
    }
}
