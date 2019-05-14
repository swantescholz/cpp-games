#include "CLevel.h"

int CLevel::c_iNumInstances = 0;

CLevel::CLevel(CLevel &rhs)
{
	S_LevelIntern tmp;
	for (int i = 0; i < OT_LAST; i++) {
		for (m_lit = rhs.m_alObjects[i].begin(); m_lit != rhs.m_alObjects[i].end(); m_lit++) {
			tmp.pObj = DoubleObjPtr(m_lit->pObj);
			m_alObjects[i].push_back(tmp);
		}
	}
	m_lit 						= rhs.m_lit;
	m_TexBg 					= glcTexture(rhs.m_TexBg.GetName());
	m_vBgWH 					= rhs.m_vBgWH;
	m_iBGNumPasses 				= rhs.m_iBGNumPasses;
	m_pLight1 					= new glcLight(*(rhs.m_pLight1));
	m_Music 					= rhs.m_Music;
	m_fLightZ					= rhs.m_fLightZ;
	m_fLightInterpolationFactor = rhs.m_fLightInterpolationFactor;
	m_fCam1Z 					= rhs.m_fCam1Z;
	m_fCam1InterpolationFactor 	= rhs.m_fCam1InterpolationFactor;
	m_fCam1Scope[0] 			= rhs.m_fCam1Scope[0];
	m_fCam1Scope[1] 			= rhs.m_fCam1Scope[1];
	m_fLightAngle				= rhs.m_fLightAngle;
	
	c_iNumInstances++; //counting the instances
}

void CLevel::operator = (CLevel &rhs) {
	S_LevelIntern tmp;
	for (int i = 0; i < OT_LAST; i++) {
		m_alObjects[i].clear();
		for (m_lit = rhs.m_alObjects[i].begin(); m_lit != rhs.m_alObjects[i].end(); m_lit++) {
			tmp.pObj = DoubleObjPtr(m_lit->pObj);
			m_alObjects[i].push_back(tmp);
		}
	}
	m_lit 						= rhs.m_lit;
	m_TexBg 					= rhs.m_TexBg;
	m_vBgWH 					= rhs.m_vBgWH;
	m_iBGNumPasses 				= rhs.m_iBGNumPasses;
	GLC_SAFE_DELETE(m_pLight1);
	m_pLight1 					= new glcLight(*(rhs.m_pLight1));
	m_Music 					= rhs.m_Music;
	m_fLightZ					= rhs.m_fLightZ;
	m_fLightInterpolationFactor = rhs.m_fLightInterpolationFactor;
	m_fCam1Z 					= rhs.m_fCam1Z;
	m_fCam1InterpolationFactor 	= rhs.m_fCam1InterpolationFactor;
	m_fCam1Scope[0] 			= rhs.m_fCam1Scope[0];
	m_fCam1Scope[1] 			= rhs.m_fCam1Scope[1];
	m_fLightAngle				= rhs.m_fLightAngle;
}

CLevel::CLevel(string pPath)
{
	if (c_iNumInstances == 0 || glcTexture::IsTextureLoaded(string("bg/bg_level")+g_pGS->m_iLevelToPlay) == false) {
		glcTexture::LoadTexture(string("bg/bg_level")+g_pGS->m_iLevelToPlay);
	}
	m_TexBg.Load(string("bg/bg_level")+g_pGS->m_iLevelToPlay);
	
	m_pLight1 	= NULL;
	
	glcLight::LightModel(tbColor(0,0,0,1));
	
	LoadLevel(pPath);
	
	//start positions
	VCAM2 = GetPlayer()->GetCenter();
	VCAM1 = tbVector3(VCAM2.x,VCAM2.y,m_fCam1Z);
	m_pLight1->Transform(tbMatrixTranslation(tbVector3(GetPlayer()->GetCenter(),m_fLightZ) -m_pLight1->GetCenter()));
	
	//setting light angle at CGameState for CAdvise newspaper
	g_pGS->m_fLightAngle = m_pLight1->GetSpotCutoff();
	m_fLightAngle = g_pGS->m_fLightAngle;
	
	c_iNumInstances++; //counting the instances
}

CLevel::~CLevel()
{
	if (c_iNumInstances == 1) { //last instance
		glcTexture::DeleteTexture(m_TexBg.GetName());
	}
	GLC_SAFE_DELETE(m_pLight1);
	if (m_Music.IsLoaded() == true && c_iNumInstances == 1) { //deleting music, because last instance
		glcMusic::Halt(); //stop playing first
		glcMusic::DeleteMusic(m_Music.GetName());
	}
	
	list<S_LevelIntern>::iterator it1;
	for (int i= 0; i < OT_LAST; i++) {
		for (it1 = m_alObjects[i].begin(); it1 != m_alObjects[i].end(); it1++) {
			GLC_SAFE_DELETE(it1->pObj);
		}
		m_alObjects[i].clear();
	}
	
	c_iNumInstances--; //counting the instances
}

void CLevel::Update() {
	list<S_LevelIntern>::iterator it1, it2;
	try {
		int iSize = 12;
		int aiOrder[] = {OT_THOUGHT,OT_PROOF,OT_ADVICE,OT_ANSWER,OT_AXIOM,OT_UNKNOWN,OT_FALSE_BELIEF,OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_LIE,OT_IGNORANCE,OT_PLAYER};
		for (int i = 0; i < iSize; i++) {
			for (list<S_LevelIntern>::iterator it1 = m_alObjects[aiOrder[i] ].begin(); it1 != m_alObjects[aiOrder[i] ].end(); it1++) {
				it1->pObj->Update(m_alObjects);
			}
		}
	}
	catch (const glcException &ex) { // a very hard way to break the update circle with pushes and moves
		if (ex.GetErrStr().find("player") != string::npos) {
			return;
		}
		if (ex.GetErrStr().find("answer") != string::npos) {
			return;
		}
		throw;
	}
	
	//changes in causalaty of object updates?
	m_pLight1->SetSpotCutoff(g_pGS->m_fLightAngle);
	m_fLightAngle = g_pGS->m_fLightAngle;
	m_pLight1->SetSpotExponent(nmath::Min(log10(0.01)/log10(cos(TB_DEG_TO_RAD(g_pGS->m_fLightAngle))), 128.0)); //correct the exponent
	
	//other eliminations?
	for (int i = OT_LAST-1; i >= 0; i--) {
		for (it1 = m_alObjects[i].begin(); it1 != m_alObjects[i].end(); it1++) {
			if (it1->pObj->IsDeleted() == true) {;
				GLC_SAFE_DELETE(it1->pObj);  //VERY IMPORTANT
				m_alObjects[i].erase(it1--); //the POST fix decrement was here VERY important (should be used that way in every erasing list loop!!!)
			}
		}
	}
	
	//developer cheats
	if (g_pGS->m_bDeveloper == true) {
		if (g_pGL->KeyWasDown(SDLK_4)) g_pGS->m_fStdThoughtH *= 1.1f;
		if (g_pGL->KeyWasDown(SDLK_3)) g_pGS->m_fStdThoughtH /= 1.1f;
	}
	
	/*
	tbVector3 vTmp;
	tbMatrix mSca, mRot, mRot2;
	if (g_pGL->KeyIsDown(SDLK_DOWN)) 		mSca = tbMatrix(tbMatrixScaling(tbVector3(1+(SCALINGPERSECCAM*ELAPSED),1+(SCALINGPERSECCAM*ELAPSED),1+(SCALINGPERSECCAM*ELAPSED))));
	else if (g_pGL->KeyIsDown(SDLK_UP)) 	mSca = tbMatrix(tbMatrixScaling(tbVector3(1+(-SCALINGPERSECCAM*ELAPSED),1+(-SCALINGPERSECCAM*ELAPSED),1+(-SCALINGPERSECCAM*ELAPSED))));
	if (g_pGL->KeyIsDown(SDLK_LEFT)) 				mRot = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(ROTATIONPERSECCAM*ELAPSED)));
	else if (g_pGL->KeyIsDown(SDLK_RIGHT))			mRot = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(-ROTATIONPERSECCAM*ELAPSED)));
	if (g_pGL->KeyIsDown(SDLK_LCTRL))				mRot2 = tbMatrix(tbMatrixRotationAxis(tbVector3Cross(VCAM1, tbVector3(0.0f,1.0f,0.0f)),-TB_DEG_TO_RAD(ROTATIONPERSECCAM*ELAPSED)));
	else if (g_pGL->KeyIsDown(SDLK_LALT))			mRot2 = tbMatrix(tbMatrixRotationAxis(tbVector3Cross(VCAM1, tbVector3(0.0f,1.0f,0.0f)), TB_DEG_TO_RAD(ROTATIONPERSECCAM*ELAPSED)));
	if (g_pGL->KeyIsDown(SDLK_LEFT) || g_pGL->KeyIsDown(SDLK_RIGHT)) 	{VCAM1 -= VCAM2; VCAM1 = tbVector3TransformCoords(VCAM1,mRot); VCAM1 += VCAM2;} 
	if (g_pGL->KeyIsDown(SDLK_DOWN) || g_pGL->KeyIsDown(SDLK_UP)) 		{VCAM1 -= VCAM2; VCAM1 = tbVector3TransformCoords(VCAM1,mSca); VCAM1 += VCAM2;}
	if (g_pGL->KeyIsDown(SDLK_LCTRL) || g_pGL->KeyIsDown(SDLK_LALT))		{VCAM1 -= VCAM2; vTmp = VCAM1; VCAM1 = tbVector3TransformCoords(VCAM1,mRot2); if (nmath::Sign(VCAM1.x) != nmath::Sign(vTmp.x) || nmath::Sign(VCAM1.z) != nmath::Sign(vTmp.z)) VCAM1 = vTmp; VCAM1 += VCAM2;}
	
	if (glcKeyIsDown(SDLK_d)) m_pLight1->Transform(tbMatrixTranslation(tbVector3( 1,0,0) * 5.0 * g_pTimer->GetElapsed()));
	if (glcKeyIsDown(SDLK_a)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(-1,0,0) * 5.0 * g_pTimer->GetElapsed()));
	if (glcKeyIsDown(SDLK_e)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0, 1,0) * 5.0 * g_pTimer->GetElapsed()));
	if (glcKeyIsDown(SDLK_q)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0,-1,0) * 5.0 * g_pTimer->GetElapsed()));
	if (glcKeyIsDown(SDLK_w)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0,0,-1) * 5.0 * g_pTimer->GetElapsed()));
	if (glcKeyIsDown(SDLK_s)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0,0, 1) * 5.0 * g_pTimer->GetElapsed()));
	//*/
	
	//*
	//center_player
	tbVector3 vPlayerCenter(GetPlayer()->GetCenter());
	//camera update
	if (tbVector3Length(tbVector3(vPlayerCenter.x,vPlayerCenter.y,m_fCam1Z)-VCAM1) > m_fCam1Scope[0]) { //less following
		VCAM1 = tbVector3InterpolateCoords(tbVector3(vPlayerCenter.x,vPlayerCenter.y,m_fCam1Z),VCAM1,
				pow(nmath::Clamp(m_fCam1InterpolationFactor*1/nmath::Clamp((tbVector3Length(tbVector3(vPlayerCenter.x,vPlayerCenter.y,m_fCam1Z)-VCAM1)-m_fCam1Scope[0])/
				(m_fCam1Scope[1]-m_fCam1Scope[0]),0.001f,1.0f),0.0f,1.0f),ELAPSED));
	} else {} //no following
	VCAM2 = tbVector3(VCAM1.x,VCAM1.y,0.0f); //(orthografic look)
	
	//light update (full following)
	tbVector3 vNewLight = tbVector3InterpolateCoords(tbVector3(vPlayerCenter.x,vPlayerCenter.y,m_fLightZ),m_pLight1->GetCenter(),pow(m_fLightInterpolationFactor,ELAPSED));
	m_pLight1->Transform(tbMatrixTranslation(vNewLight-m_pLight1->GetCenter()));
	//*/
}

void CLevel::Render() {
	//light
	m_pLight1->Render();
	
	//good material property
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tbColor(1,1,1,1).c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tbColor(1,1,1,1).c);
	glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
	
	float fZValue = 0.000f;
	
	//bg
	fZValue = -0.001f;
	m_TexBg.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1.0f);
		poyDrawTexRect(m_iBGNumPasses,	tbVector2(0,0),tbVector3(-m_vBgWH.x/2.0f,-m_vBgWH.y/2.0f,fZValue),
						 				tbVector2(1,0),tbVector3( m_vBgWH.x/2.0f,-m_vBgWH.y/2.0f,fZValue),
						 				tbVector2(1,1),tbVector3( m_vBgWH.x/2.0f, m_vBgWH.y/2.0f,fZValue),
						 				tbVector2(0,1),tbVector3(-m_vBgWH.x/2.0f, m_vBgWH.y/2.0f,fZValue));
	glEnd();
	
	int iSize = 12;
	int aiOrder[] = {OT_PROOF,OT_ADVICE,OT_ANSWER,OT_AXIOM,OT_UNKNOWN,OT_FALSE_BELIEF,OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_LIE,OT_IGNORANCE,OT_PLAYER,OT_THOUGHT};
	for (list<S_LevelIntern>::iterator it1 = m_alObjects[OT_THOUGHT].begin(); it1 != m_alObjects[OT_THOUGHT].end(); it1++) {
		if (dynamic_cast<CThought*>(it1->pObj)->GetFade() == 0.0f) { //background text, so render first
			it1->pObj->Render();
		}
	}
	for (int i = 0; i < iSize; i++) {
		for (list<S_LevelIntern>::iterator it1 = m_alObjects[aiOrder[i] ].begin(); it1 != m_alObjects[aiOrder[i] ].end(); it1++) {
			it1->pObj->Render();
		}
	}
}

void CLevel::LoadLevel(string pPath) {
	char ch[500];
	ifstream dat_in;
	dat_in.open(pPath.data(), ios_base::in);
	if (!dat_in) {throw glcException(pPath + " fehlt");}
	
	while (!dat_in.eof()) {
		dat_in.getline (ch,498);
		if (string(ch).length() >= 3) {
			if ((string(ch).find("end")!=string::npos || string(ch).find("End")!=string::npos || string(ch).find("END")!=string::npos) && string(ch).length() <= 5) {  
				break; //the end of the file
			}
		}
		LoadObject(string(ch));
	}
	dat_in.close();
}

void CLevel::LoadObject(string ps) {
	if (ps.length() <= 0) return;	//empty line
	if (ps[0] == '/') return;		//sort of commentary
	S_LevelIntern tmpObj;
	tmpObj.pObj = NULL;
	int iType = strtoul(poyGetFirstNumberOfStringAndErase(ps).data(),NULL,10);
	float  af[40];
	int    ai[20];
	string as[20];
	switch (iType) {
		case LT_MUSIC:
			for (int i = 0; i < 1; i++) as[i] = poyGetFirstStringOfStringAndErase(ps);
			for (int i = 0; i < 1; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			if (m_Music.IsLoaded() == false) {
				if (glcMusic::IsMusicLoaded(as[0]) == false) glcMusic::LoadMusic(as[0],g_pGS->m_cfStdMusicVolume);
				m_Music.Load(as[0]);
				m_Music.SetVolume(af[0]);
				m_Music.Play(-1);
			}
			break;
		case LT_BACKGROUND:
			for (int i = 0; i < 6; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			glClearColor(af[0],af[1],af[2],1.0f);
			m_vBgWH = tbVector2(af[3],af[4]);
			m_iBGNumPasses = af[5];
			break;
		case LT_LIGHT:
			for (int i = 0; i < 16; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			m_fLightZ = af[0];
			m_fLightInterpolationFactor = af[1];
			GLC_SAFE_DELETE(m_pLight1);
			m_pLight1 = new glcLight(
				glcSphere(tbVector3(0.0,0.0,m_fLightZ), af[2]), true, true,
			 	tbColor(af[3],af[4],af[5],1.0f), tbColor(af[6],af[7],af[8],1.0f), tbColor(af[9],af[10],af[11],1.0f),
			 	af[12],af[13],af[14],tbVector3(0.0,0.0,-1.0),1.0f,af[15]);
			break;
		case LT_CAMERA:
			for (int i = 0; i < 4; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			m_fCam1Z = af[0];
			m_fCam1InterpolationFactor = af[1];
			m_fCam1Scope[0] = af[2]; m_fCam1Scope[1] = af[3];
			break;
		case OT_PLAYER:
			for (int i = 0; i < 2; i++) af[i] = strtod (poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			for (int i = 0; i < 1; i++) ai[i] = strtoul(poyGetFirstNumberOfStringAndErase(ps).data(),NULL,10);
			for (int i = 2; i < 7; i++) af[i] = strtod (poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			tmpObj.pObj = new CPlayer(tbVector2(af[0],af[1]), ai[0], af[2], tbVector2(af[3],af[4]), tbVector2(af[5],af[6]));
			break;
		case OT_THOUGHT:
			for (int i = 0; i < 2; i++)  as[i] = poyGetFirstStringOfStringAndErase(ps);
			for (int i = 0; i < 1; i++)  ai[i] = strtoul(poyGetFirstNumberOfStringAndErase(ps).data(),NULL,10);
			for (int i = 0; i < 18; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			tmpObj.pObj = new CThought(as[0],as[1],ai[0],glcMaterial(tbColor(af[0],af[1],af[2]),tbColor(af[3],af[4],af[5]),tbColor(af[6],af[7],af[8]),af[9]),
									tbVector2(af[10],af[11]),tbVector2(af[12],af[13]),tbVector2(af[14],af[15]),tbVector2(af[16],af[17]));
			dynamic_cast<CThought*>(tmpObj.pObj)->Start();
			break;
		case OT_ANSWER:
			for (int i = 0; i < 2; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(ps);
			tmpObj.pObj = new CAnswer(tbVector2(af[0],af[1]),as[0]);
			break;
		case OT_AXIOM:
			for (int i = 0; i < 2; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(ps);
			tmpObj.pObj = new CAxiom(tbVector2(af[0],af[1]),as[0]);
			break;
		case OT_UNKNOWN:
			for (int i = 0; i < 2; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(ps);
			tmpObj.pObj = new CUnknown(tbVector2(af[0],af[1]),as[0]);
			break;
		case OT_FALSE_BELIEF:
			for (int i = 0; i < 2; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(ps);
			tmpObj.pObj = new CFalseBelief(tbVector2(af[0],af[1]),as[0]);
			break;
		case OT_IGNORANCE:
			for (int i = 0; i < 2; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(ps);
			tmpObj.pObj = new CIgnorance(tbVector2(af[0],af[1]),as[0]);
			break;
		case OT_PROOF:
			for (int i = 0; i < 2; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(ps);
			tmpObj.pObj = new CProof(tbVector2(af[0],af[1]),as[0]);
			break;
		case OT_LIE:
			for (int i = 0; i < 2; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(ps);
			tmpObj.pObj = new CLie(tbVector2(af[0],af[1]),as[0]);
			break;
		case OT_ADVICE:
			for (int i = 0; i < 2; i++) af[i] = strtod (poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			for (int i = 0; i < 1; i++) ai[i] = strtoul(poyGetFirstNumberOfStringAndErase(ps).data(),NULL,10);
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(ps);
			tmpObj.pObj = new CAdvice(static_cast<EAdviceType>(ai[0]), tbVector2(af[0],af[1]),as[0]);
			break;
		case OT_RASH_ARGUMENT:
			for (int i = 0; i < 2; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(ps);
			tmpObj.pObj = new CRashArgument(tbVector2(af[0],af[1]),as[0]);
			break;
		case OT_SOLID_ARGUMENT:
			for (int i = 0; i < 2; i++) af[i] = strtod(poyGetFirstNumberOfStringAndErase(ps).data(),NULL);
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(ps);
			tmpObj.pObj = new CSolidArgument(tbVector2(af[0],af[1]),as[0]);
			break;
		default: throw glcException(string("unprogrammed object requested!: ") + iType); break;
	}
	if (tmpObj.pObj != NULL) {
		m_alObjects[tmpObj.pObj->GetOT()].push_back(tmpObj);
	}
}

CObject* CLevel::DoubleObjPtr(CObject *pObj) {
	CObject *ptr = NULL;
	switch (pObj->GetOT()) {
		case OT_PLAYER: 		ptr = new CPlayer			(*(dynamic_cast<CPlayer*		>(pObj))); break;
		case OT_AXIOM: 			ptr = new CAxiom			(*(dynamic_cast<CAxiom*			>(pObj))); break;
		case OT_SOLID_ARGUMENT: ptr = new CSolidArgument	(*(dynamic_cast<CSolidArgument*	>(pObj))); break;
		case OT_RASH_ARGUMENT: 	ptr = new CRashArgument		(*(dynamic_cast<CRashArgument*	>(pObj))); break;
		case OT_FALSE_BELIEF: 	ptr = new CFalseBelief		(*(dynamic_cast<CFalseBelief*	>(pObj))); break;
		case OT_PROOF: 			ptr = new CProof			(*(dynamic_cast<CProof*			>(pObj))); break;
		case OT_UNKNOWN: 		ptr = new CUnknown			(*(dynamic_cast<CUnknown*		>(pObj))); break;
		case OT_IGNORANCE: 		ptr = new CIgnorance		(*(dynamic_cast<CIgnorance*		>(pObj))); break;
		case OT_LIE:	 		ptr = new CLie				(*(dynamic_cast<CLie*			>(pObj))); break;
		case OT_ADVICE:	 		ptr = new CAdvice			(*(dynamic_cast<CAdvice*		>(pObj))); break;
		case OT_ANSWER: 		ptr = new CAnswer			(*(dynamic_cast<CAnswer*		>(pObj))); break;
		case OT_THOUGHT:		ptr = new CThought			(*(dynamic_cast<CThought*		>(pObj))); break;
		
		default: throw glcException(string("unprogrammed object requested to double!: ") + static_cast<int>(pObj->GetOT()) ); break;
	}
	return ptr;
}


