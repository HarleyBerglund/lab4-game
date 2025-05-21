#ifndef GOODFUNCS
#define GOODFUNCS

#include <nrfx_uarte.h>
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_systick.h>
#include <nrf_gpio.h>
#include <nrfx_rtc.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
// Define the pins used for transmitting and receiving data:
#define PIN_TXD 20
#define PIN_RXD 22

#define CLEAR_SCREEN "\033c"

/*
From the NRF5340DK documentation:
Button 1    P0.23   i.e. 23
Button 2    P0.24
Button 3    P0.08
Button 4    P0.09
LED 1       P0.28   i.e. 28
LED 2       P0.29
LED 3       P0.30
LED 4       P0.31
*/

#define LED1 28
#define LED2 29
#define LED3 30
#define LED4 31
#define LED_OFF 1
#define LED_ON 0

#define BUTTON_COUNT 4
#define BUTTON1 23
#define BUTTON2 24
#define BUTTON3 8
#define BUTTON4 9

// Make the UART instance accessible in other files
extern nrfx_uarte_t instance;
extern uint8_t uarte_enabled;
extern nrfx_rtc_t rtc_instance;

// Function declarations
void uarte_write(uint8_t *data, uint8_t length);
bool read_string(char *str, size_t max_len);
int read_int(void);
bool send_int(uint64_t num);
void init_stuff(void);
void init_uart(void);
void init_gpio(void);
int wait_for_any_button(void);
void toggle_leds(void);
uint8_t get_buttton_press();
int is_even(int num);
void set_random_seed(void);
int get_random_number(int upper, int lower);
#endif
