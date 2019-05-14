//main
//PhilosophyOfYourself
//a project for my philosophy class

#include "../GLC/GLC.h"
#include "CPhilosophyOfYourself.h"

int main (int argc, char **argv) {	//bis ~100000 lighted und texture-mapped flaechen fluessig
	try {
		char ch[500];
		ifstream dat_in;
		dat_in.open("config.ini", ios_base::in);
		if (!dat_in) {throw glcException("config.ini is missing");}
		dat_in.getline (ch,498);
		int winW = strtoul(ch,NULL,10);
		dat_in.getline (ch,498);
		int winH = strtoul(ch,NULL,10);
		dat_in.getline (ch,498);
		int iFull = strtoul(ch,NULL,10);
		dat_in.getline (ch,498);
		g_pGS->m_cfStdMusicVolume = strtod(ch,NULL);
		dat_in.getline (ch,498);
		g_pGS->m_cfStdSoundVolume = strtod(ch,NULL);
		dat_in.getline (ch,498);
		g_pGS->m_fStdThoughtH = strtod(ch,NULL);
		dat_in.getline (ch,498);
		if (string(ch) == "developer") {
			g_pGS->m_bDeveloper = true;
			dat_in.getline (ch,498);
			if (string(ch) != "end") {
				g_pGS->m_iBeginState = strtoul(ch,NULL,10);
			}
		}
		dat_in.close(); //end of file input
		
		if (iFull == 1) { //fullscreen
			glcInit(winW, winH, GLC_TEXTURE2D | GLC_BLEND | GLC_CULLING | GLC_FOG | GLC_LIGHTING, SDL_OPENGL | SDL_FULLSCREEN,
					"Philosophy Of Yourself | 2009 by S. Scholz", "Philosophy Of Yourself");
		} else {		  //windowed
			glcInit(winW, winH, GLC_TEXTURE2D | GLC_BLEND | GLC_CULLING | GLC_FOG | GLC_LIGHTING, SDL_OPENGL,
					"Philosophy Of Yourself | 2009 by S. Scholz", "Philosophy Of Yourself");
		}
		
		CPhilosophyOfYourself *pGame = new CPhilosophyOfYourself();
		pGame->Run();
		GLC_SAFE_DELETE(pGame);
	}
	catch (const glcException &ex) {
		g_Logfile << ex;
		ex.Out();
	}
	catch (const char *ch) {
		g_Logfile << glcException(string("error: ") + ch);
	}
	catch (...) {
		g_Logfile << glcException("unexpected error");
	}
	glcQuit();
	
	return 0;
}


