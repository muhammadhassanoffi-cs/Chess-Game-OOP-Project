
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

	//move();
}
void Board::move()
{
	Piece* P = nullptr;
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
	/*	display();*/
		return;
	}
	cin >> initialyaxis;
	if (cin.fail())
	{
		cout << " Input failed try again " << endl;
		cin.clear();
		cin.ignore(1000, '\n');
		//display();
		return;
	}
	bool isallowed = false;
	int ia = 8 - initialx;
	int ib = initialyaxis - 'a';
	if (ia < 0 || ia >= 8 || ib < 0 || ib >= 8)
	{
		cout << " Selected place is out of bound! Wrong input " << endl;
		cout << " Again input " << endl;
	/*	display();*/
		return;
	}
	if (board[ia][ib] == '-')
	{
		cout << " NO movement available " << endl;
	/*	display();*/
		return;
	}
	char piece = board[ia][ib];
	// WHITE TURN
		if (turn == 1) {
			if (!isupper(piece)) {
				cout << "It's White's turn (choose White piece)" << endl;
				/*display();*/
				return;
			}
			isallowed = true;
		}

	// BLACK TURN
		else if (turn == 0) {
			if (!islower(piece)) {
				cout << "It's Black's turn (choose Black piece)" << endl;
			/*	display();*/
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
		/*	display();*/
			return;
			
		}
		cin >> finaly;
		if (cin.fail())
		{
			cout << " Input failed try again " << endl;
			cin.clear();
			cin.ignore(1000, '\n');
		/*	display();*/
			return;
		}
		int fa = 8 - finalx;
		int fb = finaly - 'a';
		

		if (fa < 0 || fa >=8 || fb < 0 || fb >=8)
		{
			cout << " Selected place is out of bound! Wrong input " << endl;
			/*display();*/
			return;
		}
		if (islower(board[ia][ib]) && islower(board[fa][fb]))
		{
			cout << " Invalid move (moving to your same position)" << endl;
		/*	display();*/
			return;
		}
		if (isupper(board[ia][ib]) && isupper(board[fa][fb]))
		{
			cout << " Invalid move (moving to your same position)" << endl;
			/*display();*/
			return;
		}
		bool validmove = false;
		if (board[ia][ib] == 'P' || board[ia][ib] == 'p')
		{
			P = &pawn;
		}
		
		if (board[ia][ib] == 'n' || board[ia][ib] == 'N')
		{
			P = &knight;
		}
		
		if (board[ia][ib] == 'r' || board[ia][ib] == 'R')
		{
			P = &rook;
		
		}
		if (board[ia][ib] == 'b' || board[ia][ib] == 'B')
		{
			P = &bishop;

		}
		if (board[ia][ib] == 'q' || board[ia][ib] == 'Q')
		{
			P = &queen;

		}
		if (board[ia][ib] == 'k' || board[ia][ib] == 'K')
		{
			P = &king;

		}
		if (P != nullptr)
		{
			validmove=P->ismoveLegal( board,  ia,  ib,  fa,  fb);
		}
		if (!validmove)
		{
			cout << "This movement of piece is not allowed " << endl;
			/*display();*/
			return;
		}
		if (board[fa][fb] == 'K' || board[fa][fb] == 'k')
		{
			cout << "King cannot be captured!" << endl;
			/*display();*/
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
			/*display();*/
			return;
		}
		if (board[fa][fb] == 'P' && fa == 0)
		{
			pawn.pawnpromotion(board, 'P', fa, fb);
		}

		if (board[fa][fb] == 'p' && fa == 7)
		{
			pawn.pawnpromotion(board, 'p', fa, fb);
		}
		if (turn == 1) // White just moved
		{
			if (isblack_kingincheckmate())
			{
				cout << "CHECKMATE! White Wins." << endl;
				gameOver = true;
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
				gameOver = true;
				return;
			}
			else if (iswhite_king_incheck(board))
			{
				cout << "White King is in check" << endl;
			}
		}
	
		if (turn == 1)
		{
			turn = 0;
		}
		else
		{
			turn = 1;
		}
		
	}
}
bool Board::iswhite_king_incheck(char board[8][8])
{
	int kingx;
	int kingy;

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
					if (kingx == i + 1 && (kingy == j - 1 || kingy == j + 1))
					{
						return true;
					}

				}
				else if (board[i][j] == 'q')
				{
					if (queen.ismoveLegal(board, i, j, kingx, kingy))
					{
						return true;
					}
				
				}
				else if (board[i][j] == 'n')
				{
					if (knight.ismoveLegal(board, i, j, kingx, kingy))
					{
						return true;
					}
				}
				else if (board[i][j] == 'b')
				{
					if (bishop.ismoveLegal(board, i, j, kingx, kingy))
					{
						return true;
					}
					
				}
				else if (board[i][j] == 'r')
				{
					if (rook.ismoveLegal(board, i, j, kingx, kingy))
					{
						return true;
					}
					
				}
				else if (board[i][j] == 'k')
				{
					if (king.ismoveLegal(board, i, j, kingx, kingy))
					{
						return true;
					}
					
				}
			}

		}
	}

		return false;
	
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
					if (queen.ismoveLegal(board, i, j, kingx, kingy))
					{
						return true;
					}

				}
				else if (board[i][j] == 'N')
				{
					if (knight.ismoveLegal(board, i, j, kingx, kingy))
					{
						return true;
					}
				}
				else if (board[i][j] == 'B')
				{
					if (bishop.ismoveLegal(board, i, j, kingx, kingy))
					{
						return true;
					}

				}
				else if (board[i][j] == 'R')
				{
					if (rook.ismoveLegal(board, i, j, kingx, kingy))
					{
						return true;
					}

				}
				else if (board[i][j] == 'K')
				{
					if (king.ismoveLegal(board, i, j, kingx, kingy))
					{
						return true;
					}

				}
			}

		}
	}
	
		return false;
	
}
bool Board::iswhite_kingincheckmate()
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
							validmove = pawn.ismoveLegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'N')
						{
							validmove = knight.ismoveLegal(board,i, j, fa, fb);
						}
						else if (board[i][j] == 'R')
						{
							validmove = rook.ismoveLegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'B')
						{
							validmove = bishop.ismoveLegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'Q')
						{
							validmove = queen.ismoveLegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'K')
						{
							validmove = king.ismoveLegal(board,i, j, fa, fb);
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
bool Board::isblack_kingincheckmate()
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
							validmove = pawn.ismoveLegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'n')
						{
							validmove = knight.ismoveLegal(board,i, j, fa, fb);
						}
						else if (board[i][j] == 'r')
						{
							validmove = rook.ismoveLegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'b')
						{
							validmove = bishop.ismoveLegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'q')
						{
							validmove = queen.ismoveLegal(board, i, j, fa, fb);
						}
						else if (board[i][j] == 'k')
						{
							validmove = king.ismoveLegal(board,i, j, fa, fb);
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