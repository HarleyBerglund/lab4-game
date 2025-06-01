#include "goodFuncs.h"
#include "chess.h"
#include <nrfx_gpiote.h>
#include <nrf_gpio.h>
#include <soc/nrfx_irqs.h>
#include <nrf5340_application.h>
#include <nrf_gpiote.h>

int main()
{
    init_stuff();
    init_button_interrupt();
    int8_t clearscreen[] = CLEAR_SCREEN;
    uarte_write(clearscreen, sizeof(clearscreen));

    chess_board game;
    piece *move_piece;

    starting_positions(&game);

    int turn_tracker = 1; // 1 for white, 0 for black

    char input_buffer[10]; // Buffer for input
    int current_y, current_x, destination_y, destination_x;

    int white_won = 0;
    int black_won = 0;
    char msg[100];

    while (!white_won && !black_won && !resign)
    {
        print_board(&game);

        if (turn_tracker)
        {
            strcpy(msg, "White move");
        }
        else
        {
            strcpy(msg, "Black move");
        }

        uarte_write(msg, strlen(msg));
        strcpy(msg, " (e.g., 'e2 e4'): ");
        uarte_write(msg, strlen(msg));

        // Reading instructions from user
        read_string(input_buffer, 10);

        // Remove newline character
        input_buffer[strcspn(input_buffer, "\n")] = '\0';

        if (turn_tracker)
        { // Whites turn
            if (parse_input(input_buffer, &current_y, &current_x, &destination_y, &destination_x))
            {
                move_piece = game.board[current_y][current_x];
                if (is_white(move_piece->type))
                {
                    if (is_valid_move(&game, move_piece, destination_y, destination_x))
                    {
                        white_won = make_move(&game, move_piece, destination_y, destination_x);
                        strcpy(msg, "Move successful!\n\r");
                        uarte_write(msg, strlen(msg));
                        turn_tracker = 0; // Switch turns
                    }
                }
                else
                {
                    strcpy(msg, "This is not your piece!!!\n\r");
                    uarte_write(msg, strlen(msg));
                }
            }
        }
        else
        { // Blacks turn
            if (parse_input(input_buffer, &current_y, &current_x, &destination_y, &destination_x))
            {
                move_piece = game.board[current_y][current_x];
                if (!is_white(move_piece->type))
                {
                    if (is_valid_move(&game, move_piece, destination_y, destination_x))
                    {
                        black_won = make_move(&game, move_piece, destination_y, destination_x);
                        strcpy(msg, "Move successful!\n\r");
                        uarte_write(msg, strlen(msg));
                        turn_tracker = 1; // Switch turns
                    }
                }
                else
                {
                    strcpy(msg, "This is not your piece!!!\n\r");
                    uarte_write(msg, strlen(msg));
                }
            }
        }
        uarte_write(clearscreen, sizeof(clearscreen));
    }

    // Winning logic
    if (white_won)
    {
        strcpy(msg, "White won!!!\n\r");
        uarte_write(msg, strlen(msg));
    }

    else if (black_won)
    {
        strcpy(msg, "Black won!!!\n\r");
        uarte_write(msg, strlen(msg));
    }

    // Resigning logic
    if (resign)
    {

        if (turn_tracker)
        {
            strcpy(msg, "White resigned!!!\n\r");
            uarte_write(msg, strlen(msg));
        }

        else if (!turn_tracker)
        {
            strcpy(msg, "black resigned!!!\n\r");
            uarte_write(msg, strlen(msg));
        }
        strcpy(msg, "Click reset button to reset the game");
        uarte_write(msg, strlen(msg));
    }

    return 0;
}
