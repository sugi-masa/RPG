
#include "DxLib.h"
#include "RPG.h"

void GoRule()//�����ݒ�@��񂾂�
{
	g_gamestate = GAME_RULE;//�Q�[����Ԃ��u���[���v�ɐݒ�
}

void DrawRule()
{
	int key = GetJoypadInputState(DX_INPUT_KEY);
	int color1,color2,color3;
	color1 = GetColor(0, 200, 200);
	color2 = GetColor(255, 140, 0);
	color3 = GetColor(255, 255, 255);

	DrawFormatString(300, 100, color1, "���[������");
	DrawFormatString(100, 150, color2, "W:��ֈړ�");
	DrawFormatString(100, 170, color2, "A:���ֈړ�");
	DrawFormatString(100, 190, color2, "S:���ֈړ�");
	DrawFormatString(100, 210, color2, "D:�E�ֈړ�");
	DrawFormatString(100, 230, color2, "J;�U��");
	DrawFormatString(100, 250, color2, "K:���펝���ւ�");
	DrawFormatString(100, 300, color3, "SPASE�L�[�������Ă�������");
	
	if (CheckHitKey(KEY_INPUT_SPACE) == 1) //�^�C�g����
	{
		GoOption();
		WaitTimer(500);
	}
	
}

