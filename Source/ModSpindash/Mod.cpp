/// <summary>
/// The main entry point for the injection.
/// </summary>
#include "Player.h"
extern "C" _declspec(dllexport) void Init()
{



	Player::Install();
}

extern "C" __declspec(dllexport) void PostInit() {

		MessageBox(nullptr, TEXT("This mod is in work in progress"), TEXT("Modren Sonic Spindash"), MB_ICONERROR);


	}
