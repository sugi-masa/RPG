#include "DxLib.h"
#include "RPG.h"

//�O���[�o���ϐ��̒�`
static int g_img_title;//static=���̃\�|�X�t�@�C���ł����g���Ȃ�����

//GameTitle�̏����ݒ�
void GoTitle()//�����ݒ�@��񂾂�
{
	g_gamestate = GAME_TITLE;//�Q�[����Ԃ��u�^�C�g���v�ɐݒ�
	g_img_title = LoadGraph("img/title.png");
}

//GameTitle�̍X�V���e
void DrawTitle()//����Ăяo�����
{
	DrawGraph(0, 0, g_img_title, TRUE);
	WaitTimer(17);
	DrawFormatString(180, 300, GetColor(0, 0, 0), "�X�^�[�g����ɂ�space�������Ă�������");
	int key = GetJoypadInputState(DX_INPUT_KEY);
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		GoRule();
		WaitTimer(500);
	}
}
