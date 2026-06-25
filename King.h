#pragma once
#include"Pawn.h"
#include"Knight.h"
#include"piece.h"
class King : public Piece {


public:
	bool ismoveLegal(char board[8][8],int ia, int ib, int fa, int fb) override;

};
