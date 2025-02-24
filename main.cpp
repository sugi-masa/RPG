#include "DxLib.h"
#include "RPG.h"

//�O���[�o���ϐ��̒�`
int g_gamestate = GAME_TITLE;//������Ԃ��Q�[���^�C�g���ɐݒ�
static int g_img_result;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//�E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	//DX���C�u�����̏�����
	if (DxLib_Init() == -1)
		return -1;
	
	//������ʂ��u�Q�[���^�C�g���v�ɐݒ�
	GoTitle();

	SetDrawScreen(DX_SCREEN_BACK);
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		//�e��ʂ̕`��
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

	//�L�[���͑҂�
	WaitKey();

	//DX���C�u�����̏I������
	DxLib_End();
	return 0;
}