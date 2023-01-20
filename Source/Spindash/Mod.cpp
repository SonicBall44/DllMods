/// <summary>
/// The main entry point for the injection.
/// </summary>
#include "Player.h"
extern "C" _declspec(dllexport) void Init()
{



	Player::Install();
	INIReader reader("Spindash.ini");
	Player::cSonic_spindashSpeed = reader.GetInteger("Speed", "SpeedMax", 110);
	if (Player::cSonic_spindashSpeed < 10 )
	{
		MessageBox(nullptr, TEXT("The mininum Spindash speed should be 15, setting to default speed"), TEXT("Spindash Speed"), MB_ICONERROR);
		Player::cSonic_spindashSpeed = 110;

	}
}

extern "C" __declspec(dllexport) void PostInit() {

		MessageBox(nullptr, TEXT("This mod is in work in progress"), TEXT("Modren Sonic Spindash"), MB_ICONERROR);


	}
