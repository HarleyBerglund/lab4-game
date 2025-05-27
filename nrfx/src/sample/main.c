#include "goodFuncs.h"
#include "chess.h"
#include <nrfx_gpiote.h>
#include <nrf_gpio.h>
#include <soc/nrfx_irqs.h>
#include <nrf5340_application.h>
#include <nrf_gpiote.h>
#include <soc/nrfx_irqs.h>


#define BUTTON1_PIN 23
volatile int resign= 0;


void GPIOTE0_IRQHandler(void) {
    if (NRF_GPIOTE0->EVENTS_IN[0]) {  // Check if event triggered
        NRF_GPIOTE0->EVENTS_IN[0] = 0;  // Clear event
        resign = 1;  // Set flag when button is pressed
    }
}


void init_button_interrupt(void) {
    // Configure button pin as input with pull-up resistor
    NRF_P0->PIN_CNF[BUTTON1_PIN] = 
        (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
        (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
        (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);

    // Configure GPIOTE0 channel 0 for pin change interrupt
    NRF_GPIOTE0->CONFIG[0] = 
        (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos) |
        (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos) |
        (BUTTON1_PIN << GPIOTE_CONFIG_PSEL_Pos);

    // Enable interrupt for GPIOTE0 channel 0
    NRF_GPIOTE0->INTENSET = GPIOTE_INTENSET_IN0_Msk;

    // Enable GPIOTE interrupt in NVIC
    NVIC_EnableIRQ(GPIOTE0_IRQn);
}



int main() {
    init_stuff();
    init_button_interrupt();
    NVIC_SetPriority(GPIOTE0_IRQn, 1);  // Set priority
    NVIC_EnableIRQ(GPIOTE0_IRQn);       // Enable IRQ

	int8_t clearscreen[] = CLEAR_SCREEN;
	uarte_write(clearscreen, sizeof(clearscreen));

    chess_board game;
    piece* move_piece;

    starting_positions(&game);

    int turn_tracker = 1; // 1 for white, 0 for black
    int game_running = 1;

    char input_buffer[10]; // Buffer for input
    int current_y, current_x, destination_y, destination_x;

    int white_won = 0;
    int black_won = 0;
    char msg[100];

 while (!white_won && !black_won && !resign) {
        print_board(&game);

        if (turn_tracker) {
            strcpy(msg, "White move");
        } else {
            strcpy(msg, "Black move");
        }
        
	    uarte_write(msg, strlen(msg));

        strcpy(msg, " (e.g., 'e2 e4'): ");
	    uarte_write(msg, strlen(msg));

        read_string(input_buffer, 10);

        // Remove newline character
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
       

        if (turn_tracker) { //Whites turn
            if (parse_input(input_buffer, &current_y, &current_x, &destination_y, &destination_x)) {
                move_piece = game.board[current_y][current_x];
                if (is_white(move_piece->type)) {
                    if (is_valid_move(&game, move_piece, destination_y, destination_x)) {
                        white_won = make_move(&game, move_piece, destination_y, destination_x);
                        strcpy(msg, "Move successful!\n\r");
	                    uarte_write(msg, strlen(msg));
                        turn_tracker = 0; // Switch turns
                    }
                }
                else {
                    strcpy(msg, "This is not your piece!!!\n\r");
	                uarte_write(msg, strlen(msg));
                }
                
            }
        }
        else {  // Blacks turn
            if (parse_input(input_buffer, &current_y, &current_x, &destination_y, &destination_x)) {
                move_piece = game.board[current_y][current_x];
                if (!is_white(move_piece->type)) {
                    if (is_valid_move(&game, move_piece, destination_y, destination_x)) {
                        black_won = make_move(&game, move_piece, destination_y, destination_x);
                        strcpy(msg, "Move successful!\n\r");
	                    uarte_write(msg, strlen(msg));
                        turn_tracker = 1; // Switch turns
                    }
                }
                else {
                    strcpy(msg, "This is not your piece!!!\n\r");
	                uarte_write(msg, strlen(msg));
                }

            }
        }
        uarte_write(clearscreen, sizeof(clearscreen));

    }

    if (white_won) {
        strcpy(msg, "White won!!!\n\r");
	    uarte_write(msg, strlen(msg));
            
    }
    if(resign&& turn_tracker == 1){
        strcpy(msg, "White resigned!!!\n\r");
	    uarte_write(msg, strlen(msg));
    }
    else if(resign && turn_tracker == 0){
        strcpy(msg, "black resigned!!!\n\r");
	    uarte_write(msg, strlen(msg));
    }

    else if (black_won) {
        strcpy(msg, "Black won!!!\n\r");
	    uarte_write(msg, strlen(msg));
    }

    return 0;
}
