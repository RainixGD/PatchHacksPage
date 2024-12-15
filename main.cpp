#include "./includes.h"
#include "./PatchHacksPageManager.h"

bool(__thiscall* MenuLayer_init)(MenuLayer* self);
bool __fastcall MenuLayer_init_H(MenuLayer* self, void*) {
	if (!MenuLayer_init(self)) return false;
	PatchHacksPageManager::getInstance()->onMenuLayer(self);
	return true;
}

void(__thiscall* PauseLayer_customSetup)(PauseLayer* self);
void __fastcall PauseLayer_customSetup_H(PauseLayer* self, void*) {
	PauseLayer_customSetup(self);
	PatchHacksPageManager::getInstance()->onPauseLayer(self);
}

void(__thiscall* EditorPauseLayer_customSetup)(CCLayer* self);
void __fastcall EditorPauseLayer_customSetup_H(CCLayer* self, void*) {
	EditorPauseLayer_customSetup(self);
	PatchHacksPageManager::getInstance()->onEditorPauseLayer(self);
}

void(__thiscall* AppDelegate_trySaveGame)(void* self);
void __fastcall AppDelegate_trySaveGame_H(void* self, void*) {
	AppDelegate_trySaveGame(self);
	PatchHacksPageManager::getInstance()->onTrySaveGame();
}

void inject() {
#if _WIN32
	auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));

	PatchHacksPageManager::getInstance();// to patch the game before gamemanager init (to fix the unlock icons saving)

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x1907b0),
		reinterpret_cast<void*>(&MenuLayer_init_H),
		reinterpret_cast<void**>(&MenuLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x1E4620),
		reinterpret_cast<void*>(&PauseLayer_customSetup_H),
		reinterpret_cast<void**>(&PauseLayer_customSetup)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x73550),
		reinterpret_cast<void*>(&EditorPauseLayer_customSetup_H),
		reinterpret_cast<void**>(&EditorPauseLayer_customSetup)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x3D5E0),
		reinterpret_cast<void*>(&AppDelegate_trySaveGame_H),
		reinterpret_cast<void**>(&AppDelegate_trySaveGame)
	);

	MH_EnableHook(MH_ALL_HOOKS);
#endif
}

#if _WIN32
WIN32CAC_ENTRY(inject)
#endif