#include "DxLib.h"

int player[8], enemy, chip[3];
int px = 1, py = 1;
int pd = 0, pa = 0;
int ex = 18, ey = 13;
int score = 0;
int esa_num = 0;
enum {PLAY, OVER, CLEAR} status = PLAY;
int eat, bgm, clr, over;
bool game = true;

int map[15][20] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,1,2,2,1,1,2,2,2,1,0,0,0,0,0,1},
  {1,0,1,1,0,0,2,2,2,2,2,1,2,1,0,0,1,1,0,1},
  {1,0,0,1,1,0,1,1,1,1,0,0,0,1,1,0,0,1,0,1},
  {1,2,2,0,1,0,2,2,2,1,2,1,2,0,0,0,0,2,2,1},
  {1,1,1,0,1,0,2,1,2,1,2,1,2,0,1,1,1,1,2,1},
  {1,2,2,0,0,0,2,2,2,0,2,2,2,0,0,0,0,1,2,1},
  {1,2,1,1,1,0,1,1,1,1,1,1,1,1,0,1,0,1,2,1},
  {1,2,2,1,0,0,2,2,2,0,2,2,2,0,0,0,0,2,2,1},
  {1,1,2,0,0,1,1,1,1,1,2,1,1,0,1,0,1,1,2,1},
  {1,2,2,1,0,0,2,2,2,1,2,2,2,0,1,0,0,1,2,1},
  {1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,1,0,2,2,1},
  {1,0,0,0,0,1,2,1,2,2,2,2,1,0,0,1,0,1,0,1},
  {1,0,1,0,0,0,2,2,2,2,1,2,2,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	LoadDivGraph("assets/players.png", 8, 4, 2, 32, 32, player);
	enemy = LoadGraph("assets/enemy1.png");
	LoadDivGraph("assets/chip.png", 3, 3, 1, 32, 32, chip);

	//餌の数を数える
	for (int y = 0; y < 15; ++y) {
		for (int x = 0; x < 20; ++x) {
			if (map[y][x] == 2) esa_num++;
		}
	}

	eat = LoadSoundMem("assets/eat.ogg");
	bgm = LoadSoundMem("assets/bgm.mp3");
	clr = LoadSoundMem("assets/clear.ogg");
	over = LoadSoundMem("assets/over.ogg");
	ChangeVolumeSoundMem(100, eat);
	ChangeVolumeSoundMem(100, bgm);
	ChangeVolumeSoundMem(100, clr);
	ChangeVolumeSoundMem(100, over);
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);

	while (!ProcessMessage() && game) {
		ClearDrawScreen();  

		int kx = px, ky = py;
		if (CheckHitKey(KEY_INPUT_LEFT)) { kx--; pd = 2; }
		if (CheckHitKey(KEY_INPUT_RIGHT)) { kx++; pd = 0; }
		if (CheckHitKey(KEY_INPUT_UP)) { ky--; pd = 3; }
		if (CheckHitKey(KEY_INPUT_DOWN)) { ky++; pd = 1; }

		//自機
		if (map[ky][kx] != 1) {//進む方向が壁じゃないとき
			px = kx;
			py = ky;
		}
		if (map[py][px] == 2) { //進む方向に餌があるとき
			map[py][px] = 0;	//地面の画像に変更
			score += 10;
			esa_num--;
			PlaySoundMem(eat, DX_PLAYTYPE_BACK, TRUE);
			if (esa_num <= 0) status = CLEAR;
		}
		//敵機
		kx = ex; ky = ey;
		if (rand() % 3 == 0) {
			if (kx > px) kx--;
			if (ky < px) kx++;
			if (ky > py) ky--;
			if (ky < py) ky++;
		} else {
			kx += rand() % 3 - 1;
			ky += rand() % 3 - 1;
		}
		if (map[ky][kx] != 1) {
			ex = kx; ey = ky;
		}

		if (ex == px && ey == py) status = OVER;
		for (int y = 0; y < 15; ++y) {
			for (int x = 0; x < 20; ++x) {
				DrawGraph(x * 32, y * 32, chip[map[y][x]], FALSE);
			}
		}
		DrawGraph(px * 32, py * 32, player[pa * 4 + pd], FALSE);
		pa = 1 - pa;
		DrawGraph(ex * 32, ey * 32, enemy, FALSE);
		DrawFormatString(0, 0, GetColor(255, 255, 255), "SCORE : %d", score);
		if (status == OVER) {
			SetFontSize(32);
			DrawString(250, 200, "GAME OVER", GetColor(255, 255, 255));
			StopSoundMem(bgm);
			game = false;
		}
		if (status == CLEAR) {
			SetFontSize(32);
			DrawString(250, 200, "GAME CLEAR", GetColor(255, 255, 255));
			StopSoundMem(bgm);
			game = false;
		}
		ScreenFlip();
		WaitTimer(100);
		if (status != PLAY) WaitTimer(1000);
	}
	if (status == OVER) PlaySoundMem(over, DX_PLAYTYPE_BACK, TRUE);
	if (status == CLEAR) PlaySoundMem(clr, DX_PLAYTYPE_BACK, TRUE);

	WaitTimer(6000);

	DxLib_End();
	return 0;
}