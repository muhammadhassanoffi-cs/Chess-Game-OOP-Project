
#include<iostream>
#include"Board.h"
#include"Pawn.h"
#include<cctype>
#include<cmath>
using namespace std;
void Board::allocate()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j <8; j++)
		{
			if(i==0)
			{
				board[i][j] = spieces[j];
			}
			else if (i == 1)
			{
				board[i][j] = 'p';
			}
			else if (i == 6)
			{
				board[i][j] = 'P';
			}
			else if (i == 7)
			{
				board[i][j] = bpieces[j];
			}
			else {
				board[i][j] = '-';
			}
		}
	}
}
void Board::display()
{
	
	cout << "    " << 'a' << "  " << " " <<  'b' << "   " << 'c' << "   " << 'd' << "   "<< 'e' << "   "  << 'f' << "   "  << 'g' << "   " << 'h' << endl;

	cout << "  ---------------------------------" << endl;
	int k = 8;
	for (int i = 0; i < 8; i++)
	{
		
		
		cout << k << " ";
		for (int j = 0; j < 8; j++)
		{
			cout << "|"  << " " << board[i][j] << " ";

		}
		k--;
		cout << "|";
		cout << endl;
	}
	cout << "  ---------------------------------" << endl;

	move();
}
void Board::move()
{
	
	char initialyaxis;
	int finalx;
	int initialx;

	char finaly;
	cout << " Enter Initial position:" << endl;

	cin >> initialx;

	if (cin.fail())
	{
		cout << " Input failed try again " << endl;
		cin.clear();
		cin.ignore(1000, '\n');
		display();
		return;
	}
	cin >> initialyaxis;
	if (cin.fail())
	{
		cout << " Input failed try again " << endl;
		cin.clear();
		cin.ignore(1000, '\n');
		display();
		return;
	}
	bool isallowed = false;
	int ia = 8 - initialx;
	int ib = initialyaxis - 'a';
	if (ia < 0 || ia >= 8 || ib < 0 || ib >= 8)
	{
		cout << " Selected place is out of bound! Wrong input " << endl;
		cout << " Again input " << endl;
		display();
		return;
	}
	if (board[ia][ib] == '-')
	{
		cout << " NO movement available " << endl;
		display();
		return;
	}
	char piece = board[ia][ib];
	// WHITE TURN
		if (turn == 1) {
			if (!isupper(piece)) {
				cout << "It's White's turn (choose White piece)" << endl;
				display();
				return;
			}
			isallowed = true;
		}

	// BLACK TURN
		else if (turn == 0) {
			if (!islower(piece)) {
				cout << "It's Black's turn (choose Black piece)" << endl;
				display();
				return;
			}
			isallowed = true;
		}
	if (isallowed==true)
	{
		
		cout << " Enter Final position:" << endl;
		cin >> finalx;

		if (cin.fail())
		{
			cout << " Input failed try again " << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			display();
			return;
			
		}
		cin >> finaly;
		if (cin.fail())
		{
			cout << " Input failed try again " << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			display();
			return;
		}
		int fa = 8 - finalx;
		int fb = finaly - 'a';
		

		if (fa < 0 || fa >=8 || fb < 0 || fb >=8)
		{
			cout << " Selected place is out of bound! Wrong input " << endl;
			display();
			return;
		}
		if (islower(board[ia][ib]) && islower(board[fa][fb]))
		{
			cout << " Invalid move (moving to your same position)" << endl;
			display();
			return;
		}
		if (isupper(board[ia][ib]) && isupper(board[fa][fb]))
		{
			cout << " Invalid move (moving to your same position)" << endl;
			display();
			return;
		}
		bool validmove = false;
		if (board[ia][ib] == 'P' || board[ia][ib] == 'p')
		{
			validmove= ispawnmovelegal(board, ia, ib, fa, fb);
		}
		
		if (board[ia][ib] == 'n' || board[ia][ib] == 'N')
		{
			validmove = isknightmovelegal( ia, ib, fa, fb);
		}
		
		if (board[ia][ib] == 'r' || board[ia][ib] == 'R')
		{
			validmove = isrookmovelegal(board, ia, ib, fa, fb);
		
		}
		if (board[ia][ib] == 'b' || board[ia][ib] == 'B')
		{
			validmove = isbishopmovelegal(board, ia, ib, fa, fb);

		}
		if (board[ia][ib] == 'q' || board[ia][ib] == 'Q')
		{
			validmove = isqueenmovelegal(board, ia, ib, fa, fb);

		}
		if (board[ia][ib] == 'k' || board[ia][ib] == 'K')
		{
			validmove = iskingmovelegal(ia, ib, fa, fb);

		}
		if (!validmove)
		{
			cout << "This movement of piece is not allowed " << endl;
			display();
			return;
		}
		if (board[fa][fb] == 'K' || board[fa][fb] == 'k')
		{
			cout << "King cannot be captured!" << endl;
			display();
			return;
		}

		char capturedPiece = board[fa][fb];

		// temporary move
		board[fa][fb] = board[ia][ib];
		board[ia][ib] = '-';

		bool illegal = false;

		if (turn == 1) // White moved
		{
			if (iswhite_king_incheck(board))
			{
				illegal = true;
			}
		}
		else // Black moved
		{
			if (isblack_king_incheck(board))
			{
				illegal = true;
			}
		}

		if (illegal)
		{
			// undo move
			board[ia][ib] = board[fa][fb];
			board[fa][fb] = capturedPiece;

			cout << "Illegal move! Your king would be in check." << endl;
			display();
			return;
		}
		if (turn == 1) // White just moved
		{
			if (isblack_kingincheckmate())
			{
				cout << "CHECKMATE! White Wins." << endl;
				return;
			}
			else if (isblack_king_incheck(board))
			{
				cout << "Black King is in check" << endl;
			}
		}
		else // Black just moved
		{
			if (iswhite_kingincheckmate())
			{
				cout << "CHECKMATE! Black Wins." << endl;
				return;
			}
			else if (iswhite_king_incheck(board))
			{
				cout << "White King is in check" << endl;
			}
		}
		if (board[fa][fb] == 'P' && fa == 0)
		{
			pawnpromotion(board, 'P', fa, fb);
		}

		if (board[fa][fb] == 'p' && fa == 7)
		{
			pawnpromotion(board, 'p', fa, fb);
		}
		if (turn == 1)
		{
			turn = 0;
		}
		else
		{
			turn = 1;
		}
		display();
	}
}
bool Board :: ispawnmovelegal(char board[8][8], int ia, int ib, int fa, int fb)
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
void Board :: pawnpromotion(char board[8][8],char piece ,  int fa, int fb)
{
	if (piece == 'P')
	{
		
		char choice;

		do
		{
			cout << "Promote pawn to (Q/R/B/N): ";
			cin >> choice;
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
bool Board :: isknightmovelegal( int ia, int ib, int fa, int fb) {
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
	else if (fa == ia + 2 && fb == ib +1)
	{
		return true;
	}
	else if (fa == ia -1 && fb == ib +2)
	{
		return true;
	}
	else if (fa == ia +1 && fb == ib +2)
	{
		return true;
	}
	else if (fa == ia -1 && fb == ib - 2)
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
bool Board::  isrookmovelegal(char board[8][8], int ia, int ib, int fa, int fb)
{
	bool allow = true;
	if (ia != fa && ib!=fb)
	{
		return false;
	}
	if (fb == ib)
	{
		if (fa > ia)
		{
			for (int i = ia + 1; i < fa; i++ )
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
			if ( allow == true)
			{

				return true;
			}
			else
			{
				return false;
			}

		}

	}

	if (fa==ia)
	{
		if (fb > ib)
		{
			for (int i = ib+1; i < fb; i++)
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
bool Board::isbishopmovelegal(char board[8][8], int ia, int ib, int fa, int fb)
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
		else if (fa>ia && fb>ib)
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
		else if (fa>ia && fb<ib)
		{
			int i = ia + 1;
			for (int j = ib -1; j > fb; j--)
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
bool Board :: isqueenmovelegal(char board[8][8], int ia, int ib, int fa, int fb)
{
	if (isrookmovelegal(board, ia, ib, fa, fb) == true)
	{
		return true;
	}
	else if (isbishopmovelegal(board, ia, ib, fa, fb) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Board::iskingmovelegal(int ia, int ib, int fa, int fb)
{
	if (fa == ia && fb == ib + 1)
	{
		return true;
	}
	else if (fa == ia && fb == ib - 1)
	{
		return true;
	}
	else if (fa==ia-1 && fb==ib)
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
	else if (fa == ia + 1 && fb==ib-1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Board:: iswhite_king_incheck(char board[8][8])
{
	int kingx;
	int kingy;
	bool incheck = false;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'K')
			{
				kingx = i;
				kingy = j;
			}
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (islower(board[i][j]))
			{
				if (board[i][j] == 'p')
				{
					if (kingx == i + 1 &&(kingy == j - 1 || kingy == j + 1))
					{
						return true;
					}
				
				}
				else if (board[i][j] == 'q')
				{
					incheck = isqueenmovelegal(board, i, j, kingx, kingy);
					if (incheck == true)
					{
						break;
					}
				}
				else if (board[i][j] == 'n')
				{
					incheck = isknightmovelegal(i, j, kingx, kingy);
					if (incheck == true)
					{
						break;
					}
				}
				else if (board[i][j] == 'b')
				{
					incheck = isbishopmovelegal(board, i, j, kingx, kingy);
					if (incheck == true)
					{
						break;
					}
				}
				else if (board[i][j] == 'r')
				{
					incheck = isrookmovelegal(board, i, j, kingx, kingy);
					if (incheck == true)
					{
						break;
					}
				}
				else if (board[i][j] == 'k')
				{
					incheck = iskingmovelegal( i, j, kingx, kingy);
					if (incheck == true)
					{
						break;
					}
				}
			}
			
		}
	}
	if (incheck == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Board::isblack_king_incheck(char board[8][8])
{
	int kingx;
	int kingy;
	bool incheck = false;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 'k')
			{
				kingx = i;
				kingy = j;
			}
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (isupper(board[i][j]))
			{
				if (board[i][j] == 'P')
				{
					if (kingx == i - 1 && (kingy == j - 1 || kingy == j + 1))
					{
						return true;
					}

				}
				else if (board[i][j] == 'Q')
				{
					incheck = isqueenmovelegal(board, i, j, kingx, kingy);
					if (incheck == true)
					{
						break;
					}
				}
				else if (board[i][j] == 'N')
				{
					incheck = isknightmovelegal(i, j, kingx, kingy);
					if (incheck == true)
					{
						break;
					}
				}
				else if (board[i][j] == 'B')
				{
					incheck = isbishopmovelegal(board, i, j, kingx, kingy);
					if (incheck == true)
					{
						break;
					}
				}
				else if (board[i][j] == 'R')
				{
					incheck = isrookmovelegal(board, i, j, kingx, kingy);
					if (incheck == true)
					{
						break;
					}
				}
				else if (board[i][j] == 'K')
				{
					incheck = iskingmovelegal(i, j, kingx, kingy);
					if (incheck == true)
					{
						break;
					}
				}
			}

		}
	}
	if (incheck == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Board:: iswhite_kingincheckmate()
{
	
	if (!iswhite_king_incheck(board))
	{
		return false;
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (isupper(board[i][j]))
			{
				for (int fa = 0; fa < 8; fa++)
				{
					for (int fb = 0; fb < 8; fb++)
					{
						bool validmove = false;
						if (isupper(board[fa][fb]))
						{
							continue;
						}

						if (board[i][j] == 'P')
						{
							validmove = ispawnmovelegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'N')
						{
							validmove = isknightmovelegal(i, j, fa, fb);
						}
						else if (board[i][j] == 'R')
						{
							validmove = isrookmovelegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'B')
						{
							validmove = isbishopmovelegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'Q')
						{
							validmove = isqueenmovelegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'K')
						{
							validmove = iskingmovelegal(i, j, fa, fb);
						}
						if (!validmove)
						{
							continue;
						}
						char captured = board[fa][fb];

						board[fa][fb] = board[i][j];
						board[i][j] = '-';
						if (!iswhite_king_incheck(board))
						{
							board[i][j] = board[fa][fb];
							board[fa][fb] = captured;
							// White found a move that escapes check
							return false;
						}
						board[i][j] = board[fa][fb];
						board[fa][fb] = captured;
					}
				}
			}
		}
	}
	return true;
}
bool Board:: isblack_kingincheckmate()
{
	if (!isblack_king_incheck(board))
	{
		return false;
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (islower(board[i][j]))
			{
				for (int fa = 0; fa < 8; fa++)
				{
					for (int fb = 0; fb < 8; fb++)
					{
						bool validmove = false;
						if (islower(board[fa][fb]))
						{
							continue;
						}

						if (board[i][j] == 'p')
						{
							validmove = ispawnmovelegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'n')
						{
							validmove = isknightmovelegal(i, j, fa, fb);
						}
						else if (board[i][j] == 'r')
						{
							validmove = isrookmovelegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'b')
						{
							validmove = isbishopmovelegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'q')
						{
							validmove = isqueenmovelegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'k')
						{
							validmove = iskingmovelegal(i, j, fa, fb);
						}
						if (!validmove)
						{
							continue;
						}
						char captured = board[fa][fb];

						board[fa][fb] = board[i][j];
						board[i][j] = '-';
						if (!isblack_king_incheck(board))
						{
							board[i][j] = board[fa][fb];
							board[fa][fb] = captured;
							// White found a move that escapes check
							return false;
						}
						board[i][j] = board[fa][fb];
						board[fa][fb] = captured;
					}
				}
			}
		}
	}
	return true;
}