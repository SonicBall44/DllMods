/// <summary>
/// The main entry point for the injection.
/// </summary>
#include "Player.h"
extern "C" _declspec(dllexport) void Init()
{



	Player::Install();
	INIReader reader("Spindash.ini");
	Player::cSonic_spindashSpeed = reader.GetInteger("Speed", "SpeedMax", 110);


}

extern "C" __declspec(dllexport) void PostInit() {

		


	}
