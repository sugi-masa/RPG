#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include "RPG.h"

int SHandle[100];

void GoSound() {
	SHandle[0] = LoadSoundMem("sample.MP3");
	SHandle[1] = LoadSoundMem("Sslash.MP3");
	for (int i = 0; i < 100; i++) {
		ChangeVolumeSoundMem(150, SHandle[i]);
	}
}

void Sound(int a) {
	switch (a) {
	case 0://Œ•Œ‚
		PlaySoundMem(SHandle[0], DX_PLAYTYPE_BACK, TRUE);
		break;
	case 1://‹|
		PlaySoundMem(SHandle[1], DX_PLAYTYPE_BACK, TRUE);
		break;
	}
}
