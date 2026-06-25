#include"Rook.h"
bool Rook::ismoveLegal(char board[8][8], int ia, int ib, int fa, int fb)
{
	bool allow = true;
	if (ia != fa && ib != fb)
	{
		return false;
	}
	if (fb == ib)
	{
		if (fa > ia)
		{
			for (int i = ia + 1; i < fa; i++)
			{
				if (board[i][fb] != '-')
				{
					allow = false;
					break;
				}
			}
			if (allow == true)
			{

				return true;
			}
			else
			{
				return false;
			}
		}
		else if (fa < ia)
		{

			for (int i = ia - 1; i > fa; i--)
			{
				if (board[i][fb] != '-')
				{
					allow = false;
					break;
				}
			}
			if (allow == true)
			{

				return true;
			}
			else
			{
				return false;
			}

		}

	}

	if (fa == ia)
	{
		if (fb > ib)
		{
			for (int i = ib + 1; i < fb; i++)
			{
				if (board[fa][i] != '-')
				{
					allow = false;
					break;
				}
			}
			if (allow == true)
			{

				return true;
			}
			else
			{
				return false;
			}
		}
		else if (fb < ib)
		{
			for (int i = ib - 1; i > fb; i--)
			{
				if (board[fa][i] != '-')
				{
					allow = false;
					break;
				}
			}
			if (allow == true)
			{

				return true;
			}
			else
			{
				return false;
			}
		}

	}
	return false;

}