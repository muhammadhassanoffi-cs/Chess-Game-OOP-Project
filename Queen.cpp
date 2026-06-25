#include"Queen.h"


bool Queen::ismoveLegal(char board[8][8], int ia, int ib, int fa, int fb)
{
	if (rook.ismoveLegal(board, ia, ib, fa, fb) == true)
	{
		return true;
	}
	else if (bishop.ismoveLegal(board, ia, ib, fa, fb) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}