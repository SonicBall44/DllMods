#include "Player.h"
#include <iostream>

float RollingTime = 0.0f;
int Player::cSonic_spindashSpeed = 110;




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


//Spindashing State
HOOK(void, __fastcall, Sonic_SlideStarts, 0x11D7110, hh::fnd::CStateMachineBase::CStateBase* This) {
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
	auto player = sonic->m_pPlayer;
	sonic->PlaySound(2002033, false);
	Hedgehog::Math::CQuaternion rot = sonic->m_spMatrixNode->m_Transform.m_Rotation;
	Eigen::Vector3f playerPosition;
	Eigen::Quaternionf playerRotation = rot;
	Eigen::Vector3f playerDir = playerRotation * Eigen::Vector3f::UnitZ();
	Eigen::Vector3f spinVel = playerDir * Player::cSonic_spindashSpeed;
	SharedPtrTypeless ChargePart;
	
	Hedgehog::Base::CSharedString Ch[] = {"Squat"};

	spinVel.y() = sonic->m_Velocity.y();
		//Replaces slide Animation with rolling animation
		WRITE_MEMORY(0x1230C3A, uint32_t, 0x15F5108);
		WRITE_MEMORY(0x11D7124, uint32_t, 0x15F84F4);
		WRITE_MEMORY(0x11D6E6A, uint32_t, 0x15F84F4);
		WRITE_MEMORY(0x11D6EDB, uint32_t, 0x15F84F4);

	
		Common::fCGlitterCreate(sonic, ChargePart, middlematrixNode, "ef_ch_sng_yh1_spinattack", 1);
		sonic->PlaySound(2002043, true);
		//WRITE_MEMORY(0x11D943D, uint8_t, 0x74); //Unlock direction while charging



		// Spindashing after being in the squat state
		 //Switches the state to sliding
		//sonic->m_Velocity.x() = cSonic_spindashSpeed * playerDir;

		sonic->m_Velocity = spinVel;
		//sonic->m_Velocity.z() = cSonic_slidingSpeedZ;
		//sonic->m_HorizontalVelocity.x() = cSonic_spindashSpeed;
		
		//sonic->m_Velocity.y() = cSonic_slidingSpeedMax;
		
		return originalSonic_SlideStarts(This);



	}


//Charging state

HOOK(void, __fastcall, Sonic_ChargeStart, 0x1230A30, hh::fnd::CStateMachineBase::CStateBase* This) {
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	//printf("In ChargeStart");
	auto player = sonic->m_pPlayer;
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	Eigen::Vector3f playerPosition;
	sonic->PlaySound(2002033, true);
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
	SharedPtrTypeless ChargePart;
	sonic->ChangeAnimation("JumpBall");
	Common::fCGlitterCreate(sonic, ChargePart, middlematrixNode, "ef_ch_sng_yh1_sliding", 1);
	return originalSonic_ChargeStart(This);
	

	
	}
		

HOOK(void, __fastcall, Sonic_ChargeNext, 0x1230B60, hh::fnd::CStateMachineBase::CStateBase* This) {
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
	SharedPtrTypeless ChargePart;
	//printf("In ChargeNext State");
	auto player = sonic->m_pPlayer;
	//sonic->ChangeAnimation("JumpBall");
	//Common::fCGlitterCreate(sonic, ChargePart, middlematrixNode, "ch_sng_yh1_spinball1", 1);
	sonic->PlaySound(2002033, false);
	return originalSonic_ChargeNext(This);
	
	
}


HOOK(void, __fastcall, Sonic_ChargeEnd, 0x12309A0, hh::fnd::CStateMachineBase::CStateBase* This) {
	//WRITE_MEMORY(0x1230C3A, uint32_t, 0x15F5108);
	printf("In ChargeEnd State");
	return originalSonic_ChargeEnd(This);
	

}



void Player::Install()

{
	//CreateConsole();
    printf("Spindash for Modern Sonic");  
	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
	INSTALL_HOOK(Sonic_ChargeStart);
	INSTALL_HOOK(Sonic_ChargeNext);
	INSTALL_HOOK(Sonic_ChargeEnd);
	INSTALL_HOOK(Sonic_SlideStarts);
	
   


