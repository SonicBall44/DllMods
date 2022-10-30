#include "Player.h"
#include <iostream>








HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo) {
	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);

    Sonic::Player::CPlayerSpeedContext* sonic = This->GetContext();
    Hedgehog::Base::CSharedString state = This->m_StateMachine.GetCurrentState()->GetStateName();
    Hedgehog::Base::CSharedString anim = sonic->GetCurrentAnimationName();
    Sonic::SPadState input = Sonic::CInputState::GetInstance()->GetPadState();
	


	




	if (input.IsDown(Sonic::eKeyState_X)) {
		sonic->ChangeState("NormalDamageDeadAir");
	}
		
}










void Player::Install()

{
	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
}
