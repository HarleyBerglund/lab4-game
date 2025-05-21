
typedef struct piece { 
    char type;  // K=king Q=queen B=bishop K=knight P=pawn //Stora bokstäver för vit små för svart
    int x_cordiante;
    int y_cordinate;
    int isCaptured;
}; 

void move_piece(piece p){ 
    if(p.type == 'K' || p.type == 'k'){
        king_moves(p);
    } else if(p.type == 'Q' || p.type == 'q'){
        queen_moves(p);
    } else if(p.type == 'N' || p.type == 'n'){
        knight_moves(p);
    } else if(p.type == 'P' || p.type == 'p'){
        pawn_moves(p);
    } else if(p.type == 'R' || p.type == 'r'){
        rook_moves(p);
    } else if(p.type == 'B' || p.type == 'b'){
        bishop_moves(p);
    } else {
        //print illegal move
    }
}

void capture(peice piece){
    piece.isCaptured = 1;
};

void isChecked(){
    
}

void en_passant(){

}

void castle(){

};

void bishop_moves(piece piece){

};


void knight_moves(piece piece){

};

void pawn_moves(piece piece){

};

void queen_moves(piece piece){

};

void king_moves(){

};

void rook_moves(piece piece){

};

