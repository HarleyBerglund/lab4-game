#include "goodFuncs.h"

int main(void) {
    init_stuff();
    int ticks;

    while (1) {
        int8_t clearscreen[] = CLEAR_SCREEN;
        uarte_write(clearscreen, sizeof(clearscreen));
		// Nollställer timern
        nrfx_rtc_counter_clear(&rtc_instance);
		// Väntar på input från användaren
        wait_for_any_button();
		// Sparar ett tick
        ticks = nrfx_rtc_counter_get(&rtc_instance);
		// använder tick för att randomisera 
        srand(ticks);

        int randomNum = (rand() % (10000 - 1 + 1)) + 1;
        send_int(randomNum);
        nrfx_systick_delay_ms(300);
        wait_for_any_button();
    }

    return 0;
}