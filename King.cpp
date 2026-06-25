#include"King.h"
#include<cctype>

bool King::ismoveLegal(char board[8][8],int ia, int ib, int fa, int fb)
{
	if (fa == ia && fb == ib + 1)
	{
		return true;
	}
	else if (fa == ia && fb == ib - 1)
	{
		return true;
	}
	else if (fa == ia - 1 && fb == ib)
	{
		return true;
	}
	else if (fa == ia + 1 && fb == ib)
	{
		return true;
	}
	else if (fa == ia - 1 && fb == ib + 1)
	{
		return true;
	}
	else if (fa == ia - 1 && fb == ib - 1)
	{
		return true;
	}
	else if (fa == ia + 1 && fb == ib + 1)
	{
		return true;
	}
	else if (fa == ia + 1 && fb == ib - 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
