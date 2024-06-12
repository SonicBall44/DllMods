





int ctimer = 0;
bool isstop = false;



inline uint32_t UseMultAdr(uint32_t baseAddress, const std::vector<uint32_t>& offsets)
{
	uint32_t currentAddress = *(uint32_t*)baseAddress;
	for (size_t i = 0; i < offsets.size(); ++i)
	{
		currentAddress += offsets[i];

		if (i < offsets.size() - 1)
		{
			currentAddress = *(uint32_t*)currentAddress;
		}
	}
	return currentAddress;
}




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
	Chao::CSD::RCPtr<Chao::CSD::CScene> m_timer;
	
    velNoY.y() = 0;
    bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
	bool PressedB = input.IsTapped(Sonic::eKeyState_B);
	//ctimer += 1;

	isstop = false;

	


	//Note: Those buttons are just for testing, they will be changed later on

	if (PressedY ) {
        
       // sonic->ChangeState("NormalDamageDead"); (Was used to test to make sure it passed)


		*(float*)UseMultAdr(0x1E0BE5C, { 0x8, 0x1A4 }) = 0.1f; // This code works similar to one of the HedgeMod code expect it goes way slower
		*(bool*)UseMultAdr(0x1E0BE5C, { 0x8, 0x19D }) = true;
		//isstop = true;
		printf("YOU COMMITED CHAOS CONTROL");
		
	

    }


	if (PressedB) {
		*(float*)UseMultAdr(0x1E0BE5C, { 0x8, 0x1A4 }) = 1.0f; //This will resume back to normal speed when pushed
		*(bool*)UseMultAdr(0x1E0BE5C, { 0x8, 0x19D }) = false;

	}

    //printf("%s\n", anim);
	//printf("%d\n", m_timer);
	printf("%i\n", ctimer);
    //printf("%s\n", state);
}

EXPORT void Init()
{

    INSTALL_HOOK(CPlayerSpeedUpdateParallel);
	CreateConsole();

}
