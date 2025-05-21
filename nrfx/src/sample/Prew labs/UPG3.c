#include "goodFuncs.h"

int main(void) {
	init_stuff();
	int millisek;

	while(1) {
		int buttonToPress;

		int8_t clearscreen[] = CLEAR_SCREEN; 
		uarte_write(clearscreen, sizeof(clearscreen));
		char startMsg[] = "When you press any button a light will randomly turn on in the span of 1-10s,\r\npress the corresponding button to the led to test your reaction time.\r\n\nPress any button to start the test!\n\r"; 
    	uarte_write(startMsg, sizeof(startMsg));
		wait_for_any_button();
		uarte_write(clearscreen, sizeof(clearscreen));

		int randomLed = (rand()%(31 - 28 + 1)) + 28;
		int randomDelay = (rand()%(10000 - 1000 + 1)) + 1000;


		if (randomLed == 28) 
		{
			buttonToPress = BUTTON1;
		} else if (randomLed == 29) {
			buttonToPress = BUTTON2;
		} else if (randomLed == 30) {
			buttonToPress = BUTTON3;
		} else if (randomLed == 31) {
			buttonToPress = BUTTON4;
		}

		nrfx_systick_delay_ms(randomDelay);
		nrfx_rtc_counter_clear(&rtc_instance);
		millisek = nrfx_rtc_counter_get(&rtc_instance)/32.768;
		nrf_gpio_pin_toggle(randomLed);

		while (1) {if (nrf_gpio_pin_read(buttonToPress) == 0) {break;}}

		int timePressed = millisek;
		nrf_gpio_pin_toggle(randomLed);
		nrfx_systick_delay_ms(500);

		uarte_write(clearscreen, sizeof(clearscreen));
		send_int(timePressed);

		wait_for_any_button();
		
	
		

	}

    return 0;
}
