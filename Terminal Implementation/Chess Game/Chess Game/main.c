#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef struct piece {
    char type;
    int x_coordinate;
    int y_coordinate;
} piece;


typedef struct chess_board {
    piece* board[8][8];
} chess_board;

// Initialize the board to starting position
void starting_positions(chess_board* chess) {
    // Clear the board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            chess->board[i][j] = NULL;
        }
    }

	// This is how the starting position looks like
    char initial_setup[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

	// Creating pieces and placing them on the board
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            char t = initial_setup[y][x];
            if (t != ' ') {
				// Creating a new piece and set its properties for each non-empty square
                piece* p = malloc(sizeof(piece));
				if (p == NULL) {
					return; // Handle memory allocation failure
				}
                p->type = t;
                p->x_coordinate = x;            // The coordinates are placed in reference to the upper left corner
                p->y_coordinate = y;
                chess->board[y][x] = p;
            }
        }
    }
}


void print_board(chess_board* chess) {
    printf("\n==== Chess Game ====\n");

    printf("    a b c d e f g h\n");
    printf("   ----------------\n");

    for (int i = 0; i < 8; i++) {
        printf("%d | ", 8 - i);
        for (int j = 0; j < 8; j++) {
            // Set board colors (optional)
            if ((i + j) % 2 == 0) {
                printf("\033[47m\033[30m"); // White background, black text
            }
            else {
                printf("\033[40m\033[97m"); // Black background, white text
            }

            // Print Unicode chess pieces
            if (chess->board[i][j] != NULL) {
                char piece;
                switch (chess->board[i][j]->type) {
                case 'K': piece = '♔'; break; // White King
                case 'Q': piece = '♕'; break; // White Queen
                case 'R': piece = '♖'; break; // White Rook
                case 'B': piece = '♗'; break; // White Bishop
                case 'N': piece = '♘'; break; // White Knight
                case 'P': piece = '♙'; break; // White Pawn
                case 'k': piece = '♚'; break; // Black King
                case 'q': piece = '♛'; break; // Black Queen
                case 'r': piece = '♜'; break; // Black Rook
                case 'b': piece = '♝'; break; // Black Bishop
                case 'n': piece = '♞'; break; // Black Knight
                case 'p': piece = '♟'; break; // Black Pawn
                default:  piece = ' '; break;
                }
                printf("%c ", piece);
            }
            else {
                printf("  ");
            }

            // Reset colors after each square
            printf("\033[0m");
        }
        printf("\n");
    }
}

int is_white(char type) {
	return isupper(type);   // Check if the piece is white
}


int pawn_moves(chess_board* chess, piece* p, int dest_y, int dest_x) {
	int direction = is_white(p->type) ? -1 : 1;     // If the selected pawn is white, the pawn should move up, otherwise down
	int startRow = is_white(p->type) ? 6 : 1;    // The starting row for white pawns is 6, and for black pawns is 1

	// Calculatiting the deltas of the destination coordinates and the cordinates of the piece
    int dx = dest_x - p->x_coordinate;
    int dy = dest_y - p->y_coordinate;

    // Logic for moving forward
    if (dx == 0) {
		if (dy == direction && chess->board[dest_y][dest_x] == NULL)    // The pawn can move one square forward if the square is empty
            return 1;
		if (p->y_coordinate == startRow && dy == 2 * direction &&   // The pawn can move two squares forward from its starting position if both squares are empty and its at its starting row
            chess->board[p->y_coordinate + direction][p->x_coordinate] == NULL && 
            chess->board[dest_y][dest_x] == NULL)
            return 1;
    }

	// Logic for capturing diagonally
	if (abs(dx) == 1 && dy == direction &&  // One step on x-axis and one step on y in its direction
		chess->board[dest_y][dest_x] != NULL && // The destination square is occupied
		is_white(p->type) != is_white(chess->board[dest_y][dest_x]->type)) { // The piece on the destination square is of the opposite color
        return 1;
    }

    return 0;
}

int rook_moves(chess_board* chess, piece* p, int dest_y, int dest_x) {
    // Calculatiting the deltas of the destination coordinates and the cordinates of the piece
    int dx = dest_x - p->x_coordinate;
    int dy = dest_y - p->y_coordinate;

	if (dx != 0 && dy != 0) return 0; // The rook can only move in straight lines
    if (dx == 0 && dy == 0) return 0; // The rook cannot move to the same square
    
	int step_dir_x = (dx != 0) ? (dx > 0 ? 1 : -1) : 0;  // Geting the x direction of the rook
	int step_dir_y = (dy != 0) ? (dy > 0 ? 1 : -1) : 0;  // Geting the y direction of the rook

	// The first step in the direction of the rook
	int current_step_x = p->x_coordinate + step_dir_x; 
	int current_step_y = p->y_coordinate + step_dir_y;

    while (current_step_x != dest_x || current_step_y != dest_y) {
        if (chess->board[current_step_y][current_step_x] != NULL) return 0; // Another piece is blocking the rooks path
        current_step_x += step_dir_x;
        current_step_y += step_dir_y;
    }

    if (chess->board[dest_y][dest_x] == NULL || 
        is_white(p->type) != is_white(chess->board[dest_y][dest_x]->type)) {
		return 1; // The piece on the destination square is of the opposite color or is empty
    }
    
    return 0;
}

int knight_moves(chess_board* chess, piece* p, int dest_y, int dest_x) {
    // Calculatiting the deltas of the destination coordinates and the cordinates of the piece
    int dx = dest_x - p->x_coordinate;
    int dy = dest_y - p->y_coordinate;

	if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) {
        if (chess->board[dest_y][dest_x] == NULL ||
            is_white(p->type) != is_white(chess->board[dest_y][dest_x]->type)) {
            return 1; // The piece on the destination square is of the opposite color or is empty
        }
	}

    return 0;
}

int bishop_moves(chess_board* chess, piece* p, int dest_y, int dest_x) {
    // Calculatiting the deltas of the destination coordinates and the cordinates of the piece
	int dx = dest_x - p->x_coordinate;
	int dy = dest_y - p->y_coordinate;

	if (abs(dx) != abs(dy)) return 0; // The bishop can only move diagonally
	if (dx == 0 && dy == 0) return 0; // The bishop cannot move to the same square

    int step_dir_x = (dx != 0) ? (dx > 0 ? 1 : -1) : 0;  // Geting the x direction of the rook
	int step_dir_y = (dy != 0) ? (dy > 0 ? 1 : -1) : 0;  // Geting the y direction of the rook

    // The first step in the direction of the bishop
    int current_step_x = p->x_coordinate + step_dir_x;
    int current_step_y = p->y_coordinate + step_dir_y;

    while (current_step_x != dest_x && current_step_y != dest_y) {
        if (chess->board[current_step_y][current_step_x] != NULL) return 0; // Another piece is blocking the bishops path
        current_step_x += step_dir_x;
        current_step_y += step_dir_y;
    }

    if (chess->board[dest_y][dest_x] == NULL ||
        is_white(p->type) != is_white(chess->board[dest_y][dest_x]->type)) {
        return 1; // The piece on the destination square is of the opposite color or is empty
    }
	return 0;
}

int queen_moves(chess_board* chess, piece* p, int dest_y, int dest_x) {
    // Calculatiting the deltas of the destination coordinates and the cordinates of the piece
    int dx = dest_x - p->x_coordinate;
    int dy = dest_y - p->y_coordinate;

    if (dx == 0 && dy == 0) return 0;
    if (abs(dx) != abs(dy)) {
		// Diagonal move}
	}
    else if (dx != 0 || dy != 0) {
		// Straight move
    }
    else { 
        return 0;
    }

    int step_dir_x = (dx != 0) ? (dx > 0 ? 1 : -1) : 0;  // Geting the x direction of the queen
    int step_dir_y = (dy != 0) ? (dy > 0 ? 1 : -1) : 0;  // Geting the y direction of the queen

    // The first step in the direction of the queen
    int current_step_x = p->x_coordinate + step_dir_x;
    int current_step_y = p->y_coordinate + step_dir_y;

    while (current_step_x != dest_x || current_step_y != dest_y) {
        if (chess->board[current_step_y][current_step_x] != NULL) return 0; // Another piece is blocking the queens path
        current_step_x += step_dir_x;
        current_step_y += step_dir_y;
    }

    if (chess->board[dest_y][dest_x] == NULL ||
        is_white(p->type) != is_white(chess->board[dest_y][dest_x]->type)) {
        return 1; // The piece on the destination square is of the opposite color or is empty
    }
    return 0;
}


int king_moves(chess_board* chess, piece* p, int dest_y, int dest_x) {
    // Calculatiting the deltas of the destination coordinates and the cordinates of the piece
    int dx = dest_x - p->x_coordinate;
    int dy = dest_y - p->y_coordinate;

	if (dx == 0 && dy == 0) return 0; // The king cannot move to the same square
	if (abs(dx) > 1 || abs(dy) > 1) return 0; // The king can only move one square in any direction
    if (chess->board[dest_y][dest_x] == NULL || is_white(p->type) != is_white(chess->board[dest_y][dest_x]->type))  return 1;  // The piece on the destination square is of the opposite color or is empty

    return 0;
}

int is_valid_move(chess_board* chess, piece* p, int dest_y, int dest_x) {
	if (p == NULL) return 0; // Check if the piece is valid
	if (dest_y < 0 || dest_y > 7 || dest_x < 0 || dest_x > 7) return 0; // Checking if the destination is within the board limits

    switch (tolower(p->type)) {
    case 'p': return pawn_moves(chess, p, dest_y, dest_x);
	case 'r': return rook_moves(chess, p, dest_y, dest_x);
	case 'n': return knight_moves(chess, p, dest_y, dest_x);
	case 'b': return bishop_moves(chess, p, dest_y, dest_x);
	case 'q': return queen_moves(chess, p, dest_y, dest_x);
	case 'k': return king_moves(chess, p, dest_y, dest_x);
    default: return 0;
    }
}

int make_move(chess_board* chess, piece* p, int dest_y, int dest_x) {
    // Checking if the king was overwritten
    if (chess->board[dest_y][dest_x] != NULL) {
        if (chess->board[dest_y][dest_x]->type == 'k') {
            printf("test");
            return 1;   // The current player won
        }
    }
    chess->board[p->y_coordinate][p->x_coordinate] = NULL;  // Remove the piece from its old position
    p->x_coordinate = dest_x;
    p->y_coordinate = dest_y;
    chess->board[dest_y][dest_x] = p;
 
    return 0;   // The piece was moved without winning
}

// Returns:
//   - 1 if the king is in check (under threat)
int check_for_check(chess_board* chess, int is_white_var) {
    int king_x = -1, king_y = -1;//felhanterings kod

    // Locate the king of the given color on the board
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            piece* p = chess->board[y][x];
            // If a piece exists and it's a king of the correct color
            if (p != NULL && tolower(p->type) == 'k' && is_white(p->type) == is_white_var) {
                king_x = x;          // Save the x position of the king
                king_y = y;          // Save the y position of the king
                break;               // Exit inner loop once king is found
            }
        }
    }

    // If no king was found(felhantering i guess känns som vi kan lyckas tappa bort kungen)
    if (king_x == -1 || king_y == -1) return 0;

    // Loop through the entire board again to find opposing pieces
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            piece* attacker = chess->board[y][x];
            // If there's a piece and it belongs to the opponent
            if (attacker != NULL && is_white(attacker->type) != is_white_var) {
                // Check if this piece can move to the king's position
                if (is_valid_move(chess, attacker, king_y, king_x)) {
                    return 1; //king is in check
                }
            }
        }
    }

    // No opponent piece can attack the king => king is not in check
    return 0; //king is safe
}
int check_for_checkmate(chess_board* chess, int is_white_var) {

    if (!check_for_check(chess, is_white_var)) return 0; //king is not in check so returns

    // Try every possible move by current player's pieces
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            piece* p = chess->board[y][x];
            if (p != NULL && is_white(p->type) == is_white_var) {
                for (int dy = 0; dy < 8; dy++) {
                    for (int dx = 0; dx < 8; dx++) {
                        if (is_valid_move(chess, p, dy, dx)) {
                            // Simulate move to check for a way to escape check
                            piece* captured = chess->board[dy][dx];
                            int old_x = p->x_coordinate;
                            int old_y = p->y_coordinate;
                            chess->board[old_y][old_x] = NULL;
                            p->x_coordinate = dx;
                            p->y_coordinate = dy;
                            chess->board[dy][dx] = p;

                            int still_in_check = check_for_check(chess, is_white_var);

                            // Undo the move
                            p->x_coordinate = old_x;
                            p->y_coordinate = old_y;
                            chess->board[old_y][old_x] = p;
                            chess->board[dy][dx] = captured;

                            if (!still_in_check) {
                                return 0; // A move exists that gets out of check so no check mate ;)
                            }
                        }
                    }
                }
            }
        }
    }

    return 1; // No valid move gets out of check → checkmate and game over
}



int parse_input(char input[], int* from_y, int* from_x, int* to_y, int* to_x) {
    // Check for correct input length and space in the middle
    if (strlen(input) < 5 || input[2] != ' ') {
        printf("Invalid input format. Use format like 'e2 e4'.\n");
        return 0;
    }

    // Convert columns (a-h to 0-7)
    *from_x = input[0] - 'a';
    *to_x = input[3] - 'a';

    // Convert rows (1-8 to 7-0)
    *from_y = 8 - (input[1] - '0');
    *to_y = 8 - (input[4] - '0');

    // Validate coordinates are on the board
    if (*from_x < 0 || *from_x > 7 || *from_y < 0 || *from_y > 7 ||
        *to_x < 0 || *to_x > 7 || *to_y < 0 || *to_y > 7) {
        printf("Coordinates out of range. Use a-h and 1-8.\n");
        return 0;
    }

    return 1;
}

int main() {

    chess_board game;
    piece* move_piece;

    starting_positions(&game);

    int turn_tracker = 1; // 1 for white, -1 for black
    int game_running = 1;

    char input_buffer[10]; // Buffer for input
    int current_y, current_x, destination_y, destination_x;

    int white_won = 0;
    int black_won = 0;

    do {
        print_board(&game);
        printf("Enter move (e.g., 'e2 e4'): ");

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            printf("Error reading input.\n");
            return 1;
        }


        // Remove newline character
        input_buffer[strcspn(input_buffer, "\n")] = '\0';

        if (turn_tracker) { //Whites turn
            if (parse_input(input_buffer, &current_y, &current_x, &destination_y, &destination_x)) {
                move_piece = game.board[current_y][current_x];
                if (is_white(move_piece->type)) {
                    if (is_valid_move(&game, move_piece, destination_y, destination_x)) {
                        white_won = make_move(&game, move_piece, destination_y, destination_x);
                        printf("Move successful!\n");
                        turn_tracker = 0; // Switch turns
                    }
                }
                else {
                    printf("This is not your piece!!!");
                }
                
            }
        }
        else {  // Blacks turn
            if (parse_input(input_buffer, &current_y, &current_x, &destination_y, &destination_x)) {
                move_piece = game.board[current_y][current_x];
                if (!is_white(move_piece->type)) {
                    if (is_valid_move(&game, move_piece, destination_y, destination_x)) {
                        black_won = make_move(&game, move_piece, destination_y, destination_x);
                        printf("Move successful!\n");
                        turn_tracker = 1; // Switch turns
                    }
                }
                else {
                    printf("This is not your piece!!!");
                }

            }
        }

        if (white_won) {
            printf("White won!!!");
            game_running = 0;
        }
        else if (black_won) {
            printf("Black won!!!");
            game_running = 0;
        }

        //system("cls");

    } while (game_running);

    return 0;
}

