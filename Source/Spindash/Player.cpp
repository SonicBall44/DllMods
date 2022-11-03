#include "Player.h"
#include <iostream>
bool SpinDash = false;
bool Rolling = false;
float RollingTime = 0.0f;
float SlidingSpeed = 0.0f;
float SlidingTime = 0.0f;
bool Sliding = false;
bool Ducking = false;
float const cSonic_spindashSpeed = 30.0f;
float const cSonic_slidingSpeedMin = 10.0f;
float const cSonic_slidingSpeedMax = 70.0f;
float const cSonic_spindashTime = 3.0f;
float const cSonic_slidingTime = 3.0f;



void CreateConsole()
{
	if (!AllocConsole()) {
		// Add some error handling here.
		// You can call GetLastError() to get more info about the error.
		return;
	}

	// std::cout, std::clog, std::cerr, std::cin
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();

	// std::wcout, std::wclog, std::wcerr, std::wcin
	HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hConIn = CreateFile(_T("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
	SetStdHandle(STD_ERROR_HANDLE, hConOut);
	SetStdHandle(STD_INPUT_HANDLE, hConIn);
	std::wcout.clear();
	std::wclog.clear();
	std::wcerr.clear();
	std::wcin.clear();
}

HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);

    Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
    Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
    Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
    Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();
    Hedgehog::Math::CVector velNoY = sonic->m_Velocity;
    velNoY.y() = 0;
	//printf("%s\n", anim);
	//printf("%s\n", state);
}


//When Sliding
HOOK(void, __fastcall, Sonic_SlideStarts, 0x11D7110, hh::fnd::CStateMachineBase::CStateBase* This) {
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto player = sonic->m_pPlayer;
	if (SpinDash) {
		printf("SpinDash Mode");

	}
	else {
		printf("Sliding Mode");
		WRITE_MEMORY(0x1230A85, uint32_t, 0x15F84F4); // slide begin animation
		WRITE_MEMORY(0x1230A9F, uint32_t, 0x15F84F4); // slide begin animation
		WRITE_MEMORY(0x1230D74, uint32_t, 0x15F84F4); // slide hold animation
		
		//Replaces slide Animation with rolling animation
		WRITE_MEMORY(0x11D7124, uint32_t, 0x15F84F4);
		WRITE_MEMORY(0x11D6E6A, uint32_t, 0x15F84F4);
		WRITE_MEMORY(0x11D6EDB, uint32_t, 0x15F84F4);

		// Spindashing after being in the squat state
		WRITE_MEMORY(0x1230C3A, uint32_t, 0x15F5108); //Switches the state to sliding
		sonic->m_Velocity.x() = cSonic_slidingSpeedMax;
		//sonic->m_Velocity.y() += cSonic_slidingSpeedMax;
		//sonic->m_Velocity.z() += cSonic_slidingSpeedMax;
		return originalSonic_SlideStarts(This);

	}
}



void Player::Install()

{
	CreateConsole();
    printf("Spindash for Modren Sonic");  
	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
	INSTALL_HOOK(Sonic_SlideStarts);
    
   

}

