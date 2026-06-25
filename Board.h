#pragma once
#include<cstring>
using namespace std;

class Board {
	char board[8][8];
	char spieces[8] = { 'r','n','b','q','k','b','n','r'};
	char bpieces[8] = { 'R','N','B','Q','K','B','N','R'};
	int turn = 1;

public:
	void allocate();
	void display();
	void move();
	bool ispawnmovelegal(char board[8][8], int ia, int ib, int fa, int fb);
	void pawnpromotion(char board[8][8], char piece, int fa, int fb);
	bool isknightmovelegal( int ia, int ib, int fa, int fb);
	bool isrookmovelegal(char board[8][8], int ia, int ib, int fa, int fb);
	bool isbishopmovelegal(char board[8][8], int ia, int ib, int fa, int fb);
	bool isqueenmovelegal();
};
