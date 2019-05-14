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
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	
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
	
	if (m_vWH.x == 0.0f || m_vWH.y == 0.0f) {
		throw glcException("one block has no area");
	}
	
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
	}
}

void CBlock::Render() {
	//coordinatensystem in die mitte verschieben, (ist einfacher)
	m_vPos -= tbVector2(CUBE_SIZE / 2.0f, CUBE_SIZE / 2.0f);
	
	//eckenberechnung (unterschiede fuer texturing)
	tbVector2 vTmp[4];
	vTmp[0] = m_vPos;
	vTmp[1] = tbVector2 (m_vPos.x + m_vWH.x, m_vPos.y);
	vTmp[2] = m_vPos + m_vWH;
	vTmp[3] = tbVector2 (m_vPos.x, m_vPos.y + m_vWH.y);
			
	
	//2d zu 3d
	tbVector3 vTmp3[4];
	for (int i = 0; i < 4; i++)
		vTmp3[i] = tbVector3(vTmp[i].x, vTmp[i].y, CUBE_SIZE / 2.0f);
	
	
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

string CBlock::GetStr() {
	string sTmp;
	char cTmp[200];
	sprintf(cTmp, "%i %f %f %f %f %i", m_Face, m_vPos.x, m_vPos.y, m_vWH.x, m_vWH.y, m_Type);
	
	sTmp = cTmp;
	
	return sTmp;
}

bool CBlock::Collision(tbVector2 vPos, tbVector2 vWH) {
	//collisions nach GD aufteilen
	if (g_pGS->m_UserFace == m_Face) {
		bool bCol = false;
		switch (g_pGS->m_DownDir) {
			case DD_DOWN:
				if (vPos.x <  m_vPos.x + m_vWH.x &&
					vPos.x + vWH.x > m_vPos.x	&&
					vPos.y <  m_vPos.y + m_vWH.y &&
					vPos.y + vWH.y > m_vPos.y)		{
					bCol = true;
				}
				break;
			case DD_UP:
				if (vPos.x <  m_vPos.x + m_vWH.x &&
					vPos.x + vWH.x > m_vPos.x	&&
					vPos.y <  m_vPos.y + m_vWH.y &&
					vPos.y + vWH.y > m_vPos.y)		{
					bCol = true;
				}
				break;
			case DD_LEFT:
				if (vPos.x <  m_vPos.x + m_vWH.y &&
					vPos.x + vWH.x > m_vPos.x	&&
					vPos.y <  m_vPos.y + m_vWH.x &&
					vPos.y + vWH.y > m_vPos.y)		{
					bCol = true;
				}
				break;
			case DD_RIGHT:
				if (vPos.x <  m_vPos.x + m_vWH.y &&
					vPos.x + vWH.x > m_vPos.x	&&
					vPos.y <  m_vPos.y + m_vWH.x &&
					vPos.y + vWH.y > m_vPos.y)		{
					bCol = true;
				}
				break;
				
			default: break;
		}
		
		if (bCol) {
			switch (m_Type) {
				case BT_SOLID:
				case BT_GOAL:
				case BT_KILL:	return true; break;
				
				default: break;
			}
		}
	}
	
	return false;
}

