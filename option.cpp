#include "DxLib.h"
#include "RPG.h"

//�O���[�o���錾

int g_box_x;
int g_box_y;
int g_box_a;
int g_box_b;
int g_volume;
int g_choose;
int g_level;

void GoOption()
{

	g_gamestate = GAME_OPTION;//�Q�[����Ԃ��I�v�V����
	//������Փx��Normal
	g_box_x = 250;
	g_box_y = 140;
	g_box_a = 140;
	g_box_b = 65;
	g_level = 1;//Normal
	//�������ʂ�10�i�K�̂�����5
	g_volume = 5 * volume;
	g_choose = 0;
}

void DrawOption()
{
	int color1,color2,color3;
	int img_option[2];

	img_option[0]= LoadGraph("img/level.png");
	color1 = GetColor(200, 120, 120);
	color2 = GetColor(0, 0, 0);
	color3 = GetColor(200, 100, 100);

	DrawGraph(0, 0, img_option[0], TRUE);

	

	int key = GetJoypadInputState(DX_INPUT_KEY);
	//��Փx�I�������ʒ����I��
	if (CheckHitKey(KEY_INPUT_W) == 1)//��Փx�I��
	{
		g_box_x = 250;
		g_box_y = 140;
		g_box_a = 140;
		g_box_b = 65;
		g_choose = 0;
		Sound(6);
	}
	if (CheckHitKey(KEY_INPUT_S) == 1) //�{�����[��
	{
		g_box_x = 135;
		g_box_y = 275;
		g_box_a = 385;
		g_box_b = 110;

		DrawBox(g_box_x, g_box_y, g_box_x + g_box_a, g_box_y + g_box_b, color1, FALSE);
		
		g_choose = 1;
		Sound(6);
	}
	//��Փx�I���̎�
	if (g_choose == 0) {
		if (CheckHitKey(KEY_INPUT_A) == 1)//box������
		{
			g_box_x -= 140;
			if (g_box_x <= 250 - 140)//Normal����Easy��
			{
				g_box_x = 250 - 140;
				g_box_a = 110;
				g_level = 0;
			}
			if (g_box_x == 430 - 140)//Hard����Normal
			{
				g_box_x = 250;
				g_box_a = 140;
				g_level = 1;
			}
			Sound(6);
		}
		if (CheckHitKey(KEY_INPUT_D) == 1)//�E��
		{
			g_box_x += 140;
			if (g_box_x == 250)//Easy����Normal
			{
				g_box_a = 140;
				g_level = 1;
			}
			if (g_box_x >= 390)//Normal����Hard
			{
				g_box_x = 430;
				g_box_a = 110;
				g_level = 2;
			}
			Sound(6);
		}
	}
	//�{�����[��
	if (g_choose == 1) {
		if (CheckHitKey(KEY_INPUT_A) == 1)//���ʂ�������
		{
			if (g_volume != 0)
			{
				g_volume -= volume;
			}
			Sound(6);
		}
		if (CheckHitKey(KEY_INPUT_D) == 1)//���ʂ��グ��
		{
			if (g_volume != 10*volume)g_volume+=volume;
			Sound(6);
		}
	}


	//�Q�[����
	if (CheckHitKey(KEY_INPUT_SPACE) == 1)
	{
		GoSound();
		GoDisplay();
		GoProcess();
		Sound(6);
	}
	
	//��Փx���͂�box
	DrawBox(g_box_x, g_box_y, g_box_x + g_box_a, g_box_y+ g_box_b, color1, FALSE);

	
	//���݂̉��ʂ�h��Ԃ��ŕ\��
	switch (g_volume/volume) {
	case 10:
		DrawBox(125 + 375, 330 - 50, 125 + 385, 330 + 50, color3, TRUE);
	case 9:
		DrawBox(125 + 335, 330 - 45, 125 + 345, 330 + 45, color3, TRUE);
	case 8:
		DrawBox(125 + 295, 330 - 40, 125 + 305, 330 + 40, color3, TRUE);
	case 7:
		DrawBox(125 + 255, 330 - 35, 125 + 265, 330 + 35, color3, TRUE);
	case 6:
		DrawBox(125 + 215, 330 - 30, 125 + 225, 330 + 30, color3, TRUE);
	case 5:
		DrawBox(125 + 175, 330 - 25, 125 + 185, 330 + 25, color3, TRUE);
	case 4:
		DrawBox(125 + 135, 330 - 20, 125 + 145, 330 + 20, color3, TRUE);
	case 3:
		DrawBox(125 + 95, 330 - 15, 125 + 105, 330 + 15, color3, TRUE);
	case 2:
		DrawBox(125 + 55, 330 - 10, 125 + 65, 330 + 10, color3, TRUE);
	case 1:
		DrawBox(125 + 15, 330 - 5, 125 + 25, 330 + 5, color3, TRUE);
	case 0:
		break;
	}

	//10�i�K�̉��ʂ̘g��
	DrawBox(125 + 15, 330 - 5, 125 + 25, 330 + 5, color2, FALSE);
	DrawBox(125 + 55, 330 - 10, 125 + 65, 330 + 10, color2, FALSE);
	DrawBox(125 + 95, 330 - 15, 125 + 105, 330 + 15, color2, FALSE);
	DrawBox(125 + 135, 330 - 20, 125 + 145, 330 + 20, color2, FALSE);
	DrawBox(125 + 175, 330 - 25, 125 + 185, 330 + 25, color2, FALSE);
	DrawBox(125 + 215, 330 - 30,125 + 225, 330 + 30, color2, FALSE);
	DrawBox(125 + 255, 330 - 35,125 + 265, 330 + 35, color2, FALSE);
	DrawBox(125 + 295, 330 - 40, 125 + 305, 330 + 40, color2, FALSE);
	DrawBox(125 + 335, 330 - 45, 125 + 345, 330 + 45, color2, FALSE);
	DrawBox(125 + 375, 330 - 50, 125 + 385, 330 + 50, color2, FALSE);		


	DrawFormatString(250, 250, GetColor(0, 0, 0), "�{�����[��");
	DrawFormatString(200, 415, GetColor(0, 0, 0), "SPACE�������Ă�������");


	WaitTimer(100);
}
