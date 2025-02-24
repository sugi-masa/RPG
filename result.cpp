
#include "DxLib.h"
#include "RPG.h"

int g_img_result;

void GoResult(int a) {
	if (a == -1) {
		g_gamestate = GAME_OVER;
		g_img_result= LoadGraph("img/GAMEOVER.PNG");
	}
	else if (a == 0) {
		g_gamestate = GAME_CLEAR;
		g_img_result = LoadGraph("img/GAMECLEAR.PNG");
	}
}

void GameOver() {
	DrawGraph(0, 0, g_img_result, TRUE);	
	DrawFormatString(200, 400, GetColor(255, 255, 255), "SCORE:%6d", g_score);
	DrawFormatString(200, 420, GetColor(255, 255, 255), "SPACE‚ð‰Ÿ‚µ‚Ä‚­‚¾‚³‚¢");

	int key = GetJoypadInputState(DX_INPUT_KEY);
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		GoEnding();
		Sound(6);
	}
}

void GameClear() {
	DrawGraph(0, 0, g_img_result, TRUE);
	DrawFormatString(200, 400, GetColor(255, 255, 255), "SCORE:%6d", g_score);
	DrawFormatString(200, 420, GetColor(255, 255, 255), "SPACE‚ð‰Ÿ‚µ‚Ä‚­‚¾‚³‚¢");

	int key = GetJoypadInputState(DX_INPUT_KEY);
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		GoEnding();
		Sound(6);
	}
}