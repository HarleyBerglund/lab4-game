#ifndef CHESS
#define CHESS

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct piece {
    char type;
    int x_coordinate;
    int y_coordinate;
} piece;


typedef struct chess_board {
    piece* board[8][8];
} chess_board;


void starting_positions(chess_board* chess);
void print_board(chess_board* chess);
int is_white(char type);
int pawn_moves(chess_board* chess, piece* p, int dest_y, int dest_x);
int rook_moves(chess_board* chess, piece* p, int dest_y, int dest_x);
int knight_moves(chess_board* chess, piece* p, int dest_y, int dest_x);
int bishop_moves(chess_board* chess, piece* p, int dest_y, int dest_x);
int queen_moves(chess_board* chess, piece* p, int dest_y, int dest_x);
int king_moves(chess_board* chess, piece* p, int dest_y, int dest_x);
int is_valid_move(chess_board* chess, piece* p, int dest_y, int dest_x);
int make_move(chess_board* chess, piece* p, int dest_y, int dest_x);
int parse_input(char input[], int* from_y, int* from_x, int* to_y, int* to_x);
#endif
