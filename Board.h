#pragma once
#include<cstring>
#include"Pawn.h"
#include"Knight.h"
#include"Rook.h"
#include"Bishop.h"
#include"Queen.h"
#include"King.h";
using namespace std;

class Board {
private:
	Pawn pawn;
	Knight knight;
	Rook rook;
	Bishop bishop;
	Queen queen;
	King king;

public:

	char board[8][8];
	char spieces[8] = { 'r','n','b','q','k','b','n','r'};
	char bpieces[8] = { 'R','N','B','Q','K','B','N','R'};
	int turn = 1;

public:
	void allocate();
	void display();
	void move();
	bool iswhite_king_incheck(char board[8][8]);
	bool isblack_king_incheck(char board[8][8]);
	bool iswhite_kingincheckmate();
	bool isblack_kingincheckmate();
	bool gameOver = false;
	bool isGameOver()
	{ return gameOver; }

	
	
};
