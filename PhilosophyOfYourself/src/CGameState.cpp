#include "CGameState.h"

CGameState::CGameState() : m_cfOffset(0.0001f), m_cfLayerGapZ(0.001f), m_cfLetterWidth(0.5f), m_cfStdThoughtFade(0.2f)
{
	m_bDeveloper = false;
	m_iLevelToPlay = 0;
	m_bRestart = false;
	m_bRespawn = false;
	m_bSavingLevel = false;
	m_iBeginState = -2;
	m_cfStdMusicVolume = 1.0f;
	m_cfStdSoundVolume = 1.0f;
	
	S_GameStateIntern tmp;
	ifstream dat_in;
	dat_in.open("save/progress.bin", ios::binary);
	if (dat_in == NULL) {
		m_iAnswerPlace = 1;
	} else {
		dat_in.read((char*) &tmp, sizeof(tmp));
		m_iAnswerPlace = tmp.m_iAnswerPlace;
	}
	dat_in.close();
	
	dat_in.open("res/lvl/all.ini", ios_base::in);
	if (dat_in == NULL) {
		throw glcException("res/lvl/all.ini is missing");
	} else {
		char ch[500];
		dat_in.getline (ch,498);
		m_ciGameShowFPS = strtoul(ch,NULL,10);
		dat_in.getline (ch,498);
		m_ciNumLevels = strtoul(ch,NULL,10);
		m_asLevelNames = new string[m_ciNumLevels];
		for (int i = 0; i < m_ciNumLevels; i++) {
			dat_in.getline (ch,498);
			m_asLevelNames[i] = string(ch);
		}
	}
	dat_in.close();
}

CGameState::~CGameState()
{
	GLC_SAFE_DELETE_ARRAY(m_asLevelNames);
	
	S_GameStateIntern tmp;
	tmp.m_iAnswerPlace = m_iAnswerPlace;
	ofstream dat_out;
	
	dat_out.open("save/progress.bin", ios::binary);
	dat_out.write((char*) &tmp, sizeof(tmp));
	
	dat_out.close();
}

int CompareFunction(const void * a, const void * b) {
	const float tmp = ((glcParticle*)a)->GetPos().z - ((glcParticle*)b)->GetPos().z; 
	if (tmp == 0) return 0;
	return (tmp < 0.0f) ? (-1) : (1);
}

bool poyCollision2(tbVector2 pvCen1, tbVector2 pvWH1, tbVector2 pvCen2, tbVector2 pvWH2) {
	/*if ((pvCen1.x + 0.5*pvWH1.x > pvCen2.x-0.5*pvWH2.x+g_pGS->m_cfOffset && pvCen1.x-0.5*pvWH1.x+g_pGS->m_cfOffset < pvCen2.x + 0.5*pvWH2.x) &&
		  (pvCen1.y + 0.5*pvWH1.y > pvCen2.y-0.5*pvWH2.y+g_pGS->m_cfOffset && pvCen1.y-0.5*pvWH1.y+g_pGS->m_cfOffset < pvCen2.y + 0.5*pvWH2.y)) {
			  return true;
	  }
	  return false;
	*/
	if ((pvCen1.x + 0.5*pvWH1.x > pvCen2.x-0.5*pvWH2.x && pvCen1.x-0.5*pvWH1.x < pvCen2.x + 0.5*pvWH2.x) &&
		(pvCen1.y + 0.5*pvWH1.y > pvCen2.y-0.5*pvWH2.y && pvCen1.y-0.5*pvWH1.y < pvCen2.y + 0.5*pvWH2.y)) {
			return true;
	}
	return false;
}

string poyGetFirstStringOfStringAndErase(string &ps) {
	string tmp;
	size_t pos = ps.find_first_of('\"');
	if (pos == string::npos) return tmp;
	ps.erase(0,pos);
	pos = ps.find('\"',1);
	if (pos == string::npos) return tmp;
	tmp = ps.substr(1,pos-1);
	pos = ps.find('\"',1);
	if (pos == string::npos) return tmp;
	ps.erase(0,pos+1);
	while (ps.length() > 0) {
		if (ps[0] == ' ' || ps[0] == '\t') {
			ps.erase(0,1);
		} else {
			break;
		}
	}
	if (tmp.length() == 0) {g_pLogfile->Textout(RED,"string.length() == 0! in string poyGetFirstStringOfStringAndErase(string &ps)");}
	return tmp;
}

string poyGetFirstNumberOfStringAndErase(string &ps) {
	string tmp = ps.substr(0,ps.find_first_of(' '));
	ps.erase(0,ps.find_first_of(' '));
	while (ps.length() > 0) {
		if (ps[0] == ' ' || ps[0] == '\t') {
			ps.erase(0,1);
		} else {
			break;
		}
	}
	if (tmp.length() == 0) {g_pLogfile->Textout(RED,"string.length() == 0! in string poyGetFirstNumberOfStringAndErase(string &ps)");}
	return tmp;
}

tbVector2 poyAdjustPushingRectX(tbVector2 pvPosPusher, tbVector2 pvWHPusher, tbVector2 pvPosSolid, tbVector2 pvWHSolid) {
	if (pvPosPusher.x < pvPosSolid.x) {
		return tbVector2(pvPosSolid.x-pvWHSolid.x/2.0f-pvWHPusher.x/2.0f,pvPosPusher.y);
	}
	else {
		return tbVector2(pvPosSolid.x+pvWHSolid.x/2.0f+pvWHPusher.x/2.0f,pvPosPusher.y);
	}
}
tbVector2 poyAdjustPushingRectY(tbVector2 pvPosPusher, tbVector2 pvWHPusher, tbVector2 pvPosSolid, tbVector2 pvWHSolid) {
	if (pvPosPusher.y < pvPosSolid.y) {
		return tbVector2(pvPosPusher.x,pvPosSolid.y-pvWHSolid.y/2.0f-pvWHPusher.y/2.0f); //bottom push ->offset
	}
	else {
		return tbVector2(pvPosPusher.x,pvPosSolid.y+pvWHSolid.y/2.0f+pvWHPusher.y/2.0f);
	}
}

void poyDrawTexRect(int piNumPasses, tbVector2 vt1, tbVector3 v1, tbVector2 vt2, tbVector3 v2,
									 tbVector2 vt3, tbVector3 v3, tbVector2 vt4, tbVector3 v4) {
	if (piNumPasses <= 0) {
		glTexCoord2fv(vt1.c); glVertex3fv(v1.c);
		glTexCoord2fv(vt2.c); glVertex3fv(v2.c);
		glTexCoord2fv(vt3.c); glVertex3fv(v3.c);
		glTexCoord2fv(vt4.c); glVertex3fv(v4.c);
		return;
	}
	poyDrawTexRect(piNumPasses-1,vt1,v1,(vt1+vt2)/2.0f,(v1+v2)/2.0f,(vt1+vt3)/2.0f,(v1+v3)/2.0f,(vt1+vt4)/2.0f,(v1+v4)/2.0f);
	poyDrawTexRect(piNumPasses-1,vt2,v2,(vt2+vt3)/2.0f,(v2+v3)/2.0f,(vt2+vt4)/2.0f,(v2+v4)/2.0f,(vt2+vt1)/2.0f,(v2+v1)/2.0f);
	poyDrawTexRect(piNumPasses-1,vt3,v3,(vt3+vt4)/2.0f,(v3+v4)/2.0f,(vt3+vt1)/2.0f,(v3+v1)/2.0f,(vt3+vt2)/2.0f,(v3+v2)/2.0f);
	poyDrawTexRect(piNumPasses-1,vt4,v4,(vt4+vt1)/2.0f,(v4+v1)/2.0f,(vt4+vt2)/2.0f,(v4+v2)/2.0f,(vt4+vt3)/2.0f,(v4+v3)/2.0f);
}

void poyMosaic(int piNumVertex, tbVector3 *pav, tbColor *pac, int piNumPasses, float pfProb1, float pfProb2,
				float pfDist1, float pfDist2, float pfMinAverageLength, float pfMinLength, int piMaxVertex) {
	bool bpoy = false;
	if (piNumVertex < 3) {
		bpoy = true;
	}
	//checking pfMinAverageLength
	float fSum = 0.0f;
	for (int i = 0; i < piNumVertex; i++) {
		fSum += tbVector3Length(pav[(i+1<piNumVertex) ? (i+1) : (0)]-pav[i]);
	}
	if (fSum / piNumVertex < pfMinAverageLength) {
		bpoy = true;
	} else {//checking pfMinLength
		for (int i = 0; i < piNumVertex; i++) {
			if (tbVector3Length(pav[(i+1<piNumVertex) ? (i+1) : (0)]-pav[i]) < pfMinLength) {
				bpoy = true;
			}
		}
	}
	
	if (piNumPasses == 0 || bpoy == true) {
		glBegin(GL_POLYGON);
			if (glIsEnabled(GL_LIGHTING)) {
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pac[RANDOM(0,3)].c);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pac[RANDOM(0,3)].c);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
			} else {
				glColor3fv(pac[RANDOM(0,3)].c);
				//glColor3fv(tbColorRandom(1.0f).c);
			}
			for (int i = 0; i < piNumVertex; i++) {
				glVertex3fv(pav[i].c);
			}
		glEnd();
		return;
	}
	
	
	
	if (pfProb1 > 1.0f) pfProb1 = 1.0f; else if (pfProb1 < 0.0f) pfProb1 = 0.0f;
	if (pfProb2 > 1.0f) pfProb2 = 1.0f; else if (pfProb2 < 0.0f) pfProb2 = 0.0f;
	if (pfProb1 + pfProb2 > 1.0f) pfProb2 = 1.0f - pfProb1;
	
	float fTmp = RANDOM(0.0f,1.0f);
	if (fTmp <= pfProb1 && piNumVertex > 3) {	//vertex--
		int iTmp1 = RANDOM(0,piNumVertex-1);
		for (int i = 0; i < piNumVertex; i++) {	//find highest obtuse angle - or random
			if (TB_RAD_TO_DEG(tbVector3Angle(pav[(i-1>=0) ? (i-1) : (piNumVertex-1)]-pav[i],pav[(i+1<piNumVertex) ? (i+1) : (0)]-pav[i])) > 90.0f) {
				if (TB_RAD_TO_DEG(tbVector3Angle(pav[(i-1>=0) ? (i-1) : (piNumVertex-1)]-pav[i],pav[(i+1<piNumVertex) ? (i+1) : (0)]-pav[i])) >
				 	TB_RAD_TO_DEG(tbVector3Angle(pav[(iTmp1-1>=0) ? (iTmp1-1) : (piNumVertex-1)]-pav[iTmp1],
				 		pav[(iTmp1+1<piNumVertex) ? (iTmp1+1) : (0)]-pav[iTmp1]))) {
					iTmp1 = i;
				}
			}
		}
		int iTmp2 = (RANDOM(1,(piNumVertex-3)) + iTmp1 +1) % piNumVertex;
		if (iTmp1 > iTmp2) {nmath::Swap(iTmp1,iTmp2);}
		int iTmp3 = piNumVertex - (iTmp2 - iTmp1) + 1;	//num vertex1
		int iTmp4 = piNumVertex - iTmp3 + 2;			//num vertex2
		
		tbVector3 av1[iTmp3];
		for (int i = 0; i <= iTmp1; i++) {
			av1[i] = pav[i];
		}
		for (int i = iTmp2, j = iTmp1+1; i < piNumVertex; i++, j++) {
			av1[j] = pav[i];
		}
		
		tbVector3 av2[iTmp4];
		for (int i = iTmp1; i <= iTmp2; i++) {
			av2[i-iTmp1] = pav[i];
		}
		poyMosaic(iTmp3,av1,pac,piNumPasses-1,pfProb1,pfProb2,pfDist1,pfDist2,pfMinAverageLength,pfMinLength,piMaxVertex);
		poyMosaic(iTmp4,av2,pac,piNumPasses-1,pfProb1,pfProb2,pfDist1,pfDist2,pfMinAverageLength,pfMinLength,piMaxVertex);
	} else if (fTmp <= pfProb1 + pfProb2 && fTmp > pfProb1 && piNumVertex + 1 <= piMaxVertex) { //vertex++
		tbVector3 tmp1, tmp2;
		int iTmp1 = RANDOM(0,piNumVertex-1);
		int iTmp2 = (RANDOM(1,(piNumVertex-1)) + iTmp1) % piNumVertex;
		for (int i = 0; i < piNumVertex; i++) {	//find longest edge - or random
			if (tbVector3LengthSq(pav[(i+1<piNumVertex) ? (i+1) : (0)]-pav[i]) >
			 	tbVector3LengthSq(pav[(iTmp1+1<piNumVertex) ? (iTmp1+1) : (0)]-pav[iTmp1])) {
			 	iTmp2 = iTmp1;
				iTmp1 = i;
			} else if (	tbVector3LengthSq(pav[(i+1<piNumVertex) ? (i+1) : (0)]-pav[i]) >
			 			tbVector3LengthSq(pav[(iTmp2+1<piNumVertex) ? (iTmp2+1) : (0)]-pav[iTmp2])) {
			 	if (i != iTmp1) {
			 		iTmp2 = i;
			 	}
			 }
		}
		
		if (iTmp1 > iTmp2) {nmath::Swap(iTmp1,iTmp2);}
		tmp1 = tbVector3InterpolateCoords(pav[iTmp1],pav[(iTmp1+1 < piNumVertex) ? (iTmp1+1) : (0)],RANDOM(pfDist1,pfDist2));
		tmp2 = tbVector3InterpolateCoords(pav[iTmp2],pav[(iTmp2+1 < piNumVertex) ? (iTmp2+1) : (0)],RANDOM(pfDist1,pfDist2));
		int iTmp3 = piNumVertex - (iTmp2 - iTmp1) + 2;	//num vertex1
		int iTmp4 = piNumVertex - iTmp3 + 4;			//num vertex2
		
		tbVector3 av1[iTmp3];
		for (int i = 0; i <= iTmp1; i++) {
			av1[i] = pav[i];
		}
		av1[iTmp1+1] = tmp1;
		av1[iTmp1+2] = tmp2;
		for (int i = iTmp2+1, j = iTmp1+3; i < piNumVertex; i++, j++) {
			av1[j] = pav[i];
		}
		
		tbVector3 av2[iTmp4];
		av2[0] = tmp1;
		for (int i = iTmp1+1; i <= iTmp2; i++) {
			av2[i-(iTmp1+1)+1] = pav[i];
		}
		av2[iTmp4-1] = tmp2;
		poyMosaic(iTmp3,av1,pac,piNumPasses-1,pfProb1,pfProb2,pfDist1,pfDist2,pfMinAverageLength,pfMinLength,piMaxVertex);
		poyMosaic(iTmp4,av2,pac,piNumPasses-1,pfProb1,pfProb2,pfDist1,pfDist2,pfMinAverageLength,pfMinLength,piMaxVertex);
	} else {	//vertex ==
		tbVector3 tmp2;
		int iTmp1 = RANDOM(0,piNumVertex-1);
		
		for (int i = 0; i < piNumVertex; i++) {	//find highest obtuse angle - or random
			if (TB_RAD_TO_DEG(tbVector3Angle(pav[(i-1>=0) ? (i-1) : (piNumVertex-1)]-pav[i],pav[(i+1<piNumVertex) ? (i+1) : (0)]-pav[i])) > 90.0f) {
				if (TB_RAD_TO_DEG(tbVector3Angle(pav[(i-1>=0) ? (i-1) : (piNumVertex-1)]-pav[i],pav[(i+1<piNumVertex) ? (i+1) : (0)]-pav[i])) >
				 	TB_RAD_TO_DEG(tbVector3Angle(pav[(iTmp1-1>=0) ? (iTmp1-1) : (piNumVertex-1)]-pav[iTmp1],
				 		pav[(iTmp1+1<piNumVertex) ? (iTmp1+1) : (0)]-pav[iTmp1]))) {
					iTmp1 = i;
				}
			}
		}
		
		int iTmp2 = (RANDOM(1,(piNumVertex-2)) + iTmp1) % piNumVertex;
		for (int i = 0; i < piNumVertex; i++) {	//find longest edge - or random
			if (tbVector3LengthSq(pav[(i+1<piNumVertex) ? (i+1) : (0)]-pav[i]) >
			 	tbVector3LengthSq(pav[(iTmp2+1<piNumVertex) ? (iTmp2+1) : (0)]-pav[iTmp2])) {
			 		if (i != iTmp1) {
			 			if (i != 0 || iTmp1 != piNumVertex-1) {
			 				if (iTmp1 != 0 || i != piNumVertex-1) {
			 					iTmp2 = i;
			 				}
			 			}
			 		}
			 }
		}
		
		if (iTmp1 > iTmp2) {nmath::Swap(iTmp1,iTmp2);}
		if (iTmp2 == piNumVertex-1 && iTmp1 == 0) {
			iTmp1 = RANDOM(1,(piNumVertex-2));
		}
		tmp2 = tbVector3InterpolateCoords(pav[iTmp2],pav[(iTmp2+1 < piNumVertex) ? (iTmp2+1) : (0)],RANDOM(pfDist1,pfDist2));
		int iTmp3 = piNumVertex - (iTmp2 - iTmp1) +1;	//num vertex1
		int iTmp4 = piNumVertex - iTmp3 + 3;			//num vertex2
		 
		tbVector3 av1[iTmp3];
		for (int i = 0; i <= iTmp1; i++) {
			av1[i] = pav[i];
		}
		av1[iTmp1+1] = tmp2;
		for (int i = iTmp2+1, j = iTmp1+2; i < piNumVertex; i++, j++) {
			av1[j] = pav[i];
		}
		
		tbVector3 av2[iTmp4];
		for (int i = iTmp1; i <= iTmp2; i++) {
			av2[i-iTmp1] = pav[i];
		}
		av2[iTmp2-iTmp1+1] = tmp2;
		poyMosaic(iTmp3,av1,pac,piNumPasses-1,pfProb1,pfProb2,pfDist1,pfDist2,pfMinAverageLength,pfMinLength,piMaxVertex);
		poyMosaic(iTmp4,av2,pac,piNumPasses-1,pfProb1,pfProb2,pfDist1,pfDist2,pfMinAverageLength,pfMinLength,piMaxVertex);
	}
}

