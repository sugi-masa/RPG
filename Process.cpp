
#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <time.h>
#include "RPG.h"

//グローバル変数の定義

int g_P_damage[5];//どの方向から攻撃を受けたか
int g_E2_HP[12][100];

int g_score;
int g_score_count;
int g_mapnum;
int g_mapdata[13][15][10];//map*x*y
int g_E1_num;
int g_E2_num;
int g_Bpattern;//1～4:移動 5:範囲攻撃 6:前方攻撃

int g_item;
int g_getitem[4];//0:剣 1:弓 2:鍵 3:盾
int g_box2_x, g_box2_y;
int g_map_item;//0:剣 1:弓　2:鍵 3:盾 4:宝箱無し

static int g_heal_count;
static int g_poison_count;

static int g_lasttime;
static int g_firsttime;
int g_time;

stutas P;//player
stutas E1[100];//enemy1
stutas E2[100];//enemy2
stutas E2_attack[100];
stutas B;//boss
stutas A;//arrow

void GoProcess() {
	SRand((int)time(NULL));

	FILE* fp[13];

	fp[0] = fopen("map/map1.csv", "r");
	fp[1] = fopen("map/map2.csv", "r");
	fp[2] = fopen("map/map3.csv", "r");
	fp[3] = fopen("map/map4.csv", "r");
	fp[4] = fopen("map/map5.csv", "r");
	fp[5] = fopen("map/map6.csv", "r");
	fp[6] = fopen("map/map7.csv", "r");
	fp[7] = fopen("map/map8.csv", "r");
	fp[8] = fopen("map/map9.csv", "r");
	fp[9] = fopen("map/map10.csv", "r");
	fp[10] = fopen("map/map11.csv", "r");
	fp[11] = fopen("map/map12.csv", "r");
	fp[12] = fopen("map/map13.csv", "r");

	for (int k = 0; k < 13; k++) {
		for (int j = 0; j < 10; j++) {
			for (int i = 0; i < 15; i++) {
				fscanf(fp[k], "%d,", &g_mapdata[k + 1][i][j]);
			}
		}
	}

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 100; j++) {
			g_E2_HP[i][j] = 8 + 2 * g_level;
		}
	}

	g_gamestate = GAME_PROCESS;

	g_getitem[0] = 0;//初期装備
	g_getitem[1] = 0;
	g_getitem[2] = 0;
	g_getitem[3] = 0;

	P.x = 600 / 3 + display_x;//画面の1/3
	P.y = 400 / 2 + display_y;//画面の半分

	P.HP = 12;//ハート×3
	P.AP = 2;
	P.DP = 1;
	P.direction = DOWN;
	g_mapnum = 6;

	for (int i = 0; i < 100; i++) {
		E1[i].count = GetRand(130) + 90;//90～220F
		E2[i].count = 240;//240F
	}
	B.count = 180;

	g_heal_count = 0;
	g_poison_count = 0;

	g_item = 0;//0で剣,1で弓,2で鍵

	FieldData();
	Enemy1(0);

	g_score = 30000;//初期スコア
	g_lasttime = GetNowCount() & INT_MAX;	//現在時刻を取得
	g_firsttime = g_lasttime;

	Sound(50);
}

void Process() {
	P.vxR = 0;
	P.vxL = 0;
	P.vyU = 0;
	P.vyD = 0;

	int curtime;
	int a = 0;
	int P_HP = P.HP;

	for (int i = 1; i < 5; i++) {
		if (g_P_damage[i] > 0) {
			g_P_damage[i]--;
			Damage();
			a = 1;
		}
	}
	if (a == 0) {
		if (A.count != 0) {
			A.count--;
			Arrow();
		}
		if (P.count != 0) {
			P.count--;
		}
		else {
			HitKey();
		}
	}

	P = Stop(P);
	Area();

	EnemyAct();
	EnemyAttack();
	if (g_mapnum == 13)BossAct();

	if (a > 0) {
		P.HP = P_HP;//無敵
	}

	P.x += P.vxR;
	P.x += P.vxL;
	P.y += P.vyU;
	P.y += P.vyD;

	Score();
	Display();

	curtime = GetNowCount() & INT_MAX;
	WaitTimer((1000 / 60) - (curtime - g_lasttime));
	g_lasttime = curtime;
	g_time = (curtime - g_firsttime) / 1000;

	if (P.HP <= 0)
	{
		Sound(51);
		GoResult(-1);
		WaitTimer(1000);
		Sound(13);
	}
	else if (B.HP <= 0)
	{
		Sound(51);
		GoResult(0);
		WaitTimer(1000);
		Sound(14);
	}
}

void HitKey() {
	if (CheckHitKey(KEY_INPUT_D) == 1) {
		P.direction = RIGHT;
		P.vxR = 2;
		if (P.x + square >= 600 + display_x) Scroll(RIGHT);//右端到達
	}
	else if (CheckHitKey(KEY_INPUT_A) == 1) {
		P.direction = LEFT;
		P.vxL = -2;
		if (P.x <= 0 + display_x) Scroll(LEFT);//左端到達
	}
	else if (CheckHitKey(KEY_INPUT_W) == 1) {
		P.direction = UP;
		P.vyU = -2;
		if (P.y <= 0 + display_y) Scroll(UP);//上端到達
	}
	else if (CheckHitKey(KEY_INPUT_S) == 1) {
		P.direction = DOWN;
		P.vyD = 2;
		if (P.y + square >= 400 + display_y) Scroll(DOWN);//下端到達
	}
	if (CheckHitKey(KEY_INPUT_J) == 1) {//attack
		Attack();
	}
	if (CheckHitKey(KEY_INPUT_K) == 1) {//change
		Select();
	}
}

void Scroll(int a) {
	if (g_mapnum == 13) {
		if (P.x <= display_x)P.x = display_x;
		if (P.x + square >= 600 + display_x)P.x = 600 + display_x - square;
		if (P.y <= display_y)P.y = display_y;
		if (P.y + square >= 400 + display_y)P.y = 400 + display_y - square;

		return;
	}
	switch (a) {
	case RIGHT:
		if (g_mapnum % 4 == 0) {
			P.vxR = 0;
			break;
		}
		P.x -= 600 - square;
		g_mapnum += 1;
		FieldData();
		Sound(7);
		break;
	case LEFT:
		if (g_mapnum % 4 == 1) {
			P.vxL = 0;
			break;
		}
		P.x += 600 - square;
		g_mapnum -= 1;
		FieldData();
		Sound(7);
		break;
	case UP:
		if (g_mapnum <= 4) {
			P.vyU = 0;
			break;
		}
		P.y += 400 - square;
		g_mapnum -= 4;
		FieldData();
		Sound(7);
		break;
	case DOWN:
		if (g_mapnum >= 9) {
			P.vyD = 0;
			break;
		}
		P.y -= 400 - square;
		g_mapnum += 4;
		FieldData();
		Sound(7);
		break;
	default:
		break;
	}
}

void FieldData() {
	SRand((int)time(NULL));
	Enemy2();
	Boss();
	Box();
	switch (g_mapnum) {
	case 1:
		Enemy1(GetRand(1) + g_level);
		break;
	case 2:
		Enemy1(GetRand(1) + g_level);
		break;
	case 3:
		Enemy1(GetRand(2) + g_level);
		break;
	case 4:
		Enemy1(GetRand(1) + g_level);
		break;
	case 5:
		Enemy1(GetRand(1) + g_level);
		break;
	case 6:
		Enemy1(GetRand(2) + g_level);
		break;
	case 7:
		Enemy1(GetRand(3) + g_level);
		break;
	case 8:
		Enemy1(GetRand(2) + g_level);
		break;
	case 9:
		Enemy1(GetRand(1) + g_level);
		break;
	case 10:
		Enemy1(GetRand(2) + g_level);
		break;
	case 11:
		Enemy1(GetRand(1) + g_level);
		break;
	case 12:
		Enemy1(g_level);
		break;
	default:
		break;
	}
}

stutas Stop(stutas A) {
	if ((A.x - display_x) % square == 0) {
		if ((A.y - display_y) % square == 0) {
			if (g_mapdata[g_mapnum][(A.x - display_x) / square + 1][(A.y - display_y) / square] < 0)A.vxR = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square - 1][(A.y - display_y) / square] < 0)A.vxL = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square][(A.y - display_y) / square - 1] < 0)A.vyU = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square][(A.y - display_y) / square + 1] < 0)A.vyD = 0;
		}
		else {
			if (g_mapdata[g_mapnum][(A.x - display_x) / square + 1][(A.y - display_y) / square] < 0
				|| g_mapdata[g_mapnum][(A.x - display_x) / square + 1][(A.y - display_y) / square + 1] < 0)A.vxR = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square - 1][(A.y - display_y) / square] < 0
				|| g_mapdata[g_mapnum][(A.x - display_x) / square - 1][(A.y - display_y) / square + 1] < 0)A.vxL = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square][(A.y - display_y) / square] < 0)A.vyU = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square][(A.y - display_y) / square + 1] < 0)A.vyD = 0;
		}
	}
	else {
		if ((A.y - display_y) % square == 0) {
			if (g_mapdata[g_mapnum][(A.x - display_x) / square + 1][(A.y - display_y) / square] < 0)A.vxR = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square][(A.y - display_y) / square] < 0)A.vxL = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square][(A.y - display_y) / square - 1] < 0
				|| g_mapdata[g_mapnum][(A.x - display_x) / square + 1][(A.y - display_y) / square - 1] < 0)A.vyU = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square][(A.y - display_y) / square + 1] < 0
				|| g_mapdata[g_mapnum][(A.x - display_x) / square + 1][(A.y - display_y) / square + 1] < 0)A.vyD = 0;
		}
		else {
			if (g_mapdata[g_mapnum][(A.x - display_x) / square + 1][(A.y - display_y) / square] < 0
				|| g_mapdata[g_mapnum][(A.x - display_x) / square + 1][(A.y - display_y) / square + 1] < 0)A.vxR = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square][(A.y - display_y) / square] < 0
				|| g_mapdata[g_mapnum][(A.x - display_x) / square][(A.y - display_y) / square + 1] < 0)A.vxL = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square][(A.y - display_y) / square] < 0
				|| g_mapdata[g_mapnum][(A.x - display_x) / square + 1][(A.y - display_y) / square] < 0)A.vyU = 0;
			if (g_mapdata[g_mapnum][(A.x - display_x) / square][(A.y - display_y) / square + 1] < 0
				|| g_mapdata[g_mapnum][(A.x - display_x) / square + 1][(A.y - display_y) / square + 1] < 0)A.vyD = 0;
		}
	}
	return A;
}

void Area() {
	//落下
	if (g_mapdata[g_mapnum][(P.x + square / 2 - display_x) / square][(P.y + square / 2 - display_y) / square] == 2) {
		P.HP -= 8;
		if (P.HP <= 0)P.HP = 1;
		Sound(12);
		WaitTimer(1000);

		switch (g_mapnum) {
		case 3:
			P.x = 3 * square + display_x;
			P.y = 1 * square + display_y;
			break;
		case 4:
			P.x = 1 * square + display_x;
			P.y = 8 * square + display_y;
			break;
		case 8:
			P.x = 6 * square + display_x;
			P.y = 1 * square + display_y;
			break;
		case 12:
			P.x = 1 * square + display_x;
			P.y = 4 * square + display_y;
			break;
		}
	}

	//回復
	if (g_mapdata[g_mapnum][(P.x + square / 2 - display_x) / square][(P.y + square / 2 - display_y) / square] == 3) {
		g_heal_count++;
		if (g_heal_count == 30) {
			if (P.HP < 20)P.HP++;
			g_heal_count = 0;
			Sound(2);
		}
	}
	//毒
	if (g_mapdata[g_mapnum][(P.x + square / 2 - display_x) / square][(P.y + square / 2 - display_y) / square] == 4) {
		g_poison_count++;
		if (g_poison_count == 60) {
			if (P.HP > 1)P.HP--;
			g_poison_count = 0;
			Sound(3);
		}
	}

	//ワープ
	if (g_mapdata[g_mapnum][(P.x + square / 2 - display_x) / square][(P.y + square / 2 - display_y) / square] == 9) {
		g_mapnum = 13;
		P.x = 600 / 3 + display_x;//画面の1/3
		P.y = 400 / 2 + display_y;//画面の半分
		P.direction = RIGHT;
		Sound(10);
		WaitTimer(1000);
	}
}

void Box() {
	g_map_item = 4;
	for (int x = 0; x < 15; x++) {
		for (int y = 0; y < 10; y++) {
			if (g_mapdata[g_mapnum][x][y] == -3) {
				g_box2_x = x * square + display_x;
				g_box2_y = y * square + display_y;
				switch (g_mapnum) {
				case 2:
					g_map_item = 2;
					break;
				case 4:
					g_map_item = 1;
					break;
				case 8:
					g_map_item = 3;
					break;
				case 12:
					g_map_item = 0;
					break;

				}
			}
		}
	}
}

void Enemy1(int a) {//enemy1の配置
	SRand((int)time(NULL));
	int x, y;
	for (int i = 0; i < a; i++) {
		int count = 0;
		E1[i].HP = 6 + 2 * g_level;
		E1[i].AP = 2 + g_level;
		x = GetRand(10) + 2;
		y = GetRand(5) + 2;
		E1[i].x = x * square + display_x;
		E1[i].y = y * square + display_y;
		if (g_mapdata[g_mapnum][x][y] == 0) {
			for (int j = 0; j < a; j++) {
				if (E1[i].x == E1[j].x && E1[i].y == E1[j].y) {
					count++;
				}
			}
			if (count != 1) {
				i--;
			}
		}
		else {
			i--;
		}
	}
	g_E1_num = a;
}

void Enemy2() {//enemy2の配置
	g_E2_num = 0;
	for (int x = 0; x < 15; x++) {
		for (int y = 0; y < 10; y++) {
			if (g_mapdata[g_mapnum][x][y] == -2) {
				E2[g_E2_num].x = x * square + display_x;
				E2[g_E2_num].y = y * square + display_y;
				g_E2_num++;
			}
		}
	}
	for (int i = 0; i < g_E2_num; i++) {
		E2_attack[i].count = 1;
	}
}

void Boss() {//Bossの配置
	B.x = 7 * square + display_x;
	B.y = 3 * square + display_y;
	B.AP = 2 + g_level;
	B.HP = 40 + 10 * g_level;
	B.direction = LEFT;
}

void Damage() {
	if (g_P_damage[RIGHT] > 0) {
		P.vxR = 4;
		if (P.x + square >= 600 + display_x) Scroll(RIGHT);//右端到達
	}
	else if (g_P_damage[LEFT] > 0) {
		P.vxL = -4;
		if (P.x <= 0 + display_x) Scroll(LEFT);//左端到達
	}
	else if (g_P_damage[UP] > 0) {
		P.vyU = -4;
		if (P.y <= 0 + display_y) Scroll(UP);//上端到達
	}
	else if (g_P_damage[DOWN] > 0) {
		P.vyD = 4;
		if (P.y + square >= 400 + display_y) Scroll(DOWN);//下端到達
	}
	P = Stop(P);
}

void Attack() {
	switch (g_item) {
	case 0:
		P.count = 30;//全体フレームが30F
		Sword();
		Sound(0);
		break;
	case 1:
		P.count = 40;//硬直は40F
		A.count = 600;//矢の持続が600F
		A.direction = P.direction;
		A.x = P.x;
		A.y = P.y;
		Arrow();
		Sound(1);
		break;
	case 2:
		P.count = 20;//硬直は20F
		Rock();
		break;
	default:
		break;
	}

}

void Select() {
	if (g_getitem[1] == 1 && g_getitem[2] == 1) {
		if (g_item != 2)g_item ++;
		else g_item = 0;

	}
	else if (g_getitem[1] == 1) {
		if (g_item != 1)g_item++;
		else g_item = 0;
	}
	else if (g_getitem[2] == 1) {
		if (g_item != 2)g_item += 2;
		else g_item = 0;
	}
	Sound(6);
	WaitTimer(1000 / 60 * 10);
}

void Sword() {
	switch (P.direction) {
	case RIGHT:
		for (int i = 0; i < 100; i++) {
			if (P.x + square <= E1[i].x && P.x + square + square >= E1[i].x) {
				if (P.y <= E1[i].y + square && P.y + square >= E1[i].y) {
					E1[i].HP -= P.AP;
					if (E1[i].HP <= 0) g_score += 1000;
				}
			}
			if (P.x + square <= E2[i].x && P.x + square + square >= E2[i].x) {
				if (P.y <= E2[i].y + square && P.y + square >= E2[i].y) {
					g_E2_HP[g_mapnum][i] -= P.AP;
					if (g_E2_HP[g_mapnum][i] <= 0) g_score += 5000;
				}
			}
		}
		if (P.x + square <= B.x && P.x + square + square >= B.x) {
			if (P.y <= B.y + 5 * square && P.y + square >= B.y) {
				B.HP -= P.AP;
			}
		}

		break;
	case LEFT:
		for (int i = 0; i < 100; i++) {
			if (P.x <= E1[i].x + 2 * square && P.x >= E1[i].x + square) {
				if (P.y <= E1[i].y + square && P.y >= E1[i].y - square) {
					E1[i].HP -= P.AP;
					if (E1[i].HP <= 0) g_score += 1000;
				}
			}
			if (P.x <= E2[i].x + 2 * square && P.x >= E2[i].x + square) {
				if (P.y <= E2[i].y + square && P.y + square >= E2[i].y) {
					g_E2_HP[g_mapnum][i] -= P.AP;
					if (g_E2_HP[g_mapnum][i] <= 0) g_score += 5000;
				}
			}
		}
		if (P.x <= B.x + 6 * square && P.x >= B.x + 5 * square) {
			if (P.y <= B.y + 5 * square && P.y >= B.y - square) {
				B.HP -= P.AP;
			}
		}

		break;
	case UP:
		for (int i = 0; i < 100; i++) {
			if (P.x <= E1[i].x + square && P.x >= E1[i].x - square) {
				if (P.y <= E1[i].y + 2 * square && P.y >= E1[i].y + square) {
					E1[i].HP -= P.AP;
					if (E1[i].HP <= 0) g_score += 1000;
				}
			}
			if (P.x <= E2[i].x + square && P.x >= E2[i].x - square) {
				if (P.y <= E2[i].y + 2 * square && P.y >= E2[i].y + square) {
					g_E2_HP[g_mapnum][i] -= P.AP;
					if (g_E2_HP[g_mapnum][i] <= 0) g_score += 5000;
				}
			}
		}
		if (P.x <= B.x + 5 * square && P.x >= B.x - square) {
			if (P.y <= B.y + 6 * square && P.y >= B.y + 5 * square) {
				B.HP -= P.AP;
			}
		}
		if (P.x <= g_box2_x + square && P.x >= g_box2_x - square) {
			if (P.y <= g_box2_y + 2 * square && P.y >= g_box2_y + square) {
				g_getitem[g_map_item] = 1;
				Sound(8);
				g_mapdata[g_mapnum][(g_box2_x - display_x) / square][(g_box2_y - display_y) / square] = 0;
				switch (g_map_item) {
				case 0:
					P.AP *= 2;
					break;
				case 3:
					P.DP *= 2;
					break;
				default:
					break;
				}
				g_score += 10000;
			}
		}
		break;
	case DOWN:
		for (int i = 0; i < 100; i++) {
			if (P.x <= E1[i].x + square && P.x >= E1[i].x - square) {
				if (P.y <= E1[i].y - square && P.y >= E1[i].y - 2 * square) {
					E1[i].HP -= P.AP;
					if (E1[i].HP <= 0) g_score += 1000;
				}
			}
			if (P.x <= E2[i].x + square && P.x >= E2[i].x - square) {
				if (P.y <= E2[i].y - square && P.y >= E2[i].y - 2 * square) {
					g_E2_HP[g_mapnum][i] -= P.AP;
					if (g_E2_HP[g_mapnum][i] <= 0) g_score += 5000;
				}
			}
		}
		if (P.x <= B.x + 5 * square && P.x >= B.x - square) {
			if (P.y <= B.y - square && P.y >= B.y - 2 * square) {
				B.HP -= P.AP;
			}
		}
		break;
	default:
		break;
	}
}

	void Arrow() {
		switch (A.direction) {
		case RIGHT:
			A.x += 10;
			for (int i = 0; i < 100; i++) {
				if (A.x >= E1[i].x && A.x - square / 2 <= E1[i].x) {
					if (A.y <= E1[i].y + square && A.y + square >= E1[i].y) {
						if (E1[i].HP > 0) {
							E1[i].HP--;
							A.count = 0;
							if (E1[i].HP <= 0) g_score += 1000;
						}
					}
				}
				if (A.x >= E2[i].x && A.x - square / 2 <= E2[i].x) {
					if (A.y <= E2[i].y + square && A.y + square >= E2[i].y) {
						if (g_E2_HP[g_mapnum][i] > 0) {
							g_E2_HP[g_mapnum][i] --;
							A.count = 0;
							if (g_E2_HP[g_mapnum][i] <= 0) g_score += 5000;
						}
					}
				}
			}
			if (A.x <= B.x && A.x + square >= B.x) {
				if (A.y <= B.y + 5 * square && A.y + square >= B.y) {
					if (B.HP > 0) {
						B.HP--;
						A.count = 0;
					}
				}
			}
			break;
		case LEFT:
			A.x -= 10;
			for (int i = 0; i < 100; i++) {
				if (A.x <= E1[i].x && A.x + square / 2 >= E1[i].x) {
					if (A.y <= E1[i].y + square && A.y >= E1[i].y - square) {
						if (E1[i].HP > 0) {
							E1[i].HP--;
							A.count = 0;
							if (E1[i].HP <= 0) g_score += 1000;
						}
					}
				}
				if (A.x <= E2[i].x && A.x + square / 2 >= E2[i].x) {
					if (A.y <= E2[i].y + square && A.y >= E2[i].y - square) {
						if (g_E2_HP[g_mapnum][i] > 0) {
							g_E2_HP[g_mapnum][i] --;
							A.count = 0;
							if (g_E2_HP[g_mapnum][i] <= 0) g_score += 5000;
						}
					}
				}
			}
			if (A.x <= B.x + 5 * square && A.x >= B.x + 4 * square) {
				if (A.y <= B.y + 5 * square && A.y >= B.y - square) {
					if (B.HP > 0) {
						B.HP--;
						A.count = 0;
					}
				}
			}
			break;
		case UP:
			A.y -= 10;
			for (int i = 0; i < 100; i++) {
				if (A.x <= E1[i].x + square && A.x >= E1[i].x - square) {
					if (A.y <= E1[i].y && A.y + square / 2 >= E1[i].y) {
						if (E1[i].HP > 0) {
							E1[i].HP--;
							A.count = 0;
							if (E1[i].HP <= 0) g_score += 1000;
						}
					}
				}
				if (A.x <= E2[i].x + square && A.x >= E2[i].x - square) {
					if (A.y <= E2[i].y && A.y + square / 2 >= E2[i].y) {
						if (g_E2_HP[g_mapnum][i] > 0) {
							g_E2_HP[g_mapnum][i] --;
							A.count = 0;
							if (g_E2_HP[g_mapnum][i] <= 0) g_score += 5000;
						}
					}
				}
			}
			if (A.x <= B.x + 5 * square && A.x >= B.x - square) {
				if (A.y <= B.y + 5 * square && A.y >= B.y + 4 * square) {
					if (B.HP > 0) {
						B.HP--;
						A.count = 0;
					}
				}
			}
			break;
		case DOWN:
			A.y += 10;
			for (int i = 0; i < 100; i++) {
				if (A.x <= E1[i].x + square && A.x >= E1[i].x - square) {
					if (A.y >= E1[i].y && A.y - square / 2 <= E1[i].y) {
						if (E1[i].HP > 0) {
							E1[i].HP--;
							A.count = 0;
							if (E1[i].HP <= 0) g_score += 1000;
						}
					}
				}
				if (A.x <= E2[i].x + square && A.x >= E2[i].x - square) {
					if (A.y >= E2[i].y && A.y - square / 2 <= E2[i].y) {
						if (g_E2_HP[g_mapnum][i] > 0) {
							g_E2_HP[g_mapnum][i] --;
							A.count = 0;
							if (g_E2_HP[g_mapnum][i] <= 0) g_score += 5000;
						}
					}
				}
			}
			if (A.x <= B.x + 5 * square && A.x >= B.x - square) {
				if (A.y <= B.y && A.y >= B.y - square) {
					if (B.HP > 0) {
						B.HP--;
						A.count = 0;
					}
				}
			}
			break;
		default:
			break;
		}
		if (A.x <= 0 + display_x || A.x + square >= 600 + display_x) {
			A.count = 0;
		}
		else if (A.y <= 0 + display_y || A.y >= 400 + display_y) {
			A.count = 0;
		}
	}

void Rock() {
	if (P.direction == RIGHT) {
		if ((P.x- display_x) % square == 0) {
			if (g_mapdata[g_mapnum][(P.x + square - display_x) / square][(P.y - display_y) / square] == -4) {
				for (int x = 0; x < 15; x++) {
					for (int y = 0; y < 10; y++) {
						if (g_mapdata[g_mapnum][x][y] == -4) {
							g_mapdata[g_mapnum][x][y] = 0;
							Sound(9);
						}
					}
				}
			}
		}
	}
}

void BossAct() {
	B.vxR = 0;
	B.vxL = 0;
	B.vyU = 0;
	B.vyD = 0;

	if (B.count <= 0) {
		B.count = 180;
	}

	else if (B.count > 120) {
		if (P.x + square < B.x - 3 * square) {//左に進む
			g_Bpattern = LEFT;
		}
		else if (P.x > B.x + (5 + 3) * square) {//右に進む
			g_Bpattern = RIGHT;
		}
		else if (P.y + square < B.y - square) {//上に進む
			g_Bpattern = UP;
		}
		else if (P.y > B.y + 6 * square) {//下に進む
			g_Bpattern = DOWN;
		}
		else if (P.x >= B.x - square && P.x <= B.x + 5 * square) {
			if ((P.y >= B.y - 2 * square && P.y <= B.y + 6 * square)) {
				g_Bpattern = 5;
			}
		}
		else {
			g_Bpattern = 6;
		}
	}
	else if (B.count <= 120) {
		switch (g_Bpattern) {
		case RIGHT:
			B.vxR = 2;
			B.count -= 2;
			B.direction = RIGHT;
			break;
		case LEFT:
			B.vxL = 2;
			B.count -= 2;
			B.direction = LEFT;
			break;
		case UP:
			B.vyU = 2;
			B.count -= 2;
			break;
		case DOWN:
			B.vyD = 2;
			B.count -= 2;
			break;
		case 5:
			if (B.count == 60) {
				Sound(4);
			}
			if (B.count <= 60) {
				if (P.x >= B.x - 2 * square && P.x <= B.x + 6 * square) {
					if ((P.y >= B.y - 2 * square && P.y <= B.y + 6 * square)) {
						if (P.x < B.x + 2 * square) {//左側
							if (P.y < B.y + 2 * square) {//左上
								if ((B.x + 2 * square) - P.x > (B.y + 2 * square) - P.y) {
									P.HP -= B.AP / P.DP;
									g_P_damage[LEFT] = 20;
									Sound(1);
								}
								else {
									P.HP -= B.AP / P.DP;
									g_P_damage[UP] = 20;
									Sound(1);
								}
							}
							else {//左下
								if ((B.x + 2 * square) - P.x > P.y - (B.y + 2 * square)) {
									P.HP -= B.AP / P.DP;
									g_P_damage[LEFT] = 20;
									Sound(1);
								}
								else {
									P.HP -= B.AP / P.DP;
									g_P_damage[DOWN] = 20;
									Sound(1);
								}
							}
						}
						else {//右側
							if (P.y < B.y + 2 * square) {//右上
								if (P.x - (B.x + 2 * square) > (B.y + 2 * square) - P.y) {
									P.HP -= B.AP / P.DP;
									g_P_damage[RIGHT] = 20;
									Sound(1);
								}
								else {
									P.HP -= B.AP / P.DP;
									g_P_damage[UP] = 20;
									Sound(1);
								}
							}
							else {//右下
								if (P.x - (B.x + 2 * square) > P.y - (B.y + 2 * square)) {
									P.HP -= B.AP / P.DP;
									g_P_damage[RIGHT] = 20;
									Sound(1);
								}
								else {
									P.HP -= B.AP / P.DP;
									g_P_damage[DOWN] = 20;
									Sound(1);
								}
							}
						}
					}
				}
			}
		case 6:
			if (B.count == 60) {
				Sound(15);
			}
			if (B.count <= 60) {
				if (B.direction == LEFT) {
					if (P.x >= B.x - 4 * square && P.x <= B.x) {//前方3マス
						if (P.y >= B.y && P.y <= B.y + 5 * square) {//横3マス
							P.HP -= 4 * B.AP / P.DP;
							g_P_damage[LEFT] = 20;
							Sound(1);
						}
					}
				}
				else if (B.direction == RIGHT) {
					if (P.x >= B.x + 5 * square && P.x >= B.x + 3 * square) {//後方3マス
						if (P.y >= B.y && P.y <= B.y + 5 * square) {//横3マス
							P.HP -= 4 * B.AP / P.DP;
							g_P_damage[RIGHT] = 20;
							Sound(1);
						}
					}
				}
			}
		default:
			break;
		}
	}
	//当たり判定
	if (P.x >= B.x - square && P.x <= B.x + 5 * square) {
		if (P.y >= B.y - square && P.y <= B.y + 5 * square) {
			if (P.x < B.x + 2 * square) {//左側
				if (P.y < B.y + 2 * square) {//左上
					if ((B.x + 2 * square) - P.x > (B.y + 2 * square) - P.y) {
						P.HP -= 2 * B.AP / P.DP;
						g_P_damage[LEFT] = 20;
						Sound(1);
					}
					else {
						P.HP -= 2 * B.AP / P.DP;
						g_P_damage[UP] = 20;
						Sound(1);
					}
				}
				else {//左下
					if ((B.x + 2 * square) - P.x > P.y - (B.y + 2 * square)) {
						P.HP -= 2 * B.AP / P.DP;
						g_P_damage[LEFT] = 20;
						Sound(1);
					}
					else {
						P.HP -= 2 * B.AP / P.DP;
						g_P_damage[DOWN] = 20;
						Sound(1);
					}
				}
			}
			else {//右側
				if (P.y < B.y + 2 * square) {//右上
					if (P.x - (B.x + 2 * square) > (B.y + 2 * square) - P.y) {
						P.HP -= 2 * B.AP / P.DP;
						g_P_damage[RIGHT] = 20;
						Sound(1);
					}
					else {
						P.HP -= 2 * B.AP / P.DP;
						g_P_damage[UP] = 20;
						Sound(1);
					}
				}
				else {//右下
					if (P.x - (B.x + 2 * square) > P.y - (B.y + 2 * square)) {
						P.HP -= 2 * B.AP / P.DP;
						g_P_damage[RIGHT] = 20;
						Sound(1);
					}
					else {
						P.HP -= 2 * B.AP / P.DP;
						g_P_damage[DOWN] = 20;
						Sound(1);
					}
				}
			}
		}
	}
	if (B.x <= 3 * square + display_x)B.vxL = 0;
	if (B.x + 5 * square >= 600 - 3 * square + display_x)B.vxR = 0;
	if (B.y <= 2 * square + display_y)B.vyU = 0;
	if (B.y + 5 * square >= 400 -2 * square + display_y)B.vyD = 0;

	B.x += B.vxR;
	B.x -= B.vxL;
	B.y -= B.vyU;
	B.y += B.vyD;

	B.count--;

}

void EnemyAttack() {
	for (int i = 0; i < g_E1_num; i++) {
		if (E1[i].HP > 0) {
			if (abs(P.x - E1[i].x) <= square && abs(P.y - E1[i].y) <= square) {
				if (P.x < E1[i].x) {//左側
					if (P.y < E1[i].y) {//左上
						if (E1[i].x - P.x > E1[i].y - P.y) {
							P.HP -= E1[i].AP / P.DP;
							g_P_damage[LEFT] = 20;
							Sound(4);
						}
						else {
							P.HP -= E1[i].AP / P.DP;
							g_P_damage[UP] = 20;
							Sound(4);
						}
					}
					else {//左下
						if (E1[i].x - P.x > P.y - E1[i].y) {
							P.HP -= E1[i].AP / P.DP;
							g_P_damage[LEFT] = 20;
							Sound(4);
						}
						else {
							P.HP -= E1[i].AP / P.DP;
							g_P_damage[DOWN] = 20;
							Sound(4);
						}
					}
				}
				else {//右側
					if (P.y < E1[i].y) {//右上
						if (P.x - E1[i].x > E1[i].y - P.y) {
							P.HP -= E1[i].AP / P.DP;
							g_P_damage[RIGHT] = 20;
							Sound(4);
						}
						else {
							P.HP -= E1[i].AP / P.DP;
							g_P_damage[UP] = 20;
							Sound(4);
						}
					}
					else {//右下
						if (P.x - E1[i].x > P.y - E1[i].y) {
							P.HP -= E1[i].AP / P.DP;
							g_P_damage[RIGHT] = 20;
							Sound(4);
						}
						else {
							P.HP -= E1[i].AP / P.DP;
							g_P_damage[DOWN] = 20;
							Sound(4);
						}
					}
				}
			}
		}
	}
	DrawFormatString(200, 30, GetColor(255, 0, 0), "%d", g_P_damage[LEFT]);
		//2
		for (int i = 0; i < g_E2_num; i++) {
			if (g_E2_HP[g_mapnum][i] > 0)
			{
				if (E2_attack[i].count == 0) {
					switch (E2_attack[i].direction) {
					case RIGHT:
						E2_attack[i].x += 5;
						if (E2_attack[i].x >= P.x && E2_attack[i].x - square / 2 <= P.x) {
							if (E2_attack[i].y <= P.y + square && E2_attack[i].y + square >= P.y) {
								P.HP -= E2_attack[i].AP / P.DP;
								g_P_damage[RIGHT] = 30;
								E2_attack[i].count = 1;
							}
						}
						break;
					case LEFT:
						E2_attack[i].x -= 5;
						if (E2_attack[i].x <= P.x && E2_attack[i].x + square / 2 >= P.x) {
							if (E2_attack[i].y <= P.y + square && E2_attack[i].y >= P.y - square) {
								P.HP -= E2_attack[i].AP / P.DP;
								g_P_damage[LEFT] = 30;
								E2_attack[i].count = 1;
							}
						}
						break;
					case UP:
						E2_attack[i].y -= 5;
						if (E2_attack[i].x <= P.x + square && E2_attack[i].x >= P.x - square) {
							if (E2_attack[i].y <= P.y && E2_attack[i].y + square / 2 >= P.y) {
								P.HP -= E2_attack[i].AP / P.DP;
								g_P_damage[UP] = 30;
								E2_attack[i].count = 1;
							}
						}
						break;
					case DOWN://E1->E2
						E2_attack[i].y += 5;
						if (E2_attack[i].x <= E1[i].x + square && E2_attack[i].x >= E1[i].x - square) {
							if (A.y >= P.y && A.y - square / 2 <= P.y) {
								P.HP -= E2_attack[i].AP / P.DP;
								g_P_damage[DOWN] = 30;
								E2_attack[i].count = 1;
							}
						}
						break;
					default:
						break;
					}
				}
			}
			else {
				g_mapdata[g_mapnum][(E2[i].x - display_x) / square][(E2[i].y - display_y) / square] = 0;
			}
		}
}
void EnemyAct() {
	SRand((int)time(NULL));
	for (int i = 0; i < g_E1_num; i++) {
		if (E1[i].count == 0)E1[i].count = GetRand(130) + 90;//90～220F
	}
	for (int i = 0; i < g_E2_num; i++) {
		if (E2[i].count == 0)E2[i].count = 240;//240F
	}

	for (int i = 0; i < g_E1_num; i++) {
		E1[i].vxR = 0;
		E1[i].vxL = 0;
		E1[i].vyU = 0;
		E1[i].vyD = 0;
		if (E1[i].count == 20) {
			E1[i].direction = GetRand(7);
		}
		if (E1[i].count <= 20) {
			switch (E1[i].direction) {
			case 0://上
				E1[i].vyU = 2;
				break;
			case 1://右上
				E1[i].vxR = 2;
				E1[i].vyU = 2;
				break;
			case 2://右
				E1[i].vxR = 2;
				break;
			case 3://右下
				E1[i].vxR = 2;
				E1[i].vyD = 2;
				break;
			case 4://下
				E1[i].vyD = 2;
				break;
			case 5://左下
				E1[i].vxL = 2;
				E1[i].vyD = 2;
				break;
			case 6://左
				E1[i].vxL = 2;
				break;
			case 7://左上
				E1[i].vxL = 2;
				E1[i].vyU = 2;
				break;
			default:
				break;
			}
		}
		E1[i] = Stop(E1[i]);
		if (g_mapdata[g_mapnum][(E1[i].x + square / 2 - display_x) / square][(E1[i].y + square / 2 - display_y) / square] == 2) {
			E1[i].HP = 0;
		}
			if (E1[i].x <= 0 + display_x)E1[i].vxL = 0;
			if (E1[i].x + square >= 600 + display_x)E1[i].vxR = 0;
			if (E1[i].y <= 0 + display_y)E1[i].vyU = 0;
			if (E1[i].y + square >= 400 + display_y)E1[i].vyD = 0;


			E1[i].x += E1[i].vxR;
			E1[i].x -= E1[i].vxL;
			E1[i].y -= E1[i].vyU;
			E1[i].y += E1[i].vyD;
			E1[i].count--;
		}

		//2
		for (int i = 0; i < g_E2_num; i++) {
			//向き変更
			if (P.x < E2[i].x) {//左側
				if (P.y < E2[i].y) {//左上
					if (E2[i].x - P.x > E2[i].y - P.y) {
						E2[i].direction = LEFT;
					}
					else {
						E2[i].direction = UP;
					}
				}
				else {//左下
					if (E2[i].x - P.x > P.y - E2[i].y) {
						E2[i].direction = LEFT;
					}
					else {
						E2[i].direction = DOWN;
					}
				}
			}
			else {//右側
				if (P.y < E2[i].y) {//右上
					if (P.x - E2[i].x > E2[i].y - P.y) {
						E2[i].direction = RIGHT;
					}
					else {
						E2[i].direction = UP;
					}
				}
				else {//右下
					if (P.x - E2[i].x > P.y - E2[i].y) {
						E2[i].direction = RIGHT;
					}
					else {
						E2[i].direction = DOWN;
					}
				}
			}

			//砲弾発射
			if (g_E2_HP[g_mapnum][i] > 0) {
				if (E2[i].count == 240) {
					E2_attack[i].x = E2[i].x;
					E2_attack[i].y = E2[i].y;
					E2_attack[i].direction = E2[i].direction;
					E2_attack[i].count = 0;
					if (i == 0)Sound(5);
					E2_attack[i].AP = 2 * (1 + g_level);
				}
			}
			E2[i].count--;
		}
	}

	void Score() {
		g_score_count++;
		if (g_time > 60) {//1分経過後
			if (g_score_count % 60 == 0) {
				g_score -= 250;
			}
		}
		if (B.HP <= 0) {
			g_score += 50000;
		}
	}
