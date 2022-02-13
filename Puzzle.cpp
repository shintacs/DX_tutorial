#include "DxLib.h"

int pics[16];
int pic_all;
int panel[16];
enum { TITLE, MAIN, CLEAR } status = TITLE;

//パネルの入れ替え
void change(int x, int y)
{
	int p1 = y * 4 + x;
	int p2 = -1;
	if (x > 0 && panel[p1 - 1] == 15) p2 = p1 - 1;
	if (x < 3 && panel[p1 + 1] == 15) p2 = p1 + 1;
	if (y > 0 && panel[p1 - 4] == 15) p2 = p1 - 4;
	if (y < 3 && panel[p1 + 4] == 15) p2 = p1 + 4;
	if (p2 != -1) {
		panel[p2] = panel[p1];
		panel[p1] = 15;
	}
}

void gameTitle() {
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		//初期パネルのシャッフル
		for (int i = 0; i < 16; i++) panel[i] = i;
		for (int i = 0; i < 1000; i++) {
			change(GetRand(3), GetRand(3));
		}
		status = MAIN;
	}
	DrawGraph(0, 0, pic_all, FALSE);
	DrawString(46, 90, "CLICK TO START", GetColor(255, 0, 0));
}

//メイン画面
void gameMain() {
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		int x, y;
		GetMousePoint(&x, &y);
		change(x / 52, y /52);
		//パネルの完成判断
		bool clear = true;
		for (int i = 0; i < 16; i++) {
			if (panel[i] != i)clear = false;
		}
		if (clear) status = CLEAR;
	}
	//パネルの描画
	for (int i = 0; i < 16; i++) {
		if (panel[i] < 15) {
			DrawGraph((i % 4) * 52, (i / 4) * 52, pics[panel[i]], FALSE);
		}
	}
}

//終了画面
void gameClear() {
	DrawGraph(0, 0, pic_all, FALSE);
	DrawString(52, 90, "GAME_CLEAR!", GetColor(255, 0, 0));
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetGraphMode(208, 208, 32);
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	LoadDivGraph("assets/karakai.png", 16, 4, 4, 52, 52, pics);
	pic_all = LoadGraph("assets/karakai.png");
	while (!ProcessMessage()) {
		ClearDrawScreen();
		switch (status) {
		case TITLE: gameTitle(); break;
		case MAIN: gameMain(); break;
		case CLEAR: gameClear(); break;
		}
		ScreenFlip();
	}
	DxLib_End();
	return 0;

}