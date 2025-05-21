#include "goodFuncs.h"
#include "list.h"

int main(void) {
    init_stuff();
    int8_t clearscreen[] = CLEAR_SCREEN;
	uarte_write(clearscreen, sizeof(clearscreen));
	List *queue = create_empty_list();
    List *stack = create_empty_list();


    while(1) {
		char menu[6][50] = {"====== MENU =====\n\r", "1. Add a number to stack/queue\n\r", "2. Pop from stack\n\r", "3. Dequeue from queue\n\r", "4. Print stack and queue\n\r", "Press button!\n\r"};
  
		for (int i = 0; i < 6; i++)
		{
			uarte_write(menu[i], strlen(menu[i]));
		}
			

        int button_pressed = 0;
        button_pressed = get_buttton_press();
        
        switch (button_pressed) {
        case 1:
    
        button1_pressed(&queue, &stack);
        break;
        case 2:

        button2_pressed(&stack);
        break;
        case 3:
    
        button3_pressed(&queue);
        break;
        case 4:
    
        button4_pressed(queue, stack);
        break;
       }
        nrfx_systick_delay_ms(1000);

    }
    

    return 0;
}


void front_peek(List stack, List queue) {
    char peek_msg[100];
    if (queue != NULL) {
        strcpy(peek_msg, "\n\rFront: ");
        uarte_write(peek_msg, strlen(peek_msg));
        send_int(get_first_element(queue));
    } else {
        strcpy(peek_msg, "\n\rThe stack is empty!");
        uarte_write(peek_msg, strlen(peek_msg));
    }

    if (stack != NULL) {
        strcpy(peek_msg, "\n\rPeek: ");
        uarte_write(peek_msg, strlen(peek_msg));
        send_int(get_first_element(stack));
    } else {
        strcpy(peek_msg, "\n\rThe queue is empty!");
        uarte_write(peek_msg, strlen(peek_msg));
    }

}