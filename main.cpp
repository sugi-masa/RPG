#include "DxLib.h"
#include "RPG.h"

//グローバル変数の定義
int g_gamestate = GAME_TITLE;//初期状態をゲームタイトルに設定
static int g_img_result;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	//DXライブラリの初期化
	if (DxLib_Init() == -1)
		return -1;
	
	//初期画面を「ゲームタイトル」に設定
	GoTitle();

	SetDrawScreen(DX_SCREEN_BACK);
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		//各画面の描画
		switch (g_gamestate)
		{
		case GAME_TITLE:
			DrawTitle();
			break;
		case GAME_RULE:
			DrawRule();
			break;
		case GAME_OPTION:
			DrawOption();
			break;
		case GAME_PROCESS:
			Process();
			break;
		case GAME_OVER:
			GameOver();
			break;
		case GAME_CLEAR:
			GameClear();
			break;
		case GAME_ENDING:
			DrawEnding();
			break;
		}

		ScreenFlip();
	}

	//キー入力待ち
	WaitKey();

	//DXライブラリの終了処理
	DxLib_End();
	return 0;
}