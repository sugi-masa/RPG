#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include "RPG.h"

int SHandle[100];

void GoSound() {
	SHandle[0] = LoadSoundMem("sound/Sslash.MP3");
	SHandle[1] = LoadSoundMem("sound/Sarrow.mp3");
	SHandle[2] = LoadSoundMem("sound/Sheal.mp3");
	SHandle[3] = LoadSoundMem("sound/Spoison.mp3");
	SHandle[4] = LoadSoundMem("sound/Sstrike.mp3");
	SHandle[5] = LoadSoundMem("sound/Scannon.mp3");
	SHandle[6] = LoadSoundMem("sound/Sselect.mp3");
	SHandle[7] = LoadSoundMem("sound/Sscroll.mp3");
	SHandle[8] = LoadSoundMem("sound/Sopen.mp3");
	SHandle[9] = LoadSoundMem("sound/Srock.mp3");
	SHandle[10] = LoadSoundMem("sound/Swarp.mp3");
	SHandle[11] = LoadSoundMem("sound/Smiss.mp3");
	SHandle[12] = LoadSoundMem("sound/Sfall.mp3");
	SHandle[13] = LoadSoundMem("sound/Sgameover.mp3");
	SHandle[14] = LoadSoundMem("sound/Sgameclear.mp3");
	SHandle[15] = LoadSoundMem("sound/Sbomb.mp3");
	SHandle[50] = LoadSoundMem("sound/S_BGM.mp3");
	SHandle[51] = LoadSoundMem("sound/魔王魂 シャイニングスター.mp3");

	for (int i = 0; i < 100; i++) {
		ChangeVolumeSoundMem(g_volume * volume, SHandle[i]);
	}
	ChangeVolumeSoundMem(g_volume * volume / 2, SHandle[50]);
	ChangeVolumeSoundMem(g_volume * volume / 2, SHandle[51]);
}

void Sound(int a) {
	switch (a) {
	case 0://剣撃
		PlaySoundMem(SHandle[0], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 1://弓
		PlaySoundMem(SHandle[1], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 2://回復
		PlaySoundMem(SHandle[2], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 3://毒ダメージ
		PlaySoundMem(SHandle[3], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 4://ダメージを受ける
		PlaySoundMem(SHandle[4], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 5://大砲
		PlaySoundMem(SHandle[5], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 6:
		PlaySoundMem(SHandle[6], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 7:
		PlaySoundMem(SHandle[7], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 8:
		PlaySoundMem(SHandle[8], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 9:
		PlaySoundMem(SHandle[9], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 10:
		PlaySoundMem(SHandle[10], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 11:
		PlaySoundMem(SHandle[11], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 12:
		PlaySoundMem(SHandle[12], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 13:
		PlaySoundMem(SHandle[13], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 14:
		PlaySoundMem(SHandle[14], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 15:
		PlaySoundMem(SHandle[15], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 50://BGM
		PlaySoundMem(SHandle[50], DX_PLAYTYPE_LOOP, TRUE);
		break;
	case 51:
		StopSoundMem(SHandle[50]);
		break;
	case 52://BGM
		PlaySoundMem(SHandle[51], DX_PLAYTYPE_LOOP, TRUE);
		break;
	case 53:
		StopSoundMem(SHandle[51]);
		break;


	}
}