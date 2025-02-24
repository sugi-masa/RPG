#include "DxLib.h"
#include "RPG.h"
#include <math.h>

int g_img_map[13];
int g_img_player[4];
int g_img_sword[4];//剣で攻撃
int g_img_archer[4];//弓で攻撃
int g_img_arrow[4];//矢
int g_img_enemy1;
int g_img_enemy2[4];
int g_img_boss;
int g_img_boss_A;
int g_img_item[6];
int g_img_door;
int g_img_enemy2_attack;
int g_img_heart[4];
int g_display;

void GoDisplay() {

	g_display= LoadGraph("img/display.PNG");

	g_img_map[0] = LoadGraph("img/MAP1.PNG");
	g_img_map[1] = LoadGraph("img/MAP2.PNG");
	g_img_map[2] = LoadGraph("img/MAP3.PNG");
	g_img_map[3] = LoadGraph("img/MAP4.PNG");
	g_img_map[4] = LoadGraph("img/MAP5.PNG");
	g_img_map[5] = LoadGraph("img/MAP6.PNG");
	g_img_map[6] = LoadGraph("img/MAP7.PNG");
	g_img_map[7] = LoadGraph("img/MAP8.PNG");
	g_img_map[8] = LoadGraph("img/MAP9.PNG");
	g_img_map[9] = LoadGraph("img/MAP10.PNG");
	g_img_map[10] = LoadGraph("img/MAP11.PNG");
	g_img_map[11] = LoadGraph("img/MAP12.PNG");
	g_img_map[12] = LoadGraph("img/MAP13.PNG");
	g_img_player[0] = LoadGraph("img/プレイヤーR.png");
	g_img_player[1] = LoadGraph("img/プレイヤーL.png");
	g_img_player[2] = LoadGraph("img/プレイヤーU.png");
	g_img_player[3] = LoadGraph("img/プレイヤーD.png");
	g_img_sword[0] = LoadGraph("img/プレイヤーR_S.png");
	g_img_sword[1] = LoadGraph("img/プレイヤーL_S.png");
	g_img_sword[2] = LoadGraph("img/プレイヤーU_S.png");
	g_img_sword[3] = LoadGraph("img/プレイヤーD_S.png");
	g_img_archer[0] = LoadGraph("img/プレイヤーR_A.png");
	g_img_archer[1] = LoadGraph("img/プレイヤーL_A.png");
	g_img_archer[2] = LoadGraph("img/プレイヤーU_A.png");
	g_img_archer[3] = LoadGraph("img/プレイヤーD_A.png");
	g_img_heart[0] = LoadGraph("img/ハート1.png");
	g_img_heart[1] = LoadGraph("img/ハート2.png");
	g_img_heart[2] = LoadGraph("img/ハート3.png");
	g_img_heart[3] = LoadGraph("img/ハート4.png");


	g_img_arrow[0] = LoadGraph("img/矢.png");
	g_img_arrow[1] = LoadGraph("img/矢L.png");
	g_img_arrow[2] = LoadGraph("img/矢U.png");
	g_img_arrow[3] = LoadGraph("img/矢D.png");

	g_img_item[0] = LoadGraph("img/剣2.png");
	g_img_item[1] = LoadGraph("img/弓.png");
	g_img_item[2] = LoadGraph("img/盾.png");
	g_img_item[3] = LoadGraph("img/宝.png");
	g_img_item[4] = LoadGraph("img/剣.png");
	g_img_item[5] = LoadGraph("img/鍵.png");

	g_img_door = LoadGraph("img/扉.png");

	g_img_enemy1 = LoadGraph("img/スライム.png");
	g_img_enemy2[0] = LoadGraph("img/大砲R.png");
	g_img_enemy2[1] = LoadGraph("img/大砲L.png");
	g_img_enemy2[2] = LoadGraph("img/大砲U.png");
	g_img_enemy2[3] = LoadGraph("img/大砲D.png");
	g_img_boss = LoadGraph("img/B.png");
	g_img_boss_A = LoadGraph("img/トロル_A.png");

	g_img_enemy2_attack = LoadGraph("img/鉄球.png");

}

void Display() {

	DrawGraph(display_x, display_y, g_img_map[g_mapnum - 1], FALSE);

	//enemy1
	for (int i = 0; i < g_E1_num; i++) {
		if (E1[i].HP > 0) {
			DrawGraph(E1[i].x, E1[i].y, g_img_enemy1, TRUE);
			//DrawFormatString(E1[i].x, E1[i].y, GetColor(0, 0, 0), "%d", E1[i].HP);
		}
	}

	//enemy2
	for (int i = 0; i < g_E2_num; i++) {
		if (g_E2_HP[g_mapnum][i] > 0) {
			switch (E2[i].direction) {
			case RIGHT:
				DrawGraph(E2[i].x, E2[i].y, g_img_enemy2[RIGHT - 1], TRUE);
				break;
			case LEFT:
				DrawGraph(E2[i].x, E2[i].y, g_img_enemy2[LEFT - 1], TRUE);
				break;
			case UP:
				DrawGraph(E2[i].x, E2[i].y, g_img_enemy2[UP - 1], TRUE);
				break;
			case DOWN:
				DrawGraph(E2[i].x, E2[i].y, g_img_enemy2[DOWN - 1], TRUE);
				break;
			}
			//DrawFormatString(E2[i].x, E2[i].y, GetColor(0, 0, 0), "%d", g_E2_HP[g_mapnum][i]);
		}

		//enemy2_attack
		if (E2_attack[i].count == 0) {
			if (E2_attack[i].x >= 0 && E2_attack[i].x <= 600) {
				if (E2_attack[i].y >= 0 && E2_attack[i].y <= 400) {
					DrawGraph(E2_attack[i].x, E2_attack[i].y, g_img_enemy2_attack, TRUE);
				}
			}
		}
	}

	//boss
	if (g_mapnum == 13) {
		switch (B.direction) {
		case LEFT:
			if (g_Bpattern == 6 && B.count <= 60) {
				DrawGraph(B.x - 5 * square, B.y, g_img_boss_A, TRUE);

			}
			else {
				DrawGraph(B.x, B.y, g_img_boss, TRUE);
			}
			break;
		case RIGHT:
			if (g_Bpattern == 6 && B.count <= 60) {
				DrawTurnGraph(B.x, B.y, g_img_boss_A, TRUE);

			}
			else {
				DrawTurnGraph(B.x, B.y, g_img_boss, TRUE);
			}
			break;
		}
		DrawBox(B.x, (int)B.y + 4.8 * square, B.x + (5 * square * B.HP / (40 + 10 * g_level)), B.y + 5 * square, GetColor(255, 0, 0), TRUE);
	}


	//player
	switch (P.direction) {
	case RIGHT:
		if (P.count != 0) {
			switch (g_item) {
			case 0:
				DrawGraph(P.x, P.y, g_img_sword[RIGHT - 1], TRUE);
				break;
			case 1:
				DrawGraph(P.x, P.y, g_img_archer[RIGHT - 1], TRUE);
				break;
			default:
				DrawGraph(P.x, P.y, g_img_player[RIGHT - 1], TRUE);
				break;
			}
		}
		else {
			DrawGraph(P.x, P.y, g_img_player[RIGHT - 1], TRUE);
		}
		break;
	case LEFT:
		if (P.count != 0) {
			switch (g_item) {
			case 0:
				DrawGraph(P.x - square, P.y, g_img_sword[LEFT - 1], TRUE);
				break;
			case 1:
				DrawGraph(P.x, P.y, g_img_archer[LEFT - 1], TRUE);
				break;
			default:
				DrawGraph(P.x, P.y, g_img_player[LEFT - 1], TRUE);
				break;
			}
		}
		else {
		DrawGraph(P.x, P.y, g_img_player[LEFT - 1], TRUE);
	}
		break;
	case UP:
		if (P.count != 0) {
			switch (g_item) {
			case 0:
				DrawGraph(P.x, P.y - square, g_img_sword[UP - 1], TRUE);
				break;
			case 1:
				DrawGraph(P.x, P.y, g_img_archer[UP - 1], TRUE);
				break;
			default:
				DrawGraph(P.x, P.y, g_img_player[UP - 1], TRUE);
				break;
			}
		}
		else {
			DrawGraph(P.x, P.y, g_img_player[UP - 1], TRUE);
		}
		break;
	case DOWN:
		if (P.count != 0) {
			switch (g_item) {
			case 0:
				DrawGraph(P.x, P.y, g_img_sword[DOWN - 1], TRUE);
				break;
			case 1:
				DrawGraph(P.x, P.y, g_img_archer[DOWN - 1], TRUE);
				break;
			default:
				DrawGraph(P.x, P.y, g_img_player[DOWN - 1], TRUE);
				break;
			}
		}
		else {
			DrawGraph(P.x, P.y, g_img_player[DOWN - 1], TRUE);
		}
		break;
	}

	DrawGraph(0, 0, g_display, TRUE);

	//heart
	int i;
	for (i = 0; i < P.HP / 4; i++) {
		DrawGraph(420 + i * square, 29, g_img_heart[3], TRUE);
	}
	if (P.HP % 4 != 0) {
		DrawGraph(420 + i * square, 29, g_img_heart[P.HP % 4 - 1], TRUE);
	}

	//box
	if (g_map_item != 4
		&& g_mapdata[g_mapnum][(g_box2_x - display_x) / square][(g_box2_y - display_y) / square] == -3) {
		DrawGraph(g_box2_x, g_box2_y, g_img_item[3], TRUE);
	}

	//door
	for (int x = 0; x < 15; x++) {
		for (int y = 0; y < 10; y++) {
			if (g_mapdata[g_mapnum][x][y] == -4) {
				DrawGraph(x * square + display_x, y * square + display_y, g_img_door, TRUE);
				break;
			}
		}
	}

	//item
	if (g_getitem[0] == 1) {
		DrawGraph(220, 29, g_img_item[0], TRUE);//剣2
	}
	else {
		DrawGraph(220, 29, g_img_item[4], TRUE);//剣
	}
	if (g_getitem[1] == 1) {
		DrawGraph(220 + square, 29, g_img_item[1], TRUE);//弓
	}
	if (g_getitem[2] == 1) {
		DrawGraph(220 + 2 * square, 29, g_img_item[5], TRUE);//鍵
	}
	if (g_getitem[3] == 1) {
		DrawGraph(220 + 3 * square, 29, g_img_item[2], TRUE);//盾
	}
	DrawBox(220 + g_item * square, 29, 220 + square + g_item * square, 29 + square, GetColor(0, 0, 0), FALSE);


	if (A.count != 0) {
		switch (A.direction) {
		case RIGHT:
			DrawGraph(A.x, A.y, g_img_arrow[0], TRUE);//矢
			break;
		case LEFT:
			DrawGraph(A.x, A.y, g_img_arrow[1], TRUE);//矢
			break;
		case UP:
			DrawGraph(A.x, A.y, g_img_arrow[2], TRUE);//矢
			break;
		case DOWN:
			DrawGraph(A.x, A.y, g_img_arrow[3], TRUE);//矢
			break;
		default:
			break;
		}
	}

	DrawFormatString(40, 20, GetColor(255, 255, 255), " time:%6d", g_time);
	DrawFormatString(40, 50, GetColor(255, 255, 255), "score:%6d", g_score);
}