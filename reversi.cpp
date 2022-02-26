#include "DxLib.h"
#include <string>

int board[8][8];
std::string msg;
int msg_wait;

int putPiece(int x, int y, int turn, bool put_flag) {
	int sum = 0;
	if (board[y][x] > 0) return 0;
	
}