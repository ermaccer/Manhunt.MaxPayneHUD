#include "..\pch.h"
#include "eMPHUD.h"
#include "ManhuntSDK.h"
#include "MemoryMgr.h"
#include <iostream>

using namespace ManhuntFunctions;


int pTextureDictionary;

int pAwareStatus;
int pCashDamageTextures[TOTAL_DAMAGE_TEXTURES] = {};
int pHellDamageTextures[TOTAL_DAMAGE_TEXTURES] = {};
int pPigDamageTextures[TOTAL_DAMAGE_TEXTURES] = {};
int pBunnyDamageTextures[TOTAL_DAMAGE_TEXTURES] = {};
int pBulletTimeTextures[16] = {};

wchar_t wt[260] = {};
char temp[260] = {};


bool bItemChanged;
int nSelectedItem;
int  pWeaponIcons[4] = {};
CRGBA  pWeaponColors[4] = {};

float pHealthPosY;


int& nCurrentItemSlot = *(int*)0x7C9CD0;
int& nCurrentRedItem = *(int*)0x7C9CE8;
int& nCurrentBlueItem = *(int*)0x7C9CEC;
int& nCurrentGreenItem = *(int*)0x7C9CF0;
int& nCurrentLureItem = *(int*)0x7C9CF4;
int& iTimer = *(int*)0x756270;
int& iCurrentSkin = *(int*)0x6A94C0;

int timer = iTimer;


// bullet time stuff
int nCurrentBulletTimeAmount = 0;
bool bBulletTimeEnabled = false;
int player = 0;
bool bBulletTimeSoundPlayed = false;
int iBulletTimeTimer = iTimer;

void eMPHUD::LoadResources()
{

	pTextureDictionary = CallAndReturn<int,0x5EA510,const char*>("pictures\\mp_hud.txd");

	if (!pTextureDictionary)
		MessageBoxA(0, "Failed to load mp_hud.txd!", 0, MB_ICONERROR);

	if (pTextureDictionary)
	{
		printf("MaxPayneHUD | Loaded TXD!\n");

		for (int i = 0; i < TOTAL_DAMAGE_TEXTURES; i++)
		{
			char temp[64];
			sprintf_s(temp, "cashFrame_%d", i);
			pCashDamageTextures[i] = LoadTexture(pTextureDictionary, temp);
			if (pCashDamageTextures[i])
				printf("MaxPayneHUD | Loaded texture %s!\n", temp);
			else
			{
				printf("MaxPayneHUD | Failed to load texture %s!\n", temp);
				pCashDamageTextures[i] = 0;
			}
		}
		for (int i = 0; i < TOTAL_DAMAGE_TEXTURES; i++)
		{
			char temp[64];
			sprintf_s(temp, "hellFrame_%d", i);
			pHellDamageTextures[i] = LoadTexture(pTextureDictionary, temp);
			if (pHellDamageTextures[i])
				printf("MaxPayneHUD | Loaded texture %s!\n", temp);
			else
			{
				printf("MaxPayneHUD | Failed to load texture %s!\n", temp);
				pHellDamageTextures[i] = 0;
			}
		}
		for (int i = 0; i < TOTAL_DAMAGE_TEXTURES; i++)
		{
			char temp[64];
			sprintf_s(temp, "bunnyFrame_%d", i);
			pBunnyDamageTextures[i] = LoadTexture(pTextureDictionary, temp);
			if (pBunnyDamageTextures[i])
				printf("MaxPayneHUD | Loaded texture %s!\n", temp);
			else
			{
				printf("MaxPayneHUD | Failed to load texture %s!\n", temp);
				pBunnyDamageTextures[i] = 0;
			}
		}
		for (int i = 0; i < TOTAL_DAMAGE_TEXTURES; i++)
		{
			char temp[64];
			sprintf_s(temp, "pigFrame_%d", i);
			pPigDamageTextures[i] = LoadTexture(pTextureDictionary, temp);
			if (pPigDamageTextures[i])
				printf("MaxPayneHUD | Loaded texture %s!\n", temp);
			else
			{
				printf("MaxPayneHUD | Failed to load texture %s!\n", temp);
				pPigDamageTextures[i] = 0;
			}
		}

		for (int i = 0; i < 16; i++)
		{
			char temp[64];
			sprintf_s(temp, "bulletTime_%d", i);
			pBulletTimeTextures[i] = LoadTexture(pTextureDictionary, temp);
			if (pBulletTimeTextures[i])
				printf("MaxPayneHUD | Loaded texture %s!\n", temp);
			else
			{
				printf("MaxPayneHUD | Failed to load texture %s!\n", temp);
				pBulletTimeTextures[i] = 0;
			}
		}


		pAwareStatus = LoadTexture(pTextureDictionary, "awareStatus");

		if (pAwareStatus)
			printf("MaxPayneHUD | Loaded texture %s!\n", "awareStatus");
		else
		{
			printf("MaxPayneHUD | Failed to load texture %s!\n", "awareStatus");
			pAwareStatus = 0;
		}

	}
}

int eMPHUD::ProcessHealthMeter()
{
	int damageID = DAMAGE_14;
	int texture = 0;




	CEntity* plr = FindPlayer();


	if (plr)
	{
		if (plr->fHealth >= 100.0f)
			damageID = DAMAGE_14;
		if (plr->fHealth < 95.0F)
			damageID = DAMAGE_13;
		if (plr->fHealth < 80.0F)
			damageID = DAMAGE_12;
		if (plr->fHealth < 75.0F)
			damageID = DAMAGE_9;
		if (plr->fHealth < 65.0F)
			damageID = DAMAGE_8;
		if (plr->fHealth < 55.0F)
			damageID = DAMAGE_7;
		if (plr->fHealth < 50.0F)
			damageID = DAMAGE_6;
		if (plr->fHealth < 45.0F)
			damageID = DAMAGE_5;
		if (plr->fHealth < 40.0F)
			damageID = DAMAGE_4;
		if (plr->fHealth < 30.0F)
			damageID = DAMAGE_3;
		if (plr->fHealth < 25.0F)
			damageID = DAMAGE_2;
		if (plr->fHealth < 10.0F)
			damageID = DAMAGE_1;
		if (plr->fHealth < 0.0F)
			damageID = DAMAGE_0;
	}

	int result = (TOTAL_DAMAGE_TEXTURES - 1) - damageID;

	switch (iCurrentSkin)
	{
	case 0:  texture = pCashDamageTextures[result];
		break;
	case 1:  texture = pBunnyDamageTextures[result];
		break;
	case 2:  texture = pHellDamageTextures[result];
		break;
	case 3:  texture = pPigDamageTextures[result];
		break;
	default:  texture = pCashDamageTextures[result];
		break;
	}

	return texture;
}

void eMPHUD::HookDrawLifeBar(float posx, float posY, float scalex, float scaley, int r, int g, int b, int a, int pTexture)
{
	static float y = posY;
	pHealthPosY = y;

}

void eMPHUD::HookStatusTexture(float posx, float posY, float scalex, float scaley, int r, int g, int b, int a, int pTexture)
{
	DrawTexture(0.0F, pHealthPosY, 0.065f, 0.2f, r,g,b,a, ProcessHealthMeter());
	if (FindPlayer())
	{
		int plrStatus = *(int*)(*(int*)0x715B9C + 2416);

		if (*(int*)0x7D343C && plrStatus == 2 || plrStatus == 3 || plrStatus == 5)
		{
			float temp = *(float*)0x7D3458 * *(float*)0x7CE274;
			float x = *(float*)0x7CD240 - temp * *(float*)0x7CD240;
			DrawTexture(x - 0.034f, *(float*)0x7CE278 - 0.04, 0.08f, 0.08f, 255, 255, 255, 128, pAwareStatus);
		}
	}
	DrawTexture(0.063f, pHealthPosY + 0.085f, 0.022f, 0.11f, 255,255,255,180, pBulletTimeTextures[nCurrentBulletTimeAmount]);
	if (bBulletTimeEnabled)
	{
		DrawTexture(*(float*)0x7CF250, *(float*)0x7CF250, *(float*)0x7CF258, *(float*)0x7CF25C, 255, 102, 0, 15, 0);
	}
}

void eMPHUD::HookResourcesLoader()
{
	LoadResources();
	Call<0x5EA540>();
}

void eMPHUD::HookClipAmountText(char * text, float posX, float posY, float sizex, float sizey, float unk, EFontType font)
{
	PrintText(text, posX, posY + 0.15f, sizex, sizey, unk, font);
}

void eMPHUD::HookClipAmountTextTwo(char * text, float posX, float posY, float sizex, float sizey, float unk, EFontType font)
{
	if (strcmp(text, " / ") == 0)
		text = (char*)" +";
	PrintText(text, posX, posY + 0.15f, sizex, sizey, unk, font);

}

void eMPHUD::HookDrawWeaponName()
{
	if (FindPlayer())
	{
	    Memory::VP::Patch<int>(0x7C9C80, 1);
	    SetDrawRGBA(0, 0, 0, 255);
	    PrintText(GetCollectableNameAsChar().c_str(), ((*(float*)0x7C9DB8) - (strlen(GetCollectableNameAsChar().c_str()) * 0.0079f)) + 0.002f, ((*(float*)0x7C9DFC) + 0.25f) + 0.002f, 0.70, 0.70f, 1.0f, FONT_TYPE_DEFAULT);
	    SetDrawRGBA(255, 255, 255, 255);
	    PrintText(GetCollectableNameAsChar().c_str(), (*(float*)0x7C9DB8) - (strlen(GetCollectableNameAsChar().c_str()) * 0.0079f), (*(float*)0x7C9DFC) + 0.25f, 0.70, 0.70f, 1.0f, FONT_TYPE_DEFAULT);
	}


}

void eMPHUD::HookItemListDisplay(int id)
{
	bItemChanged = true;

	Call<0x5DDE20, int>(id);
}

void eMPHUD::HookShowItems()
{
	Call<0x5DBDD0>();

	if (bItemChanged && *(int*)0x7C86F8 == -1)
	{
		switch (nCurrentItemSlot)
		{
		case BELT_LEFT:  nSelectedItem = 0;
			break;
		case BELT_RIGHT: nSelectedItem = 1;
			break;
		case BELT_REAR:nSelectedItem = 3;
			break;
		case BACK:	nSelectedItem = 2;
			break;
		default: nSelectedItem = 0;
			break;

		}

		pWeaponIcons[0] = CallAndReturn<int, 0x5DF9A0, int, int>(nCurrentGreenItem, 0);
		pWeaponIcons[1] = CallAndReturn<int, 0x5DF9A0, int, int>(nCurrentBlueItem, 0);
		pWeaponIcons[2] = CallAndReturn<int, 0x5DF9A0, int, int>(nCurrentRedItem, 0);
		pWeaponIcons[3] = CallAndReturn<int, 0x5DF9A0, int, int>(nCurrentLureItem, 0);

		pWeaponColors[0] = { 45,45,45,255 };
		pWeaponColors[1] = { 45,45,45,255 };
		pWeaponColors[2] = { 45,45,45,255 };
		pWeaponColors[3] = { 45,45,45,255 };

		Call<0x5F5CF0>();
		Call<0x5F5C90, int, int>(2, 2);

		for (int i = 0; i < 4; i++)
		{
			if (i == nSelectedItem)
				pWeaponColors[i] = { 255,255,255,255 };
			else
				pWeaponColors[i] = { 45,45,45, 255 };

			if (pWeaponIcons[i] != *(int*)0x7D395C)
				DrawTexture(0.35 + (i * 0.08f), *(float*)0x7CE278 - 0.47f, 0.06f, 0.06f, pWeaponColors[i].r, pWeaponColors[i].g, pWeaponColors[i].b, pWeaponColors[i].a, *(int*)pWeaponIcons[i]);
		}

		if (iTimer - timer <= 2500) return;
		bItemChanged = false;
		timer = iTimer;

	}


	

}

void eMPHUD::HookResetTimer()
{
	//bItemChanged = false;
	iBulletTimeTimer = 0;
	bBulletTimeEnabled = false;
	nCurrentBulletTimeAmount = 15;
	Call<0x5B5CD0>();
}

std::string eMPHUD::GetCollectableNameAsChar()
{
	if (FindPlayer())
	{
		int id = 0;

		switch (nCurrentItemSlot)
		{
		case BELT_LEFT: id = nCurrentGreenItem;
			break;
		case BELT_RIGHT: id = nCurrentBlueItem;
			break;
		case BELT_REAR: id = nCurrentLureItem;
			break;
		case BACK:	id = nCurrentRedItem;
			break;
		default: id = 16;
			break;

		}



		wchar_t* name = GetCollectableName(id);
		if (name)
		{
			std::wstring wstr(name, wcslen(name));
			std::string out("", wcslen(name));
			std::copy(wstr.begin(), wstr.end(), out.begin());
			return out;
		}
		else
			return "Unknown";



	}

}

void __declspec(naked) eMPHUD::HookIncreaseExecuted()
{
	static int jmpPoint = 0x5B62E7;
	_asm
	{
		add ds:0x7B7D84, 1
		add nCurrentBulletTimeAmount, 4
		jmp jmpPoint
	}
}

void __declspec(naked) eMPHUD::HookIncreaseKilled()
{
	static int jmpContinue = 0x5B63A7;
	_asm {
		add ds:0x7B7DA0, 1
		add nCurrentBulletTimeAmount, 2
		jmp jmpContinue
	}
}

void __declspec(naked) eMPHUD::HookBulletTimeControl()
{

	static int jmpPoint = 0x463700;
	static int jmpPointFalse = 0x463653;
	_asm mov player, ebp;


	BulletTimeControl();


	if (*(int*)(player + 1272)) // is inventory pressed?
	{
		if (nCurrentBulletTimeAmount > 0)
		{
			bBulletTimeEnabled ^= 1;
			bBulletTimeSoundPlayed = false;
		}

	}
	else
	_asm jmp jmpPointFalse







	_asm jmp jmpPoint;

}

void __declspec(naked) eMPHUD::HookTimeControl()
{
	if (!bBulletTimeEnabled)
	{
		_asm {
			mov eax, ebx
			sub eax, ds:0x756278
			mov ds:0x756280, eax
		}
	}
	else
	{
		_asm {
			mov ds:0x756280, 5
		}
	}
}

void eMPHUD::ProcessBarDepletion()
{
	if (iTimer - iBulletTimeTimer <= 600) return;
	nCurrentBulletTimeAmount--;
	iBulletTimeTimer = iTimer;
}

void eMPHUD::BulletTimeControl()
{

	if (nCurrentBulletTimeAmount > 15)
		nCurrentBulletTimeAmount = 15;

	if (nCurrentBulletTimeAmount < 0)
		nCurrentBulletTimeAmount = 0;

	if (nCurrentBulletTimeAmount == 0)
		bBulletTimeEnabled = false;



	if (bBulletTimeEnabled)
	{


		Memory::VP::Nop(0x4D8845, 11);
		Memory::VP::Patch<int>(0x756280, 8);
		if (!bBulletTimeSoundPlayed)
		{
			Call<0x456280, int, float>(6, *(float*)0x7C9D10);
			bBulletTimeSoundPlayed = true;
		}
		ProcessBarDepletion();

	}
	else
	{
		// restore code
		Memory::VP::Patch<short>(0x4D8845, 0xD889);
		Memory::VP::Patch<int>(0x4D8847, 0x6278052B);
		Memory::VP::Patch<short>(0x4D8847 + 4, 0x0075);
		Memory::VP::Patch<int>(0x4D884D, 0x756280A3);
		Memory::VP::Patch<char>(0x4D884D + 4, 0x00);
	}


}



