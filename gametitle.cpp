#include "DxLib.h"
#include "RPG.h"

//グローバル変数の定義
static int g_img_title;//static=このソ－スファイルでしか使えなくする

//GameTitleの初期設定
void GoTitle()//初期設定　一回だけ
{
	g_gamestate = GAME_TITLE;//ゲーム状態を「タイトル」に設定
	g_img_title = LoadGraph("img/title.png");
}

//GameTitleの更新内容
void DrawTitle()//毎回呼び出される
{
	DrawGraph(0, 0, g_img_title, TRUE);
	WaitTimer(17);
	DrawFormatString(180, 300, GetColor(0, 0, 0), "スタートするにはspaceを押してください");
	int key = GetJoypadInputState(DX_INPUT_KEY);
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		GoRule();
		WaitTimer(500);
	}
}
