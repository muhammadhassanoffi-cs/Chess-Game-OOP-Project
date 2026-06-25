#include"Knight.h"
bool Knight::ismoveLegal(char board[8][8], int ia, int ib, int fa, int fb) {
	if (fa == ia - 2 && fb == ib - 1)
	{
		return true;
	}
	else if (fa == ia - 2 && fb == ib + 1)
	{
		return true;
	}
	else if (fa == ia + 2 && fb == ib - 1)
	{
		return true;
	}
	else if (fa == ia + 2 && fb == ib + 1)
	{
		return true;
	}
	else if (fa == ia - 1 && fb == ib + 2)
	{
		return true;
	}
	else if (fa == ia + 1 && fb == ib + 2)
	{
		return true;
	}
	else if (fa == ia - 1 && fb == ib - 2)
	{
		return true;
	}
	else if (fa == ia + 1 && fb == ib - 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}