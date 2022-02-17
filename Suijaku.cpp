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
	LoadDivGraph("assets/number.png", 13, 13, 1, 28, 28, graph_numbers);
	//シャッフル
	bool deal[4][13] = {};
	for (int y = 0; y < 4; y++) for (int x = 0; x < 13; x++) {
		int mk, no;
		do { mk = GetRand(3); no = GetRand(12); } while (deal[mk][no]);
		cards[y][x].mark = mk;
		cards[y][x].number = no;
		cards[y][x].reverse = true;
		deal[mk][no] = true;
	}
	start_time = GetNowCount();
	while (!ProcessMessage()) {
		ClearDrawScreen();
		//カードめくり処理
		if (GetMouseInput() & MOUSE_INPUT_LEFT) {
			if (!mouse_flag) {
				mouse_flag = true;
				if (turns == 2) {
					cards[oy][ox].reverse = true;
					cards[ny][nx].reverse = true;
					turns = 0;
				}
				else {
					GetMousePoint(&nx, &ny);
					nx = (nx - 60) / 40;
					ny = (ny - 96) / 64;
					if (nx >= 0 && nx <= 12 && ny <= 3 && cards[ny][nx].reverse) {
						cards[ny][nx].reverse = false;
						if (turns == 0) {
							ox = nx; oy = ny;
							turns = 1;
						}
						else {
							if (cards[ny][nx].number == cards[oy][ox].number) {
								remains -= 2;
								turns = 0;
							}
							else turns = 2;
						}
					}
				}
			}
		}
		else mouse_flag = false;
		//場のカードの表示
		for (int y = 0; y < 4; y++)for (int x = 0; x < 13; x++) {
			int x2 = x * 40 + 60;
			int y2 = y * 64 + 96;
			DrawGraph(x2, y2, graph_cards[cards[y][x].reverse], FALSE);
			if (!cards[y][x].reverse) {
				DrawGraph(x2 + 6, y2 + 5, graph_marks[cards[y][x].mark], FALSE);
				DrawGraph(x2 + 6, y2 + 32, graph_numbers[cards[y][x].number], FALSE);
			}
		}
		//メッセージ表示
		int color = GetColor(255, 255, 255);
		if (remains <= 0) DrawString(8, 8, "クリア！", color);
		else elasped_time = (GetNowCount() - start_time) / 1000;
		DrawFormatString(8, 450, color, "残り%d毎 経過時間%d秒", remains, elasped_time);
		ScreenFlip();
	}
	DxLib_End();
}