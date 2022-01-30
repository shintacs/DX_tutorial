#include "Dxlib.h"

int ship, enemy, shot;
int px = 304;	//デフォルトのウィンドウが640
int ex = 0, es = 5;
int sx, sy;
bool sf = false;
int score = 0;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);	//裏画面に描画

	ship = LoadGraph("assets/ship.png");
	enemy = LoadGraph("assets/enemy.png");
	shot = LoadGraph("assets/shot.png");

	while (!ProcessMessage())
	{
		ClearDrawScreen();	//裏画面に掛かれた以前の画像を消す

		if (CheckHitKey(KEY_INPUT_LEFT)){
			px -= 8;
			if (px < 0) px = 0;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT)){
			px += 8;
			if (px > 640-32) px = 640-32;
		}
		if (CheckHitKey(KEY_INPUT_SPACE)){
			if (!sf){
				sf = true;
				sx = px + 8;
				sy = 400;
			}
		}

		ex += es;
		if (ex <= 0 || ex >= 640 - 32) es = -es;

		if (sf){
			sy -= 16;
			if (sy < 0) sf = false;
		}

		if (sf) {
			if (sx + 16 > ex && sx < ex + 32 && sy + 16 > 16 && sy < 16 + 32) {
				sf = false;
				ex = 0;
				es = 5;
				score += 100;
			}
		}

		if (sf) DrawGraph(sx, sy, shot, FALSE);
		DrawGraph(px, 400, ship, FALSE);
		DrawGraph(ex, 16, enemy, FALSE);
		DrawFormatString(0, 0, GetColor(255, 255, 255), "SCORE ; %d", score);
		ScreenFlip();
	}

	DxLib_End();
	return 0;
}