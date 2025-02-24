#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <time.h>
#include <math.h>
#include "RPG.h"

void movie(){
	int color = GetColor(0, 0, 0);

	for (int i = 1; i < 15; i ++) {
		for (int j = 0; j < 8 * i; j++) {//8.16.24.32-//pow(2 * i - 1, 2) - pow(2 * (i - 1) - 1, 2)
			if (j < 2 * i) {
				DrawBox(P.x + (j - i) * square, P.y - i * square, P.x + (j - i) * square + square, P.y, color, TRUE);
			}
			else if (j < 4 * i) {
				DrawBox(P.x + i * square, P.y + ((j - 2 * i) - i) * square, P.x + i * square + square, P.y + (j - 2 * i) * square + square, color, TRUE);
			}
			else if (j < 6 * i) {
				DrawBox(P.x - ((j - 4 * i) - i) * square, P.y + i * square, P.x - ((j - 4 * i) - i) * square + square, P.y + i * square + square, color, TRUE);
			}
			else if (j < 8 * i) {
				DrawBox(P.x - i * square, P.y - ((j - 6 * i) - i) * square, P.x - i * square + square, P.y - (j - 2 * i) * square + square, color, TRUE);
			}
			WaitTimer(1);
		}
		Sound(1);
	}

	//DrawBox(P.x, P.y - square, P.x + square, P.y, color, TRUE);
	//DrawBox(P.x + square, P.y - square, P.x + 2 * square, P.y, color, TRUE);
	//DrawBox(P.x + square, P.y, P.x + 2 * square, P.y + square, color, TRUE);
	//DrawBox(P.x + square, P.y + square, P.x + 2 * square, P.y + 2 * square, color, TRUE);
	//DrawBox(P.x, P.y - square, P.x + square, P.y, color, TRUE);
	//DrawBox(P.x, P.y - square, P.x + square, P.y, color, TRUE);
	//DrawBox(P.x, P.y - square, P.x + square, P.y, color, TRUE);
	//DrawBox(P.x, P.y - square, P.x + square, P.y, color, TRUE);
	//DrawBox(P.x, P.y - square, P.x + square, P.y, color, TRUE);

}