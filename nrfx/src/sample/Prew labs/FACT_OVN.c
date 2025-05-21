#include "goodFuncs.h"
#include <nrfx_systick.h>

#define CLEAR_SCREEN "\033c"

uint64_t factorial(uint64_t inputNum){
    if(inputNum == 0 || inputNum == 1){
        return 1;
    } else {
        return (inputNum*factorial(inputNum - 1));
    }
}


int main(void) {
    int8_t clearscreen[] = CLEAR_SCREEN; 
    // Use default settings and specify our TX and RX pins:
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
    
    // Initialize UARTE:
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
    if (errr != 0) {
        // Implement error handling here, but for now, let's hope everything works
    }
    uarte_write(clearscreen, sizeof(clearscreen));

    int8_t enterMsg[] = "Enter a number to take the factorial of: "; 
    uarte_write(enterMsg, sizeof(enterMsg));

    uint8_t inputNumber = read_int();
    uarte_write(clearscreen, sizeof(clearscreen));
    uint64_t factorialNumber = factorial(inputNumber);

    int8_t returMsg[] = "The factorial is: "; 
    uarte_write(returMsg, sizeof(returMsg));
    send_int(factorialNumber);
    return 0;
}