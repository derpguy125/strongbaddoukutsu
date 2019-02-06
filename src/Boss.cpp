#include <stdint.h>
#include <string.h>

#include "WindowsWrapper.h"

#include "Boss.h"
#include "BossFrog.h"
#include "BossOhm.h"
#include "BossX.h"
#include "NpChar.h"
#include "MyChar.h"
#include "Sound.h"
#include "NpcHit.h"
#include "Caret.h"
#include "TextScr.h"
#include "ValueView.h"
#include "Draw.h"
#include "Map.h"
#include "Bullet.h"

NPCHAR gBoss[BOSS_MAX];

void InitBossChar(int code)
{
	memset(gBoss, 0, sizeof(gBoss));
	gBoss[0].cond = 0x80;
	gBoss[0].code_char = code;
}

void PutBossChar(int fx, int fy)
{
	for (int b = BOSS_MAX - 1; b >= 0; b--)
	{
		if (gBoss[b].cond & 0x80)
		{
			int a;
			if (gBoss[b].shock)
			{
				a = 2 * ((gBoss[b].shock >> 1) & 1) - 1;
			}
			else
			{
				a = 0;
				
				if (gBoss[b].bits & npc_showDamage && gBoss[b].damage_view)
				{
					SetValueView(&gBoss[b].x, &gBoss[b].y, gBoss[b].damage_view);
					gBoss[b].damage_view = 0;
				}
			}
			
			int side;
			if (gBoss[b].direct)
				side = gBoss[b].view.back;
			else
				side = gBoss[b].view.front;
			
			PutBitmap3(
				&grcGame,
				(gBoss[b].x - side) / 0x200 - fx / 0x200 + a,
				(gBoss[b].y - gBoss[b].view.top) / 0x200 - fy / 0x200,
				&gBoss[b].rect,
				22);
		}
	}
}

void SetBossCharActNo(int a)
{
	gBoss[0].act_no = a;
}

void HitBossBullet()
{
	for (int bos = 0; bos < BOSS_MAX; bos++)
	{
		if (gBoss[bos].cond & 0x80)
		{
			for (int bul = 0; bul < BULLET_MAX; bul++)
			{
				if (gBul[bul].cond & 0x80 && gBul[bul].damage != -1)
				{
					//Check if bullet touches boss
					bool bHit = false;
					if (gBoss[bos].bits & npc_shootable
						&& gBoss[bos].x - gBoss[bos].hit.back < gBul[bul].x + gBul[bul].enemyXL
						&& gBoss[bos].x + gBoss[bos].hit.back > gBul[bul].x - gBul[bul].enemyXL
						&& gBoss[bos].y - gBoss[bos].hit.top < gBul[bul].y + gBul[bul].enemyYL
						&& gBoss[bos].y + gBoss[bos].hit.bottom > gBul[bul].y - gBul[bul].enemyYL)
						bHit = true;
					else if (gBoss[bos].bits & npc_invulnerable
						&& gBoss[bos].x - gBoss[bos].hit.back < gBul[bul].x + gBul[bul].blockXL
						&& gBoss[bos].x + gBoss[bos].hit.back > gBul[bul].x - gBul[bul].blockXL
						&& gBoss[bos].y - gBoss[bos].hit.top < gBul[bul].y + gBul[bul].blockYL
						&& gBoss[bos].y + gBoss[bos].hit.bottom > gBul[bul].y - gBul[bul].blockYL)
						bHit = true;
						
					if (bHit)
					{
						//Damage boss
						if (gBoss[bos].bits & npc_shootable)
						{
							int bos_;
							if (gBoss[bos].cond & 0x10)
								bos_ = 0;
							else
								bos_ = bos;
							
							gBoss[bos_].life -= gBul[bul].damage;
							
							if (gBoss[bos_].life > 0)
							{
								if (gBoss[bos].shock < 14)
								{
									SetCaret(gBul[bul].x, gBul[bul].y, 11, 0);
									SetCaret(gBul[bul].x, gBul[bul].y, 11, 0);
									SetCaret(gBul[bul].x, gBul[bul].y, 11, 0);
									PlaySoundObject(gBoss[bos_].hit_voice, 1);
								}
								
								gBoss[bos].shock = 8;
								gBoss[bos_].shock = 8;
								gBoss[bos_].damage_view -= gBul[bul].damage;
							}
							else
							{
								gBoss[bos_].life = bos_;
								
								if ((gMC.cond & 0x80) && gBoss[bos_].bits & npc_eventDie)
								{
									StartTextScript(gBoss[bos_].code_event);
								}
								else
								{
									PlaySoundObject(gBoss[bos_].destroy_voice, 1);
									
									switch (gBoss[bos_].size)
									{
										case 1:
											SetDestroyNpChar(gBoss[bos_].x, gBoss[bos_].y, gBoss[bos_].view.back, 4);
											break;
										case 2:
											SetDestroyNpChar(gBoss[bos_].x, gBoss[bos_].y, gBoss[bos_].view.back, 8);
											break;
										case 3:
											SetDestroyNpChar(gBoss[bos_].x, gBoss[bos_].y, gBoss[bos_].view.back, 16);
											break;
									}
									
									gBoss[bos_].cond = 0;
								}
							}
							
							if (--gBul[bul].life <= 0)
								gBul[bul].cond = 0;
						}
						else if (gBul[bul].code_bullet != 13
							&& gBul[bul].code_bullet != 14
							&& gBul[bul].code_bullet != 15
							&& gBul[bul].code_bullet != 28
							&& gBul[bul].code_bullet != 29
							&& gBul[bul].code_bullet != 30)
						{
							if (!(gBul[bul].bbits & 0x10))
							{
								SetCaret(gBul[bul].x, gBul[bul].y, 2, 2);
								PlaySoundObject(31, 1);
								gBul[bul].cond = 0;
							}
						}
						else
						{
							gBul[bul].life--;
						}
					}
				}
			}
		}
	}
}

void ActBossChar_0()
{
	;
}

BOSSFUNCTION gpBossFuncTbl[10] =
{
	ActBossChar_0,
	ActBossChar_Omega,
	ActBossChar_Frog,
	ActBossChar_MonstX,
	nullptr, //ActBossChar_Core,
	nullptr, //ActBossChar_Ironhead,
	nullptr, //ActBossChar_Twin,
	nullptr, //ActBossChar_Undead,
	nullptr, //ActBossChar_Press,
	nullptr, //ActBossChar_Ballos
};

void ActBossChar()
{
	if (gBoss[0].cond & 0x80)
	{
		if (gpBossFuncTbl[gBoss[0].code_char] != nullptr)
			gpBossFuncTbl[gBoss[0].code_char]();
		
		for (int bos = 0; bos < BOSS_MAX; bos++)
		{
			if (gBoss[bos].shock)
				gBoss[bos].shock--;
		}
	}
}

void HitBossMap()
{
	int offx[16];
	int offy[16];
	uint8_t atrb[16];
	
	offx[0] = 0;
	offx[1] = 1;
	offx[2] = 0;
	offx[3] = 1;
	offx[4] = 2;
	offx[5] = 2;
	offx[6] = 2;
	offx[7] = 0;
	offx[8] = 1;
	offx[9] = -1;
	offx[10] = -1;
	offx[11] = -1;
	offx[12] = -1;
	offx[13] = 0;
	offx[14] = 1;
	offx[15] = 2;
	
	offy[0] = 0;
	offy[1] = 0;
	offy[2] = 1;
	offy[3] = 1;
	offy[4] = 0;
	offy[5] = 1;
	offy[6] = 2;
	offy[7] = 2;
	offy[8] = 2;
	offy[9] = -1;
	offy[10] = 0;
	offy[11] = 1;
	offy[12] = 2;
	offy[13] = -1;
	offy[14] = -1;
	offy[15] = -1;
	
	for (int b = 0; b < BOSS_MAX; b++)
	{
		if ((gBoss[b].cond & 0x80) && !(gBoss[b].bits & npc_ignoreSolid))
		{
			int judg;
			if (gBoss[b].size < 3)
				judg = 4;
			else
				judg = 16;
			
			int x = gBoss[b].x / 0x2000;
			int y = gBoss[b].y / 0x2000;
			
			gBoss[b].flag = 0;
			for (int j = 0; j < judg; j++)
			{
				atrb[j] = GetAttribute(x + offx[j], y + offy[j]);
				
				switch (atrb[j])
				{
					case 0x02:
					case 0x60:
					case 0x61:
					case 0x64:
						JadgeHitNpCharBlock(&gBoss[b], x + offx[j], y + offy[j]);
						JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x05:
					case 0x41:
					case 0x43:
						JadgeHitNpCharBlock(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x44:
						if (!(gBoss[b].bits & npc_ignore44))
							JadgeHitNpCharBlock(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x50:
						JudgeHitNpCharTriangleA(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x51:
						JudgeHitNpCharTriangleB(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x52:
						JudgeHitNpCharTriangleC(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x53:
						JudgeHitNpCharTriangleD(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x54:
						JudgeHitNpCharTriangleE(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x55:
						JudgeHitNpCharTriangleF(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x56:
						JudgeHitNpCharTriangleG(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x57:
						JudgeHitNpCharTriangleH(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x70:
						JudgeHitNpCharTriangleA(&gBoss[b], x + offx[j], y + offy[j]);
						JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x71:
						JudgeHitNpCharTriangleB(&gBoss[b], x + offx[j], y + offy[j]);
						JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x72:
						JudgeHitNpCharTriangleC(&gBoss[b], x + offx[j], y + offy[j]);
						JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x73:
						JudgeHitNpCharTriangleD(&gBoss[b], x + offx[j], y + offy[j]);
						JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x74:
						JudgeHitNpCharTriangleE(&gBoss[b], x + offx[j], y + offy[j]);
						JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x75:
						JudgeHitNpCharTriangleF(&gBoss[b], x + offx[j], y + offy[j]);
						JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x76:
						JudgeHitNpCharTriangleG(&gBoss[b], x + offx[j], y + offy[j]);
						JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					case 0x77:
						JudgeHitNpCharTriangleH(&gBoss[b], x + offx[j], y + offy[j]);
						JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
						break;
					default:
						continue;
				}
			}
		}
	}
}