#include<iostream>
#include"Board.h";
using namespace std;
int main()
{
	Board B;
	B.allocate();
	B.display();
	while (!B.isGameOver())
	{
		B.move();
		B.display();
	}
}