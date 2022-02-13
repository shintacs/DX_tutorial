#include "DxLib.h"

//場のカードデータ
struct {
	int mark;
	int number;
	bool reverse;
}cards[4][13];

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	int turns = 0;
	int ox, oy, nx, ny;
	int remains = 52;
	int start_time, elasped_time;
	bool mouse_flag = false;
	int graph_cards[2], graph_marks[4], graph_numbers[13];
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	LoadDivGraph("assets/card.png", 2, 2, 1, 40, 64, graph_cards);
	LoadDivGraph("assets/mark.png", 4, 4, 1, 28, 28, graph_marks);
	LoadDivGraph("assets/numbers.png", 13, 13, 1, 28, 28, graph_numbers);

}