#include "CPhilosophyOfYourself.h"

CPhilosophyOfYourself::CPhilosophyOfYourself()
{
	m_bQuit 	= false;
	m_LastGS 	= GS_NONE;
	m_pUse 		= NULL;
	if (g_pGS->m_iBeginState == -2) {
		g_pGS->m_GS = GS_INTRO;
		g_pGS->m_iLevelToPlay = -1;
	} else if (g_pGS->m_iBeginState == -1) {
		g_pGS->m_GS = GS_MAIN_MENU;
		g_pGS->m_iLevelToPlay = -1;
	} else if (g_pGS->m_iBeginState >= 0) {
		g_pGS->m_GS = GS_GAME;
		g_pGS->m_iLevelToPlay = g_pGS->m_iBeginState;
	}
	
	//std. paths
	glcModel::Init	(	"res/models/"	,".off");
	glcFont::Init	(	"res/fonts/"	,".ttf");
	glcMusic::Init	(	"res/music/"	,".ogg");
	glcSound::Init	(20,"res/sounds/"	,".ogg");
	glcTexture::Init(	"res/gfx/png/"	,".png");
	
	//music
		//dynamically loaded
	//loading sounds
	glcSound::LoadSound("advice_taken"		,g_pGS->m_cfStdSoundVolume);
	glcSound::LoadSound("answer_reached"	,g_pGS->m_cfStdSoundVolume);
	glcSound::LoadSound("ignorance_destroy"	,g_pGS->m_cfStdSoundVolume);
	glcSound::LoadSound("key_respawn"		,g_pGS->m_cfStdSoundVolume);
	glcSound::LoadSound("key_restart"		,g_pGS->m_cfStdSoundVolume);
	glcSound::LoadSound("lie_explode"		,g_pGS->m_cfStdSoundVolume);
	glcSound::LoadSound("main_menu_step"	,g_pGS->m_cfStdSoundVolume);
	glcSound::LoadSound("player_die"		,g_pGS->m_cfStdSoundVolume);
	glcSound::LoadSound("player_jump"		,g_pGS->m_cfStdSoundVolume);
	glcSound::LoadSound("proof_proven"		,g_pGS->m_cfStdSoundVolume);
	//loading fonts
	glcFont::LoadTTF("FreeSans",8,130);
	glcFont::LoadTTF("Sniglet",8,130);
	glcFont::LoadTTF("Chunkfive",8,130);
	glcFont::LoadTTF("Junction 02",8,130);
	glcFont::LoadTTF("raleway_thin",8,130);
	glcFont::LoadTTF("orbitron-medium",8,130);
	glcFont::LoadTTF("OFLGoudyStMTT",8,130);
	//loading object types
	glcTexture::LoadTexture("answer");
	glcTexture::LoadTexture("player");
	glcTexture::LoadTexture("arrow");
	glcTexture::LoadTexture("ladder");
	glcTexture::LoadTexture("wood");
	glcTexture::LoadTexture("solid_argument");
	glcTexture::LoadTexture("axiom");
	glcTexture::LoadTexture("unknown");
	glcTexture::LoadTexture("false_belief");
	glcTexture::LoadTexture("ignorance");
	glcTexture::LoadTexture("lie");
	glcTexture::LoadTexture("rash_argument");
	glcTexture::LoadTexture("advice_ground");
	glcTexture::LoadTexture("advice_top");
	glcTexture::LoadTexture("advice_item_newspaper");
	glcTexture::LoadTexture("advice_item_internet");
	glcTexture::LoadTexture("advice_item_book");
	glcTexture::LoadTexture("proof_ground_1");
	glcTexture::LoadTexture("proof_ground_2");
	glcTexture::LoadTexture("proof_top_1");
	glcTexture::LoadTexture("proof_top_2");
	
	//loading particle effects
	glcTexture::LoadTexture("particles/circle1111");
	glcTexture::LoadTexture("particles/fire");
	glcTexture::LoadTexture("particles/snow");
}

CPhilosophyOfYourself::~CPhilosophyOfYourself()
{
	//deleting sounds
	glcSound::DeleteSound("advice_taken");
	glcSound::DeleteSound("answer_reached");
	glcSound::DeleteSound("ignorance_destroy");
	glcSound::DeleteSound("key_respawn");
	glcSound::DeleteSound("key_restart");
	glcSound::DeleteSound("lie_explode");
	glcSound::DeleteSound("main_menu_step");
	glcSound::DeleteSound("player_die");
	glcSound::DeleteSound("player_jump");
	glcSound::DeleteSound("proof_proven");
	//deleting fonts
	glcFont::DeleteTTF("FreeSans");
	glcFont::DeleteTTF("Sniglet");
	glcFont::DeleteTTF("Chunkfive");
	glcFont::DeleteTTF("Junction 02");
	glcFont::DeleteTTF("raleway_thin");
	glcFont::DeleteTTF("orbitron-medium");
	glcFont::DeleteTTF("OFLGoudyStMTT");
	//deleting object types
	glcTexture::DeleteTexture("answer");
	glcTexture::DeleteTexture("player");
	glcTexture::DeleteTexture("arrow");
	glcTexture::DeleteTexture("ladder");
	glcTexture::DeleteTexture("wood");
	glcTexture::DeleteTexture("solid_argument");
	glcTexture::DeleteTexture("axiom");
	glcTexture::DeleteTexture("unknown");
	glcTexture::DeleteTexture("false_belief");
	glcTexture::DeleteTexture("ignorance");
	glcTexture::DeleteTexture("lie");
	glcTexture::DeleteTexture("rash_argument");
	glcTexture::DeleteTexture("advice_ground");
	glcTexture::DeleteTexture("advice_top");
	glcTexture::DeleteTexture("advice_item_newspaper");
	glcTexture::DeleteTexture("advice_item_internet");
	glcTexture::DeleteTexture("advice_item_book");
	glcTexture::DeleteTexture("proof_ground_1");
	glcTexture::DeleteTexture("proof_ground_2");
	glcTexture::DeleteTexture("proof_top_1");
	glcTexture::DeleteTexture("proof_top_2");
	//deleting particle effects
	glcTexture::DeleteTexture("particles/circle1111");
	glcTexture::DeleteTexture("particles/fire");
	glcTexture::DeleteTexture("particles/snow");
	
	CGameState::Del();
	GLC_SAFE_DELETE(m_pUse);
}

void CPhilosophyOfYourself::Run() {
	while (!m_bQuit) {
		Update();
		Render();
	}
}

void CPhilosophyOfYourself::Update() {
	glcUpdate(1.0f);
	if (g_pGL->MouseQuit()) {g_pGS->m_GS = GS_QUIT;}
	if (g_pGL->KeyIsDown(SDLK_LALT) && (g_pGL->KeyIsDown(SDLK_F4) || g_pGL->KeyIsDown(SDLK_4))) {g_pGS->m_GS = GS_QUIT;}
	if (g_pGS->m_GS != m_LastGS) SwitchGS();
	
	if (!m_bQuit) {
		m_pUse->Update();
	}
	
	if (g_pGS->m_GS != m_LastGS) SwitchGS();
}

void CPhilosophyOfYourself::Render() {
	if (!m_bQuit) {
		m_pUse->Render();
	}
}

void CPhilosophyOfYourself::SwitchGS() {
	GLC_SAFE_DELETE(m_pUse);
	switch (g_pGS->m_GS) {
		case GS_INTRO:		m_pUse = new CIntro();		break;
		case GS_MAIN_MENU:	m_pUse = new CMainMenu();	break;
		case GS_GAME:		m_pUse = new CGame();		break;
		case GS_QUIT:		m_bQuit = true;				break;
		default: break;
	}
	m_LastGS = g_pGS->m_GS;
	
	g_pTimer->Update(1.0f); //important for rigth elapsed after switched GS
	g_pTimer->Update(1.0f); //important for rigth elapsed after switched GS
}

