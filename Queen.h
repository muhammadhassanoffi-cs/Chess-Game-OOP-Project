#pragma once
#include"Rook.h"
#include"Bishop.h"
#include"piece.h"
class Queen : public Piece {


	Rook rook;
	Bishop bishop;
public:
	bool ismoveLegal(char board[8][8], int ia, int ib, int fa, int fb);
};
