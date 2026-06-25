#pragma once
class Piece {
public:
	virtual bool ismoveLegal(char board[8][8], int ia, int ib, int fa, int fb) = 0;
	virtual ~Piece(){}
};
