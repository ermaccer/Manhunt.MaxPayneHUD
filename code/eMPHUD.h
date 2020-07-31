#pragma once
#include "ManhuntSDK.h"
#include <string>

enum eDamageStates {
	DAMAGE_0,
    DAMAGE_1,
	DAMAGE_2,
	DAMAGE_3,
	DAMAGE_4,
	DAMAGE_5,
	DAMAGE_6,
	DAMAGE_7,
	DAMAGE_8,
	DAMAGE_9,
	DAMAGE_10,
	DAMAGE_11,
	DAMAGE_12,
	DAMAGE_13,
	DAMAGE_14,
	TOTAL_DAMAGE_TEXTURES
};


namespace eMPHUD {
	void  LoadResources();
	int   ProcessHealthMeter();
	void  HookDrawLifeBar(float posx, float posY, float scalex, float scaley, int r, int g, int b, int a, int pTexture);
	void  HookStatusTexture(float posx, float posY, float scalex, float scaley, int r, int g, int b, int a, int pTexture);
	void  HookResourcesLoader();
	void  HookClipAmountText(char* text, float posX, float posY, float sizex, float sizey, float unk, EFontType font);
	void  HookClipAmountTextTwo(char* text, float posX, float posY, float sizex, float sizey, float unk, EFontType font);
	void  HookDrawWeaponName();
	void  HookItemListDisplay(int id);
	void  HookShowItems();
	void  HookResetTimer();
	std::string GetCollectableNameAsChar();

	// bullet time
	void HookIncreaseExecuted();
	void HookIncreaseKilled();
	void HookBulletTimeControl();
	void HookTimeControl();
	void ProcessBarDepletion();
	void BulletTimeControl();
}