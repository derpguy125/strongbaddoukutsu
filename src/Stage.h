// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include "WindowsWrapper.h"

typedef enum MusicID
{
	MUS_SILENCE = 0x0,
	MUS_PIZZADX = 0x1,
	MUS_VIDELEC = 0x2,
	MUS_GAMEOVER = 0x3,
	MUS_SHOP = 0x4,
	MUS_FIRST = 0x5,
	MUS_1UP = 0x6,
	MUS_SHOP2 = 0x7,
	MUS_BOSS = 0x8,
	MUS_BOSS2 = 0x9,
	MUS_CLEAR = 0xA,
	MUS_CHOLACORAY = 0xB,
	MUS_SAARI = 0xC,
	MUS_PROME = 0xD,
} MusicID;

typedef struct STAGE_TABLE
{
	char parts[0x20];
	char map[0x20];
	int bkType;
	char back[0x20];
	char npc[0x20];
	char boss[0x20];
	signed char boss_no;
	char name[0x22];
} STAGE_TABLE;

extern int gStageNo;
extern MusicID gMusicNo;
extern unsigned int gOldPos;
extern MusicID gOldNo;
extern int gSoundtrack;

extern const STAGE_TABLE *gTMT;

BOOL LoadStageTable();
BOOL TransferStage(int no, int w, int x, int y);
void ChangeMusic(MusicID no);
void ReCallMusic(void);
BOOL CheckSoundtrackExists(int soundtrack);
