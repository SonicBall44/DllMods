#include "Player.h"
#include <iostream>
bool isDead = false;

bool SpinDash = false;
bool Rolling = false;
bool Boost = false;
bool Print = true;
float RollingTime = 0.0f;



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
	printf("%s\n", anim);
	printf("%s\n", state);


   
    if (input.IsDown(Sonic::eKeyState_Y)) {
		printf("Charging");
		sonic->ChangeState("Squat");
		sonic->ChangeAnimation("SpinAttack");

		if (input.IsReleased(Sonic::eKeyState_Y)) {
			printf("WIP");
		}
	}

    

    if (state == "Squat") {
        if (input.IsDown(Sonic::eKeyState_B)) {
			printf("Ducking");
			if (input.IsDown(Sonic::eKeyState_A)) {
				printf("Spin Mode");
				sonic->ChangeAnimation("SpinAttack");
			}

        }
    }

}


void SpinDashing(Sonic::Player::CPlayerSpeedContext* context, const hh::fnd::SUpdateInfo& updateInfo) {
    auto currentAnim = context->GetCurrentAnimationName().c_str();
    auto input = Sonic::CInputState::GetInstance()->GetPadState();




}




void Player::Install()

{
	CreateConsole();
    printf("Hi");
  
	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
    
   

}
