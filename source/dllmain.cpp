// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "code/MemoryMgr.h"
#include "code/eMPHUD.h"

using namespace Memory::VP;

void Init()
{
	InjectHook(0x474948, eMPHUD::HookResetTimer, PATCH_CALL);

	InjectHook(0x5EE344, eMPHUD::HookResourcesLoader, PATCH_CALL);
	InjectHook(0x5F0F05, eMPHUD::HookDrawLifeBar, PATCH_CALL);
	InjectHook(0x5F085E, eMPHUD::HookStatusTexture, PATCH_CALL);
	InjectHook(0x5DF27B, eMPHUD::HookClipAmountText, PATCH_CALL);
	InjectHook(0x5DF422, eMPHUD::HookClipAmountText, PATCH_CALL);
	InjectHook(0x5DF735, eMPHUD::HookClipAmountText, PATCH_CALL);
	InjectHook(0x5DF8C3, eMPHUD::HookClipAmountText, PATCH_CALL);

	InjectHook(0x5DF540, eMPHUD::HookClipAmountTextTwo, PATCH_CALL);
	InjectHook(0x5DF61B, eMPHUD::HookClipAmountTextTwo, PATCH_CALL);

	InjectHook(0x5DEFBE, eMPHUD::HookDrawWeaponName, PATCH_CALL);

	// disable bars
	Nop(0x5F0F7A, 5);
	Nop(0x5F1463, 5);
	Nop(0x5F15D0, 5);

	// disable weapon swaps
	Nop(0x5DEB57, 5);
	Nop(0x5DEBD7, 5);
	Nop(0x5DEC18, 5);
	Nop(0x5DF007, 5);
	Nop(0x5DEC18, 5);
	Nop(0x5DEC5D, 5);

	// radar
	Patch<float>(0x7CA578, 0.1200000122f);
	Patch<float>(0x7CA580, 0.0f);
	Patch<float>(0x7CA56C, 0.8200000238f);

	// ally name
	Patch<float>(0x7CD248, -1);

	// item list
	InjectHook(0x46365E, eMPHUD::HookItemListDisplay, PATCH_CALL);
	InjectHook(0x46368A, eMPHUD::HookItemListDisplay, PATCH_CALL);
	InjectHook(0x4636AC, eMPHUD::HookItemListDisplay, PATCH_CALL);
	InjectHook(0x4636CE, eMPHUD::HookItemListDisplay, PATCH_CALL);
	InjectHook(0x5DDD6A, eMPHUD::HookItemListDisplay, PATCH_CALL);
	InjectHook(0x5DDE0A, eMPHUD::HookItemListDisplay, PATCH_CALL);
	InjectHook(0x5F27E8, eMPHUD::HookShowItems, PATCH_CALL);

	// bullet time
	InjectHook(0x5B62E0, eMPHUD::HookIncreaseExecuted, PATCH_JUMP);
	InjectHook(0x5B63A0, eMPHUD::HookIncreaseKilled, PATCH_JUMP);
	InjectHook(0x463640, eMPHUD::HookBulletTimeControl, PATCH_JUMP);

}

extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		if (*(int*)0x63BC93 != 0x24448B66)
		{
			MessageBoxA(0, "Invalid executable!", "Max Payne HUD", 0);
		}
		else Init();
	}
}