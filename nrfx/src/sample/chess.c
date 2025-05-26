#include "chess.h"

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
    char buffer[100];
    strcpy(buffer, "\n==== Chess Game ====\n\n\r");
    uarte_write(buffer, strlen(buffer));

    strcpy(buffer, "    A B C D E F G H\r");
    uarte_write(buffer, strlen(buffer));

    strcpy(buffer, "\n   -----------------\n\r");
    uarte_write(buffer, strlen(buffer));

    for (int i = 0; i < 8; i++) {
        sprintf(buffer, "%d | ", 8 - i);
        uarte_write(buffer, strlen(buffer));

        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0)
                strcpy(buffer, "\033[47m");  // Light background
            else
                strcpy(buffer, "\033[100m"); // Dark gray background
            uarte_write(buffer, strlen(buffer));

            if (chess->board[i][j] != NULL) {
                char piece = chess->board[i][j]->type;
                const char* symbol = " ";
                switch (tolower(piece)) {
                    case 'p': symbol = "♟"; break;
                    case 'r': symbol = "♜"; break;
                    case 'n': symbol = "♞"; break;
                    case 'b': symbol = "♝"; break;
                    case 'q': symbol = "♛"; break;
                    case 'k': symbol = "♚"; break;
                }

                // Text color based on case
                if (isupper(piece))
                    strcpy(buffer, "\033[97m");  // Bright white text
                else
                    strcpy(buffer, "\033[30m");  // Bright blue text
                uarte_write(buffer, strlen(buffer));

                sprintf(buffer, "%s ", symbol);
                uarte_write(buffer, strlen(buffer));
            }
            else {
                strcpy(buffer, "  ");
                uarte_write(buffer, strlen(buffer));
            }

            // Reset after each square
            strcpy(buffer, "\033[0m");
            uarte_write(buffer, strlen(buffer));
        }

        strcpy(buffer, "\r\n");
        uarte_write(buffer, strlen(buffer));
    }

    strcpy(buffer, "\n--------------------\n\r");
    uarte_write(buffer, strlen(buffer));
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
