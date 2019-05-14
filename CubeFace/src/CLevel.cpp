#include "CLevel.h"

CLevel::CLevel() : m_iTextSize(20), m_colText(1.0f, 0.0f, 0.0f, 1.0f), m_Wood("wood")
{
	ttfSans.Load("FreeSans", m_iTextSize);
	m_LastGD = g_pGS->m_GravitationDir;
}

CLevel::~CLevel()
{
	m_lBlock.clear();
}

void CLevel::Update() {
	UpdateBlockGD();
	for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {
		m_itl->Update();
	}
}

void CLevel::UpdateBlockGD() {
	if (m_LastGD != g_pGS->m_GravitationDir) {
		for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {
			m_itl->UpdateGD(m_LastGD);
		}
		
		m_LastGD = g_pGS->m_GravitationDir;
	}
}

void CLevel::Render() {
	
	//blocks (DIE HINTEREN ZUERST!)
	switch (g_pGS->m_PlayerFace) {
		case CF_FRONT:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_BACK  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP || m_itl->GetFace() == CF_BOTTOM || m_itl->GetFace() == CF_LEFT || m_itl->GetFace() == CF_RIGHT) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_FRONT) m_itl->Render();}
			break;
		case CF_BACK:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_FRONT  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP || m_itl->GetFace() == CF_BOTTOM || m_itl->GetFace() == CF_LEFT || m_itl->GetFace() == CF_RIGHT) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_BACK) m_itl->Render();}
			break;
		case CF_LEFT:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_RIGHT  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP || m_itl->GetFace() == CF_BOTTOM || m_itl->GetFace() == CF_FRONT || m_itl->GetFace() == CF_BACK) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_LEFT) m_itl->Render();}
			break;
		case CF_RIGHT:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_LEFT  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP || m_itl->GetFace() == CF_BOTTOM || m_itl->GetFace() == CF_FRONT || m_itl->GetFace() == CF_BACK) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_RIGHT) m_itl->Render();}
			break;
		case CF_TOP:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_BOTTOM  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_FRONT || m_itl->GetFace() == CF_BACK || m_itl->GetFace() == CF_LEFT || m_itl->GetFace() == CF_RIGHT) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP) m_itl->Render();}
			break;
		case CF_BOTTOM:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_FRONT || m_itl->GetFace() == CF_BACK || m_itl->GetFace() == CF_LEFT || m_itl->GetFace() == CF_RIGHT) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_BOTTOM) m_itl->Render();}
			break;
	}
	
	//texte
	static float fTmp = 1.0f;
	fTmp += ELAPSED;
	static float fElapsed = 0.0f;
	if ( fTmp >= 1.0f) {
		fTmp -= 1.0f;
		fElapsed = ELAPSED;
	}
	tbVector2 vPos(0,ALLH - m_iTextSize), vTmp;
	char cTmp[100];
	glcFont::Begin();
		sprintf(cTmp, "Level %i:", g_pGS->m_iLevelToPlay + 1);
		ttfSans.RenderText(cTmp, m_colText, vPos, &vTmp); vPos.y -= vTmp.y;
		ttfSans.RenderText(m_sTitle, m_colText, vPos, &vTmp); vPos.y -= vTmp.y;
		sprintf(cTmp, "FPS: %f", 1.0f / fElapsed);
		vPos = tbVector2 (0.0f, 0.0f);
		ttfSans.RenderText(cTmp, m_colText, vPos, &vTmp); vPos.y -= vTmp.y;
	glcFont::End();
}

ECollisionState CLevel::Collision(ECubeFace face, tbVector2 vPos, tbVector2 vWH) {
	//solid und goal vorrang vor kill
	for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->Collision(face, vPos, vWH) == CS_SOLID	)	return CS_SOLID	; }
	for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->Collision(face, vPos, vWH) == CS_GOAL	)	return CS_GOAL	; }
	for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->Collision(face, vPos, vWH) == CS_KILL	)	return CS_KILL	; }
	
	
	return CS_NO;
}

void CLevel::Load(string sPath) {
	g_Logfile << "Level wird geladen: " + sPath;
	
	//level einlesen
	char cText[400];
	ifstream dat_in(sPath.data(), ios_base::in);
	
	if (!dat_in)
		throw glcException(sPath + " fehlt");
	
	//titel
	dat_in.getline (cText,398);
	m_sTitle = cText;
	
	//player
	dat_in.getline (cText,398);
	m_sPlayerInit = cText;
	
	//bloecke
	string sTmp;
	CBlock tmpBlock;
	while (! dat_in.eof() ) {
		dat_in.getline (cText,398);
		sTmp = cText;
		if (static_cast<int> (sTmp.length() ) > 5) {
			tmpBlock.Load(cText);
			m_lBlock.push_back(tmpBlock);
		}
	}
	
}
