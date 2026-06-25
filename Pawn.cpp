#include"Pawn.h"
#include<iostream>
#include<cctype>
using namespace std;
bool Pawn::ismoveLegal(char board[8][8], int ia, int ib, int fa, int fb)
{
	char piece = board[ia][ib];
	if (fa < 0 || fa >= 8 || fb < 0 || fb >= 8)
	{
		return false;
	}
	if (piece == 'P')
	{
		if (fa == ia - 1 && fb == ib && board[fa][fb] == '-')
		{
			return true;
		}
		if (ia == 6 && fa == ia - 2 && fb == ib)
		{
			if (board[ia - 1][ib] == '-' && board[fa][fb] == '-')
			{
				return true;
			}
		}
		if (fa == ia - 1 && (fb == ib - 1 || fb == ib + 1))
		{
			if (board[fa][fb] != '-' && islower(board[fa][fb]))
			{
				return true;
			}
		}


	}
	else if (piece == 'p')
	{

		if (fa == ia + 1 && fb == ib && board[fa][fb] == '-')
		{
			return true;
		}
		if (ia == 1 && fa == ia + 2 && fb == ib)
		{
			if (board[ia + 1][ib] == '-' && board[fa][fb] == '-')
			{
				return true;
			}
		}
		if (fa == ia + 1 && (fb == ib - 1 || fb == ib + 1))

		{

			if (board[fa][fb] != '-' && isupper(board[fa][fb]))
			{
				return true;
			}

		}


	}
	return false;
}
void Pawn::pawnpromotion(char board[8][8], char piece, int fa, int fb)
{
	if (piece == 'P')
	{

		char choice;

		do
		{
			cout << "Promote pawn to (Q/R/B/N): ";
			cin >> choice;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(1000, '\n');
				choice = ' '; // forces another loop iteration
			}
		} while (choice != 'Q' &&
			choice != 'R' &&
			choice != 'B' &&
			choice != 'N');
		if (choice == 'Q')
		{
			board[fa][fb] = 'Q';
		}
		else  if (choice == 'R')
		{
			board[fa][fb] = 'R';
		}
		else  if (choice == 'B')
		{
			board[fa][fb] = 'B';
		}
		else  if (choice == 'N')
		{
			board[fa][fb] = 'N';
		}
	}
	else if (piece == 'p')
	{


		char choice;
		do
		{
			cout << "Promote pawn to (q/r/b/n): ";
			cin >> choice;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(1000, '\n');
				choice = ' '; // forces another loop iteration
			}
		} while (choice != 'q' &&
			choice != 'r' &&
			choice != 'b' &&
			choice != 'n');
		if (choice == 'q')
		{
			board[fa][fb] = 'q';
		}
		else  if (choice == 'r')
		{
			board[fa][fb] = 'r';
		}
		else  if (choice == 'b')
		{
			board[fa][fb] = 'b';
		}
		else  if (choice == 'n')
		{
			board[fa][fb] = 'n';
		}
	}
}
