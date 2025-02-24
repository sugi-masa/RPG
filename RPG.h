#pragma once

#define volume 10
#define square 40
#define playersize 40
#define display_x 20
#define display_y 80
#define RIGHT 1
#define LEFT 2
#define UP 3
#define DOWN 4

enum GameState {
	GAME_TITLE,
	GAME_RULE,
	GAME_OPTION,
	GAME_PROCESS,
	GAME_CLEAR,
	GAME_OVER,
	GAME_ENDING
};

typedef struct{
	int x, y;
	int vxR, vxL, vyU, vyD;
	int count;
	int direction;
	int HP;
	int AP;
	int DP;
}stutas;

//グローバル構造体変数の宣言
extern stutas P;
extern stutas E1[100];
extern stutas E2[100];
extern stutas E2_attack[100];
extern stutas B;
extern stutas A;

//グローバル変数の宣言
extern int g_volume;
extern int g_level;

extern int g_gamestate;//ゲーム状態
extern int g_mapnum;
extern int g_mapdata[13][15][10];
extern int g_E1_num;
extern int g_E2_num;

extern int g_E2_HP[12][100];

extern int g_Bpattern;
extern int g_item;
extern int g_box2_x, g_box2_y;
extern int g_getitem[4];
extern int g_map_item;
extern int g_time;
extern int g_score;


//プロトタイプ宣言
void GoSound();
void GoTitle();
void DrawTitle();
void GoEnding();
void DrawEnding();

void GoRule();
void DrawRule();
void GoOption();
void DrawOption();

void GoProcess();
void GoDisplay();
void GoResult(int a);
void Process();
stutas Stop(stutas A);
void Area();
void Scroll(int i);
void FieldData();
void Box();
void HitKey();
void Enemy1(int a);
void Enemy2();
void Boss();
void Damage();
void Attack();
void Select();
void Sword();
void Arrow();
void Rock();
void EnemyAct();
void EnemyAttack();
void BossAct();
void Score();
void Display();
void GameOver();
void GameClear();

void Sound(int a);