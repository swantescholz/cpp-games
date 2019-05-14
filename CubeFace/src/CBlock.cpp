#include "CBlock.h"

CBlock::CBlock() {
	
}
CBlock::CBlock(string sLoad)
{
	Load(sLoad);
}

CBlock::~CBlock()
{
}

void CBlock::Load(string sLoad) {
	m_Type = static_cast<EBlockType> (strtoul( (sLoad.substr(sLoad.rfind(" ") + 1) ).data(),NULL,10) );
	sLoad.erase(sLoad.rfind(" "));
	
	m_vWH.y = strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_vWH.x = strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	
	m_vPos.y = strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_vPos.x = strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	
	m_Face = static_cast<ECubeFace> (strtoul(sLoad.data(),NULL,10));
	sLoad.clear();
	
	if (m_vWH.x == 0.0f || m_vWH.y == 0.0f)
		throw glcException("one block has no area");
	
	//Texture
	string sTmp;
	switch (m_Type) {
		case BT_SOLID:	sTmp = "solid"; break;
		case BT_GOAL:	sTmp = "goal"; break;
		case BT_KILL:	sTmp = "kill"; break;
		default: break; 
	}
	m_Tex.Load(sTmp);
	
	//wrap
	switch (m_Type) {
		case BT_SOLID:	m_vWrap = tbVector2 (10.0f, 10.0f); break;
		case BT_GOAL:	m_vWrap = tbVector2 (10.0f, 10.0f); break;
		case BT_KILL:	m_vWrap = tbVector2 (10.0f, 10.0f); break;
		
		default: m_vWrap = m_vWH; break;	//wrap = false -> wrap = vWH
	}
}

void CBlock::Update() {
	switch (m_Type) {
		default: break;
		//default: m_Tex.SetMatrix(m_Tex.GetMatrix() * tbMatrixRotationZ(TB_DEG_TO_RAD((9.0f * ELAPSED)))); break;	//just a fun-test
	}
}

void CBlock::UpdateGD(EGravitationDirection lastGD) {
	//drehen
	if (g_pGS->m_GravitationDir != lastGD) {
		m_vPos -= tbVector2(CUBE_SIZE / 2.0f, CUBE_SIZE / 2.0f);
		switch (lastGD) {
			case GD_DOWN:
				switch (g_pGS->m_GravitationDir) {
					case GD_LEFT: 	m_vPos = tbVector2(m_vPos.x, m_vPos.y + m_vWH.y); 			m_vPos = tbVector2(-m_vPos.y, m_vPos.x); break;
					case GD_UP: 	m_vPos = tbVector2(m_vPos.x + m_vWH.x, m_vPos.y + m_vWH.y); m_vPos = tbVector2(-m_vPos.x,-m_vPos.y); break;
					case GD_RIGHT: 	m_vPos = tbVector2(m_vPos.x + m_vWH.x, m_vPos.y);  			m_vPos = tbVector2( m_vPos.y,-m_vPos.x); break;
					default: break;
				}
				break;
			case GD_LEFT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP: 	m_vPos = tbVector2(m_vPos.x, m_vPos.y + m_vWH.x); 			m_vPos = tbVector2(-m_vPos.y, m_vPos.x); break;
					case GD_RIGHT: 	m_vPos = tbVector2(m_vPos.x + m_vWH.y, m_vPos.y + m_vWH.x); m_vPos = tbVector2(-m_vPos.x,-m_vPos.y); break;
					case GD_DOWN: 	m_vPos = tbVector2(m_vPos.x + m_vWH.y, m_vPos.y);  			m_vPos = tbVector2( m_vPos.y,-m_vPos.x); break;
					default: break;
				}
				break;
			case GD_UP:
				switch (g_pGS->m_GravitationDir) {
					case GD_RIGHT: 	m_vPos = tbVector2(m_vPos.x, m_vPos.y + m_vWH.y); 			m_vPos = tbVector2(-m_vPos.y, m_vPos.x); break;
					case GD_DOWN: 	m_vPos = tbVector2(m_vPos.x + m_vWH.x, m_vPos.y + m_vWH.y); m_vPos = tbVector2(-m_vPos.x,-m_vPos.y); break;
					case GD_LEFT: 	m_vPos = tbVector2(m_vPos.x + m_vWH.x, m_vPos.y);  			m_vPos = tbVector2( m_vPos.y,-m_vPos.x); break;
					default: break;
				}
				break;
			case GD_RIGHT:
				switch (g_pGS->m_GravitationDir) {
					case GD_DOWN: 	m_vPos = tbVector2(m_vPos.x, m_vPos.y + m_vWH.x); 			m_vPos = tbVector2(-m_vPos.y, m_vPos.x); break;
					case GD_LEFT: 	m_vPos = tbVector2(m_vPos.x + m_vWH.y, m_vPos.y + m_vWH.x); m_vPos = tbVector2(-m_vPos.x,-m_vPos.y); break;
					case GD_UP: 	m_vPos = tbVector2(m_vPos.x + m_vWH.y, m_vPos.y);  			m_vPos = tbVector2( m_vPos.y,-m_vPos.x); break;
					default: break;
				}
				break;
			default: break;
		}
		m_vPos += tbVector2(CUBE_SIZE / 2.0f, CUBE_SIZE / 2.0f);
	}
}

void CBlock::Render() {
	//coordinatensystem in die mitte verschieben, (ist einfacher)
	m_vPos -= tbVector2(CUBE_SIZE / 2.0f, CUBE_SIZE / 2.0f);
	
	//eckenberechnung (unterschiede fuer texturing)
	tbVector2 vTmp[4];
	switch (g_pGS->m_GravitationDir) {
		case GD_DOWN:
			vTmp[0] = m_vPos;
			vTmp[1] = tbVector2 (m_vPos.x + m_vWH.x, m_vPos.y);
			vTmp[2] = m_vPos + m_vWH;
			vTmp[3] = tbVector2 (m_vPos.x, m_vPos.y + m_vWH.y);
			break;
		case GD_UP:
			vTmp[2] = m_vPos;
			vTmp[3] = tbVector2 (m_vPos.x + m_vWH.x, m_vPos.y);
			vTmp[0] = m_vPos + m_vWH;
			vTmp[1] = tbVector2 (m_vPos.x, m_vPos.y + m_vWH.y);
			break;
		case GD_LEFT:
			vTmp[3] = m_vPos;
			vTmp[0] = tbVector2 (m_vPos.x + m_vWH.y, m_vPos.y);
			vTmp[1] = tbVector2 (m_vPos.x + m_vWH.y, m_vPos.y + m_vWH.x);
			vTmp[2] = tbVector2 (m_vPos.x, m_vPos.y + m_vWH.x);
			break;
		case GD_RIGHT:
			vTmp[1] = m_vPos;
			vTmp[2] = tbVector2 (m_vPos.x + m_vWH.y, m_vPos.y);
			vTmp[3] = tbVector2 (m_vPos.x + m_vWH.y, m_vPos.y + m_vWH.x);
			vTmp[0] = tbVector2 (m_vPos.x, m_vPos.y + m_vWH.x);
			break;
	}
	
	//2d zu 3d
	tbVector3 vTmp3[4];
	for (int i = 0; i < 4; i++)
		vTmp3[i] = tbVector3(vTmp[i].x, vTmp[i].y, CUBE_SIZE / 2.0f);
	
	//noch entsprechend der gravitation drehen
	switch (g_pGS->m_GravitationDir) {
		case GD_DOWN:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationZ(TB_DEG_TO_RAD(0.0f)));
			break;
		case GD_UP:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationZ(TB_DEG_TO_RAD(180.0f)));
			break;
		case GD_LEFT:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationZ(TB_DEG_TO_RAD(-90.0f)));
			break;
		case GD_RIGHT:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationZ(TB_DEG_TO_RAD( 90.0f)));
			break;
	}
	
	//aufs richtige face drehen
	switch (m_Face) {
		case CF_FRONT: break;
		case CF_RIGHT:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationY(TB_DEG_TO_RAD(90)) );
			break;
		case CF_BACK:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationY(TB_DEG_TO_RAD(180)) );
			break;
		case CF_LEFT:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationY(TB_DEG_TO_RAD(-90)) );
			break;
		case CF_TOP:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationX(TB_DEG_TO_RAD(-90)) );
			break;
		case CF_BOTTOM:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationX(TB_DEG_TO_RAD( 90)) );
			break;
		
		default: break;
	}
	
	//texturen mit wrapping (TODO: right texture dir bei changing GD)
	m_Tex.Bind();
	glBegin (GL_QUADS);
		glTexCoord2f(vTmp[0].x / m_vWrap.x, vTmp[0].y / m_vWrap.y); glVertex3fv(vTmp3[0].c);
		glTexCoord2f(vTmp[1].x / m_vWrap.x, vTmp[1].y / m_vWrap.y); glVertex3fv(vTmp3[1].c);
		glTexCoord2f(vTmp[2].x / m_vWrap.x, vTmp[2].y / m_vWrap.y); glVertex3fv(vTmp3[2].c);
		glTexCoord2f(vTmp[3].x / m_vWrap.x, vTmp[3].y / m_vWrap.y); glVertex3fv(vTmp3[3].c);
	glEnd();
	
	//coordinatensystem zurueckverschieben
	m_vPos += tbVector2(CUBE_SIZE / 2.0f, CUBE_SIZE / 2.0f);
}

ECollisionState CBlock::Collision(ECubeFace face, tbVector2 vPos, tbVector2 vWH) {
	//collisions nach GD aufteilen
	if (face == m_Face) {
		bool bCol = false;
		switch (g_pGS->m_GravitationDir) {
			case GD_DOWN:
				if (vPos.x < m_vPos.x + m_vWH.x &&
					vPos.x + vWH.x > m_vPos.x	&&
					vPos.y < m_vPos.y + m_vWH.y &&
					vPos.y + vWH.y > m_vPos.y)		{
					bCol = true;
				}
				break;
			case GD_UP:
				if (vPos.x < m_vPos.x + m_vWH.x &&
					vPos.x + vWH.x > m_vPos.x	&&
					vPos.y < m_vPos.y + m_vWH.y &&
					vPos.y + vWH.y > m_vPos.y)		{
					bCol = true;
				}
				break;
			case GD_LEFT:
				if (vPos.x < m_vPos.x + m_vWH.y &&
					vPos.x + vWH.y > m_vPos.x	&&
					vPos.y < m_vPos.y + m_vWH.x &&
					vPos.y + vWH.x > m_vPos.y)		{
					bCol = true;
				}
				break;
			case GD_RIGHT:
				if (vPos.x < m_vPos.x + m_vWH.y &&
					vPos.x + vWH.y > m_vPos.x	&&
					vPos.y < m_vPos.y + m_vWH.x &&
					vPos.y + vWH.x > m_vPos.y)		{
					bCol = true;
				}
				break;
				
			default: break;
		}
		
		if (bCol) {
			switch (m_Type) {
				case BT_SOLID:	return CS_SOLID; break;
				case BT_GOAL:	return CS_GOAL; break;
				case BT_KILL:	return CS_KILL; break;
				
				default: break;
			}
		}
	}
	
	
	return CS_NO;
}

