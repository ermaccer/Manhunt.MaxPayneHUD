#include "..\pch.h"
#include "eMPHUD.h"
#include "ManhuntSDK.h"
#include "MemoryMgr.h"
#include <iostream>

using namespace ManhuntFunctions;


int pTextureDictionary;

int pAwareStatus;
int pCashDamageTextures[TOTAL_DAMAGE_TEXTURES] = {};


wchar_t wt[260] = {};
char temp[260] = {};


bool bItemChanged;
int nSelectedItem;\
int  pWeaponIcons[4] = {};
CRGBA  pWeaponColors[4] = {};

float pHealthPosY;


int& nCurrentItemSlot = *(int*)0x7C9CD0;
int& nCurrentRedItem = *(int*)0x7C9CE8;
int& nCurrentBlueItem = *(int*)0x7C9CEC;
int& nCurrentGreenItem = *(int*)0x7C9CF0;
int& nCurrentLureItem = *(int*)0x7C9CF4;
int& iTimer = *(int*)0x756270;

int timer = iTimer;


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
	int result = DAMAGE_14;

	CEntity* plr = FindPlayer();


	if (plr)
	{
		if (plr->fHealth >= 100.0f)
			result = DAMAGE_14;
		if (plr->fHealth < 95.0F)
			result = DAMAGE_13;
		if (plr->fHealth < 80.0F)
			result = DAMAGE_12;
		if (plr->fHealth < 75.0F)
			result = DAMAGE_9;
		if (plr->fHealth < 65.0F)
			result = DAMAGE_8;
		if (plr->fHealth < 55.0F)
			result = DAMAGE_7;
		if (plr->fHealth < 50.0F)
			result = DAMAGE_6;
		if (plr->fHealth < 45.0F)
			result = DAMAGE_5;
		if (plr->fHealth < 40.0F)
			result = DAMAGE_4;
		if (plr->fHealth < 30.0F)
			result = DAMAGE_3;
		if (plr->fHealth < 25.0F)
			result = DAMAGE_2;
		if (plr->fHealth < 10.0F)
			result = DAMAGE_1;
		if (plr->fHealth < 0.0F)
			result = DAMAGE_0;
	}

	return (TOTAL_DAMAGE_TEXTURES - 1) - result;
}

void eMPHUD::HookDrawLifeBar(float posx, float posY, float scalex, float scaley, int r, int g, int b, int a, int pTexture)
{
	static float y = posY;
	pHealthPosY = y;
//	
}

void eMPHUD::HookStatusTexture(float posx, float posY, float scalex, float scaley, int r, int g, int b, int a, int pTexture)
{
	DrawTexture(0.0F, pHealthPosY, 0.075f, 0.2f, r,g,b,a, pCashDamageTextures[ProcessHealthMeter()]);
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

	printf("timer %d game %d   \r", timer, iTimer);

	//DrawTexture(posx - 0.79f, posY + 0.14f, 0.05f, 0.05f, r, g, b, a, pAwareStatus);
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
	timer = 0;
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



