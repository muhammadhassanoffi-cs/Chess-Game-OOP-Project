#include"Bishop.h"
#include<cmath>
bool Bishop::ismoveLegal(char board[8][8], int ia, int ib, int fa, int fb)
{
	int a = abs(fa - ia);
	int b = abs(fb - ib);
	bool allow = true;
	if (a == b)
	{
		if (fa < ia && fb > ib)
		{
			int i = ia - 1;
			for (int j = ib + 1; j < fb; j++)
			{
				if (i > fa)
				{

					if (board[i][j] != '-')
					{
						allow = false;
						break;
					}
					i--;
				}

			}
			if (allow == true)
			{
				return true;
			}

		}
		else if (fa < ia && fb < ib)
		{
			int i = ia - 1;
			for (int j = ib - 1; j > fb; j--)
			{
				if (i > fa)
				{

					if (board[i][j] != '-')
					{
						allow = false;
						break;
					}
					i--;
				}

			}
			if (allow == true)
			{
				return true;
			}
		}
		else if (fa > ia && fb > ib)
		{
			int i = ia + 1;
			for (int j = ib + 1; j < fb; j++)
			{
				if (i < fa)
				{

					if (board[i][j] != '-')
					{
						allow = false;
						break;
					}
					i++;
				}

			}
			if (allow == true)
			{
				return true;
			}
		}
		else if (fa > ia && fb < ib)
		{
			int i = ia + 1;
			for (int j = ib - 1; j > fb; j--)
			{
				if (i < fa)
				{

					if (board[i][j] != '-')
					{
						allow = false;
						break;
					}
					i++;
				}

			}
			if (allow == true)
			{
				return true;
			}
		}

	}
	return false;
}