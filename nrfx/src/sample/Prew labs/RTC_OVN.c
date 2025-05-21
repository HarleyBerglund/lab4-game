#include "goodFuncs.h"

// Define NRFX_RTC_ENABLED 1 in nrfx_config_nrf5340_application.h
// Define NRFX_RTC0_ENABLED 1 in nrfx_config_nrf5340_application.h

int main(void) {
	init_stuff();
	char str[50];
	int millisek, sek, minute = 0;
	while(1) {
		millisek = nrfx_rtc_counter_get(&rtc_instance)/32.768;
		sek = millisek/1000;
		
		if(sek > 60 ){
			sek = 0;
			minute = minute + 1;
			nrfx_rtc_counter_clear(&rtc_instance);
			
		}

		sprintf(str, "%d minutes %d s, or %d ms \r\n", minute,sek, millisek);
		uarte_write(str, strlen(str));
		nrfx_systick_delay_ms(1000);

	}
    return 0;
}
