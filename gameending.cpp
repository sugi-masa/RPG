#include "DxLib.h"
#include "RPG.h"

//グローバル変数の定義
static   int g_endroll;
static int g_img_endroll[2];
static int g_img_endroll_back;
//GameTitleの初期設定
void GoEnding()//初期設定　一回だけ
{
	g_gamestate = GAME_ENDING;//ゲーム状態を「エンディング」に設定
	g_endroll = 0;
	//g_img_endroll[0] = LoadGraph("img/Endroll1.PNG");
	g_img_endroll[1] = LoadGraph("img/Endroll2.PNG");
	g_img_endroll_back = LoadGraph("img/Endroll_back.PNG");
	Sound(52);
	WaitTimer(500);
}


void DrawEnding()
{
	int key = GetJoypadInputState(DX_INPUT_KEY);
	int color;
	color = GetColor(255, 0, 0);
	g_endroll -= 1;
	DrawGraph(0, 0, g_img_endroll_back, TRUE);
	//DrawGraph(0, 480 + g_endroll, g_img_endroll[0], TRUE);
	DrawGraph(0, 480 + 620 + g_endroll, g_img_endroll[1], TRUE);

	if (1400 + g_endroll < 0)
	{
		DrawFormatString(100,300 , color, "タイトル画面に戻るにはSPACEキーを押してください");
	}
	if (CheckHitKey(KEY_INPUT_SPACE) == 1) //タイトルへ
	{
		Sound(53);
		GoTitle();
		WaitTimer(500);
	}
}