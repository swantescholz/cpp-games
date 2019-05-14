#define FILL(RCT,COL){SDL_FillRect (screen, &RCT, SDL_MapRGB (screen->format, COL.r, COL.g, COL.b));}
#define COORDS(V) V.x,V.y,V.z
#define TRANSLATIONPERSEC (2500 * (static_cast<float>(options.iSpeedTranslationInPercent) / 100))
#define TRANSLATIONPERSECCAM (2000 * (static_cast<float>(options.iSpeedTranslationInPercentCam) / 100))
#define ROTATIONPERSEC (90 * (static_cast<float>(options.iSpeedRotationInPercent) / 100))
#define ROTATIONPERSECCAM (90 * (static_cast<float>(options.iSpeedRotationInPercentCam) / 100))
#define SCALINGPERSEC (1 * (static_cast<float>(options.iSpeedScalingInPercent) / 100))
#define SCALINGPERSECCAM (1 * (static_cast<float>(options.iSpeedScalingInPercentCam) / 100))
#define STANDARTFIGURES 5
#define ANZCOLORS 10
#define BETRIEBSSYSTEM_WINDOWS //Betriebssystem (WINDOWS oder LINUX) fuer die kompilation
#define ALLW 1000 //min:600, middle: 1000, max:1480
#define RANDW 200
#define ALLH 600 //min:300, middle: 600, max:960
#define TB_DEG_TO_RAD(X) ((X) * 0.0174532925199)
#define TB_RAD_TO_DEG(X) ((X) * 57.295779513082)
#define BEREICH 10000.0f
#define VCOUT(V) (cout<<V.x<< "," << V.y << "," << V.z << endl)
#define g_pPixelDraw CPixelDraw::Get()
#define CLOSED true
#define OPEN false

#include <iostream>
#include <math.h>
#include <string>
#include <time.h>
#include <list>
#include <vector>
#include <fstream>
#include <stdarg.h>

using namespace std;

#include "SDL.h"
#include "SDL_ttf.h"


#ifndef NNAMESPACE_H_
#define NNAMESPACE_H_

namespace {
	const SDL_Color colBlack = {0,0,0};
	const SDL_Color colBrown = {200,113,55};
	const SDL_Color colGrey = {128,128,128};
	const SDL_Color colWhite = {255,255,255};
	const SDL_Color colRed = {255,0,0};
	const SDL_Color colGreen = {0,255,0};
	const SDL_Color colBlue = {0,0,255};
	const SDL_Color colYellow = {255,255,0};
	const SDL_Color colOrange = {255,153,85};
	const SDL_Color colPink = {255,0,255};
	const SDL_Color colPurple = {128,0,128};
	SDL_Color allColor[11] = {colBlack,colBrown,colGrey,colWhite,colRed,colGreen,colBlue,colYellow,colOrange,colPink,colPurple};
	
	const double dPi = 3.1415926535897f;
	SDL_Color GetColor(unsigned int R,unsigned int G,unsigned int B) {SDL_Color tmp = {R,G,B}; return tmp;}
	SDL_Rect GetRect(unsigned int x,unsigned int y,unsigned int w = 0, unsigned int h = 0) {SDL_Rect tmp = {x,y,w,h}; return tmp;}
	
	template<class T> inline void Safe_Freen(T **SUR) {if(*SUR!=NULL) {SDL_FreeSurface(*SUR); *SUR = NULL;}}
	template<class T> inline void Fast_Color(T &COL, int A, int B, int C) {COL.r = A; COL.g = B; COL.b = C;}
	template<class T> inline void Fast_Rect(T &RCT, int A, int B, int C, int D) {RCT.x = A; RCT.y = B; RCT.w = C; RCT.h = D;}
	template<class T> inline T Betrag(T I) {if (I < 0.0f) I*=(-1); return I;}
	template<class T> inline T Min(T A, T B) {if(A < B) return A; return B;}
	template<class T> inline T Max(T A, T B) {if(A < B) return B; return A;}
	template<class T> inline T Nearest(T A, T B, T C) {if(Betrag(C-B) < Betrag(C-A)) return B; return A;}
	template<class T> inline int Round(T A) {if (A - static_cast<int>(A) < 0.5f) return static_cast<int>(A); return static_cast<int>(A) + 1;}
	template<class T> inline void Switch(T &B){if(B==true){B=false;}else{B=true;}}
	template<class T> inline void Change (T *Var1, T *Var2) {T Var3 = *Var1; *Var1 = *Var2; *Var2 = Var3;}
	inline int Vorzeichen(int A) {if (A < 0) return -1; return 1;}
	inline double PythagorasC(double A, double B) {double tmp; tmp = sqrt(A*A+B*B); return tmp;}
	inline double PythagorasAB(double A, double C) {double tmp; tmp = sqrt(C*C-A*A); return tmp;}
	inline double PythagorasX(double x1, double y1, double x2, double y2) {double tmp; tmp = sqrt(Betrag(x2-x1)*Betrag(x2-x1)+Betrag(y2-y1)*Betrag(y2-y1)); return tmp;}
	template<class T> inline bool Kollision(T A, T B) {if ((A).x<(B).x+(B).w&&(A).x+(A).w>(B).x&&(A).y<(B).y+(B).h&&(A).y+(A).h>(B).y) return true; return false;}   //kollisionscheck von 2 rects
	//int Zufall(int A, int B){return ((rand()%(B-A))+A);}
	bool bLeftOfLine(SDL_Rect rct1, SDL_Rect rct2, SDL_Rect rct3) {//1: punkt, 2: liniebeginn, 3: linieende
		rct3.y-=rct2.y; rct3.x-=rct2.x; rct1.x -= rct2.x; rct1.y -= rct2.y;
		if ((-2*rct3.x) != 0 && rct3.x != 0) {
			if (rct3.x > 0) {
				if ((rct1.y) < (static_cast<float>(rct3.y) / static_cast<float>(rct3.x) * static_cast<float>(rct1.x))) return true;
				else return false;
			} else {
				if ((rct1.y) > (static_cast<float>(rct3.y) / static_cast<float>(rct3.x) * static_cast<float>(rct1.x))) return true;
				else return false;
			}
		} else if (rct3.y == 0) {
			if (rct1.y < 0) return true;	
		} else if (rct3.x == 0) {
			if (rct1.x > 0) return true;	
		}
		return false; 
	}
}

#endif /*NNAMESPACE_H_*/

#ifndef CTEXTBOX_H_
#define CTEXTBOX_H_

//TextBox: eine klasse zum (anzeigen) von textnachrichten, abfragen von strings/ints und zum speichern/laden
//todo: alt, \ eingabe



#define BLA cout<<"bla"<<endl;
enum{TEXTBOX_WINDOWS, TEXTBOX_LINUX};



class CTextBox
{
public:
	CTextBox();
	~CTextBox();
	//CTextBox(const CTextBox &rhs);
	
	void init(SDL_Surface **sur, SDL_Rect rct, int IFONT1, int IFONT2, int IFONT3, SDL_Color COL, int iMax = 2000);
	void quit();
	
	void open() {bOpen = true;bFlip = true;SDL_FillRect (*screen, &rctBox, SDL_MapRGB ((*screen)->format, Color[COL_BACK].r, Color[COL_BACK].g, Color[COL_BACK].b));} //fenster oeffnen
	void close() {bOpen = false;bFlip = true;SDL_FillRect (*screen, &rctBox, SDL_MapRGB ((*screen)->format, Color[COL_BLACK].r, Color[COL_BLACK].g, Color[COL_BLACK].b));} //fenster schliessen
	
	void load(const char *chPfad, bool bAppend = false, bool bNormalText = false, SDL_Color col = colBlack); //laden
	void save(const char *chPfad); //speichern
	
	void up(int i = 1);
	void down(int i = 1);
	
	void setBetriebssystem(unsigned int i) {iBetriebssystem = i%2;}
	
	void write(const char *chText, SDL_Color COL = colBlack, bool bFrage = false, bool bEingabe = false); // textausgabe
	
	int getInt(const char *chFrage = "Zahl eingeben: ", SDL_Color COL = colRed); //integer eingabe
	int getStr(string *chText, const char *chFrage = "Text eingeben ", SDL_Color COL = colRed); //char abfrage
	
	void clear() {lText.clear();} //vektor leeren
	void erase();
	int size() {return static_cast<int> (lText.size());}
	
	void update(bool bRendern = true);
	
	void operator += (CTextBox &rhs);
	void operator = (CTextBox &rhs);
	CTextBox operator + (CTextBox &rhs);
	
	bool bOpen, bFlip;
	int iZeilen;// anzahl max im bild
private:

	void eingabe(int typ, int *pZahl, string *sText);
	void render();
	void eingabenSwitch(int iDirection, string *sText, bool *bBeimSwitchen = NULL, int *piStelle = NULL);
	int countStr(string str) {if(static_cast<int>(str.length()) == 0) return 0;list<S_Text>::iterator itS_Text2; int i=0; for(itS_Text2 = lText.begin(); itS_Text2 != lText.end(); ++itS_Text2) {if((*itS_Text2).sText[OHNE_DAT] == str) i++;} return i;} 
	
	SDL_Surface **screen;
	SDL_Surface *ttfsurText;
	SDL_Color Color[3]; //hintergrund
	enum{MIT_DAT = 0, OHNE_DAT};
	enum{UP = 0, DOWN, LEFT, RIGHT};
	enum{COL_BACK = 0, COL_BLACK, COL_WHITE};
	
	SDL_Rect rctBox;//bereich
	SDL_Event event;
	TTF_Font *pFont[3];
	
	struct S_Text {
		string sText[2]; //0 mit, 1 ohne datum
		bool bEingabe;//string eingabe gewesen
		SDL_Rect rctText;
		SDL_Color colText;
		int sizeArt;
	} sText2;
	list<S_Text> lText; // hauptvektor
	list<S_Text>::iterator itS_Text; //iterator
	
	int iFont[4];//nur die groessen
	int iTextMax;//max nachrichten
	int iBetriebssystem; // 0: windows; 1: linux
	
	enum{FONT_NORMAL = 0, FONT_NEW, FONT_ASK, FONT_MAX};
	
	time_t zeit;//zeit
};

#endif /*CTEXTBOX_H_*/


CTextBox::CTextBox():
bOpen (false), bFlip(false), screen (NULL), ttfsurText (NULL), iBetriebssystem(TEXTBOX_WINDOWS) {
	Fast_Color(Color[COL_WHITE],255,255,255);
	Fast_Color(Color[COL_BLACK],0,0,0);
}

CTextBox::~CTextBox() {
}

/*CTextBox::CTextBox(const CTextBox &rhs):
bOpen (rhs.bOpen), bFlip(rhs.bFlip), screen (rhs.screen), ttfsurText (rhs.ttfsurText) {
	Fast_Color(Color[COL_WHITE],255,255,255);
	Fast_Color(Color[COL_BLACK],0,0,0);
}*/

void CTextBox::init(SDL_Surface **sur, SDL_Rect rct, int IFONT1, int IFONT2, int IFONT3, SDL_Color COL, int iMax) {
	screen = sur;
	rctBox = rct;
	iFont[FONT_NORMAL]=IFONT1; iFont[FONT_NEW]=IFONT2; iFont[FONT_ASK]=IFONT3;
	iFont[FONT_MAX] = Max((Max(iFont[FONT_NORMAL], iFont[FONT_ASK])), iFont[FONT_NEW]);
	/*schriftart*/
	for (int i = 0; i < 3; i++) {pFont[i] = NULL; pFont[i] = TTF_OpenFont ("media/FreeSans.ttf", iFont[i]);}
	/*hintergrundfarbe*/
	Fast_Color(Color[COL_BACK],COL.r,COL.g,COL.b);
	iZeilen = rctBox.h/iFont[FONT_MAX];
	iTextMax = iMax;
}

void CTextBox::quit() {
	iFont[FONT_NORMAL]=0; iFont[FONT_NEW]=0; iFont[FONT_ASK]=0;
	clear();
}

void CTextBox::load(const char *chPfad, bool bAppend, bool bNormalText, SDL_Color col) {
	if (bAppend == false) clear();//liste leeren
	
	ifstream dat_ein;
	dat_ein.open (chPfad, ios_base::in);
	
	char chZeichen[1000];
	
	if (bNormalText == false) { // eigen abgespeichertes file lesen
		while (! dat_ein.eof() ) {
			dat_ein.getline (chZeichen,398,'#');
			
	  	  	for (int i = 1; i < 10; i++) {
	  	  		if (i != 9) dat_ein.getline (chZeichen,398,',');
	  	  		else dat_ein.getline (chZeichen,398,'#');
	  	  			
	  	  		if (i == 1) sText2.sText[MIT_DAT] = chZeichen;
	  	  		else if (i == 2) sText2.sizeArt =  strtoul(chZeichen,NULL,10);
	  	  		else if (i == 3) sText2.colText.r =  strtoul(chZeichen,NULL,10);
	  	  		else if (i == 4) sText2.colText.g =  strtoul(chZeichen,NULL,10);
	  	  		else if (i == 5) sText2.colText.b =  strtoul(chZeichen,NULL,10);
	  	  		else if (i == 6) sText2.rctText.x =  strtoul(chZeichen,NULL,10);
	  	  		else if (i == 7) sText2.rctText.y =  strtoul(chZeichen,NULL,10);
	  	  		else if (i == 8) sText2.rctText.w =  strtoul(chZeichen,NULL,10);
	  	  		else if (i == 9) sText2.rctText.h =  strtoul(chZeichen,NULL,10);
	  	  	}
	  	  	lText.push_back(sText2);
		}
	} else { //normales textfile lesen
		while (!dat_ein.eof()) {
			dat_ein.getline (chZeichen,998);
			write(chZeichen,col);
		}
	}
	lText.pop_back(); //lesefehler korrigieren
	
	dat_ein.close();
	
	up(); //damit ein absatz da ist
}

void CTextBox::save(const char *chPfad) {
	ofstream dat_aus;
	dat_aus.open(chPfad, ios_base::out | ios_base::trunc);
	
	int i = 0;
	
	for (itS_Text = lText.begin(); itS_Text!=lText.end(); itS_Text++) {
		i++;
		dat_aus << "#" << (*itS_Text).sText[MIT_DAT] << "," << (*itS_Text).sizeArt << "," << static_cast<int> ((*itS_Text).colText.r) << ",";
		dat_aus << static_cast<int> ((*itS_Text).colText.g) << "," << static_cast<int> ((*itS_Text).colText.b) << "," << (*itS_Text).rctText.x << "," << (*itS_Text).rctText.y << ",";
		dat_aus << (*itS_Text).rctText.w << "," << (*itS_Text).rctText.h << "\t#";
		if (i!=static_cast<int> (lText.size())) dat_aus << "\n";
	}
	
	dat_aus.close(); 
}

void CTextBox::write(const char *chText, SDL_Color COL, bool bFrage, bool bEingabe) {
	//Datum
	struct tm *LokalZeit;
	time (&zeit);
	LokalZeit = localtime(&zeit);
	char ch[1200];
	sprintf(ch,"%s   %i-%i-%i_%i-%i-%i", chText, LokalZeit->tm_hour,LokalZeit->tm_min, LokalZeit->tm_sec,
	 LokalZeit->tm_mday, LokalZeit->tm_mon+1, LokalZeit->tm_year+1900);//alle daten manuell gemacht, weil beim standart ein zeilenumbruch kommt den man nicht abspeichern kann
	sText2.sText[MIT_DAT] = ch; sText2.sText[OHNE_DAT] = chText;
	if (bFrage == false) sText2.sizeArt = FONT_NEW; else sText2.sizeArt = FONT_ASK;
	Fast_Color(sText2.colText,COL.r,COL.g,COL.b);
	Fast_Rect(sText2.rctText,rctBox.x,rctBox.h-iFont[FONT_MAX],rctBox.w,iFont[FONT_NEW]);
	sText2.bEingabe = bEingabe;
	for(itS_Text = lText.begin(); itS_Text != lText.end(); ++itS_Text) {
		(*itS_Text).rctText.y -= iFont[FONT_MAX];
		if ((*itS_Text).sizeArt == FONT_NEW) (*itS_Text).sizeArt = FONT_NORMAL;
	}
	lText.push_back(sText2);
	update(bOpen);
}

int CTextBox::getInt(const char *chFrage, SDL_Color COL) {
	if (bOpen == true) {
		int i = 0;
		write(chFrage, COL, true);
		up();
		eingabe(1,&i,NULL);
		return i;
	}
	return -1;
}

int CTextBox::getStr(string *sText, const char *chFrage, SDL_Color COL) {
	if (bOpen == true) {
		write(chFrage, COL, true);
		up();
		eingabe(2, NULL, sText);
		
		return 0;
	}
	return -1;
}

void CTextBox::eingabe(int typ, int *pZahl, string *sText) {
	SDL_Rect rctText = {rctBox.x,rctBox.y+rctBox.h-iFont[FONT_MAX],rctBox.w,iFont[FONT_NEW]};
	bool bFertig = false, bTaste = false, bShift = false, bBeimSwitchen = false, bAlt = false;
	string sLastText;
	char chText[400];
	int iStelle = 1;//wievieltes gleiches wort es ist
	
	if(typ == 1) {*pZahl = 0;}
	if(typ == 2) {}
	
	while (bFertig == false) {
		while(SDL_PollEvent(&event)) {//EingabeAbfrage
			switch(event.type){
				case SDL_KEYDOWN: {//Taste unten?
					bTaste = true;
					if (typ == 1)
						switch(event.key.keysym.sym){
							case SDLK_1: *pZahl *= 10; *pZahl += 1; break;
							case SDLK_2: *pZahl *= 10; *pZahl += 2; break;
							case SDLK_3: *pZahl *= 10; *pZahl += 3; break;
							case SDLK_4: *pZahl *= 10; *pZahl += 4; break;
							case SDLK_5: *pZahl *= 10; *pZahl += 5; break;
							case SDLK_6: *pZahl *= 10; *pZahl += 6; break;
							case SDLK_7: *pZahl *= 10; *pZahl += 7; break;
							case SDLK_8: *pZahl *= 10; *pZahl += 8; break;
							case SDLK_9: *pZahl *= 10; *pZahl += 9; break;
							case SDLK_0: *pZahl *= 10; *pZahl += 0; break;
							case SDLK_BACKSPACE:  *pZahl/=10; 		break;
							case SDLK_RETURN: 				bFertig = true;		break;
							case SDLK_ESCAPE: *pZahl = 0; 	bFertig = true; 	break;
							default: {bTaste = false;} break;
						}
					else if (typ == 2)
						switch(event.key.keysym.sym){
							case SDLK_1: if(bShift== true)sLastText += "!";else if(bAlt == true)sLastText += "¹"; else sLastText += "1";bBeimSwitchen = false;break;
							case SDLK_2: if(bShift== true)sLastText += "\"";else if(bAlt == true)sLastText += "²"; else sLastText += "2";bBeimSwitchen = false;break;
							case SDLK_3: if(bShift== true)sLastText += "§";else if(bAlt == true)sLastText += "³"; else sLastText += "3";bBeimSwitchen = false;break;
							case SDLK_4: if(bShift== true)sLastText += "$";else if(bAlt == true)sLastText += "¼"; else sLastText += "4";bBeimSwitchen = false;break;
							case SDLK_5: if(bShift== true)sLastText += "%";else if(bAlt == true)sLastText += "½"; else sLastText += "5";bBeimSwitchen = false;break;
							case SDLK_6: if(bShift== true)sLastText += "&";else if(bAlt == true)sLastText += "¬"; else sLastText += "6";bBeimSwitchen = false;break;
							case SDLK_7: if(bShift== true)sLastText += "/";else if(bAlt == true)sLastText += "{"; else sLastText += "7";bBeimSwitchen = false;break;
							case SDLK_8: if(bShift== true)sLastText += "(";else if(bAlt == true)sLastText += "["; else sLastText += "8";bBeimSwitchen = false;break;
							case SDLK_9: if(bShift== true)sLastText += ")";else if(bAlt == true)sLastText += "]"; else sLastText += "9";bBeimSwitchen = false;break;
							case SDLK_0: if(bShift== true)sLastText += "=";else if(bAlt == true)sLastText += "}"; else sLastText += "0";bBeimSwitchen = false;break;
							
							case SDLK_a: if(bShift== false)sLastText += "a";else sLastText+="A"; bBeimSwitchen = false; break;
							case SDLK_b: if(bShift== false)sLastText += "b";else sLastText+="B"; bBeimSwitchen = false; break;
							case SDLK_c: if(bShift== false)sLastText += "c";else sLastText+="C"; bBeimSwitchen = false; break;
							case SDLK_d: if(bShift== false)sLastText += "d";else sLastText+="D"; bBeimSwitchen = false; break;
							case SDLK_e: if(bShift== false)sLastText += "e";else sLastText+="E"; bBeimSwitchen = false; break;
							case SDLK_f: if(bShift== false)sLastText += "f";else sLastText+="F"; bBeimSwitchen = false; break;
							case SDLK_g: if(bShift== false)sLastText += "g";else sLastText+="G"; bBeimSwitchen = false; break;
							case SDLK_h: if(bShift== false)sLastText += "h";else sLastText+="H"; bBeimSwitchen = false; break;
							case SDLK_i: if(bShift== false)sLastText += "i";else sLastText+="I"; bBeimSwitchen = false; break;
							case SDLK_j: if(bShift== false)sLastText += "j";else sLastText+="J"; bBeimSwitchen = false; break;
							case SDLK_k: if(bShift== false)sLastText += "k";else sLastText+="K"; bBeimSwitchen = false; break;
							case SDLK_l: if(bShift== false)sLastText += "l";else sLastText+="L"; bBeimSwitchen = false; break;
							case SDLK_m: if(bShift== false)sLastText += "m";else sLastText+="M"; bBeimSwitchen = false; break;
							case SDLK_n: if(bShift== false)sLastText += "n";else sLastText+="N"; bBeimSwitchen = false; break;
							case SDLK_o: if(bShift== false)sLastText += "o";else sLastText+="O"; bBeimSwitchen = false; break;
							case SDLK_p: if(bShift== false)sLastText += "p";else sLastText+="P"; bBeimSwitchen = false; break;
							case SDLK_q: if(bShift== false)sLastText += "q";else sLastText+="Q"; bBeimSwitchen = false; break;
							case SDLK_r: if(bShift== false)sLastText += "r";else sLastText+="R"; bBeimSwitchen = false; break;
							case SDLK_s: if(bShift== false)sLastText += "s";else sLastText+="S"; bBeimSwitchen = false; break;
							case SDLK_t: if(bShift== false)sLastText += "t";else sLastText+="T"; bBeimSwitchen = false; break;
							case SDLK_u: if(bShift== false)sLastText += "u";else sLastText+="U"; bBeimSwitchen = false; break;
							case SDLK_v: if(bShift== false)sLastText += "v";else sLastText+="V"; bBeimSwitchen = false; break;
							case SDLK_w: if(bShift== false)sLastText += "w";else sLastText+="W"; bBeimSwitchen = false; break;
							case SDLK_x: if(bShift== false)sLastText += "x";else sLastText+="X"; bBeimSwitchen = false; break;
							
							case SDLK_PERIOD: 	if(bShift== false)sLastText += ".";else sLastText += ":";bBeimSwitchen = false;break;
							case SDLK_COMMA: 	if(bShift== false)sLastText += ",";else sLastText += ";";bBeimSwitchen = false;break;
							case SDLK_PLUS: 	if(bShift== false)sLastText += "+";else sLastText += "*";bBeimSwitchen = false;break;
							case SDLK_MINUS: 	if(bShift== false)sLastText += "-";else sLastText += "_";bBeimSwitchen = false;break;
							
							case SDLK_LCTRL: sLastText += "\\"; break;
							
							case SDLK_LSHIFT:
							case SDLK_RSHIFT:	bShift = true; 	break;
							case SDLK_LALT:
							case SDLK_RALT:		bAlt = true;	break;
							case SDLK_BACKSPACE: sLastText.erase(static_cast<int> (sLastText.length() ) -1); bBeimSwitchen = false; break;
							case SDLK_RETURN: bFertig = true; break;
							case SDLK_ESCAPE: sLastText = "***Escape_Abbruch***"; bFertig = true; break; 
							case SDLK_UP: eingabenSwitch(UP,&sLastText,&bBeimSwitchen,&iStelle); break;
							case SDLK_DOWN: eingabenSwitch(DOWN,&sLastText,&bBeimSwitchen,&iStelle); break;
							case SDLK_LEFT: eingabenSwitch(LEFT,&sLastText,&bBeimSwitchen,&iStelle); break;
							case SDLK_RIGHT: eingabenSwitch(RIGHT,&sLastText,&bBeimSwitchen,&iStelle); break;
							default: {} break;
						}
						//betriebssystems-spezifizierungen
						if (iBetriebssystem == TEXTBOX_WINDOWS) {
							switch(event.key.keysym.sym){
								case SDLK_y: if(bShift== false)sLastText += "z";else sLastText+="Z"; bBeimSwitchen = false; break;//wegen amerikanischer tastatur bei SDL-Windows vertauscht
								case SDLK_z: if(bShift== false)sLastText += "y";else sLastText+="Y"; bBeimSwitchen = false; break;
								default: {} break;
							}
								
						} else if (iBetriebssystem == TEXTBOX_LINUX) {
							switch(event.key.keysym.sym){
								case SDLK_y: if(bShift== false)sLastText += "y";else sLastText+="Y"; bBeimSwitchen = false; break;
								case SDLK_z: if(bShift== false)sLastText += "z";else sLastText+="Z"; bBeimSwitchen = false; break;
								default: {} break;
							}
						}
				} break;
				
				case SDL_KEYUP: {
					if (typ == 1) {}
					if (typ == 2)
						switch(event.key.keysym.sym){
							case SDLK_LSHIFT: case SDLK_RSHIFT: bShift = false; break;
							case SDLK_LALT: case SDLK_RALT: bAlt = false; break;
							default: {} break;
						}
				} break;
				
				case SDL_QUIT: {
					if (typ == 1) *pZahl = 0;
					else if (typ == 2) sLastText = "***Escape_Abbruch***";
					bFertig = true;
				} break;
				
				default: {} break;
			}
		}
		if (bTaste == true) {
			bTaste = false;
			SDL_FillRect (*screen, &rctText, SDL_MapRGB ((*screen)->format, Color[COL_BACK].r, Color[COL_BACK].g, Color[COL_BACK].b));
			
			Safe_Freen(&ttfsurText);//alte surface freen
			
			if (typ == 1) {
				sprintf(chText, "%i", *pZahl);
				ttfsurText = TTF_RenderUTF8_Blended(pFont[FONT_NEW], chText, Color[COL_BLACK]);
				SDL_BlitSurface (ttfsurText, NULL, *screen, &rctText);
				update(false);
			}
			if (typ == 2) {
				ttfsurText = TTF_RenderUTF8_Blended(pFont[FONT_NEW], sLastText.data(), Color[COL_BLACK]);
				SDL_BlitSurface (ttfsurText, NULL, *screen, &rctText);
				update(false);
			}
		}
	}
	down();//damit die hoehe passt
	if (typ == 1) {
		write(chText, colBrown);
	}
	if (typ == 2) {
		if(static_cast<int>(sLastText.length()) == 0) sLastText = "-";//keine eingabe - zeichen
		sprintf(chText,"%s",sLastText.data());
		write(chText, colBrown, false, true);
		*sText = sLastText;
	}
}

void CTextBox::update(bool bRender) {
	bFlip = true;
	//anzahl der nachrichten auf 150 begrenzen
	if( static_cast<int> (lText.size()) > iTextMax) for (int i = static_cast<int> (lText.size()) % iTextMax; i > 0; i--) lText.pop_front();
	if (bOpen) {
		if (bRender) render();
		SDL_UpdateRect(*screen, rctBox.x, rctBox.y, rctBox.w, rctBox.h);//textbox updaten
	}
}

void CTextBox::render() {
	SDL_FillRect (*screen, &rctBox, SDL_MapRGB ((*screen)->format, Color[COL_BACK].r, Color[COL_BACK].g, Color[COL_BACK].b));
	for(itS_Text = lText.begin(); itS_Text != lText.end(); ++itS_Text) {
		if ((*itS_Text).rctText.y >= rctBox.y && (*itS_Text).rctText.y < rctBox.y + rctBox.h && (*itS_Text).rctText.x >= rctBox.x && (*itS_Text).rctText.x < rctBox.x + rctBox.w) {
			Safe_Freen(&ttfsurText);//alte surface freen
			ttfsurText = TTF_RenderUTF8_Blended(pFont[(*itS_Text).sizeArt], (*itS_Text).sText[OHNE_DAT].data(), (*itS_Text).colText);
			SDL_BlitSurface (ttfsurText, NULL, *screen, &(*itS_Text).rctText);
		}
	}
}

void CTextBox::up(int i) {
	for(itS_Text = lText.begin(); itS_Text != lText.end(); ++itS_Text) {
		(*itS_Text).rctText.y -= iFont[FONT_MAX] * i;
		if ((*itS_Text).sizeArt == FONT_NEW) (*itS_Text).sizeArt = FONT_NORMAL;
		if ((*itS_Text).rctText.y == rctBox.y+rctBox.h- iFont[FONT_MAX]) (*itS_Text).sizeArt = FONT_NEW;
	}
	update();
}

void CTextBox::down(int i) {
	for(itS_Text = lText.begin(); itS_Text != lText.end(); ++itS_Text) {
		(*itS_Text).rctText.y += iFont[FONT_MAX] * i;
		if ((*itS_Text).sizeArt == FONT_NEW) (*itS_Text).sizeArt = FONT_NORMAL;
		if ((*itS_Text).rctText.y == rctBox.y+rctBox.h- iFont[FONT_MAX]) (*itS_Text).sizeArt = FONT_NEW;
	}
	update();
}

void CTextBox::eingabenSwitch(int iDirection, string *sText, bool *bBeimSwitchen, int *piStelle/*wievieltes gleiches es ist*/) {
	string sText2;
	bool bGefunden = false;
	int i = 1, j = *piStelle, k = countStr(*sText);
	list<S_Text>::iterator itS_Text2; //iterator2 zum vergleichen
	
	if (iDirection == UP) {
		if (static_cast<int>((*sText).length()) > 0 && *bBeimSwitchen == true) {
			for(itS_Text = lText.end(); itS_Text != lText.begin() && bGefunden == false; --itS_Text) {
				if (itS_Text == lText.end()) itS_Text--;
				if ((*itS_Text).sText[OHNE_DAT] == (*sText)) {
					if(j==k) {
						while (itS_Text != lText.begin() && bGefunden == false) {
							--itS_Text;
							if ((*itS_Text).bEingabe == true) {
								bGefunden = true;
								*sText = (*itS_Text).sText[OHNE_DAT];
								itS_Text2 = itS_Text;
								for(itS_Text = lText.begin(); itS_Text != lText.end() && (*itS_Text).sText[MIT_DAT] != (*itS_Text2).sText[MIT_DAT]; ++itS_Text) {
									if ((*itS_Text).sText[OHNE_DAT] == *sText) 
										i++; // stelle dieses wortes zaehlen
								}
								*piStelle = i;
							}
						}
						bGefunden = true;
					}
					j++;
				}
			}
		}
		if (bGefunden == false) {
			for(itS_Text = lText.end(); itS_Text != lText.begin() && bGefunden == false; --itS_Text) {
				if (itS_Text == lText.end()) itS_Text--;
				if ((*itS_Text).bEingabe == true) {
					bGefunden = true;
					*sText = (*itS_Text).sText[OHNE_DAT];
					itS_Text2 = itS_Text;
					for(itS_Text = lText.begin(); itS_Text != lText.end() && (*itS_Text).sText[MIT_DAT] != (*itS_Text2).sText[MIT_DAT]; ++itS_Text) {
						if ((*itS_Text).sText[OHNE_DAT] == *sText) 
							i++; // stelle dieses wortes zaehlen
					}
					*piStelle = i;
				}
			}
		}
		*bBeimSwitchen = true;
	} else if (iDirection == DOWN) {
		if (static_cast<int>((*sText).length()) > 0 && *bBeimSwitchen == true) {
			for(itS_Text = lText.begin(); itS_Text != lText.end() && bGefunden == false; ++itS_Text) {
				if ((*itS_Text).sText[OHNE_DAT] == *sText) {
					j--;
					if(j==0) {
						++itS_Text;
						while (itS_Text != lText.end() && bGefunden == false) {
							if ((*itS_Text).bEingabe == true) {
								bGefunden = true;
								*sText = (*itS_Text).sText[OHNE_DAT];
								itS_Text2 = itS_Text;
								for(itS_Text = lText.begin(); itS_Text != lText.end() && (*itS_Text).sText[MIT_DAT] != (*itS_Text2).sText[MIT_DAT]; ++itS_Text)
									if ((*itS_Text).sText[OHNE_DAT] == *sText) i++; // stelle dieses wortes zaehlen
								*piStelle = i;
							}
							++itS_Text;
						}
						if (bGefunden == false) {
							*sText = sText2;
							*piStelle = 1;
						}
						bGefunden = true;
					}
				}
			}
		}
	} else if (iDirection == LEFT) {
		for(itS_Text = lText.begin(); itS_Text != lText.end() && bGefunden == false; ++itS_Text)
			if ((*itS_Text).bEingabe == true) {
				bGefunden = true;
				*sText = (*itS_Text).sText[OHNE_DAT];
				*bBeimSwitchen = true;
				*piStelle = 1;
			} 
	} else if (iDirection == RIGHT) {
		*sText = sText2;
		*piStelle = 1;
	}
}

void CTextBox::erase() {
	for(itS_Text = lText.begin(); itS_Text != lText.end(); ++itS_Text)
		if ((*itS_Text).sizeArt == FONT_NEW) lText.erase(itS_Text);
}

void CTextBox::operator +=(CTextBox &rhs) {
	for(rhs.itS_Text = rhs.lText.begin(); rhs.itS_Text != rhs.lText.end(); ++rhs.itS_Text) {
		write((*itS_Text).sText[OHNE_DAT].data(),(*itS_Text).colText,((*itS_Text).sizeArt == FONT_ASK ? true : false),(*itS_Text).bEingabe);	
	}
}

void CTextBox::operator =(CTextBox &rhs) {
	lText.clear();
	*this += rhs;
}

CTextBox CTextBox::operator +(CTextBox &rhs) {
	CTextBox tmp;
	
	tmp = *this;
	tmp += rhs;
	
	return (tmp);
}

#ifndef CTBVECTOR_H_
#define CTBVECTOR_H_

/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVector3.h
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Rechnen mit 3D-Vektoren

	Autor:
	David Scherfgen

********************************************************************/




// Verwendete Symbole vordefinieren
class tbMatrix;

// ******************************************************************
// Die 3D-Vektor-Klasse
class tbVector3
{
public:
	// Variablen
	union
	{
		struct
		{
			float x;	// Koordinaten
			float y;
			float z;
		};

		struct
		{
			float u;	// Texturkoordinaten
			float v;
			float w;
		};

		float		c[3];		// Array der Koordinaten
		//D3DVECTOR	vD3DVector;	// Vektor in Form eines Direct3D-Vektors
	};

	// Konstruktoren
	tbVector3()																						{}
	tbVector3(const tbVector3& v) : x(v.x), y(v.y), z(v.z)											{}
	tbVector3(const float f) : x(f), y(f), z(f)														{}
	tbVector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)					{}
	tbVector3(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2])	{}
	//tbVector3(const D3DVECTOR& v) : vD3DVector(v)													{}

	// Casting-Operatoren
	operator float* ()					{return (float*)(c);}
	//operator D3DVECTOR& ()				{return vD3DVector;}
	//operator const D3DVECTOR& () const	{return vD3DVector;}

	// Zuweisungsoperatoren
	tbVector3& operator = (const tbVector3& v)	{x = v.x; y = v.y; z = v.z; return *this;}
	tbVector3& operator += (const tbVector3& v)	{x += v.x; y += v.y; z += v.z; return *this;}
	tbVector3& operator -= (const tbVector3& v)	{x -= v.x; y -= v.y; z -= v.z; return *this;}
	tbVector3& operator *= (const tbVector3& v)	{x *= v.x; y *= v.y; z *= v.z; return *this;}
	tbVector3& operator *= (const float f)		{x *= f; y *= f; z *= f; return *this;}
	tbVector3& operator /= (const tbVector3& v)	{x /= v.x; y /= v.y; z /= v.z; return *this;}
	tbVector3& operator /= (const float f)		{x /= f; y /= f; z /= f; return *this;}
};

// Arithmetische Operatoren
inline tbVector3 operator + (const tbVector3& a, const tbVector3& b)	{return tbVector3(a.x + b.x, a.y + b.y, a.z + b.z);}
inline tbVector3 operator - (const tbVector3& a, const tbVector3& b)	{return tbVector3(a.x - b.x, a.y - b.y, a.z - b.z);}
inline tbVector3 operator - (const tbVector3& v)						{return tbVector3(-v.x, -v.y, -v.z);}
inline tbVector3 operator * (const tbVector3& a, const tbVector3& b)	{return tbVector3(a.x * b.x, a.y * b.y, a.z * b.z);}
inline tbVector3 operator * (const tbVector3& v, const float f)			{return tbVector3(v.x * f, v.y * f, v.z * f);}
inline tbVector3 operator * (const float f, const tbVector3& v)			{return tbVector3(v.x * f, v.y * f, v.z * f);}
inline tbVector3 operator / (const tbVector3& a, const tbVector3& b)	{return tbVector3(a.x / b.x, a.y / b.y, a.z / b.z);}
inline tbVector3 operator / (const tbVector3& v, const float f)			{return tbVector3(v.x / f, v.y / f, v.z / f);}

// Vergleichsoperatoren
inline bool operator == (const tbVector3& a, const tbVector3& b) {if(a.x != b.x) return false; if(a.y != b.y) return false; return a.z == b.z;}
inline bool operator != (const tbVector3& a, const tbVector3& b) {if(a.x != b.x) return true; if(a.y != b.y) return true; return a.z != b.z;}

// ******************************************************************
// Funktionen deklarieren
inline float		tbVector3Length(const tbVector3& v)													{return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);}
inline float		tbVector3LengthSq(const tbVector3& v)												{return v.x * v.x + v.y * v.y + v.z * v.z;}
inline tbVector3	tbVector3Normalize(const tbVector3& v)												{return v / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);}
inline tbVector3	tbVector3NormalizeEx(const tbVector3& v)											{return v / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);}
inline tbVector3	tbVector3Cross(const tbVector3& v1, const tbVector3& v2)							{return tbVector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);}
inline float		tbVector3Dot(const tbVector3& v1, const tbVector3& v2)								{return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;}
inline float		tbVector3Angle(const tbVector3& v1, const tbVector3& v2)							{return acosf((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) / sqrtf((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z) * (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)));}
inline tbVector3	tbVector3InterpolateCoords(const tbVector3& v1, const tbVector3& v2, const float p)	{return v1 + p * (v2 - v1);}
inline tbVector3	tbVector3InterpolateNormal(const tbVector3& v1, const tbVector3& v2, const float p)	{return tbVector3NormalizeEx(v1 + p * (v2 - v1));}
//inline tbVector3	tbVector3Min(const tbVector3& v1, const tbVector3& v2)								{return tbVector3(TB_MIN(v1.x, v2.x), TB_MIN(v1.y, v2.y), TB_MIN(v1.z, v2.z));}
//inline tbVector3	tbVector3Max(const tbVector3& v1, const tbVector3& v2)								{return tbVector3(TB_MAX(v1.x, v2.x), TB_MAX(v1.y, v2.y), TB_MAX(v1.z, v2.z));}
//inline tbVector3	tbVector3Random()																	{return tbVector3NormalizeEx(tbVector3(tbFloatRandom(-1.0f, 1.0f), tbFloatRandom(-1.0f, 1.0f), tbFloatRandom(-1.0f, 1.0f)));}

//selbst**

/*tbVector3 tbVector3TransformNormal(const tbVector3& v, const tbMatrix& m) {
	const float fLength = tbVector3Length(v);
	if(fLength == 0.0f) return v;
	const tbMatrix mNew(tbMatrixTranspose(tbMatrixInvert(m)));
	return tbVector3Normalize(tbVector3(v.x * mNew.m11 + v.y * mNew.m21 + v.z * mNew.m31,
										v.x * mNew.m12 + v.y * mNew.m22 + v.z * mNew.m32,
										v.x * mNew.m13 + v.y * mNew.m23 + v.z * mNew.m33,))
			* fLength;
}
tbVector3 tbVector3TransformNormal_TranspInv(const tbVector3& v, const tbMatrix& m) {
	const float fLength = tbVector3Length(v);
	if(fLength == 0.0f) return v;
	return fLegth * tbVector3Normalize(tbVector3(v.x * m.m11 + v.y * m.m21 + v.z * m.m31,
												 v.x * m.m12 + v.y * m.m22 + v.z * m.m32,
												 v.x * m.m13 + v.y * m.m23 + v.z * m.m33));
}*/
// ******************************************************************

#endif /*CTBVECTOR_H_*/

#ifndef CTBMATRIX_H_
#define CTBMATRIX_H_

/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbMatrix.h
	==========
	Diese Datei ist Teil der TriBase-Engine

	Zweck:
	Matrizenrechnung

	Autor:
	David Scherfgen

********************************************************************/



// Verwendete Symbole vordefinieren

class tbVector3;
class tbMatrix;
tbMatrix tbMatrixInvert(const tbMatrix& m);

// ******************************************************************
// Die Matrixklasse
class tbMatrix
{
public:
	// Variablen
	union
	{
		struct
		{
			float m11, m12, m13, m14,	// Elemente der Matrix
				  m21, m22, m23, m24,
				  m31, m32, m33, m34,
				  m41, m42, m43, m44;
		};

		float		m[4][4];			// Zweidimensionales Array der Elemente
		float		n[16];				// Eindimensionales Array der Elemente
		//D3DMATRIX	mD3DMatrix;			// Matrix als D3DMATRIX
	};

	// Konstruktoren
	tbMatrix() {}

	tbMatrix(const tbMatrix& m) : m11(m.m11), m12(m.m12), m13(m.m13), m14(m.m14),
                                  m21(m.m21), m22(m.m22), m23(m.m23), m24(m.m24),
								  m31(m.m31), m32(m.m32), m33(m.m33), m34(m.m34),
								  m41(m.m41), m42(m.m42), m43(m.m43), m44(m.m44) {}

	tbMatrix(float _m11, float _m12, float _m13, float _m14,
			 float _m21, float _m22, float _m23, float _m24,
			 float _m31, float _m32, float _m33, float _m34,
			 float _m41, float _m42, float _m43, float _m44) : m11(_m11), m12(_m12), m13(_m13), m14(_m14),
			                                                   m21(_m21), m22(_m22), m23(_m23), m24(_m24),
															   m31(_m31), m32(_m32), m33(_m33), m34(_m34),
															   m41(_m41), m42(_m42), m43(_m43), m44(_m44) {}

	//tbMatrix(const float* pfValue) : mD3DMatrix(*((D3DMATRIX*)(pfValue)))	{}
	//tbMatrix(const D3DMATRIX& m) : mD3DMatrix(m)							{}

	// Casting-Opeatoren
	operator float* ()					{return (float*)(n);}
	//operator D3DMATRIX& ()				{return mD3DMatrix;}
	//operator const D3DMATRIX& () const	{return mD3DMatrix;}

	// Zugriffsoperatoren
	float& operator () (int iRow, int iColumn) {return m[iRow - 1][iColumn - 1];}
	float operator () (int iRow, int iColumn) const {return m[iRow - 1][iColumn - 1];}

	// Zuweisungsoperatoren
	//tbMatrix& operator = (const tbMatrix& m) {mD3DMatrix = m.mD3DMatrix; return *this;}
	
	//** selbst gemacht()()
	tbMatrix& operator = (const tbMatrix& m)
	{
		m11 = m.m11; m12 = m.m12; m13 = m.m13; m14 = m.m14;
		m21 = m.m21; m22 = m.m22; m23 = m.m23; m24 = m.m24;
		m31 = m.m31; m32 = m.m32; m33 = m.m33; m34 = m.m34;
		m41 = m.m41; m42 = m.m42; m43 = m.m43; m44 = m.m44;
		return *this;
	}
	
	tbMatrix& operator += (const tbMatrix& m)
	{
		m11 += m.m11; m12 += m.m12; m13 += m.m13; m14 += m.m14;
		m21 += m.m21; m22 += m.m22; m23 += m.m23; m24 += m.m24;
		m31 += m.m31; m32 += m.m32; m33 += m.m33; m34 += m.m34;
		m41 += m.m41; m42 += m.m42; m43 += m.m43; m44 += m.m44;
		return *this;
	}

	tbMatrix& operator -= (const tbMatrix& m)
	{
		m11 -= m.m11; m12 -= m.m12; m13 -= m.m13; m14 -= m.m14;
		m21 -= m.m21; m22 -= m.m22; m23 -= m.m23; m24 -= m.m24;
		m31 -= m.m31; m32 -= m.m32; m33 -= m.m33; m34 -= m.m34;
		m41 -= m.m41; m42 -= m.m42; m43 -= m.m43; m44 -= m.m44;
		return *this;
	}

	tbMatrix& operator *= (const tbMatrix& m)
	{
		return *this = tbMatrix(m.m11 * m11 + m.m21 * m12 + m.m31 * m13 + m.m41 * m14,
								m.m12 * m11 + m.m22 * m12 + m.m32 * m13 + m.m42 * m14,
								m.m13 * m11 + m.m23 * m12 + m.m33 * m13 + m.m43 * m14,
								m.m14 * m11 + m.m24 * m12 + m.m34 * m13 + m.m44 * m14,
								m.m11 * m21 + m.m21 * m22 + m.m31 * m23 + m.m41 * m24,
								m.m12 * m21 + m.m22 * m22 + m.m32 * m23 + m.m42 * m24,
								m.m13 * m21 + m.m23 * m22 + m.m33 * m23 + m.m43 * m24,
								m.m14 * m21 + m.m24 * m22 + m.m34 * m23 + m.m44 * m24,
								m.m11 * m31 + m.m21 * m32 + m.m31 * m33 + m.m41 * m34,
								m.m12 * m31 + m.m22 * m32 + m.m32 * m33 + m.m42 * m34,
								m.m13 * m31 + m.m23 * m32 + m.m33 * m33 + m.m43 * m34,
								m.m14 * m31 + m.m24 * m32 + m.m34 * m33 + m.m44 * m34,
								m.m11 * m41 + m.m21 * m42 + m.m31 * m43 + m.m41 * m44,
								m.m12 * m41 + m.m22 * m42 + m.m32 * m43 + m.m42 * m44,
								m.m13 * m41 + m.m23 * m42 + m.m33 * m43 + m.m43 * m44,
								m.m14 * m41 + m.m24 * m42 + m.m34 * m43 + m.m44 * m44);
	}

	tbMatrix& operator *= (const float f)
	{
		m11 *= f; m12 *= f; m13 *= f; m14 *= f;
		m21 *= f; m22 *= f; m23 *= f; m24 *= f;
		m31 *= f; m32 *= f; m33 *= f; m34 *= f;
		m41 *= f; m42 *= f; m43 *= f; m44 *= f;
		return *this;
	}
	
	tbMatrix& operator /= (const tbMatrix& m)
	{
		return *this *= tbMatrixInvert(m);
	}

	tbMatrix& operator /= (const float f)
	{
		m11 /= f; m12 /= f; m13 /= f; m14 /= f;
		m21 /= f; m22 /= f; m23 /= f; m24 /= f;
		m31 /= f; m32 /= f; m33 /= f; m34 /= f;
		m41 /= f; m42 /= f; m43 /= f; m44 /= f;
		return *this;
	}
};

// Arithmetische Operatoren
inline tbMatrix operator + (const tbMatrix& a, const tbMatrix& b)	{return tbMatrix(a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14, a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23, a.m24 + b.m24, a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33, a.m34 + b.m34, a.m41 + b.m41, a.m42 + b.m42, a.m43 + b.m43, a.m44 + b.m44);}
inline tbMatrix operator - (const tbMatrix& a, const tbMatrix& b)	{return tbMatrix(a.m11 - b.m11, a.m12 - b.m12, a.m13 - b.m13, a.m14 - b.m14, a.m21 - b.m21, a.m22 - b.m22, a.m23 - b.m23, a.m24 - b.m24, a.m31 - b.m31, a.m32 - b.m32, a.m33 - b.m33, a.m34 - b.m34, a.m41 - b.m41, a.m42 - b.m42, a.m43 - b.m43, a.m44 - b.m44);}
inline tbMatrix operator - (const tbMatrix& m)						{return tbMatrix(-m.m11, -m.m12, -m.m13, -m.m14, -m.m21, -m.m22, -m.m23, -m.m24, -m.m31, -m.m32, -m.m33, -m.m34, -m.m41, -m.m42, -m.m43, -m.m44);}

inline tbMatrix operator * (const tbMatrix& a,
							const tbMatrix& b)
{
	return tbMatrix(b.m11 * a.m11 + b.m21 * a.m12 + b.m31 * a.m13 + b.m41 * a.m14,
					b.m12 * a.m11 + b.m22 * a.m12 + b.m32 * a.m13 + b.m42 * a.m14,
					b.m13 * a.m11 + b.m23 * a.m12 + b.m33 * a.m13 + b.m43 * a.m14,
					b.m14 * a.m11 + b.m24 * a.m12 + b.m34 * a.m13 + b.m44 * a.m14,
					b.m11 * a.m21 + b.m21 * a.m22 + b.m31 * a.m23 + b.m41 * a.m24,
					b.m12 * a.m21 + b.m22 * a.m22 + b.m32 * a.m23 + b.m42 * a.m24,
					b.m13 * a.m21 + b.m23 * a.m22 + b.m33 * a.m23 + b.m43 * a.m24,
					b.m14 * a.m21 + b.m24 * a.m22 + b.m34 * a.m23 + b.m44 * a.m24,
					b.m11 * a.m31 + b.m21 * a.m32 + b.m31 * a.m33 + b.m41 * a.m34,
					b.m12 * a.m31 + b.m22 * a.m32 + b.m32 * a.m33 + b.m42 * a.m34,
					b.m13 * a.m31 + b.m23 * a.m32 + b.m33 * a.m33 + b.m43 * a.m34,
					b.m14 * a.m31 + b.m24 * a.m32 + b.m34 * a.m33 + b.m44 * a.m34,
					b.m11 * a.m41 + b.m21 * a.m42 + b.m31 * a.m43 + b.m41 * a.m44,
					b.m12 * a.m41 + b.m22 * a.m42 + b.m32 * a.m43 + b.m42 * a.m44,
					b.m13 * a.m41 + b.m23 * a.m42 + b.m33 * a.m43 + b.m43 * a.m44,
					b.m14 * a.m41 + b.m24 * a.m42 + b.m34 * a.m43 + b.m44 * a.m44);
}

inline tbMatrix operator * (const tbMatrix& m,
							const float f)
{
	return tbMatrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
			        m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
					m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
					m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
}

inline tbMatrix operator * (const float f,
							const tbMatrix& m)
{
	return tbMatrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
			        m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
					m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
					m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
}

inline tbMatrix operator / (const tbMatrix& a, const tbMatrix& b) {return a * tbMatrixInvert(b);}

inline tbMatrix operator / (const tbMatrix& m,
							const float f)
{
	return tbMatrix(m.m11 / f, m.m12 / f, m.m13 / f, m.m14 / f,
			        m.m21 / f, m.m22 / f, m.m23 / f, m.m24 / f,
					m.m31 / f, m.m32 / f, m.m33 / f, m.m34 / f,
					m.m41 / f, m.m42 / f, m.m43 / f, m.m44 / f);
}

// Vergleichsoperatoren
inline bool operator == (const tbMatrix& a,
						 const tbMatrix& b)
{
	if(a.m11 != b.m11) return false;
	if(a.m12 != b.m12) return false;
	if(a.m13 != b.m13) return false;
	if(a.m14 != b.m14) return false;
	if(a.m21 != b.m21) return false;
	if(a.m22 != b.m22) return false;
	if(a.m23 != b.m23) return false;
	if(a.m24 != b.m24) return false;
	if(a.m31 != b.m31) return false;
	if(a.m32 != b.m32) return false;
	if(a.m33 != b.m33) return false;
	if(a.m34 != b.m34) return false;
	if(a.m41 != b.m41) return false;
	if(a.m42 != b.m42) return false;
	if(a.m43 != b.m43) return false;
	return a.m44 == b.m44;
}

// Vergleichsoperatoren
inline bool operator != (const tbMatrix& a,
						 const tbMatrix& b)
{
	if(a.m11 != b.m11) return true;
	if(a.m12 != b.m12) return true;
	if(a.m13 != b.m13) return true;
	if(a.m14 != b.m14) return true;
	if(a.m21 != b.m21) return true;
	if(a.m22 != b.m22) return true;
	if(a.m23 != b.m23) return true;
	if(a.m24 != b.m24) return true;
	if(a.m31 != b.m31) return true;
	if(a.m32 != b.m32) return true;
	if(a.m33 != b.m33) return true;
	if(a.m34 != b.m34) return true;
	if(a.m41 != b.m41) return true;
	if(a.m42 != b.m42) return true;
	if(a.m43 != b.m43) return true;
	return a.m44 != b.m44;
}

// ******************************************************************
// Funktionen deklarieren
inline		tbMatrix	tbMatrixIdentity() {return tbMatrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);}	// Identitätsmatrix liefern
tbMatrix	tbMatrixTranslation(const tbVector3& v);																								// Translationsmatrix (Verschiebungsmatrix) berechnen
tbMatrix	tbMatrixRotationX(const float f);																										// Rotationsmatrix um die X-Achse berechnen
tbMatrix	tbMatrixRotationY(const float f);																										// Rotationsmatrix um die Y-Achse berechnen
tbMatrix	tbMatrixRotationZ(const float f);																										// Rotationsmatrix um die Z-Achse berechnen
tbMatrix	tbMatrixRotation(const float x, const float y, const float z);																			// Rotiert um alle drei Achsen
tbMatrix	tbMatrixRotation(const tbVector3& v);																									// Rotiert um alle drei Achsen
tbMatrix	tbMatrixRotationAxis(const tbVector3& v, const float f);																				// Rotationsmatrix um eine beliebige Achse berechnen
tbMatrix	tbMatrixScaling(const tbVector3& v);																									// Skalierungsmatrix berechnen
tbMatrix	tbMatrixAxes(const tbVector3& vXAxis, const tbVector3& vYAxis, const tbVector3& vZAxis);												// Liefert eine Achsenmatrix
float		tbMatrixDet(const tbMatrix& m, float* pfOut);																							// Determinante berechnen
tbMatrix	tbMatrixInvert(const tbMatrix& m);																										// Invertierte (umgekehrte) Matrix berechnen
tbMatrix	tbMatrixTranspose(const tbMatrix& m);																									// Transponierte Matrix berechnen
tbMatrix	tbMatrixProjection(const float fFOV, const float fAspect, const float fNearPlane, const float fFarPlane);								// Projektionsmatrix berechnen
tbMatrix	tbMatrixCamera(const tbVector3& vPos, const tbVector3& vLookAt, const tbVector3& vUp);// = tbVector3(0.0f, 1.0f, 0.0f));					// Kameramatrix erzeugen
tbMatrix	tbMatrixToTex2DMatrix(const tbMatrix& m);																								// In Texturmatrix umwandeln

// ******************************************************************

#endif /*CTBMATRIX_H_*/

/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbMatrix.h
	==========
	Diese Datei ist Teil der TriBase-Engine

	Zweck:
	Matrizenrechnung

	Autor:
	David Scherfgen

********************************************************************/



// ******************************************************************
// Translationsmatrix berechnen
tbMatrix tbMatrixTranslation(const tbVector3& v)
{
	return tbMatrix(1.0f, 0.0f, 0.0f, 0.0f,
		            0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					v.x,  v.y,  v.z,  1.0f);
}

// ******************************************************************
// Rotationsmatrix für Rotation um die x-Achse berechnen
tbMatrix tbMatrixRotationX(const float f)
{
	tbMatrix mResult;

	// Rotationsmatrix berechnen
	mResult.m11 = 1.0f; mResult.m12 = 0.0f; mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	mResult.m21 = 0.0f;                                         mResult.m24 = 0.0f;
	mResult.m31 = 0.0f;                                         mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m22 = mResult.m33 = cosf(f);
	mResult.m23 = sinf(f);
	mResult.m32 = -mResult.m23;

	return mResult;
}

// ******************************************************************
// Rotationsmatrix für Rotation um die y-Achse berechnen
tbMatrix tbMatrixRotationY(const float f)
{
	tbMatrix mResult;

	// Rotationsmatrix berechnen
	                    mResult.m12 = 0.0f;                     mResult.m14 = 0.0f;
	mResult.m21 = 0.0f; mResult.m22 = 1.0f; mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	                    mResult.m32 = 0.0f;                     mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m33 = cosf(f);
	mResult.m31 = sinf(f);
	mResult.m13 = -mResult.m31;

	return mResult;
}

// ******************************************************************
// Rotationsmatrix für Rotation um die z-Achse berechnen
tbMatrix tbMatrixRotationZ(const float f)
{
	tbMatrix mResult;

	// Rotationsmatrix berechnen
	                                        mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	                                        mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	mResult.m31 = 0.0f; mResult.m32 = 0.0f; mResult.m33 = 1.0f; mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m22 = cosf(f);
	mResult.m12 = sinf(f);
	mResult.m21 = -mResult.m12;

	return mResult;
}

// ******************************************************************
// Rotiert um alle drei Achsen
tbMatrix tbMatrixRotation(const float x,
									  const float y,
									  const float z)
{
	return tbMatrixRotationZ(z) *
		   tbMatrixRotationX(x) *
		   tbMatrixRotationY(y);
}

// ******************************************************************
// Rotiert um alle drei Achsen (Winkel in Vektor)
tbMatrix tbMatrixRotation(const tbVector3& v)
{
	return tbMatrixRotationZ(v.z) *
		   tbMatrixRotationX(v.x) *
		   tbMatrixRotationY(v.y);
}

// ******************************************************************
// Rotationsmatrix für Rotation um eine beliebige Achse berechnen
tbMatrix tbMatrixRotationAxis(const tbVector3& v,
										  const float f)
{
	// Sinus und Kosinus berechnen
	const float fSin = sinf(-f);
	const float fCos = cosf(-f);
	const float fOneMinusCos = 1.0f - fCos;

	// Achsenvektor normalisieren
	const tbVector3 vAxis(tbVector3Normalize(v));

	// Matrix erstellen
	return tbMatrix((vAxis.x * vAxis.x) * fOneMinusCos + fCos,
		            (vAxis.x * vAxis.y) * fOneMinusCos - (vAxis.z * fSin),
				    (vAxis.x * vAxis.z) * fOneMinusCos + (vAxis.y * fSin),
					0.0f,
					(vAxis.y * vAxis.x) * fOneMinusCos + (vAxis.z * fSin),
					(vAxis.y * vAxis.y) * fOneMinusCos + fCos,
					(vAxis.y * vAxis.z) * fOneMinusCos - (vAxis.x * fSin),
					0.0f,
					(vAxis.z * vAxis.x) * fOneMinusCos - (vAxis.y * fSin),
					(vAxis.z * vAxis.y) * fOneMinusCos + (vAxis.x * fSin),
					(vAxis.z * vAxis.z) * fOneMinusCos + fCos,
					0.0f,
					0.0f,
					0.0f,
					0.0f,
					1.0f);
}

// ******************************************************************
// Skalierungsmatrix berechnen
tbMatrix tbMatrixScaling(const tbVector3& v)
{
	// Skalierungsmatrix berechnen
	return tbMatrix(v.x,  0.0f, 0.0f, 0.0f,
		            0.0f, v.y,  0.0f, 0.0f,
					0.0f, 0.0f, v.z,  0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}

// ******************************************************************
// Liefert eine Achsenmatrix
tbMatrix tbMatrixAxes(const tbVector3& vXAxis,
								  const tbVector3& vYAxis,
								  const tbVector3& vZAxis)
{
	return tbMatrix(vXAxis.x, vXAxis.y, vXAxis.z, 0.0f,
		            vYAxis.x, vYAxis.y, vYAxis.z, 0.0f,
					vZAxis.x, vZAxis.y, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}

// ******************************************************************
// Determinante einer Matrix berechnen
float tbMatrixDet(const tbMatrix& m)
{
	// Determinante der linken oberen 3x3-Teilmatrix berechnen
	return m.m11 * (m.m22 * m.m33 - m.m23 * m.m32) -
           m.m12 * (m.m21 * m.m33 - m.m23 * m.m31) +
           m.m13 * (m.m21 * m.m32 - m.m22 * m.m31);
}

// ******************************************************************
// Invertierte Matrix berechnen
tbMatrix tbMatrixInvert(const tbMatrix& m)
{
	// Kehrwert der Determinante vorberechnen
	float fInvDet = tbMatrixDet(m);
	if(fInvDet == 0.0f) return tbMatrixIdentity();
	fInvDet = 1.0f / fInvDet;

	// Invertierte Matrix berechnen
	tbMatrix mResult;
	mResult.m11 =  fInvDet * (m.m22 * m.m33 - m.m23 * m.m32);
	mResult.m12 = -fInvDet * (m.m12 * m.m33 - m.m13 * m.m32);
	mResult.m13 =  fInvDet * (m.m12 * m.m23 - m.m13 * m.m22);
	mResult.m14 =  0.0f;
	mResult.m21 = -fInvDet * (m.m21 * m.m33 - m.m23 * m.m31);
	mResult.m22 =  fInvDet * (m.m11 * m.m33 - m.m13 * m.m31);
	mResult.m23 = -fInvDet * (m.m11 * m.m23 - m.m13 * m.m21);
	mResult.m24 =  0.0f;
	mResult.m31 =  fInvDet * (m.m21 * m.m32 - m.m22 * m.m31);
	mResult.m32 = -fInvDet * (m.m11 * m.m32 - m.m12 * m.m31);
	mResult.m33 =  fInvDet * (m.m11 * m.m22 - m.m12 * m.m21);
	mResult.m34 =  0.0f;
	mResult.m41 = -(m.m41 * mResult.m11 + m.m42 * mResult.m21 + m.m43 * mResult.m31);
	mResult.m42 = -(m.m41 * mResult.m12 + m.m42 * mResult.m22 + m.m43 * mResult.m32);
	mResult.m43 = -(m.m41 * mResult.m13 + m.m42 * mResult.m23 + m.m43 * mResult.m33);
	mResult.m44 =  1.0f;

	return mResult;
}

// ******************************************************************
// Transponierte Matrix berechnen
tbMatrix tbMatrixTranspose(const tbMatrix& m)
{
	// Matrix transponieren
	return tbMatrix(m.m11, m.m21, m.m31, m.m41,
		            m.m12, m.m22, m.m32, m.m42,
					m.m13, m.m23, m.m33, m.m43,
					m.m14, m.m24, m.m34, m.m44);
}

// ******************************************************************
// Projektionsmatrix berechnen
tbMatrix tbMatrixProjection(const float fFOV,
										const float fAspect,
										const float fNearPlane,
										const float fFarPlane)
{
	const float s = 1.0f / tanf(fFOV * 0.5f);
	const float Q = fFarPlane / (fFarPlane - fNearPlane);

	return tbMatrix(s / fAspect, 0.0f, 0.0f, 0.0f,
		            0.0f, s, 0.0f, 0.0f,
					0.0f, 0.0f, Q, 1.0f,
					0.0f, 0.0f, -Q * fNearPlane, 0.0f);
}

// ******************************************************************
// Kameramatrix berechnen
tbMatrix tbMatrixCamera(const tbVector3& vPos,
									const tbVector3& vLookAt,
									const tbVector3& vUp) // = tbVector3(0.0f, 1.0f, 0.0f)
{
	// Die z-Achse des Kamerakoordinatensystems berechnen
	tbVector3 vZAxis(tbVector3Normalize(vLookAt - vPos));

	// Die x-Achse ist das Kreuzprodukt aus y- und z-Achse
	tbVector3 vXAxis(tbVector3Normalize(tbVector3Cross(vUp, vZAxis)));

	// y-Achse berechnen
	tbVector3 vYAxis(tbVector3Normalize(tbVector3Cross(vZAxis, vXAxis)));

	// Rotationsmatrix erzeugen und die Translationsmatrix mit ihr multiplizieren
	return tbMatrixTranslation(-vPos) *
	       tbMatrix(vXAxis.x, vYAxis.x, vZAxis.x, 0.0f,
		            vXAxis.y, vYAxis.y, vZAxis.y, 0.0f,
				    vXAxis.z, vYAxis.z, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}

// ******************************************************************
// Texturmatrix berechnen
tbMatrix tbMatrixToTex2DMatrix(const tbMatrix& m)
{
	// Texturmatrix berechnen
	return tbMatrix(m.m11, m.m12, m.m14, 0.0f,
		            m.m21, m.m22, m.m24, 0.0f,
					m.m41, m.m42, m.m44, 0.0f,
					0.0f,  0.0f,  0.0f,  1.0f);
}

// ******************************************************************

#ifndef SINGLETON_H_
#define SINGLETON_H_

template <class T>
class TSingleton
{
protected:
	static T *m_pSingleton;	
	
public:
	
	virtual ~TSingleton()
	{
	}
	
	inline static T* Get ()
	{
		if (!m_pSingleton)
		m_pSingleton = new T;
		
		return (m_pSingleton);
	}
	
	static void Del ()
	{
		if (m_pSingleton)
		{
			delete (m_pSingleton);
			m_pSingleton = NULL;
		}
	}
	
};

	template <class T>
	T* TSingleton<T>::m_pSingleton = 0;

#endif /*SINGLETON_H_*/

#ifndef CPIXELDRAW_H_
#define CPIXELDRAW_H_




struct S_Plane {
	float d;
	tbVector3 n;
};
/*struct S_zPixel {
	float z;
	Uint32 col;
};*/
struct S_Triangle {
	tbVector3 A;
	tbVector3 B;
	tbVector3 C;
};

/*class CWindow {
	private:
		static int iAllW, iAllH, iRandW;
		static SDL_Surface *screen;
		static bool bFullScreen;
		void setWindow(int i1 , int i2, int i3, bool b1) {iAllW = i1; iAllH = i2; iRandW = i3; bFullScreen = b1;}
};
int CWindow::iAllW = 1000, CWindow::iAllH = 600, CWindow::iRandW = 200;
SDL_Surface CWindow::(*screen) = NULL;
bool CWindow::bFullScreen = false;*/ // allgemein window versuch

//prototypen
inline tbVector3 tbVector3TransformCoords(const tbVector3& v, const tbMatrix& m, float* const pfOutW = NULL);
inline void pointOnLineToZ(tbVector3 &v1, tbVector3 &v2, float fZ);

class CPixelDraw : public TSingleton<CPixelDraw> {
public:
	CPixelDraw();
	~CPixelDraw();
	
	//funktionen
	inline void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) { // pixel setzen /*surface must be locked*/
		if (x < ALLW - RANDW && x > 0 && y > 0 && y < ALLH) {
		    int bpp = surface->format->BytesPerPixel;
		    /* Here p is the address to the pixel we want to set */
		    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
		    switch(bpp) {
				case 1: *p = pixel; break;
				case 2: *(Uint16 *)p = pixel;  break;
				case 3:
			        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			            p[0] = (pixel >> 16) & 0xff;
			            p[1] = (pixel >> 8) & 0xff;
			            p[2] = pixel & 0xff;
			        } else {
			            p[0] = pixel & 0xff;
			            p[1] = (pixel >> 8) & 0xff;
			            p[2] = (pixel >> 16) & 0xff;
			        }
			        break;
				case 4: *(Uint32 *)p = pixel; break;
		    }
		}
	}
	inline void putpixelZ3D(int x, int y, int z, Uint32 col) { // writes pixel to zpuffer
		if (x < ALLW - RANDW && x > 0 && y > 0 && y < ALLH) {
			if (z < zBufferZ[x][y]) {
				zBufferZ[x][y] = z;
				zBufferCol[x][y] = col;
			}
		}
	}
	inline Uint32 getpixel(SDL_Surface *surface, int x, int y) { //Return the pixel value at (x, y)  NOTE: The surface must be locked before calling this!
		int bpp = surface->format->BytesPerPixel;
	    /* Here p is the address to the pixel we want to retrieve */
	    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	
	    switch(bpp) {
	    case 1: return *p;
	    case 2: return *(Uint16 *)p;
	    case 3: if(SDL_BYTEORDER == SDL_BIG_ENDIAN) return p[0] << 16 | p[1] << 8 | p[2]; else return p[0] | p[1] << 8 | p[2] << 16;
	    case 4: return *(Uint32 *)p;
	    default: return 0;       /* shouldn't happen, but avoids warnings */
	    }
	}
	/*inline void drawline(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 pixel) { // linie zeichnen
		float fy, fUp, fx, fLeft;
		int iTmp = 0;
		
		if (Betrag(x2-x1) > Betrag(y2-y1)) {
			if (x1 > x2) {Change(&x1,&x2);Change(&y1,&y2);}
			fy = y1; fUp = static_cast<float>(y2-y1)/(Betrag(x2-x1));
			
			if (x1 < 0) {iTmp = (-x1); fy += fUp * iTmp;}
			for (int i = iTmp; i <= Betrag(x2 - x1) && x1 + (i) < surface->w; i++) {
				fy += fUp;
				putpixel(surface,x1 + (i), Round(fy),pixel);
			}
		} else {
			if (y1 > y2) {Change(&x1,&x2);Change(&y1,&y2);}
			fx = x1; fLeft = static_cast<float>(x2-x1)/(Betrag(y2-y1));
			
			if (y1 < 0) {iTmp = (-y1); fx += fLeft * iTmp;}
			for (int i = iTmp; i <= Betrag(y2 - y1) && y1 + (i) < surface->h; i++) {
				fx += fLeft;
				putpixel(surface, Round(fx), y1 + (i),pixel);
			}
		}
	}*/
	inline void drawlineZ3D(const tbVector3& v1, const tbVector3& v2, Uint32 col, const tbMatrix &mPro) { // speichert 3d linie im zbuffer
		tbVector3 v3, v4;
		
		v3 = tbVector3TransformCoords(v1,mPro);
		v4 = tbVector3TransformCoords(v2,mPro);
		
		v3.z = v1.z;
		v4.z = v2.z;
		
		v3.x = v3.x * ((ALLW-RANDW) / 2.0f) + (ALLW-RANDW) / 2.0f;
		v3.y = v3.y * (ALLH / -2.0f) + ALLH / 2.0f;
		
		v4.x = v4.x * ((ALLW-RANDW) / 2.0f) + (ALLW-RANDW) / 2.0f;
		v4.y = v4.y * (ALLH / -2.0f) + ALLH / 2.0f;
		
		
		float fy, fUp, fx, fLeft, fz, fzDown;
		int iTmp = 0;
		
		if (Betrag(v4.x-v3.x) > Betrag(v4.y-v3.y)) {
			if (v3.x > v4.x) {Change(&v3.x,&v4.x);Change(&v3.y,&v4.y);Change(&v3.z,&v4.z);}
			fy = v3.y; fUp = static_cast<float>(v4.y-v3.y)/(Betrag(v4.x-v3.x));
			fz = v3.z; fzDown = static_cast<float>(v4.z-v3.z)/(Betrag(v4.x-v3.x));
			
			if (v3.x < 0) {iTmp = (-v3.x); fy += fUp * iTmp;}
			for (int i = iTmp; i <= Betrag(v4.x - v3.x) && v3.x + (i) < ALLW; i++) {
				fy += fUp;
				fz += fzDown;
				putpixelZ3D(v3.x + (i), Round(fy), fz, col);
			}
		} else {
			if (v3.y > v4.y) {Change(&v3.x,&v4.x);Change(&v3.y,&v4.y);Change(&v3.z,&v4.z);}
			fx = v3.x; fLeft = static_cast<float>(v4.x-v3.x)/(Betrag(v4.y-v3.y));
			fz = v3.z; fzDown = static_cast<float>(v4.z-v3.z)/(Betrag(v4.y-v3.y));
			
			if (v3.y < 0) {iTmp = (-v3.y); fx += fLeft * iTmp;}
			for (int i = iTmp; i <= Betrag(v4.y - v3.y) && v3.y + (i) < ALLH; i++) {
				fx += fLeft;
				fz += fzDown;
				putpixelZ3D(Round(fx), v3.y + (i), fz, col);
			}
		}
	}
	inline void drawlineZ3D(const tbVector3& v1, const tbVector3& v2, SDL_Color col, const tbMatrix &mPro) { // speichert 3d linie im zbuffer
		//drawlineZ3D(v1,v2,SDL_MapRGB(screen->format, col.r, col.g, col.b)),mPro);
	}
	inline void drawtriangleZ3D(const S_Triangle& t, Uint32 col, const tbMatrix &mPro, const tbMatrix &mCam, bool bFill = true) { // zeichnet dreieck 3D in den zBuffer
		/*todo: besserer algogithmus*/
		S_Triangle t2;
		t2.A = tbVector3TransformCoords(t.A,mCam);
		t2.B = tbVector3TransformCoords(t.B,mCam);
		t2.C = tbVector3TransformCoords(t.C,mCam);
		
		//sortieren
		if(t2.C < t2.B) Change(&t2.C,&t2.B);
		if(t2.B < t2.A) Change(&t2.B,&t2.A);
		if(t2.C < t2.B) Change(&t2.C,&t2.B);
		
		tbVector3 v1, v2;
		if (bFill) {
			tbVector3 v3 = tbVector3TransformCoords(t2.A,mPro), v4 = tbVector3TransformCoords(t2.B,mPro), v5 = tbVector3TransformCoords(t2.C,mPro), v6;
			const float fMinX = Min(v4.x,v5.x);
			const float fMaxX = Max(v4.x,v5.x);
			const float fMinY = Min(v4.y,v5.y);
			const float fMaxY = Max(v4.y,v5.y);
			int iDistance = static_cast<int> (Max(Betrag(fMaxX-fMinX),Betrag(fMaxY-fMinY)) * (ALLW-RANDW) / 2);
			float fDistance = (t2.C.x - t2.B.x) / iDistance;
			
			v1 = t2.A;
			v2 = t2.B;
			v4 = t2.C-t2.B; v4 /= v4.x;
			for (int i = 0; i < iDistance; i++) {
				v2 += v4 * fDistance;
				
				if((v1.z >= fNaheClippingEbene || v2.z >= fNaheClippingEbene) && (v1.z <= fFerneClippingEbene || v2.z <= fFerneClippingEbene)) {
			
					if(v1.z < fNaheClippingEbene)
						pointOnLineToZ(v1,v2,fNaheClippingEbene);
					if(v2.z < fNaheClippingEbene)
						pointOnLineToZ(v2,v1,fNaheClippingEbene);
					if(v1.z > fFerneClippingEbene)
						pointOnLineToZ(v1,v2,fFerneClippingEbene);
					if(v2.z > fFerneClippingEbene)
						pointOnLineToZ(v2,v1,fFerneClippingEbene);
				
					drawlineZ3D (v1,v2,col,mPro);
				}
			}
			
			
		}
		for (int i = 0; i < 3; i++) {
			if (i==0) {v1 = t2.A; v2 = t2.B;}
			if (i==1) {v1 = t2.B; v2 = t2.C;}
			if (i==2) {v1 = t2.C; v2 = t2.A;}
			if((v1.z >= fNaheClippingEbene || v2.z >= fNaheClippingEbene) && (v1.z <= fFerneClippingEbene || v2.z <= fFerneClippingEbene)) {
				
				if(v1.z < fNaheClippingEbene)
					pointOnLineToZ(v1,v2,fNaheClippingEbene);
				if(v2.z < fNaheClippingEbene)
					pointOnLineToZ(v2,v1,fNaheClippingEbene);
				if(v1.z > fFerneClippingEbene)
					pointOnLineToZ(v1,v2,fFerneClippingEbene);
				if(v2.z > fFerneClippingEbene)
					pointOnLineToZ(v2,v1,fFerneClippingEbene);
					
				drawlineZ3D (v1,v2,col,mPro);
			}
		}
	}
	void zBuffer_flip(SDL_Surface *screen);												// Diese vier kleinen
	void zBuffer_init(SDL_Surface *screen, float fFerneClippingEbene, Uint32 col);		// Funktionen verbrauchen
	void zBuffer_init(SDL_Surface *screen, float fFerneClippingEbene, SDL_Color col);	// fast die gesamte, verfuegbare
	void zBuffer_init(SDL_Surface *screen, float fFerneClippingEbene);					// Framerate. (todo!!!)
	//void fillSurface (SDL_Surface *surface, int iAnz, tbVector3 v[], Uint32 pixel, bool bFill = true); // eine flaeche mit farbe fuellen
	//void fillCircle (SDL_Surface *surface, int mx, int my, float r, Uint32 pixel, bool bFill = true);
	void saveScreenshot (SDL_Surface *surface, string sPath = "standart");
	void setClippings (float f1, float f2) {if (f1 > 0.0f) fNaheClippingEbene = f1; if (f2 > fNaheClippingEbene) fFerneClippingEbene = f2;}
	
private:
	//variablen
	float zBufferZ[ALLW-RANDW][ALLH];
	Uint32 zBufferCol[ALLW-RANDW][ALLH];
	float fNaheClippingEbene, fFerneClippingEbene;
	
	//funktionen
	//...
};


inline tbVector3 tbVector3TransformCoords(const tbVector3& v, const tbMatrix& m, float* const pfOutW) {//vector transformieren
	tbVector3 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
					  v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42,
					  v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43);
	const float w = v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + m.m44;
	if (w != 1.0f) vResult /= w;
	if(pfOutW) *pfOutW = w;
	
	return vResult;
}

inline S_Plane getPlane (const tbVector3 &v1, const tbVector3 &v2, const tbVector3 &v3) {
	S_Plane tmp;
	const tbVector3 vp = v1;
	const tbVector3 vn = tbVector3Cross(v3 -v2, v1 - v2);
	tmp.n = vn;
	tmp.d = -vn.x * vp.x - vn.y * vp.y - vn.z * vp.z;
	
	return tmp;
}

inline S_Triangle getTriangle (tbVector3 a, tbVector3 b, tbVector3 c) {
	S_Triangle t; t.A = a; t.B = b; t.C = c; return t;
}

inline float getLinePointY (tbVector3 v1, tbVector3 v2, float fX) { // errechnet eine y-pos auf linie bei geg. x-pos
	float fY = 0.0f;
	v1 -= v2;
	fX -= v2.x;
	fY = (v1.y / v1.x) * fX;
	fY += v2.y;
	return fY;
}
inline float getLinePointX (tbVector3 v1, tbVector3 v2, float fY) { // errechnet eine x-pos auf linie bei geg. y-pos
	float fX = 0.0f; 
	v1 -= v2;
	fY -= v2.y;
	fX = (v1.x / v1.y) * fY;
	fX += v2.x;
	return fX;
}
inline void pointOnLineToZ(tbVector3 &v1, tbVector3 &v2, float fZ) { // verschiebt 1. auf 3.; linie: 1. - 2.
	v1 -= v2;
	fZ -= v2.z;
	v1 = (v1 / v1.z) * fZ;
	v1 += v2;
}

inline bool tbLineHitsPlane(const tbVector3 &vLineA, const tbVector3 &vLineB, const S_Plane &Plane, tbVector3 *pvOut = NULL) {
	const tbVector3 vLineDir(vLineB-vLineA);
	const float fDenominator = tbVector3Dot(Plane.n,vLineDir);
	if (fabsf(fDenominator) < 0.0001f) {
		if (fabsf(tbVector3Dot(Plane.n,vLineA) + Plane.d) < 0.0001f) {
			if(pvOut) *pvOut = vLineA;
			return true;
		} else return false;
	}
	const float s = -(tbVector3Dot(Plane.n,vLineA) + Plane.d) / fDenominator;
	if(s < 0.0f || s > 1.0f) return false;
	if(pvOut) *pvOut = vLineA + (s*vLineDir);
	return true;
}
inline bool lineKollision (tbVector3 v1, tbVector3 v2, tbVector3 v3, tbVector3 v4, tbVector3 *v5 = NULL) {// 1., 2.: 1.linie; 3.,4.:2.linie;5.:schnittpunkt
	tbVector3 vTmp(0.0f,0.0f,0.0f);
	if (v2.x < v1.x) Change(&v1,&v2);
	if (v4.x < v3.x) Change(&v3,&v4);
	
	if (v3.x == v4.x && v1.x == v2.x) return false;
	else if (v3.x == v4.x) {
		vTmp.x = v3.x;
		vTmp.y = getLinePointY ( v1,v2,vTmp.x);
	} else if (v1.x == v2.x) {
		vTmp.x = v1.x;
		vTmp.y = getLinePointY (v3,v4,vTmp.x);
	} else {
		vTmp.x = ((((v3.y - v4.y) / (v3.x - v4.x)) * v4.x) - (((v1.y - v2.y) / (v1.x - v2.x)) * v2.x) + v2.y - v4.y) / 
					(((v3.y - v4.y) / (v3.x - v4.x)) - ((v1.y - v2.y) / (v1.x - v2.x)));
		
		vTmp.y = (((v1.y - v2.y) / (v1.x - v2.x)) * (vTmp.x - (v1.x))) + v1.y;
	}
	if(vTmp.x >= Min(v1.x,v2.x) && vTmp.x <= Max(v1.x,v2.x) && vTmp.x >= Min(v3.x,v4.x) && vTmp.x <= Max(v3.x,v4.x)) {
		if (v5) (*v5) = vTmp; 
		return true;
	}
	
	return false;
}

#endif /*CPIXELDRAW_H_*/



CPixelDraw::CPixelDraw() {
	for (int i = 0; i < ALLH; i++) {
		for (int j = 0; j < ALLW-RANDW; j++) {
			zBufferZ[j][i] = -1;
		}
	}
}
CPixelDraw::~CPixelDraw() {	
}

void CPixelDraw::zBuffer_flip(SDL_Surface *screen) {
	for (int j = 0; j < screen->w - RANDW ; j++) {
		for (int i = 0; i < screen->h ; i++) {
			putpixel(screen,j,i,zBufferCol[j][i]);
		}
	}
}
void CPixelDraw::zBuffer_init(SDL_Surface *screen, float fFerneClippingEbene, Uint32 col) {
	for (int j = 0; j < screen->w - RANDW ; j++) {
		for (int i = 0; i < screen->h ; i++) {
			zBufferZ[j][i] = fFerneClippingEbene;
			zBufferCol[j][i] = col;
		}
	}
}
void CPixelDraw::zBuffer_init(SDL_Surface *screen, float fFerneClippingEbene, SDL_Color col) {
	zBuffer_init(screen,fFerneClippingEbene,SDL_MapRGB(screen->format, col.r, col.g, col.b));
}
void CPixelDraw::zBuffer_init(SDL_Surface *screen, float fFerneClippingEbene) { // ohne mit hintergrundfarbe zu bestreichen
	for (int j = 0; j < screen->w - RANDW ; j++) {
		for (int i = 0; i < screen->h ; i++) {
			zBufferZ[j][i] = fFerneClippingEbene;
		}
	}
}


/*void CPixelDraw::fillSurface (SDL_Surface *surface, int iAnz, tbVector3 v[], Uint32 pixel, bool bFill) { // eine flaeche mit farbe fuellen
	float fx, fLeft, fUp, fy;
	int iA, x1, y1, x2, y2, iTmp1, iTmp2;
	int iOldY = -1;
	
	int dots[surface->w][surface->h];
	for (int i = 0; i < surface->w; i++) {
		for (int j = 0; j < surface->h; j++) {
			dots[i][j] = 0;
		}
	}
	for (int i = 0; i < iAnz; i++) {
		x1 = v[i].x; y1 = v[i].y;
		if (i == iAnz -1) {x2 = v[0].x; y2 = v[0].y;}
		else {x2 = v[i+1].x; y2 = v[i+1].y;}
		if (x2 < x1) {Change(&x1,&x2);Change(&y1,&y2);}//linien endpunkte tauschen, um immer die linken pixel zu haben
		if (Betrag(x2-x1) >= Betrag(y2-y1)) {
			fy = y1; if (Betrag(x2-x1) == 0) fUp = static_cast<float>(y2-y1); else fUp = static_cast<float>(y2-y1)/(Betrag(x2-x1));
			(x2 > x1) ? (iA = 1) : (iA = -1);
			for (int j = 0; j <= Betrag(x2 - x1); j++) {
				fy += fUp;
				iTmp1 = x1 + (j * iA);
				iTmp2 = Round(fy);
				putpixel(surface,iTmp1, iTmp2,pixel);
				if (Round(fy) != iOldY)	{
					if (iTmp1 < 0) iTmp1 = 0; if (iTmp1 >= surface->w) iTmp1 = surface->w -1;//damit es noch im feld ist
					if (iTmp2 < 0) iTmp2 = 0; if (iTmp2 >= surface->h) iTmp2 = surface->h -1;
					dots[iTmp1][iTmp2]++;
				}
				iOldY = Round(fy);
			}
		} else {
			fx = x1; if(Betrag(y2-y1) == 0) fLeft = static_cast<float>(x2-x1); else fLeft = static_cast<float>(x2-x1)/(Betrag(y2-y1));
			(y2 > y1) ? (iA = 1) : (iA = -1);
			for (int j = 0; j <= Betrag(y2 - y1); j++) {
				fx += fLeft;
				iTmp1 = Round(fx);
				iTmp2 = y1 + (j * iA);
				putpixel(surface, iTmp1, iTmp2,pixel);
				if (iTmp1 < 0) iTmp1 = 0; if (iTmp1 >= surface->w) iTmp1 = surface->w -1;
				if (iTmp2 < 0) iTmp2 = 0; if (iTmp2 >= surface->h) iTmp2 = surface->h -1;
				dots[iTmp1][iTmp2]++;
			}
		}
	}
	if (bFill) {
		SDL_Rect tmpRct[2];tmpRct[0] = GetRect(-1,-1,0,0);tmpRct[1] = GetRect(-1,-1,0,0);
		bool bTmp = false;
		for (int i = 0; i < surface->h; i++) {
			for (int j = 0; j < surface->w; j++) {
				if (dots[j][i] >= 1 && !bTmp) {
					if (dots[j][i] >= 2) { // noch leer
						dots[j][i]--;
						j--;
					} else {
						tmpRct[0].x = j;
						tmpRct[0].y = i;
						bTmp = true;
					}
				} else if (dots[j][i] >= 1 && bTmp) {
					tmpRct[1].x = j;
					tmpRct[1].y = i;
					drawline(surface,tmpRct[0].x,tmpRct[0].y,tmpRct[1].x,tmpRct[1].y,pixel);
					if (dots[j][i] >= 2) {
						tmpRct[0].x = j;
						tmpRct[0].y = i;
					} else {
						bTmp = false;
					}
				}
			}
			bTmp = false;
		}
	}
}*/
/*void CPixelDraw::fillCircle (SDL_Surface *surface, int mx, int my, float r, Uint32 pixel, bool bFill) {
	float fTmp = sqrt(r*r / 2);
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	for (int i = mx - fTmp; i < mx + fTmp; i++) {
		x1 = i;
		y1 = sqrt(r*r - (x1 - mx) * (x1 - mx)) + my;
		y2 = -sqrt(r*r - (x1 - mx) * (x1 - mx)) + my;
		if (bFill) {
			drawline(surface, x1, y1, x1, y2, pixel);
		} else {
			putpixel(surface, x1, y1, pixel);
			putpixel(surface, x1, y2, pixel);
		}
	}
	for (int i = my - fTmp; i < my + fTmp; i++) {
		y1 = i;
		x1 = sqrt(r*r - (y1 - my) * (y1 - my)) + mx;
		x2 = -sqrt(r*r - (y1 - my) * (y1 - my)) + mx;
		if (bFill) {
			drawline(surface, x1, y1, mx-fTmp, y1, pixel);
			drawline(surface, mx+fTmp, y1, x2, y1, pixel);
		} else {
			putpixel(surface, x1, y1, pixel);
			putpixel(surface, x2, y1, pixel);
		}
	}
}*/


void CPixelDraw::saveScreenshot (SDL_Surface *surface, string sPath) {
	/* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(surface) ) { if ( SDL_LockSurface(surface) < 0 ) {fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());throw "Can't lock screen"; return;}}
	ofstream dat_aus;
	char c[400];
	if (sPath == "standart") {
		ifstream dat_ein;
		int i = 0;
		dat_ein.open("screenshots/Anzahl.txt", ios_base::in);
		if (dat_ein) {
			while (!dat_ein.eof()) {
				dat_ein.getline (c,398);
				i = strtoul(c,NULL,NULL);
			}
			dat_ein.close();
		}
		i++; sprintf(c,"%i",i);
		dat_aus.open("screenshots/Anzahl.txt", ios_base::out | ios_base::trunc);
		dat_aus << c; dat_aus.close();
		sPath = "screenshots/screenshot"; sPath += c; sPath += ".scr";
	}
	dat_aus.open(sPath.data(), ios_base::out | ios_base::trunc);
	if (!dat_aus) return;
	dat_aus << surface->w << ";" << surface->h << ";";
	for (int i = 0; i < surface->h; i++) {
		for (int j = 0; j < surface->w; j++) {
			dat_aus << getpixel(surface,j,i) << ";";
		}
		dat_aus << endl;
	}
	
	dat_aus.close();
	if ( SDL_MUSTLOCK(surface) ) { SDL_UnlockSurface(surface);}
}

#ifndef CCOLUMN_H_
#define CCOLUMN_H_





const SDL_Color colStupid = {111,222,123};


class CColumn
{
public:
	CColumn(SDL_Surface *sur, int i1 = ALLW, int i2 = ALLH, int i3 = RANDW, bool bC = CLOSED);
	~CColumn();
	
	void create (float fH, int iAnz, ...); // einen koerper erstellen (mit ellipse)
	void create (float fH, int iAnz, tbVector3 v[]); // mit einem array
	void create (int iAnz, tbVector3 v1[], tbVector3 v2[]); // mit 2 arrays
	void render (const tbMatrix &mPro, const tbMatrix &mCam, SDL_Color col = colStupid); // koerper rendern
	void render (const tbMatrix &mPro, const tbMatrix &mCam, CColumn &column, SDL_Color col = colStupid); // koerper rendern
	void vTransform (tbVector3 &v1, tbVector3 &v2, const tbMatrix &mPro, const tbMatrix &mCam, Uint32 col); // passend zum rendern transformieren
	void vTransform (tbVector3 &v1, tbVector3 &v2, const tbMatrix &mPro, const tbMatrix &mCam, SDL_Color col); // passend zum rendern transformieren
	void lineToRand (tbVector3 &v1, tbVector3 &v2); // um linienendpunkte an den rand zu veschieben (1*1 laenge) falls sie ausserhalb sind
	void transform (tbMatrix& m); // koerper mit matrix transformieren
	void translateTo (tbVector3& v); // setzt auf einen punkt
	void setColor(int c); // farbe setzen
	void setStandartFigure(bool b); // standartfigur?
	void setMiddle(tbVector3 v); // mittelpunkt festsetzen
	void setPlane(bool b);// bPlane festsetzen
	bool getbPlane() {return bPlane;}
	//void setNaheClippingEbene(float f);//NaheClippingEbene festsetzen
	int getColAuswahl() {return colAuswahl;} // farbe zurueck
	string getSaveStr(); // speicherstring zum abspeichern in einer textdatei
	
	
	static float fNaheClippingEbene, fFerneClippingEbene;
	static int iAnzahl;
	static bool bFill;//true: fuellen; false: nur kanten
private:
	
	void getPlaneHits(CColumn &column);// schnitte durch ebene
	
	vector<tbVector3> vSurface1;
	vector<tbVector3> vSurface2;
	vector<tbVector3> vPlaneHits;
	vector<tbVector3>::iterator itvVec1;
	vector<tbVector3>::iterator itvVec2;
	vector<tbVector3>::iterator itvVec3;
	tbVector3 vMiddle;
	
	SDL_Surface *screen;
	SDL_Color colKante;
	SDL_Color colSchnitt;
	float h;
	bool bClosed, bPlane, bStandartFigure;
	bool bInFront; // nur tmp
	int iAllW, iAllH, iRandW, colAuswahl;
	
};

#endif /*CCOLUMN_H_*/



CColumn::CColumn(SDL_Surface *sur, int i1, int i2, int i3,bool bC) {
	screen = sur;
	bClosed = bC;
	iAllW = i1; iAllH = i2; iRandW = i3;
	colKante = colBlack;
	colAuswahl = 0;
	vMiddle = tbVector3(0.0f,0.0f,0.0f);
	iAnzahl++;
	bPlane = false; bStandartFigure = false;
}

CColumn::~CColumn() {
}

void CColumn::create(float fH, int iAnz, ...) {
	vSurface1.clear();
	vSurface2.clear();
	
	iAnz *= 3;//wegen coords
	
	va_list ap;
	va_start(ap, iAnz);
	
	tbVector3 tmp;
	
	for (int i = 0; i < iAnz; i++) {
		if (i % 3 == 0) tmp.x = va_arg(ap,double);
		if (i % 3 == 1) tmp.y = va_arg(ap,double);
		if (i % 3 == 2) {
			tmp.z = va_arg(ap,double);
			vSurface1.push_back(tmp);
			vSurface2.push_back(tmp);
			(vSurface2.back()).y -= fH;
		}
	}
	
	va_end(ap);
}

void CColumn::create (float fH, int iAnz, tbVector3 v[]) {
	vSurface1.clear();
	vSurface2.clear();
	for (int i = 0; i < iAnz; i++) {
		vSurface1.push_back(v[i]);
		vSurface2.push_back(v[i]);
		(vSurface2.back()).y -= fH;
	}
}

void CColumn::create (int iAnz, tbVector3 v1[], tbVector3 v2[]) {
	vSurface1.clear();
	vSurface2.clear();
	for (int i = 0; i < iAnz; i++) {
		vSurface1.push_back(v1[i]);
		vSurface2.push_back(v2[i]);
	}
}

void CColumn::transform (tbMatrix& m) {
	for (itvVec1 = vSurface1.begin(); itvVec1 != vSurface1.end(); itvVec1++) {
		(*itvVec1) -= vMiddle;
		(*itvVec1) = tbVector3TransformCoords((*itvVec1), m);
		(*itvVec1) += vMiddle;
	}
	for (itvVec2 = vSurface2.begin(); itvVec2 != vSurface2.end(); itvVec2++) {
		(*itvVec2) -= vMiddle;
		(*itvVec2) = tbVector3TransformCoords((*itvVec2), m);
		(*itvVec2) += vMiddle;
	}
	tbVector3 vTmp = vMiddle;
	vMiddle -= vTmp;
	vMiddle = tbVector3TransformCoords(vMiddle,m);
	vMiddle += vTmp;
}

void CColumn::render(const tbMatrix &mPro, const tbMatrix &mCam, SDL_Color col) {
	/* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(screen) ) {if ( SDL_LockSurface(screen) < 0 ) {fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());throw "Can't lock screen";return;}}
    
    g_pPixelDraw->setClippings(fNaheClippingEbene,fFerneClippingEbene);
   
    
	tbVector3 vTmp1, vTmp2, vTmp3, vTmp4, vTmp5, vTmp6;
	tbVector3 vaTmp1[4];
	
	int iTmp = 0, iTmp2 = 0;
	Uint32 col2;
	if (col.r != colStupid.r || col.g != colStupid.g || col.b != colStupid.b) col2 = SDL_MapRGB(screen->format, col.r, col.g, col.b);
	else col2 = SDL_MapRGB(screen->format, colKante.r, colKante.g, colKante.b);
	
	itvVec2 = vSurface2.begin();
	for (itvVec1 = vSurface1.begin(); itvVec1 != vSurface1.end(); itvVec1++) {
		vTmp1 = (*itvVec1); 
		vTmp2 = (*itvVec2);
		
		if(!bFill || bStandartFigure) vTransform(vTmp1,vTmp2,mPro,mCam,col2);
		else {
			vTmp5 = vTmp1; vTmp6 = vTmp2;
			vTransform(vTmp1,vTmp2,mPro,mCam,colBlack);
		}
		if (bFill && !bStandartFigure) {//versuch zum fuellen der flaechen
			if (bInFront) {
				if (iTmp2 == 0) {
					iTmp2++;
					vTmp3 = vTmp6;
					vTmp4 = vTmp5;
				}
				if (iTmp == 0) {
					vaTmp1[0] = vTmp5;
					vaTmp1[1] = vTmp6;
					iTmp++;
				} else {
					//vaTmp1[2] = vTmp6;
					//vaTmp1[3] = vTmp5;
					g_pPixelDraw->drawtriangleZ3D(getTriangle(vaTmp1[0],vaTmp1[1],vTmp6), col2, mPro, mCam);
					g_pPixelDraw->drawtriangleZ3D(getTriangle(vTmp6,vTmp5,vaTmp1[0]), col2, mPro, mCam);
					vaTmp1[0] = vTmp5;
					vaTmp1[1] = vTmp6;
				}
			} else { 
				iTmp = 0;
			}
		}
		itvVec2++;
	}
	if (bClosed && bFill && !bStandartFigure) { // form schliessen
		//vaTmp1[2] = vTmp3;
		//vaTmp1[3] = vTmp4;
		g_pPixelDraw->drawtriangleZ3D(getTriangle(vaTmp1[0],vaTmp1[1],vTmp3), col2, mPro, mCam);
		g_pPixelDraw->drawtriangleZ3D(getTriangle(vTmp3,vTmp4,vaTmp1[0]), col2, mPro, mCam);
	}
	
	vTmp5 = vSurface1.front();
	for (itvVec1 = vSurface1.begin(); itvVec1 != vSurface1.end(); itvVec1++) {
		vTmp1 = (*itvVec1);
		vTmp3 = vTmp1;
		itvVec1++;
		if (itvVec1 == vSurface1.end()) {
			if (bClosed) {
				vTmp2 = vSurface1.front();
				vTmp4 = vTmp2;
				itvVec1--;
				if (!bFill || bStandartFigure) vTransform(vTmp1,vTmp2,mPro,mCam,col2);
				else vTransform(vTmp1,vTmp2,mPro,mCam,colBlack);
			}
		} else {
			vTmp2 = (*itvVec1);
			vTmp4 = vTmp2;
			itvVec1--;
			if (!bFill || bStandartFigure) vTransform(vTmp1,vTmp2,mPro,mCam,col2);
			else vTransform(vTmp1,vTmp2,mPro,mCam,colBlack);
		}
		if (bFill && !bStandartFigure) {//versuch zum fuellen der flaechen
			if (bInFront) {
				g_pPixelDraw->drawtriangleZ3D(getTriangle(vTmp3,vTmp4,vTmp5), col2, mPro, mCam);
			}
		}
	}
	vTmp5 = vSurface2.front();
	for (itvVec2 = vSurface2.begin(); itvVec2 != vSurface2.end(); itvVec2++) {
		vTmp1 = (*itvVec2);
		vTmp3 = vTmp1;
		itvVec2++;
		if (itvVec2 == vSurface2.end()) {
			if (bClosed) {
				vTmp2 = vSurface2.front();
				vTmp4 = vTmp2;
				itvVec2--;
				if (!bFill || bStandartFigure) vTransform(vTmp1,vTmp2,mPro,mCam,col2);
				else vTransform(vTmp1,vTmp2,mPro,mCam,colBlack);
			}
		} else {
			vTmp2 = (*itvVec2);
			vTmp4 = vTmp2;
			itvVec2--;
			if (!bFill || bStandartFigure) vTransform(vTmp1,vTmp2,mPro,mCam,col2);
			else vTransform(vTmp1,vTmp2,mPro,mCam,colBlack);
		}
		if (bFill && !bStandartFigure) {//versuch zum fuellen der flaechen
			if (bInFront) {
				g_pPixelDraw->drawtriangleZ3D(getTriangle(vTmp3,vTmp4,vTmp5), col2, mPro, mCam);
			}
		}
	}
	//die schnitte durch ebene
	if (!bStandartFigure && static_cast<int>(vPlaneHits.size()) != 0) {
		col2 = SDL_MapRGB(screen->format, colSchnitt.r, colSchnitt.g, colSchnitt.b);
		for (itvVec3 = vPlaneHits.begin(); itvVec3 != vPlaneHits.end(); itvVec3++) {
			vTmp1 = (*itvVec3);
			itvVec3++;
			if (itvVec3 == vPlaneHits.end()) {
				if (bClosed) {
					vTmp2 = vPlaneHits.front();
					itvVec3--;
					vTransform(vTmp1,vTmp2,mPro,mCam,col2);
				}
			} else {
				vTmp2 = (*itvVec3);
				itvVec3--;
				vTransform(vTmp1,vTmp2,mPro,mCam,col2);
			}
		}
		vPlaneHits.clear();
	}
	
	if ( SDL_MUSTLOCK(screen) ) { SDL_UnlockSurface(screen);}
}

void CColumn::render(const tbMatrix &mPro, const tbMatrix &mCam, CColumn &column, SDL_Color col) {
	if (!bStandartFigure) {
		getPlaneHits(column);
		colSchnitt = allColor[column.getColAuswahl()];
	}
	
	render(mPro,mCam,col);
}
void CColumn::vTransform(tbVector3 &v1, tbVector3 &v2, const tbMatrix &mPro, const tbMatrix &mCam, SDL_Color col) {
	vTransform(v1,v2,mPro,mCam,SDL_MapRGB(screen->format, col.r, col.g, col.b));
}
void CColumn::vTransform(tbVector3 &v1, tbVector3 &v2, const tbMatrix &mPro, const tbMatrix &mCam, Uint32 col) {
	v1 = tbVector3TransformCoords(v1,mCam);
	v2 = tbVector3TransformCoords(v2,mCam);
	if((v1.z >= fNaheClippingEbene || v2.z >= fNaheClippingEbene) && (v1.z <= fFerneClippingEbene || v2.z <= fFerneClippingEbene)) {
		bInFront = true;
		
		if(v1.z < fNaheClippingEbene)/*50 ist probiert*/
			pointOnLineToZ(v1,v2,fNaheClippingEbene);
		if(v2.z < fNaheClippingEbene)
			pointOnLineToZ(v2,v1,fNaheClippingEbene);
		if(v1.z > fFerneClippingEbene)/*100000 ist probiert*/
			pointOnLineToZ(v1,v2,fFerneClippingEbene);
		if(v2.z > fFerneClippingEbene)
			pointOnLineToZ(v2,v1,fFerneClippingEbene);
			
		g_pPixelDraw->drawlineZ3D(v1,v2,col,mPro);
		
	} else {bInFront = false;}
}

void CColumn::getPlaneHits(CColumn &column) {
	vPlaneHits.clear();
	
	tbVector3 vTmp1, vTmp2, vTmp3;
	S_Plane plane;
	int i = 0;
	for (column.itvVec1 = column.vSurface1.begin(); column.itvVec1 != column.vSurface1.end() && i < 3; column.itvVec1++) {
		i++;
		if (i == 1) vTmp1 = (*(column.itvVec1));
		if (i == 2) vTmp2 = (*(column.itvVec1));
		if (i == 3) vTmp3 = (*(column.itvVec1));
	}
	
	plane = getPlane(vTmp1,vTmp2,vTmp3);
	
	itvVec2 = vSurface2.begin();
	for (itvVec1 = vSurface1.begin(); itvVec1 != vSurface1.end(); itvVec1++) {
		vTmp1 = (*itvVec1); 
		vTmp2 = (*itvVec2);
		
		if (tbLineHitsPlane(vTmp1,vTmp2,plane,&vTmp3)) {
			vPlaneHits.push_back(vTmp3);
		}
		
		itvVec2++;		
	}
}

void CColumn::lineToRand(tbVector3 &v1, tbVector3 &v2) {
	/*tbVector3 vv3(1.0f,1.0f,0.0f), vv4(5.0f,1.0f,0.0f), vv1(6.0f,3.0f,0.0f), vv2(0.0f,-2.0f,0.0f), vv5(0.0f,0.0f,0.0f);
	lineKollision(vv1,vv2,vv3,vv4,&vv5);
	VCOUT(vv5);
	SDL_Delay(50000);*///test
	if (!lineKollision(v1,v2,tbVector3(1.0f,1.0f,0.0f)  ,tbVector3(1.0f,-1.0f,0.0f))  &&
		!lineKollision(v1,v2,tbVector3(1.0f,-1.0f,0.0f) ,tbVector3(-1.0f,-1.0f,0.0f)) &&
		!lineKollision(v1,v2,tbVector3(-1.0f,-1.0f,0.0f),tbVector3(-1.0f,1.0f,0.0f))  && 
		!lineKollision(v1,v2,tbVector3(-1.0f,1.0f,0.0f) ,tbVector3(1.0f,1.0f,0.0f))    ) return;  
	
	
	tbVector3 vTmp1 = v1, vTmp2 = v2;
	if (v1.x < -1.0f || v1.x > 1.0f || v1.y < -1.0f || v1.y > 1.0) {
		if (v1.x < -1.0f || v1.x > 1.0f) {
			if (v1.x < -1.0f) v1.y = getLinePointY(v1,v2,-1.0f);
			if (v1.x >  1.0f) v1.y = getLinePointY(v1,v2, 1.0f);
			v1.x /= Betrag(v1.x);
		}
		if (v1.y < -1.0f || v1.y > 1.0f) {
			VCOUT(v1);
			if (v1.y < -1.0f) v1.x = getLinePointX(v1,v2,-1.0f);
			if (v1.y >  1.0f) v1.x = getLinePointX(v1,v2, 1.0f);
			v1.y /= Betrag(v1.y);
			VCOUT(v1);
		}
		vTmp1 = v1;
	}
	if (v2.x < -1.0f || v2.x > 1.0f || v2.y < -1.0f || v2.y > 1.0) {
		if (v2.x < -1.0f || v2.x > 1.0f) {
			if (v2.x < -1.0f) v2.y = getLinePointY(v2,v1,-1.0f);
			if (v2.x >  1.0f) v2.y = getLinePointY(v2,v1, 1.0f);
			v2.x /= Betrag(v2.x);
		}
		if (v2.y < -1.0f || v2.y > 1.0f) {
			if (v2.y < -1.0f) v2.x = getLinePointX(v2,v1,-1.0f);
			if (v2.y >  1.0f) v2.x = getLinePointX(v2,v1, 1.0f);
			v2.y /= Betrag(v2.y);
		}
		vTmp2 = v2;
	}
}

void CColumn::translateTo(tbVector3& v) {
	tbVector3 v2 = vMiddle - v;
	vMiddle -= v2;
	for (itvVec1 = vSurface1.begin(); itvVec1 != vSurface1.end(); itvVec1++) {
		(*itvVec1) -= v2;
	}	
	for (itvVec2 = vSurface2.begin(); itvVec2 != vSurface2.end(); itvVec2++) {
		(*itvVec2) -= v2;
	}
}

void CColumn::setColor(int c) {colKante = allColor[c];colAuswahl = c;}
void CColumn::setMiddle(tbVector3 v) {vMiddle = v;}
void CColumn::setPlane(bool b) {bPlane = b;}
void CColumn::setStandartFigure(bool b) {bStandartFigure = b;}
//void CColumn::setNaheClippingEbene(float f) {fNaheClippingEbene = f;}

string CColumn::getSaveStr() {
	string str;
	char ch[400];
	if(bPlane)sprintf(ch,"%i;1;%i;%i;%f;%f;%f;",colAuswahl,static_cast<int>(vSurface1.size()),static_cast<int>(vSurface2.size()),vMiddle.x,vMiddle.y,vMiddle.z);
	else sprintf(ch,"%i;0;%i;%i;%f;%f;%f;",colAuswahl,static_cast<int>(vSurface1.size()),static_cast<int>(vSurface2.size()),vMiddle.x,vMiddle.y,vMiddle.z);
	str += ch;
	for (itvVec1 = vSurface1.begin(); itvVec1 != vSurface1.end(); itvVec1++) {
		sprintf(ch,"%f,%f,%f,",(*itvVec1).x,(*itvVec1).y,(*itvVec1).z);
		str += ch;
	}	
	for (itvVec2 = vSurface2.begin(); itvVec2 != vSurface2.end(); itvVec2++) {
		sprintf(ch,"%f,%f,%f,",(*itvVec2).x,(*itvVec2).y,(*itvVec2).z);
		str += ch;
	}
	
	return str;
}

#ifndef MAIN
#define MAIN

//main 
//swante scholz 16.03.09
//object_simulator
//ein programm zum anzeigen 3-demensionaler objekte






Uint32 color;
SDL_Surface *screen;
SDL_Rect rctScreen;
SDL_Rect rctRand;
CTextBox TextBox, Logfile;
SDL_Event event;

tbVector3 vCam1(400.0f,750.0f,-1500.0f), vCam2(0.0f,0.0f,0.0f), vCam3(0.0f,1.0f,0.0f), vCenter(0.0f,0.0f,0.0f)/*gelbe kiste*/;
tbMatrix mProjection(tbMatrixProjection(TB_DEG_TO_RAD(60.0f),static_cast<float>(ALLW-RANDW) / ALLH,100.0f, 10000.0f));
tbMatrix mCamera(tbMatrixCamera(vCam1, vCam2, vCam3));
tbMatrix mRotationX(tbMatrixRotationX(TB_DEG_TO_RAD(0.0f)));
tbMatrix mRotationY(tbMatrixRotationY(TB_DEG_TO_RAD(0.0f)));
tbMatrix mRotationZ(tbMatrixRotationZ(TB_DEG_TO_RAD(0.0f)));
tbMatrix mRotation(mRotationX * mRotationY * mRotationZ);
tbMatrix mTranslation(tbMatrixTranslation(tbVector3(0.0f,0.0f,0.0f)));
tbMatrix mScaling(tbMatrixScaling(tbVector3(0.0f,0.0f,0.0f)));

struct S_KoordinatenSystem {
	list<CColumn> lFigur;
} systemNormal;
list<CColumn>::iterator itlF;

struct S_Options {
	int iSpeedTranslationInPercent;
	int iSpeedTranslationInPercentCam;
	int iSpeedRotationInPercent;
	int iSpeedRotationInPercentCam;
	int iSpeedScalingInPercent;
	int iSpeedScalingInPercentCam;
	int iCameraStyle; // der style der camera
	float fNaheClippingEbene; // entfernung zu nahen clipping-ebene
	float fFerneClippingEbene; // entfernung zu fernen clipping-ebene
	bool bClearScreen; // soll der screen "gestrichen" werden
	char chSavePathObject[400]; // standart speicher-pfad (§ wird spaeter durch text ersetzt)
	char chSavePathScreen[400];
	SDL_Color colBack; // farbe fuer hintergrund
	bool bShowCoordinateSystem; // koordinatensystem zeigen?
	bool bFullscreen; // vollbildmodus?
} options;


bool bUp = false, bDown = false, bLeft = false, bRight = false, bQuit = false, bRender = false, bColChange = false, bPlaneDa = false,
		bQ = false, bW = false, bE = false, bA = false, bS = false, bD = false,
		bR = false, bT = false, bZ = false, bF = false, bG = false, bH = false,
		bK4 = false, bK5 = false, bK6 = false, bK1 = false, bK2 = false, bK3 = false,
		bLStrg = false, bLAlt = false, bLShift = false, 
		bU = false, bI = false, bO = false, bJ = false, bK = false, bL = false;
double dEllapsed = 0.0f, dLastTime = SDL_GetTicks() / 1000.0f, dCurTime = SDL_GetTicks() / 1000.0f;
int iAuswahl = STANDARTFIGURES+1, iRender = 0, colAuswahl = 0;
float CColumn::fNaheClippingEbene, CColumn::fFerneClippingEbene;
int CColumn::iAnzahl = 0;
bool CColumn::bFill = false;

void Eingabe();
void TimerUpdate() {dCurTime = SDL_GetTicks() / 1000.0f; dEllapsed = dCurTime - dLastTime; dLastTime = dCurTime;}
void Update();
void Render();
void Delete();
void Hacken();
void Copy();
void LoadObject(const char *chPfad);
void Save(const char *chPfad);
int Load(const char *chPfad);
void SaveOptions();
void LoadOptions();
void SetColFromObject();
string GetSavePath (char ch[400], string sPath);
bool file_da(const char *ch) {ifstream ifs; ifs.open(ch, ios_base::in); if (ifs) {ifs.close();return true;} return false;}

int main (int argc, char **argv) {
	CPixelDraw::Get();
	
	time_t zeit;//zeit
	time (&zeit);
	srand((unsigned)zeit);//zufallsgenerator initialisieren
	
	try {
		if(SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1) //SDL Initialisierung
			throw "SDL konnte nicht initialisiert werden"; 
		TTF_Init ();//Text
		
		LoadOptions(); // optionen laden
		
		//fenster
		screen = NULL;
		if (options.bFullscreen) 	screen = SDL_SetVideoMode (ALLW, ALLH, 24, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN); // Das gesamte Fenster
		else						screen = SDL_SetVideoMode (ALLW, ALLH, 24, SDL_HWSURFACE | SDL_DOUBLEBUF); // Das gesamte Fenster
		//fenstername
		SDL_WM_SetCaption("object_simulator | 2009 by Swante Scholz", "object_simulator");//titel
		
		
		color = SDL_MapRGB(screen->format, 0, 0, 0);
		CColumn::fNaheClippingEbene = options.fNaheClippingEbene;
		CColumn::fFerneClippingEbene = options.fFerneClippingEbene;
		Fast_Rect(rctScreen,0,0,ALLW-RANDW,ALLH);
		Fast_Rect(rctRand,ALLW-RANDW,0,RANDW,ALLH);
		
		CPixelDraw::Get()->zBuffer_init(screen,options.fFerneClippingEbene,options.colBack); // zBuffer initialisieren
		
		TextBox.init(&screen,rctRand,10,14,10,colOrange);
		Logfile.init(&screen,rctRand,10,14,10,colOrange);
		#ifdef BETRIEBSSYSTEM_LINUX
			TextBox.setBetriebssystem(TEXTBOX_LINUX);
			Logfile.setBetriebssystem(TEXTBOX_LINUX);
		#endif
		TextBox.open();
		
		TextBox.write("Programmstart", colBlack);
		Logfile.write("Programmstart", colBlack);
		
		CColumn Figur(screen); // klasseninstanz die immer wieder in die liste geschoben wird
		Figur.setStandartFigure(true);
		//standartfiguren
		tbVector3 v1(50.0f,50.0f,50.0f),v2(50.0f,50.0f,-50.0f),v3(-50.0f,50.0f,-50.0f),v4(-50.0f,50.0f,50.0f);
		Figur.create(100,4,COORDS(v1),COORDS(v2),COORDS(v3),COORDS(v4));
		systemNormal.lFigur.push_back(Figur); // koordinatenursprung (gelb)
		tbVector3 av1[1]; tbVector3 av2[1];
		av1[0] = tbVector3(-3000.0f,0.0f,0.0f); av2[0] = tbVector3(3000.0f,0.0f,0.0f);
		Figur.create(1,av1,av2);
		systemNormal.lFigur.push_back(Figur); // x-achse (rot)
		av1[0] = tbVector3(0.0f,-3000.0f,0.0f); av2[0] = tbVector3(0.0f,3000.0f,0.0f);
		Figur.create(1,av1,av2);
		systemNormal.lFigur.push_back(Figur); // y-achse (gruen)
		av1[0] = tbVector3(0.0f,0.0f,-3000.0f); av2[0] = tbVector3(0.0f,0.0f,3000.0f);
		Figur.create(1,av1,av2);
		systemNormal.lFigur.push_back(Figur); // z-achse (blau)
		v1 = tbVector3(3000.0f,0.0f,3000.0f);v2=tbVector3(3000.0f,0.0f,-3000.0f);v3=tbVector3(-3000.0f,0.0f,-3000.0f);v4=tbVector3(-3000.0f,0.0f,3000.0f);
		Figur.create(0,4,COORDS(v1),COORDS(v2),COORDS(v3),COORDS(v4));
		systemNormal.lFigur.push_back(Figur); // kordinatenquadrat auf x- und z - achse (braun)
		Figur.setStandartFigure(false);
		v1 = tbVector3(500.0f,500.0f,500.0f);v2=tbVector3(500.0f,500.0f,-500.0f);v3=tbVector3(-500.0f,500.0f,-500.0f);v4=tbVector3(-500.0f,500.0f,500.0f);
		Figur.create(1000,4,COORDS(v1),COORDS(v2),COORDS(v3),COORDS(v4));
		systemNormal.lFigur.push_back(Figur); // startfigur (wuerfel)
		
		//zum beginn streichen und rendern
		FILL(rctScreen,colWhite);
		Render();
		
		while (!bQuit) { // hauptschleife
			//if (dCurTime > 5 && dCurTime < 6) printf ("FPS: %f\n",1/dEllapsed);
			TimerUpdate();
			Eingabe();
			Update();
			
			//if(bRender) 
			Render(); // jetzt immer rendern
		}
	}
	
	catch (const char *Fehlerstring) {
		cout << "Ausnahme aufgetreten: " << Fehlerstring << "\nSpeicher wird freigegeben und das Programm sauber heruntergefahren\n";
	}
	
	CPixelDraw::Del();
	
	Logfile.save("logfiles/logfile.txt");
	
	TextBox.close();
	TextBox.quit();
	
	TTF_Quit();
	SDL_Quit();
	
	return 0;	
}

void Eingabe() { // eingaben abfragen
	while(SDL_PollEvent(&event)) {//EingabeAbfrage
		switch(event.type){
			case SDL_KEYDOWN: {//Tast unten?
				iRender++;
				switch(event.key.keysym.sym){
					case SDLK_UP:		{bUp 	= true;}break;
					case SDLK_DOWN:		{bDown 	= true;}break;
					case SDLK_LEFT:		{bLeft 	= true;}break;
					case SDLK_RIGHT:	{bRight = true;}break;
					case SDLK_ESCAPE:	{bQuit 	= true;}break;
					case SDLK_q:		{bQ		= true;}break;
					case SDLK_w:		{bW		= true;}break;
					case SDLK_e:		{bE		= true;}break;
					case SDLK_a:		{bA		= true;}break;
					case SDLK_s:		{bS		= true;}break;
					case SDLK_d:		{bD		= true;}break;
					case SDLK_r:		{bR		= true;}break;
					case SDLK_t:		{bT		= true;}break;
					#ifdef BETRIEBSSYSTEM_WINDOWS 
						case SDLK_y:		{bZ		= true;}break;
						case SDLK_z:		{}				break;
					#else
						#ifdef BETRIEBSSYSTEM_LINUX
						case SDLK_y:		{}				break;
						case SDLK_z:		{bZ		= true;}break;
						#endif
					#endif
					case SDLK_f:		{bF		= true;}break;
					case SDLK_g:		{bG		= true;}break;
					case SDLK_h:		{bH		= true;}break;
					case SDLK_u:		{bU		= true;}break;
					case SDLK_i:		{bI		= true;}break;
					case SDLK_o:		{bO		= true;}break;
					case SDLK_j:		{bJ		= true;}break;
					case SDLK_k:		{bK		= true;}break;
					case SDLK_l:		{bL		= true;}break;
					case SDLK_KP4:		{bK4	= true;}break;
					case SDLK_KP5:		{bK5	= true;}break;
					case SDLK_KP6:		{bK6	= true;}break;
					case SDLK_KP1:		{bK1	= true;}break;
					case SDLK_KP2:		{bK2	= true;}break;
					case SDLK_KP3:		{bK3	= true;}break;
					case SDLK_LCTRL:	{bLStrg	= true;}break;
					case SDLK_LALT:		{bLAlt	= true;}break;
					case SDLK_LSHIFT:	{Switch(bLShift);}break;
					case SDLK_1:		{iAuswahl--   ;SetColFromObject();	}break;
					case SDLK_2:		{iAuswahl++   ;SetColFromObject();	}break;
					case SDLK_3:		{colAuswahl-- ;bColChange = true;	}break;
					case SDLK_4:		{colAuswahl++ ;bColChange = true;	}break;
					case SDLK_5:		{Switch(CColumn::bFill);}break;
					case SDLK_6:		{}break;
					case SDLK_7:		{}break;
					case SDLK_8:		{}break;
					case SDLK_9:		{vCam2 = vCenter;}break;
					case SDLK_0:		{g_pPixelDraw->saveScreenshot(screen);	}break;
					case SDLK_PERIOD:	{g_pPixelDraw->zBuffer_init(screen,options.fFerneClippingEbene,options.colBack);}break;
					case SDLK_c:		{Copy()		  ;}break;
					case SDLK_BACKSPACE:{Delete()	  ;}break;
					case SDLK_KP0:		{Hacken()	  ;}break;
					case SDLK_TAB:		{ // reset
						while(static_cast<int>(systemNormal.lFigur.size()) > STANDARTFIGURES) systemNormal.lFigur.pop_back();
						vCenter = tbVector3(0.0f,0.0f,0.0f);
						(systemNormal.lFigur.front()).translateTo(vCenter);
						LoadObject((GetSavePath(options.chSavePathObject,"wuerfel")).data());
						if (options.iCameraStyle == 0) vCam2 = vCenter;
					} break;
					default: {} break;
				}
			} break;
			case SDL_KEYUP: {
				iRender--;
				if (!bLShift) {
					switch(event.key.keysym.sym){
						case SDLK_UP:		{bUp 	= false;}break;
						case SDLK_DOWN:		{bDown 	= false;}break;
						case SDLK_LEFT:		{bLeft 	= false;}break;
						case SDLK_RIGHT:	{bRight = false;}break;
						case SDLK_q:		{bQ		= false;}break;
						case SDLK_w:		{bW		= false;}break;
						case SDLK_e:		{bE		= false;}break;
						case SDLK_a:		{bA		= false;}break;
						case SDLK_s:		{bS		= false;}break;
						case SDLK_d:		{bD		= false;}break;
						case SDLK_r:		{bR		= false;}break;
						case SDLK_t:		{bT		= false;}break;
						#ifdef BETRIEBSSYSTEM_WINDOWS 
							case SDLK_y:		{bZ		= false;}break;
							case SDLK_z:		{}				 break;
						#else
							#ifdef BETRIEBSSYSTEM_LINUX
							case SDLK_y:		{}				 break;
							case SDLK_z:		{bZ		= false;}break;
							#endif
						#endif
						case SDLK_f:		{bF		= false;}break;
						case SDLK_g:		{bG		= false;}break;
						case SDLK_h:		{bH		= false;}break;
						case SDLK_u:		{bU		= false;}break;
						case SDLK_i:		{bI		= false;}break;
						case SDLK_o:		{bO		= false;}break;
						case SDLK_j:		{bJ		= false;}break;
						case SDLK_k:		{bK		= false;}break;
						case SDLK_l:		{bL		= false;}break;
						case SDLK_KP4:		{bK4	= false;}break;
						case SDLK_KP5:		{bK5	= false;}break;
						case SDLK_KP6:		{bK6	= false;}break;
						case SDLK_KP1:		{bK1	= false;}break;
						case SDLK_KP2:		{bK2	= false;}break;
						case SDLK_KP3:		{bK3	= false;}break;
						case SDLK_LCTRL:	{bLStrg	= false;}break;
						case SDLK_LALT:		{bLAlt	= false;}break;
						default: {} break;
					}
				}
			} break;
			case SDL_QUIT: {bQuit = true;} break;
		}
	}
}

void Update() { // alles aktualisieren
	//rendern?
	bRender = (iRender > 0);
	//auswahlen anpassen
	if (iAuswahl > static_cast<int>(systemNormal.lFigur.size())) iAuswahl = STANDARTFIGURES+1;
	if (iAuswahl < STANDARTFIGURES+1) iAuswahl = static_cast<int>(systemNormal.lFigur.size());
	if (colAuswahl > ANZCOLORS - 1) colAuswahl = ANZCOLORS - 1;
	if (colAuswahl < 0) colAuswahl = 0;
	
	//farbe wechseln
	if (bColChange == true) {
		bColChange = false;
		list<CColumn>::iterator i;
		int iTmp = 0;
		for (i = systemNormal.lFigur.begin(); i != systemNormal.lFigur.end(); i++) {
			iTmp++;
			if (iTmp == iAuswahl) (*i).setColor(colAuswahl);
		}
	}
	
	//translation
	tbVector3 vTrans(0.0f,0.0f,0.0f);
	if (bK4) vTrans += 		tbVector3(0.0f,-TRANSLATIONPERSEC*dEllapsed,0.0f);
	else if (bK6) vTrans +=	tbVector3(0.0f,TRANSLATIONPERSEC*dEllapsed,0.0f);
	if (bK2) vTrans += 		tbVector3(0.0f,0.0f,-TRANSLATIONPERSEC*dEllapsed);
	else if (bK5) vTrans +=	tbVector3(0.0f,0.0f,TRANSLATIONPERSEC*dEllapsed);
	if (bK1) vTrans += 		tbVector3(-TRANSLATIONPERSEC*dEllapsed,0.0f,0.0f);
	else if (bK3) vTrans +=	tbVector3(TRANSLATIONPERSEC*dEllapsed,0.0f,0.0f);
	if (bK4 || bK5 || bK6 || bK1 || bK2 || bK3) mTranslation = tbMatrix(tbMatrixTranslation(vTrans));
	
	//rotation
	mRotationY = tbMatrix(tbMatrixIdentity());mRotationZ = mRotationY;mRotationX = mRotationY;//matrizen zuruecksetzen
	if (bQ) mRotationY = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(-ROTATIONPERSEC*dEllapsed)));
	else if (bE) mRotationY = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(ROTATIONPERSEC*dEllapsed)));
	if (bS) mRotationZ = tbMatrix(tbMatrixRotationZ(TB_DEG_TO_RAD(-ROTATIONPERSEC*dEllapsed)));
	else if (bW) mRotationZ = tbMatrix(tbMatrixRotationZ(TB_DEG_TO_RAD(ROTATIONPERSEC*dEllapsed)));
	if (bA) mRotationX = tbMatrix(tbMatrixRotationX(TB_DEG_TO_RAD(-ROTATIONPERSEC*dEllapsed)));
	else if (bD) mRotationX = tbMatrix(tbMatrixRotationX(TB_DEG_TO_RAD(ROTATIONPERSEC*dEllapsed)));
	if (bQ || bE || bS || bW || bA || bD) mRotation = mRotationX * mRotationY * mRotationZ;
	
	//scalierung
	tbVector3 vScale(1.0f,1.0f,1.0f);
	if (bR) vScale += 		tbVector3(0.0f,(-SCALINGPERSEC)*dEllapsed,0.0f);
	else if (bZ) vScale +=	tbVector3(0.0f,SCALINGPERSEC*dEllapsed,0.0f);
	if (bG) vScale += 		tbVector3(0.0f,0.0f,-SCALINGPERSEC*dEllapsed);
	else if (bT) vScale +=	tbVector3(0.0f,0.0f,SCALINGPERSEC*dEllapsed);
	if (bF) vScale += 		tbVector3(-SCALINGPERSEC*dEllapsed,0.0f,0.0f);
	else if (bH) vScale +=	tbVector3(SCALINGPERSEC*dEllapsed,0.0f,0.0f);
	if (bR || bZ || bG || bT || bF || bH) mScaling = tbMatrix(tbMatrixScaling(vScale));
	
	//camera
	tbMatrix mSca, mRot, mRot2, mTra;
	tbVector3 vTmp = vCenter, vTmp2(0.0f,0.0f,0.0f), vTmp3(0.0f,0.0f,0.0f), vTmp4(0.0f,0.0f,0.0f);
	if (options.iCameraStyle == 0) { // style 0 (centered) ?
		if (bDown) 		mSca = tbMatrix(tbMatrixScaling(tbVector3(1+(SCALINGPERSECCAM*dEllapsed),1+(SCALINGPERSECCAM*dEllapsed),1+(SCALINGPERSECCAM*dEllapsed))));
		else if (bUp) 	mSca = tbMatrix(tbMatrixScaling(tbVector3(1+(-SCALINGPERSECCAM*dEllapsed),1+(-SCALINGPERSECCAM*dEllapsed),1+(-SCALINGPERSECCAM*dEllapsed))));
		if (bLeft) 				mRot = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(-ROTATIONPERSECCAM*dEllapsed)));
		else if (bRight) 		mRot = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(ROTATIONPERSECCAM*dEllapsed)));
		if (bLStrg)				vCam1 +=	tbVector3(0.0f,-TRANSLATIONPERSECCAM*dEllapsed,0.0f);
		else if (bLAlt)			vCam1 +=	tbVector3(0.0f,TRANSLATIONPERSECCAM*dEllapsed,0.0f);
		if (bLeft || bRight) 	{vCam1 -= vCam2; vCam1 = tbVector3TransformCoords(vCam1,mRot); vCam3 = tbVector3TransformCoords(vCam3,mRot); vCam1 += vCam2;} 
		if (bDown || bUp) 		{vCam1 -= vCam2; vCam1 = tbVector3TransformCoords(vCam1,mSca); vCam1 += vCam2;}
	} else if (options.iCameraStyle == 1) { // style 1 (mobil) ?
		if (bDown)			{vCam2 -= vCam1;vTmp3.y = 1.0f; vTmp4 = tbVector3Normalize(vCam2); vTmp3 = tbVector3Cross(vTmp4,vTmp3);vTmp4 = vCam2; mRot2 = tbMatrixRotationAxis(vTmp3,TB_DEG_TO_RAD(-ROTATIONPERSECCAM*dEllapsed)); vCam2 = tbVector3TransformCoords(vCam2,mRot2);if (Vorzeichen(vTmp4.x) != Vorzeichen(vCam2.x)) {vCam2 = vTmp4;} vCam2 += vCam1;}
		else if (bUp)		{vCam2 -= vCam1;vTmp3.y = 1.0f; vTmp4 = tbVector3Normalize(vCam2); vTmp3 = tbVector3Cross(vTmp4,vTmp3);vTmp4 = vCam2; mRot2 = tbMatrixRotationAxis(vTmp3,TB_DEG_TO_RAD(ROTATIONPERSECCAM*dEllapsed)); vCam2 = tbVector3TransformCoords(vCam2,mRot2);if (Vorzeichen(vTmp4.x) != Vorzeichen(vCam2.x)) {vCam2 = vTmp4;} vCam2 += vCam1;}
		if (bLeft)			mRot = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(-ROTATIONPERSECCAM*dEllapsed)));
		else if (bRight)	mRot = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(ROTATIONPERSECCAM*dEllapsed)));
		if (bLStrg)			{vTmp2 = vCam2 - vCam1; vTmp2 = tbVector3Normalize(vTmp2); vTmp2 *= (TRANSLATIONPERSECCAM * dEllapsed); vCam1 += vTmp2; vCam2 += vTmp2;}
		else if (bLAlt)		{vTmp2 = vCam2 - vCam1; vTmp2 = tbVector3Normalize(vTmp2); vTmp2 *= (TRANSLATIONPERSECCAM * dEllapsed); vCam1 -= vTmp2; vCam2 -= vTmp2;}
		if (bLeft || bRight){vCam2 -= vCam1; vCam2 = tbVector3TransformCoords(vCam2,mRot); vCam2 += vCam1;}
	} else if (options.iCameraStyle == 2) { // style 2 (plane) ?
		if (bDown)			{vCam2 -= vCam1; vTmp4 = tbVector3Normalize(vCam2); vTmp3 = tbVector3Cross(vTmp4,tbVector3Normalize(vCam3)); mRot2 = tbMatrixRotationAxis(vTmp3,TB_DEG_TO_RAD(ROTATIONPERSECCAM*dEllapsed)); vCam2 = tbVector3TransformCoords(vCam2,mRot2); vCam3 = tbVector3TransformCoords(vCam3,mRot2); vCam2 += vCam1;}
		else if (bUp)		{vCam2 -= vCam1; vTmp4 = tbVector3Normalize(vCam2); vTmp3 = tbVector3Cross(vTmp4,tbVector3Normalize(vCam3)); mRot2 = tbMatrixRotationAxis(vTmp3,TB_DEG_TO_RAD(-ROTATIONPERSECCAM*dEllapsed)); vCam2 = tbVector3TransformCoords(vCam2,mRot2);vCam3 = tbVector3TransformCoords(vCam3,mRot2); vCam2 += vCam1;}
		if (bLeft)			mRot = tbMatrix(tbMatrixRotationAxis(vCam2 - vCam1,TB_DEG_TO_RAD(ROTATIONPERSECCAM*dEllapsed)));
		else if (bRight)	mRot = tbMatrix(tbMatrixRotationAxis(vCam2 - vCam1,TB_DEG_TO_RAD(-ROTATIONPERSECCAM*dEllapsed)));
		if (bLStrg)			{vTmp2 = vCam2 - vCam1; vTmp2 = tbVector3Normalize(vTmp2); vTmp2 *= (TRANSLATIONPERSECCAM * dEllapsed); vCam1 += vTmp2; vCam2 += vTmp2;}
		else if (bLAlt)		{vTmp2 = vCam2 - vCam1; vTmp2 = tbVector3Normalize(vTmp2); vTmp2 *= (TRANSLATIONPERSECCAM * dEllapsed); vCam1 -= vTmp2; vCam2 -= vTmp2;}
		if (bLeft || bRight){vCam3 = tbVector3TransformCoords(vCam3,mRot);}
	}
	
	if (bU)					vCenter += tbVector3(0.0f,-TRANSLATIONPERSECCAM*dEllapsed,0.0f);
	else if (bO)			vCenter += tbVector3(0.0f,TRANSLATIONPERSECCAM*dEllapsed,0.0f);
	if (bJ)					vCenter += tbVector3(-TRANSLATIONPERSECCAM*dEllapsed,0.0f,0.0f);
	else if (bL)			vCenter += tbVector3(TRANSLATIONPERSECCAM*dEllapsed,0.0f,0.0f);
	if (bK)					vCenter += tbVector3(0.0f,0.0f,-TRANSLATIONPERSECCAM*dEllapsed);
	else if (bI)			vCenter += tbVector3(0.0f,0.0f,TRANSLATIONPERSECCAM*dEllapsed);
	if (bU || bO || bJ || bL || bK || bI) { // gelben wuerfel an Center bzw. camerazoomstelle (vCam2) stellen
		mTra = tbMatrix(tbMatrixTranslation(-vTmp));
		(systemNormal.lFigur.front()).transform(mTra);
		mTra = tbMatrix(tbMatrixTranslation(vCenter));
		(systemNormal.lFigur.front()).transform(mTra);
		if (options.iCameraStyle == 0) { // nur bei modus 1
			vCam2 = vCenter;
		}
	}
	mCamera = tbMatrix(tbMatrixCamera(vCam1, vCam2, vCam3));
	
	//object transformieren
	int iTmp = 0;
	for (itlF = systemNormal.lFigur.begin(); itlF != systemNormal.lFigur.end(); itlF++) {
		iTmp++;
		if (iTmp == iAuswahl) {
			if (bQ || bE || bS || bW || bA || bD) 		(*itlF).transform(mRotation);
			if (bK4 || bK5 || bK6 || bK1 || bK2 || bK3) (*itlF).transform(mTranslation);
			if (bR || bZ || bG || bT || bF || bH)		(*itlF).transform(mScaling);
		}
	}
}

void Hacken() { // komplexes textmenue
	TextBox.up(2); TextBox.write("Eingabe Start"); TextBox.up();
	
	bool bExit = false;
	string str;
	char ch[400];
	int i = -1;
	int i2 = -1;
	
	while (bExit == false) {
		while (i == -1) {
			TextBox.write("Menue"); TextBox.up();
			TextBox.write("[0]Exit");
			TextBox.write("[1]Load Object");
			TextBox.write("[2]Load Screen");
			TextBox.write("[3]Save Screen");
			TextBox.write("[4]ObjectSpeed");
			TextBox.write("[5]CameraSpeed");
			TextBox.write("[6]CameraStyle");
			TextBox.write("[7]Configure SavePathObj");
			TextBox.write("[8]Configure SavePathScn");
			TextBox.write("[9]Clipping-Ebenen");
			TextBox.write("[10]Streichen");
			TextBox.write("[11]Background");
			TextBox.write("[12]ShowCoordinateSystem");
			TextBox.write("[13]Fullscreen");
			TextBox.write("[14]Save Options");
			
			i = TextBox.getInt("Wahl:");
			
			switch (i) {
				case 0: bExit = true; break;
				case 1:
					TextBox.getStr(&str, "Name des Objekts:");
					str = GetSavePath(options.chSavePathObject,str);
					if (file_da(str.data())) {LoadObject(str.data()); TextBox.write("objekt geladen");}
					else TextBox.write("objekt existiert nicht");
					break;
				case 2:
					TextBox.getStr(&str, "Name des Screens:");
					str = GetSavePath(options.chSavePathScreen,str);
					if (Load (str.data()) == -1) TextBox.write("screen existiert nicht");
					else TextBox.write("screen erfolgreich geladen");
					break;
				case 3:
					TextBox.getStr(&str, "Speichername:");
					str = GetSavePath(options.chSavePathScreen,str);
					Save (str.data());
					TextBox.write("screen erfolgreich gespeichert");
					break;
				case 4:
					i2 = -1;
					while (i2 == -1) {
						TextBox.write("[0]Back");
						TextBox.write("[1]Object Translation Speed");
						TextBox.write("[2]Object Rotation Speed");
						TextBox.write("[3]Object Scaling Speed");
						
						i2 = TextBox.getInt("Wahl:");
						
						switch (i2) {
							case 0: break;
							case 1:
								sprintf(ch,"Actual TranslationSpeed: %i%s", options.iSpeedTranslationInPercent,"%");
								TextBox.write(ch);
								options.iSpeedTranslationInPercent = TextBox.getInt("new speed:");
								break;
							case 2:
								sprintf(ch,"Actual RotationSpeed: %i%s", options.iSpeedRotationInPercent,"%");
								TextBox.write(ch);
								options.iSpeedRotationInPercent = TextBox.getInt("new speed:");
								break;
							case 3:
								sprintf(ch,"Actual ScalingSpeed: %i%s", options.iSpeedScalingInPercent,"%");
								TextBox.write(ch);
								options.iSpeedScalingInPercent = TextBox.getInt("new speed:");
								break;
							default: i2 = -1; break;
						}
					}
					break;
				case 5:
					i2 = -1;
					while (i2 == -1) {
						TextBox.write("[0]Back");
						TextBox.write("[1]Camera Translation Speed");
						TextBox.write("[2]Camera Rotation Speed");
						TextBox.write("[3]Camera Scaling Speed");
						
						i2 = TextBox.getInt("Wahl:");
						
						switch (i2) {
							case 0: break;
							case 1:
								sprintf(ch,"Actual TranslationSpeed: %i%s", options.iSpeedTranslationInPercentCam,"%");
								TextBox.write(ch);
								options.iSpeedTranslationInPercentCam = TextBox.getInt("new speed:");
								break;
							case 2:
								sprintf(ch,"Actual RotationSpeed: %i%s", options.iSpeedRotationInPercentCam,"%");
								TextBox.write(ch);
								options.iSpeedRotationInPercentCam = TextBox.getInt("new speed:");
								break;
							case 3:
								sprintf(ch,"Actual ScalingSpeed: %i%s", options.iSpeedScalingInPercentCam,"%");
								TextBox.write(ch);
								options.iSpeedScalingInPercentCam = TextBox.getInt("new speed:");
								break;
							default: i2 = -1; break;
						}
					}
					break;
				case 6:
					i2 = -1;
					while (i2 == -1) {
						TextBox.write("[0]Back");
						TextBox.write("[1]Centered");
						TextBox.write("[2]Mobil");
						TextBox.write("[3]Plane");
						
						i2 = TextBox.getInt("Wahl:");
						
						switch (i2) {
							case 0: break;
							case 1: vCam2 = vCenter; vCam3 = tbVector3(0.0f,1.0f,0.0f); break;
							case 2: vCam2 = vCenter; vCam3 = tbVector3(0.0f,1.0f,0.0f); break;
							case 3: vCam2 = vCenter; vCam3 = tbVector3(0.0f,1.0f,0.0f); break;
							default: i2 = -1; break;
						}
					}
					if (i2 != 0) {
						options.iCameraStyle = i2 - 1;
						TextBox.write("CameraStyle changed");
					}
					break;
				case 7:
					sprintf(ch,"Actual SavePath(§ -> text): %s", options.chSavePathObject);
					TextBox.write(ch);
					TextBox.getStr(&str,"New SavePath:");
					sprintf(options.chSavePathObject,"%s",str.data());
					break;
				case 8:
					sprintf(ch,"Actual SavePath(§ -> text): %s", options.chSavePathScreen);
					TextBox.write(ch);
					TextBox.getStr(&str,"New SavePath:");
					sprintf(options.chSavePathScreen,"%s",str.data());
					break;
				case 9:
					i2 = -1;
					while (i2 == -1) {
						TextBox.write("[0]Back");
						TextBox.write("[1]Nahe Clipping-Ebene");
						TextBox.write("[2]Ferne Clipping-Ebene");
						
						i2 = TextBox.getInt("Wahl:");
						
						switch (i2) {
							case 0: break;
							case 1:
								sprintf(ch,"Actual Nahe Clipping-Ebene: %f", options.fNaheClippingEbene);
								TextBox.write(ch);
								options.fNaheClippingEbene = static_cast<float> (TextBox.getInt("neuer Abstand:"));
								CColumn::fNaheClippingEbene = options.fNaheClippingEbene;
								break;
							case 2:
								sprintf(ch,"Actual Ferne Clipping-Ebene: %f", options.fFerneClippingEbene);
								TextBox.write(ch);
								options.fFerneClippingEbene = static_cast<float> (TextBox.getInt("neuer Abstand:"));
								CColumn::fFerneClippingEbene = options.fFerneClippingEbene;
								break;
							default: i2 = -1; break;
						}
					}
					break;
				case 10:
					TextBox.write("[0]Back");
					TextBox.write("[1]On");
					TextBox.write("[2]Off");
					i2 = TextBox.getInt("Wahl:");
					if (i2 == 0) {
					} else if (i2 == 1) {
						options.bClearScreen = true;
					} else {
						options.bClearScreen = false;
					}
					break;
				case 11:
					i2 = -1;
					while (i2 == -1) {
						TextBox.write("[0]Back");
						TextBox.write("[1]r");
						TextBox.write("[2]g");
						TextBox.write("[3]b");
						
						i2 = TextBox.getInt("Wahl:");
							
						switch (i2) {
							case 0: break;
							case 1: options.colBack.r = TextBox.getInt("new r: "); break;
							case 2: options.colBack.g = TextBox.getInt("new g: "); break;
							case 3: options.colBack.b = TextBox.getInt("new b: "); break;
							default: i2 = -1; break;
						}
					}
					if (i2 != 0) {
						TextBox.write("Background changed");
					}
					break;
				case 12:
					TextBox.write("[0]Back");
					TextBox.write("[1]On");
					TextBox.write("[2]Off");
					i2 = TextBox.getInt("Wahl: ");
					if (i2 == 0) {}
					else if (i2 == 1) options.bShowCoordinateSystem = true;
					else if (i2 == 2) options.bShowCoordinateSystem = false;
					else TextBox.write("falsche Eingabe");
					break;
				case 13:
					TextBox.write("[0]Back");
					TextBox.write("[1]On");
					TextBox.write("[2]Off");
					i2 = TextBox.getInt("Wahl: ");
					if (i2 == 0) {}
					else if (i2 == 1) options.bFullscreen = true;
					else if (i2 == 2) options.bFullscreen = false;
					else TextBox.write("falsche Eingabe");
					break;
				case 14:
					SaveOptions();
					TextBox.write("Optionen erfolgreich gespeichert");
					break;
				default: TextBox.write("Falsche Eingabe!"); break;
			}
			if (!bExit) i = -1;
		}
	}
	
	TextBox.write("Eingabe Ende");
}

void Render() { // rendern
	// weiss streichen
	if (options.bClearScreen) {
		g_pPixelDraw->zBuffer_init(screen,options.fFerneClippingEbene,options.colBack);
	} else {
		g_pPixelDraw->zBuffer_init(screen,options.fFerneClippingEbene);
	}
	
	//ebene finden (momentan nur eine moeglich; ***todo!!!)
	list<CColumn>::iterator itlF2; bool b = false;
	for (itlF2 = systemNormal.lFigur.begin(); itlF2 != systemNormal.lFigur.end() && b == false; itlF2++) {
		if ((*itlF2).getbPlane()) {b = true; itlF2--;}
	}
	
	int iTmp = 0;
	for (itlF = systemNormal.lFigur.begin(); itlF != systemNormal.lFigur.end(); itlF++) {
		iTmp++;
		if (iTmp == 1) 		{ if(options.bShowCoordinateSystem) (*itlF).render(mProjection,mCamera,colYellow);}
		else if (iTmp == 2) { if(options.bShowCoordinateSystem) (*itlF).render(mProjection,mCamera,colRed);}
		else if (iTmp == 3) { if(options.bShowCoordinateSystem) (*itlF).render(mProjection,mCamera,colGreen);}
		else if (iTmp == 4) { if(options.bShowCoordinateSystem) (*itlF).render(mProjection,mCamera,colBlue);}
		else if (iTmp == 5) { if(options.bShowCoordinateSystem) (*itlF).render(mProjection,mCamera,colBrown);}
		else if (iTmp == iAuswahl) {
			if (!b) (*itlF).render(mProjection,mCamera,colPurple);
			else 	(*itlF).render(mProjection,mCamera,(*itlF2),colPurple);
		}
		else {
			if (!b) (*itlF).render(mProjection,mCamera);
			else	(*itlF).render(mProjection,mCamera,(*itlF2));
		}
	}
	
	g_pPixelDraw->zBuffer_flip(screen);//die pixel ins screen laden
	SDL_UpdateRect(screen, rctScreen.x, rctScreen.y, rctScreen.w, rctScreen.h); // ist vielleicht schneller als flippen
}

void LoadObject(const char *chPfad) { // ein object aus einem textfile laden
	CColumn Figur(screen);
	ifstream dat_ein;
	dat_ein.open(chPfad, ios_base::in);
	char ch[200];
	int iZeile = 0, i = 0;
	bool bPlane = false;
	dat_ein.getline (ch,398);
	int iEcken = strtoul(ch,NULL,NULL);
	dat_ein.getline (ch,398);
	i = strtoul(ch,NULL,NULL);
	if (i == 0) bPlane = false; else if(bPlaneDa == false) {bPlane = true;bPlaneDa = true;}
	tbVector3 av1[iEcken];
	tbVector3 av2[iEcken];
	while (! dat_ein.eof() ) {
		iZeile++;
		if (iZeile == iEcken + 1) dat_ein.getline (ch,398);
		else if (iZeile <= iEcken) {
			dat_ein.getline (ch,398,',');
			av1[iZeile-1].x = strtod(ch,NULL);
			dat_ein.getline (ch,398,',');
			av1[iZeile-1].y = strtod(ch,NULL);
			dat_ein.getline (ch,398);
			av1[iZeile-1].z = strtod(ch,NULL);
		} else if (iZeile > iEcken + 1) {
			dat_ein.getline (ch,398,',');
			av2[iZeile-(iEcken + 2)].x = strtod(ch,NULL);
			dat_ein.getline (ch,398,',');
			av2[iZeile-(iEcken + 2)].y = strtod(ch,NULL);
			dat_ein.getline (ch,398);
			av2[iZeile-(iEcken + 2)].z = strtod(ch,NULL);
		}
	}
	Figur.create(iEcken,av1,av2);
	Figur.setPlane(bPlane);
	tbMatrix mTmp(tbMatrixTranslation(vCenter));
	Figur.transform(mTmp); // figur auf center ziehen
	systemNormal.lFigur.push_back(Figur);
}

void Delete() { // markierte figur loeschen
	if (static_cast<int>(systemNormal.lFigur.size()) > STANDARTFIGURES+1) {
		list<CColumn>::iterator i;
		int iTmp = 0;
		for (i = systemNormal.lFigur.begin(); i != systemNormal.lFigur.end(); i++) {
			iTmp++;
			if (iTmp == iAuswahl) {
				systemNormal.lFigur.erase(i);
				iAuswahl--;
				return;
			}
		}
	}
}

void Copy() { // markierte figur ans ende der  liste kopieren
	list<CColumn>::iterator i;
	int iTmp = 0;
	for (i = systemNormal.lFigur.begin(); i != systemNormal.lFigur.end(); i++) {
		iTmp++;
		if (iTmp == iAuswahl) systemNormal.lFigur.push_back((*i));
	}
}

void SetColFromObject() { // farbe bekommen
	//auswahl anpassen (2.)
	if (iAuswahl > static_cast<int>(systemNormal.lFigur.size())) iAuswahl = STANDARTFIGURES+1;
	if (iAuswahl < STANDARTFIGURES+1) iAuswahl = static_cast<int>(systemNormal.lFigur.size());
	
	list<CColumn>::iterator i;
	int iTmp = 0;
	for (i = systemNormal.lFigur.begin(); i != systemNormal.lFigur.end(); i++) {
		iTmp++;
		if (iTmp == iAuswahl) {colAuswahl = (*i).getColAuswahl();}
	}
}

void Save (const char *chPfad) { // erstellte szenerie abstpeichern
	ofstream dat_aus;
	dat_aus.open(chPfad, ios_base::out | ios_base::trunc);
	string str;
	
	list<CColumn>::iterator i;
	for (i = systemNormal.lFigur.begin(); i != systemNormal.lFigur.end(); i++) {
		str += (*i).getSaveStr();
	}
	dat_aus << str.data();
	dat_aus.close();
}

int Load (const char *chPfad) { // abgespeicherte szenerie laden
	if (file_da(chPfad)) {
		systemNormal.lFigur.clear();
		ifstream dat_ein;
		dat_ein.open(chPfad, ios_base::in);
		char ch[400];
		bool bOnce = false;
		CColumn Figur(screen);
		int iObjects = 0;
		int iAnz = 0;
		int iPlane = 0;
		tbVector3 av1[200];
		tbVector3 av2[200];
		tbVector3 vMiddle;
		while (!dat_ein.eof()) {
			iObjects++;
			dat_ein.getline (ch,398,';');
			Figur.setColor(strtoul(ch,NULL,NULL));
			dat_ein.getline (ch,398,';');
			iPlane = strtoul(ch,NULL,NULL);
			if (iPlane == 0) Figur.setPlane(false);
			else Figur.setPlane(true);
			dat_ein.getline (ch,398,';');
			iAnz = strtoul(ch,NULL,NULL);
			dat_ein.getline (ch,398,';');
			//2. Anz nicht benoetigt
			dat_ein.getline (ch,398,';');
			vMiddle.x = strtod(ch,NULL);
			dat_ein.getline (ch,398,';');
			vMiddle.y = strtod(ch,NULL);
			dat_ein.getline (ch,398,';');
			vMiddle.z = strtod(ch,NULL);
			for (int i = 0; i < iAnz; i++) {
				dat_ein.getline (ch,398,',');
				av1[i].x = strtod(ch,NULL);
				dat_ein.getline (ch,398,',');
				av1[i].y = strtod(ch,NULL);
				dat_ein.getline (ch,398,',');
				av1[i].z = strtod(ch,NULL);
			}
			for (int i = 0; i < iAnz; i++) {
				dat_ein.getline (ch,398,',');
				av2[i].x = strtod(ch,NULL);
				dat_ein.getline (ch,398,',');
				av2[i].y = strtod(ch,NULL);
				dat_ein.getline (ch,398,',');
				av2[i].z = strtod(ch,NULL);
			}
			Figur.create(iAnz,av1,av2);
			Figur.setMiddle(vMiddle);
			if (iObjects <= STANDARTFIGURES) 	Figur.setStandartFigure(true);
			else 								Figur.setStandartFigure(false);
			systemNormal.lFigur.push_back(Figur);
			if (bOnce == false) {bOnce = true; vCam2 = vMiddle;} // die mitte des wuerfel fuer die camera
		}
		systemNormal.lFigur.pop_back();//lesefehler korrigieren
		dat_ein.close();
		iAuswahl = STANDARTFIGURES+1;
		colAuswahl = 0;
	} else return -1;
	return 0;
}

void SaveOptions() {
	ofstream dat_aus;
	dat_aus.open("options/options.bin", ios::binary);
	dat_aus.write ((char*) &options, sizeof (options));
	dat_aus.close();
}

void LoadOptions() {
	ifstream dat_ein;
	dat_ein.open("options/options.bin", ios::binary);
	dat_ein.read((char*) &options, sizeof (options));
	if (dat_ein == NULL) {
		options.iSpeedTranslationInPercent = 100;
		options.iSpeedTranslationInPercentCam = 100;
		options.iSpeedRotationInPercent = 100;
		options.iSpeedRotationInPercentCam = 100;
		options.iSpeedScalingInPercent = 100;
		options.iSpeedScalingInPercentCam = 100;
		options.iCameraStyle = 0;
		options.fNaheClippingEbene = 50.0f;
		options.fFerneClippingEbene = 100000.0f;
		options.bClearScreen = true;
		sprintf(options.chSavePathObject,"save/objects/§.obt");
		sprintf(options.chSavePathScreen,"save/screens/§.scn");
		options.colBack = colWhite;
		options.bShowCoordinateSystem = true;
		options.bFullscreen = false;
		return;
	}
	dat_ein.close();
}

string GetSavePath (char *ch, string sPath) {
	string s = ch;
	unsigned int iPos = static_cast<int> (s.find ("§",0));
	
	#ifdef BETRIEBSSYSTEM_WINDOWS
		if (iPos != string::npos) s.replace (iPos, 1, sPath.data(), static_cast<int> (sPath.length()));
	#else
		#ifdef BETRIEBSSYSTEM_LINUX
			if (iPos != string::npos) s.replace (iPos, 2/*ich weiss nicht, warum 2 (bei linux 2, bei windows 1)*/, sPath.data(), static_cast<int> (sPath.length()));
		#endif
	#endif
	
	else s += sPath;
	return s;
}

#endif

