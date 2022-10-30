/// <summary>
/// The main entry point for the injection.
/// </summary>
#include "Player.h"
extern "C" _declspec(dllexport) void Init()
{



	Player::Install();
}


