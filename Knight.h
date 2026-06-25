#pragma once
#include"piece.h"
class Knight: public Piece {
public:

	bool ismoveLegal(char board[8][8], int ia, int ib, int fa, int fb) override;
};
