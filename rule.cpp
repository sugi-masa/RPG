
#include "DxLib.h"
#include "RPG.h"

void GoRule()//初期設定　一回だけ
{
	g_gamestate = GAME_RULE;//ゲーム状態を「ルール」に設定
}

void DrawRule()
{
	int key = GetJoypadInputState(DX_INPUT_KEY);
	int color1,color2,color3;
	color1 = GetColor(0, 200, 200);
	color2 = GetColor(255, 140, 0);
	color3 = GetColor(255, 255, 255);

	DrawFormatString(300, 100, color1, "ルール説明");
	DrawFormatString(100, 150, color2, "W:上へ移動");
	DrawFormatString(100, 170, color2, "A:左へ移動");
	DrawFormatString(100, 190, color2, "S:下へ移動");
	DrawFormatString(100, 210, color2, "D:右へ移動");
	DrawFormatString(100, 230, color2, "J;攻撃");
	DrawFormatString(100, 250, color2, "K:武器持ち替え");
	DrawFormatString(100, 300, color3, "SPASEキーを押してください");
	
	if (CheckHitKey(KEY_INPUT_SPACE) == 1) //タイトルへ
	{
		GoOption();
		WaitTimer(500);
	}
	
}

