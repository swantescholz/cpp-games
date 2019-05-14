#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

//namespaces
namespace nSDL_Colors { // standart farben
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
}
namespace nSDL_Defs {
	#define SAFE_FREEN(X){if(X!=NULL){SDL_FreeSurface(X);X=NULL;}}
	#define FAST_RECT(A,X,Y,W,H){A.x=X;A.y=Y;A.w=W;A.h=H;}
	#define FAST_COLOR(A,R,G,B) {A.r=R;A.g=G;A.b=B;}
}
namespace nMath {
	#define BETRAG(I)((I) < 0.0f ? ((I)*(-1)) : (I))
	#define MIN(A,B)((A) < (B) ? (A) : (B))
	#define MAX(A,B)((A) > (B) ? (A) : (B))
	#define PI 3.1415926535897f
	#define SWITCH(B){if(B==true){B=false;}else{B=true;}}
	#define ZUFALL(A,B){((rand()%(B-A))+A)}
	#define KOLLISION(A,B) (A).x<(B).x+(B).w&&(A).x+(A).w>(B).x&&(A).y<(B).y+(B).h&&(A).y+(A).h>(B).y   //kollisionscheck von 2 rects
}

//feste defines
#define AKTVERSION 1.08
#define ZEIGERBALLW 10
#define ZEIGERBALLH 10
#define CURSORW 32
#define CURSORH 32
#define PLAYERW 40
#define PLAYERH 40
#define BLOCKW 40
#define BLOCKH 40
#define GOALW 40
#define GOALH 80
#define BOXW 150
#define BOXH iAllH
//lange defines
#define BLA cout<<"bla"<<endl;
#define FILL(RCT,COL){SDL_FillRect (SDL_GetVideoSurface(), &RCT, SDL_MapRGB (SDL_GetVideoSurface()->format, COL.r, COL.g, COL.b));bFlip = true;}
#define FILLRECT(COL) {SDL_FillRect (screen, NULL, SDL_MapRGB (screen->format, COL.r, COL.g, COL.b));bFlip = true;}//alles
#define BLIT(SUR1,RCT1,SUR2,RCT2){SDL_BlitSurface(SUR1,RCT1,SUR2,RCT2);bFlip = true;}
#define NEWSTONE(ART,X,Y,G,RI,L,R,CS){S_Block sBlock2;sBlock2.Art = ART;if(sBlock2.Art!=1){sBlock2.SetzZeitpunkt=0;}else{sBlock2.SetzZeitpunkt=CurTime;}if(sBlock2.Art==3){FAST_RECT(sBlock2.rctBlock,X,Y,BLOCKW,GOALH);}else{FAST_RECT(sBlock2.rctBlock,X,Y,BLOCKW,BLOCKH);}sBlock2.bGeradeGesetzt=G;FAST_RECT(sBlock2.rctLastBlock,0,0,0,0);sBlock2.iCloudStartX=X;sBlock2.iCloudStartY=Y;sBlock2.iCloudRichtung=RI;sBlock2.iCloudL=L;sBlock2.iCloudR=R;sBlock2.dCloudPosX=sBlock2.rctBlock.x;sBlock2.dCloudPosY=sBlock2.rctBlock.y;sBlock2.bRichtungWechsel=false;sBlock2.bStehtDrauf=false;sBlock2.iCloudSpeed=CS;lBlock.push_back(sBlock2);} 

#include <iostream>

#include <list>
#include <vector>
#include <fstream>
#include <time.h>
#include <string>

using namespace std; 
using namespace nSDL_Colors;
using namespace nSDL_Defs;
using namespace nMath;

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

#ifndef CTEXTBOX_H_
#define CTEXTBOX_H_
class CTextBox
{
public:
	CTextBox();
	~CTextBox();
	
	void init(SDL_Surface **sur, SDL_Rect rct, int IFONT1, int IFONT2, int IFONT3, SDL_Color COL);
	void quit();
	
	void open() {bOpen = true;bFlip = true;SDL_FillRect (*screen, &rctBox, SDL_MapRGB ((*screen)->format, Color[COL_BACK].r, Color[COL_BACK].g, Color[COL_BACK].b));} //fenster oeffnen
	void close() {bOpen = false;bFlip = true;SDL_FillRect (*screen, &rctBox, SDL_MapRGB ((*screen)->format, Color[COL_BLACK].r, Color[COL_BLACK].g, Color[COL_BLACK].b));} //fenster schliessen
	
	void load(const char *chPfad); //laden
	void save(const char *chPfad); //speichern
	
	void up(int i = 1);
	void down(int i = 1);
	
	void write(const char *chText, SDL_Color COL = colBlack, bool bFrage = false); // textausgabe
	
	int getInt(const char *chFrage = "Zahl eingeben: ", SDL_Color COL = colRed); //integer eingabe
	int getStr(string *chText, const char *chFrage = "Text eingeben ", SDL_Color COL = colRed); //char abfrage
	
	void clear() {lText.clear();} //vektor leeren
	void erase();
	int size() {return static_cast<int> (lText.size());}
	
	void update(bool bRendern = true);
	
	bool bOpen, bFlip;
	int iZeilen;// anzahl max im bild
private:

	void eingabe(int typ, int *pZahl, string *sText);
	void render();
	
	SDL_Surface **screen;
	SDL_Surface *ttfsurText;
	SDL_Color Color[3]; //hintergrund
	enum{COL_BACK = 0, COL_BLACK, COL_WHITE};
	SDL_Rect rctBox;//bereich
	SDL_Event event;
	TTF_Font *pFont[3];
	
	struct S_Text {
		string sText;
		SDL_Rect rctText;
		SDL_Color colText;
		int sizeArt;
	} sText2;
	list<S_Text> lText; // hauptvektor
	list<S_Text>::iterator itS_Text; //iterator
	
	int iFont[4];//nur die groessen
	
	enum{FONT_NORMAL = 0, FONT_NEW, FONT_ASK, FONT_MAX};
	
	time_t zeit;//zeit
};
#endif /*CTEXTBOX_H_*/

CTextBox::CTextBox():
bOpen (false), bFlip(false), screen (NULL), ttfsurText (NULL) {
	FAST_COLOR(Color[COL_WHITE],255,255,255);
	FAST_COLOR(Color[COL_BLACK],0,0,0);
	bOpen = false;
}

CTextBox::~CTextBox() {
}

void CTextBox::init(SDL_Surface **sur, SDL_Rect rct, int IFONT1, int IFONT2, int IFONT3, SDL_Color COL) {
	screen = sur;
	rctBox = rct;
	iFont[FONT_NORMAL]=IFONT1; iFont[FONT_NEW]=IFONT2; iFont[FONT_ASK]=IFONT3;
	iFont[FONT_MAX] = MAX((MAX(iFont[FONT_NORMAL], iFont[FONT_ASK])), iFont[FONT_NEW]);
	/*schriftart*/
	for (int i = 0; i < 3; i++) {pFont[i] = NULL; pFont[i] = TTF_OpenFont ("media/FreeSans.ttf", iFont[i]);}
	/*hintergrundfarbe*/
	FAST_COLOR(Color[COL_BACK],COL.r,COL.g,COL.b);
	iZeilen = rctBox.h/iFont[FONT_MAX];
}

void CTextBox::quit() {
	iFont[FONT_NORMAL]=0; iFont[FONT_NEW]=0; iFont[FONT_ASK]=0;
	clear();
}

void CTextBox::load(const char *chPfad) {
	clear();//liste leeren
	
	ifstream dat_ein;
	dat_ein.open (chPfad, ios_base::in);
	
	char chZeichen[400];
	
	while (! dat_ein.eof() ) {
		dat_ein.getline (chZeichen,398,'#');
		
  	  	for (int i = 1; i < 10; i++) {
  	  		if (i != 9) dat_ein.getline (chZeichen,398,',');
  	  		else dat_ein.getline (chZeichen,398,'#');
  	  			
  	  		if (i == 1) sText2.sText = chZeichen;
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
	lText.pop_back(); //lesefehler korrigieren
	
	dat_ein.close();
	
	up(); //damit ein absatz da ist
}

void CTextBox::save(const char *chPfad) {
	ofstream dat_aus;
	dat_aus.open(chPfad, ios_base::out);
	
	int i = 0;
	
	for (itS_Text = lText.begin(); itS_Text!=lText.end(); itS_Text++) {
		i++;
		dat_aus << "#" << (*itS_Text).sText << "," << (*itS_Text).sizeArt << "," << static_cast<int> ((*itS_Text).colText.r) << ",";
		dat_aus << static_cast<int> ((*itS_Text).colText.g) << "," << static_cast<int> ((*itS_Text).colText.b) << "," << (*itS_Text).rctText.x << "," << (*itS_Text).rctText.y << ",";
		dat_aus << (*itS_Text).rctText.w << "," << (*itS_Text).rctText.h << "\t#";
		if (i!=static_cast<int> (lText.size())) dat_aus << "\n";
	}
	
	dat_aus.close(); 
}

void CTextBox::write(const char *chText, SDL_Color COL, bool bFrage) {
	if (bOpen == true) {
		//Datum
		struct tm *LokalZeit;
		time (&zeit);
		LokalZeit = localtime(&zeit);
		
		char ch[400];
		
		sprintf(ch,"%s   %i-%i-%i_%i-%i-%i", chText, LokalZeit->tm_hour,LokalZeit->tm_min, LokalZeit->tm_sec,
		 LokalZeit->tm_mday, LokalZeit->tm_mon+1, LokalZeit->tm_year+1900);//alle daten manuell gemacht, weil beim standart ein zeilenumbruch kommt den man nicht abspeichern kann
		
		sText2.sText = ch;
		
		if (bFrage == false) sText2.sizeArt = FONT_NEW; else sText2.sizeArt = FONT_ASK;
		FAST_COLOR(sText2.colText,COL.r,COL.g,COL.b);
		FAST_RECT(sText2.rctText,rctBox.x,rctBox.h-iFont[FONT_MAX],rctBox.w,iFont[FONT_NEW]);
		
		for(itS_Text = lText.begin(); itS_Text != lText.end(); ++itS_Text) {
			(*itS_Text).rctText.y -= iFont[FONT_MAX];
			if ((*itS_Text).sizeArt == FONT_NEW) (*itS_Text).sizeArt = FONT_NORMAL;
		}
		
		lText.push_back(sText2);
		
		update();
	}
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
	bool bFertig = false, bTaste = false, bShift = false;
	string sLastText;
	char chText[400];
	
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
							case SDLK_BACKSPACE:  *pZahl/=10; break;
							case SDLK_RETURN: bFertig = true; break;
							default: {bTaste = false;} break;
						}
					if (typ == 2)
						switch(event.key.keysym.sym){
							case SDLK_1: if (bShift == false) sLastText += "1";break;
							case SDLK_2: sLastText += "2";break;
							case SDLK_3: sLastText += "3";break;
							case SDLK_4: sLastText += "4";break;
							case SDLK_5: sLastText += "5";break;
							case SDLK_6: sLastText += "6";break;
							case SDLK_7: sLastText += "7";break;
							case SDLK_8: sLastText += "8";break;
							case SDLK_9: sLastText += "9";break;
							case SDLK_0: sLastText += "0";break;
							
							case SDLK_a: if(bShift== false)sLastText += "a";else sLastText+="A";break;
							case SDLK_b: if(bShift== false)sLastText += "b";else sLastText+="B";break;
							case SDLK_c: if(bShift== false)sLastText += "c";else sLastText+="C";break;
							case SDLK_d: if(bShift== false)sLastText += "d";else sLastText+="D";break;
							case SDLK_e: if(bShift== false)sLastText += "e";else sLastText+="E";break;
							case SDLK_f: if(bShift== false)sLastText += "f";else sLastText+="F";break;
							case SDLK_g: if(bShift== false)sLastText += "g";else sLastText+="G";break;
							case SDLK_h: if(bShift== false)sLastText += "h";else sLastText+="H";break;
							case SDLK_i: if(bShift== false)sLastText += "i";else sLastText+="I";break;
							case SDLK_j: if(bShift== false)sLastText += "j";else sLastText+="J";break;
							case SDLK_k: if(bShift== false)sLastText += "k";else sLastText+="K";break;
							case SDLK_l: if(bShift== false)sLastText += "l";else sLastText+="L";break;
							case SDLK_m: if(bShift== false)sLastText += "m";else sLastText+="M";break;
							case SDLK_n: if(bShift== false)sLastText += "n";else sLastText+="N";break;
							case SDLK_o: if(bShift== false)sLastText += "o";else sLastText+="O";break;
							case SDLK_p: if(bShift== false)sLastText += "p";else sLastText+="P";break;
							case SDLK_q: if(bShift== false)sLastText += "q";else sLastText+="Q";break;
							case SDLK_r: if(bShift== false)sLastText += "r";else sLastText+="R";break;
							case SDLK_s: if(bShift== false)sLastText += "s";else sLastText+="S";break;
							case SDLK_t: if(bShift== false)sLastText += "t";else sLastText+="T";break;
							case SDLK_u: if(bShift== false)sLastText += "u";else sLastText+="U";break;
							case SDLK_v: if(bShift== false)sLastText += "v";else sLastText+="V";break;
							case SDLK_w: if(bShift== false)sLastText += "w";else sLastText+="W";break;
							case SDLK_x: if(bShift== false)sLastText += "x";else sLastText+="X";break;
							case SDLK_y: if(bShift== false)sLastText += "z";else sLastText+="Z";break;//wegen am. tastatur
							case SDLK_z: if(bShift== false)sLastText += "y";else sLastText+="Y";break;//auch
							
							case SDLK_LSHIFT: case SDLK_RSHIFT: bShift = true; break;
							case SDLK_BACKSPACE: sLastText.erase(static_cast<int> (sLastText.length() ) -1); break;
							case SDLK_RETURN: bFertig = true; break;
							default: {bTaste = false;} break;
						}
				} break;
				
				case SDL_KEYUP: {
					if (typ == 1) {}
					if (typ == 2)
						switch(event.key.keysym.sym){
							case SDLK_LSHIFT: case SDLK_RSHIFT: bShift = false; break;
							default: {} break;
						}
				} break;
				
				default: {} break;
			}
		}
		if (bTaste == true) {
			bTaste = false;
			SDL_FillRect (*screen, &rctText, SDL_MapRGB ((*screen)->format, Color[COL_BACK].r, Color[COL_BACK].g, Color[COL_BACK].b));
			
			SAFE_FREEN(ttfsurText);//alte surface freen
			
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
		sprintf(chText,"%s",sLastText.data());
		write(chText, colBrown);
		*sText = sLastText;
	}
}

void CTextBox::update(bool bRender) {
	bFlip = true;
	
	//anzahl der nachrichten auf 150 begrenzen
	if( static_cast<int> (lText.size()) > 150) for (int i = static_cast<int> (lText.size()) % 150; i > 0; i--) lText.pop_front();
	
	if (bRender == true) render();
	SDL_UpdateRect(*screen, rctBox.x, rctBox.y, rctBox.w, rctBox.h);//textbox updaten
}

void CTextBox::render() {
	SDL_FillRect (*screen, &rctBox, SDL_MapRGB ((*screen)->format, Color[COL_BACK].r, Color[COL_BACK].g, Color[COL_BACK].b));
	
	for(itS_Text = lText.begin(); itS_Text != lText.end(); ++itS_Text) {
		if ((*itS_Text).rctText.y >= rctBox.y && (*itS_Text).rctText.y < rctBox.y + rctBox.h && (*itS_Text).rctText.x >= rctBox.x && (*itS_Text).rctText.x < rctBox.x + rctBox.w) {
			SAFE_FREEN(ttfsurText);//alte surface freen
			ttfsurText = TTF_RenderUTF8_Blended(pFont[(*itS_Text).sizeArt], (*itS_Text).sText.data(), (*itS_Text).colText);
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

void CTextBox::erase() {
	for(itS_Text = lText.begin(); itS_Text != lText.end(); ++itS_Text)
		if ((*itS_Text).sizeArt == FONT_NEW) lText.erase(itS_Text);
}

#ifndef CTIMER_H_
#define CTIMER_H_
class CTimer
{
public:
	CTimer();
protected:
	double TimerUpdate(int anz = 1);//timer aktualisieren
	void SetStopTime (double StopTime) {Zeit3 = StopTime;}
	double GetCurTime () {return CurTime;}
	void TimerInit();
	
	double Zeit3;//stopzeit
	double CurTime;//vergangene zeit
	double newTime;//Zeit bei neustart
	
	struct S_Times//struktur für die verschiedenenereignisse
	{
		double Ellapsed;
		double LastTime;
	};
	S_Times AllTime;//insgesamt
	S_Times BlitTime;//fuer rendern
	S_Times UpdateTime;//fuer die updates
	S_Times EingabeTime;//fuer die eingabe
	S_Times FillTime;//fuer die fillrects
};
#endif /*CTIMER_H_*/

CTimer::CTimer()
{
}

double CTimer :: TimerUpdate (int anz){//timer aktualisieren
	CurTime = ((SDL_GetTicks()-Zeit3) / 1000.0f) - newTime;//curtime - gestoppte zeit und neue zeit
	
	AllTime.Ellapsed = CurTime - AllTime.LastTime;
	AllTime.LastTime = CurTime;
	
	BlitTime.Ellapsed = CurTime - BlitTime.LastTime;
	UpdateTime.Ellapsed = CurTime - UpdateTime.LastTime;
	EingabeTime.Ellapsed = CurTime - EingabeTime.LastTime;
	FillTime.Ellapsed = CurTime - FillTime.LastTime;
	
	if (anz == 2) TimerUpdate();
	
	return AllTime.Ellapsed;
}

void CTimer :: TimerInit() {
	Zeit3 = 0.0;
	newTime = SDL_GetTicks() / 1000;
	
	//strukturzeiten auf null setzen
	AllTime.Ellapsed = 0.0;
	AllTime.LastTime = 0.0;
	BlitTime.Ellapsed = 0.0;
	BlitTime.LastTime = 0.0;
	UpdateTime.Ellapsed = 0.0;
	UpdateTime.LastTime = 0.0;
	EingabeTime.Ellapsed = 0.0;
	EingabeTime.LastTime = 0.0;
	FillTime.Ellapsed = 0.0;
	FillTime.LastTime = 0.0;
	
	CurTime = SDL_GetTicks() / 1000;//insgesamt zeit
}

#ifndef CBLOCK_H_
#define CBLOCK_H_
class CBlock : public TSingleton<CBlock> , public CTimer
{
public:
	CBlock();
	~CBlock();
	
	void Init(double CurTime);
	void Quit();
	
	bool bKollisionscheck(SDL_Rect rctPlayer, int ZielRichtung = 0, int *pAbstand = NULL, bool *pImJump = NULL);//kollision ueberpruefen;1=Norden...
	void SteinSetzen(SDL_Rect rctPlayer, double CurTime) {if (VerbrauchteSteine < MaxSteine && (!(KOLLISION(rctPlayer,rctLastCheckpoint)) || rctLastCheckpoint.w==0)) {NewBlock(1, rctPlayer.x, rctPlayer.y, -1, rctPlayer.x, rctPlayer.y, true, 1, 0, 0, 0);VerbrauchteSteine++;/*ein stein wurde verbraucht*/}}//einen stein setzen
	void Aetzen(SDL_Rect rctPlayer, double CurTime, bool links, bool rechts, bool up);//einen stein wieder wegnehmen
	void Render(SDL_Surface *screen, SDL_Rect rctBlock1, SDL_Rect rctBlock2, bool bGesetzt1, bool bGesetzt2, int Art, int Anzahl, bool bAlles = false);//rendern
	bool Update(double CurTime, double dEllapsed, SDL_Rect *prctPlayer, bool bLinks, bool bRechts, double dJumpSteigung, int *pMinBlock, SDL_Rect &rctBlock1, bool bBeam = false, SDL_Rect *prctrelMouse = NULL);//updaten und schauen ob man beim ziel war
	void CloudUpdate(double dEllapsed, int iSchritt, float *pPlayerX = NULL);//die wolken updaten
	void MapMovement(int x, int y, SDL_Rect *rctBlock1 = NULL);//alle bloecke verchieben
	void Streichen();//bloecke streichen
	void BlockRaster(SDL_Rect rctMouse);//bloecke am passenden block rastern
	void Back() {if(static_cast<int> (lBlock.size()) > 0) {lBlock.pop_back();}}//letztes element loeschen
	void rctKolRender (SDL_Rect srcrct);//ebloecke in einem bestimmten bereich rendern
	void SetLevel(int Level2){Level = Level2;} //das level festsetzen
	void SetMaxSteine(int Steine2){MaxSteine=Steine2;}//die maximale anzahl steine festsetzen
	int GetVerbrauchteSteine() {return VerbrauchteSteine;} //die verbrauchten steine getten
	void NewBlock(int iArt, int XPos1, int YPos1, int iAnzahl, int XPos2, int YPos2, bool bGradGesetzt, int iRichtung = 0, int iCloudL = 0, int iCloudR = 0, int iCloudSpeed = 0);//blockreihe setzen;anz=-1:mod=0;anz=-2:mod=2
	bool GetZeigerPos(SDL_Rect *rct, int stelle);//rand zeiger zum ziel berechnen
	void SetScreen(int W, int H, int BW, int BH, bool BB, int MS, int GR, int ER, SDL_Color col) {iAllW = W; iAllH = H;iBackgroundW = BW;iBackgroundH = BH;bBack = BB;iMapSpeed = MS;iGameRand = GR; iEditRand = ER; FAST_COLOR(ColBack,col.r,col.g,col.b);}
	bool GetRestart() {return bRestart;}
	void SetRestart(bool R) {bRestart = R;}
	bool GetTot() {return bTot;}
	void SetTot(bool tot) {bTot = tot;}
	void SetCloud(int Ri, int L, int R) {iCloudRichtung = Ri; iCloudL = L; iCloudR = R;} 
	void SetBeimZiel(bool b) {bBeimZiel = b;}
	bool GetImZiel() {return bImZiel;}
	void SetImZiel(bool B) {bImZiel = B;}
	bool GetImStein1(SDL_Rect rct) {for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) if (KOLLISION(rct,(*itS_Block).rctBlock) && (*itS_Block).Art == 1 && (*itS_Block).bGeradeGesetzt == true) return true; return false;}
	bool GetbFlip(){return bFlip;}
	void SetbFlip(bool B){bFlip = B;}
	SDL_Rect GetCheckRect() {return rctLastCheckpoint;}
	
	void Erease(SDL_Rect rct) {bool bFound=false;for(itS_Block=lBlock.begin();itS_Block!=lBlock.end()&& bFound==false;itS_Block++){if(KOLLISION(rct,(*itS_Block).rctBlock)){SDL_Rect rct; bFound=true; SDL_FillRect (SDL_GetVideoSurface(), &((*itS_Block).rctBlock), SDL_MapRGB (SDL_GetVideoSurface()->format,ColBack.r, ColBack.g,ColBack.b)); FAST_RECT(rct,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);lBlock.erase(itS_Block); rctKolRender(rct); }}}
	void Save(int iSaveLevel);//liste abspeichern
	void Load(int iLoadLevel);//liste laden
	
	void SetEditor(bool bEdit2) {bEditor = bEdit2;}
	
private:
	SDL_Surface *surBlock[8];//die 4 block surfaces
	enum{FEST = 0, TRANSPARENT, AETZBAR, ZIEL, HALBTOT, TOT, CHECKPOINT, WOLKE};
	SDL_Color ColBack;
	
	struct S_Block {//wichtige blockstruktur
		SDL_Rect rctBlock;//positon
		SDL_Rect rctLastBlock;//letzte position
		int Art; //0 = fest, 1 = gestrichelt, 2 = setz- und aetzbar, 3 = goal
		double SetzZeitpunkt;//zeit in ms als block gesetzt wurde; 0 = art != 1
		bool bGeradeGesetzt;//true = neu; false = geaetzt
		int iCloudRichtung;//richtung bei cloud
		int iCloudL, iCloudR;//links/rechts abweichung
		int iCloudStartX;//startpunkt der wolke
		int iCloudStartY;//startpunkt der wolke
		int iCloudSpeed;//geschwindigkeit
		double dCloudPosX;//xposition
		double dCloudPosY;//yposition
		bool bRichtungWechsel;//wurde gerade die richtung geaendert?
		bool bStehtDrauf;//player ist drauf
	};
	list<S_Block> lBlock;				//liste fuer die bloecke
	list<S_Block>::iterator itS_Block; 	//iterator
	
	vector<SDL_Rect> vrctReihe;//reihenvektor fuer bloecke
	vector<SDL_Rect>::iterator itRct;//iterator
	
	int Level;//das level
	int VerbrauchteSteine;//die verbrauchten steine
	int MaxSteine;//die maximale anzahl steine
	
	bool bBeimZiel;//beim ziel gewesen
	bool bImZiel;//im ziel?
	bool bFlip;//flippen?
	
	bool bEditor;//editmode?
	bool bBack;//hintergrund an?
	bool bTot;//zum checkpoint?
	bool bRestart;//von anfang an?
	
	//fuers laden
	char cLevelPfad[40];
	char cZeichen[200];
	int Art,Anzahl,XPos1,XPos2,YPos1,YPos2,iGesetzt,iCRi,iCL,iCR,iCS;
	
	//screen daten
	int iAllW,iAllH,iBackgroundW,iBackgroundH,iMapSpeed,iGameRand,iEditRand,iCloudRichtung,iCloudL,iCloudR;
	
	SDL_Rect rctLastCheckpoint;//position des letztn checkpoints
};
#endif /*CBLOCK_H_*/

CBlock::CBlock()
{
	for (int i = 0; i < 8; i++) {surBlock[i] = NULL;}
		
	SDL_RWops *rwopBlock = NULL;
	
	char chPfad[70];
	for (int i = 0; i < 8; i++) {
		sprintf (chPfad, "media/MauerJaR%i.png", i);
	
		rwopBlock = SDL_RWFromFile (chPfad, "rb");
		surBlock[i] = IMG_LoadPNG_RW(rwopBlock);//blockgrafik laden
	}
	
	if (rwopBlock == NULL) {throw "Stein-Grafik konnte nicht geladen werden";}
}

CBlock::~CBlock() {
	for (int i = 0; i < 4; i++) {SAFE_FREEN(surBlock[i])}
}

void CBlock::Init(double CurTime) {
	
	Load (Level+1);
	
	FAST_RECT(rctLastCheckpoint,0,0,0,0);
	VerbrauchteSteine = 0;
	bBeimZiel = false;
	bTot = false;
	bRestart = false;
	bImZiel = false;
	bFlip = true;
}

void CBlock::Quit() {
	lBlock.clear();
}

bool CBlock::bKollisionscheck(SDL_Rect rctPlayer, int ZielRichtung, int *pAbstand, bool *pImJump) {
	bool bKollision = false;
	bool bNeuesGefunden = false;
	SDL_Rect rctPos;
	
	bool bEnde = false;
	FAST_RECT(rctPos,rctPlayer.x,rctPlayer.y+1,rctPlayer.w,rctPlayer.h);
	for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end() && bEnde == false; itS_Block++) {//um wolke zu ermitteln
		if (KOLLISION(rctPos,(*itS_Block).rctBlock) ) {
			(*itS_Block).bStehtDrauf = true;
			bEnde = true;
		}
	}
	
	FAST_RECT(rctPos,0,0,BLOCKW,BLOCKH);
	if(pAbstand!=NULL){*pAbstand = 0;}
	
	for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) {
		if ( KOLLISION(rctPlayer,(*itS_Block).rctBlock) ) {
			if ((*itS_Block).Art != ZIEL) {
				if ((*itS_Block).Art != TRANSPARENT || (*itS_Block).bGeradeGesetzt == false) {
					if (bKollision == false) {
						FAST_RECT(rctPos,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h)
						bKollision = true;
						bNeuesGefunden = true;
					} //testen ob ein passenderer stein gefunden wurde
					if (ZielRichtung == 1) {if ((*itS_Block).rctBlock.y > rctPos.y) {bNeuesGefunden = true;FAST_RECT(rctPos,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);}}
					else if (ZielRichtung == 2) {if ((*itS_Block).rctBlock.x < rctPos.x) {bNeuesGefunden = true;FAST_RECT(rctPos,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);}}
					else if (ZielRichtung == 3) {if ((*itS_Block).rctBlock.y < rctPos.y) {bNeuesGefunden = true;FAST_RECT(rctPos,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);}}
					else if (ZielRichtung == 4) {if ((*itS_Block).rctBlock.x > rctPos.x) {bNeuesGefunden = true;FAST_RECT(rctPos,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);}}
					
					if ((*itS_Block).Art == HALBTOT) {
						if (rctLastCheckpoint.w == 0 && rctLastCheckpoint.h == 0)
							bRestart = true;//wenn es noch keinen checkpoint gab
						else
							bTot = true;
					}
					else if ((*itS_Block).Art == TOT) {
						bRestart = true;
					}
					else if ((*itS_Block).Art == CHECKPOINT) {
						FAST_RECT(rctLastCheckpoint,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y-PLAYERH,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);
					}
						
						
				}
			}
			
			else {
				if ((*itS_Block).Art != TRANSPARENT || (*itS_Block).bGeradeGesetzt == false) {
					if (rctPlayer.y <= (*itS_Block).rctBlock.y) {
						bBeimZiel = true;
						bImZiel = true;
						rctKolRender(rctPlayer);//damit das haus sichtbar bleibt
					}
					else {
						if (bKollision == false) {
							FAST_RECT(rctPos,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h)
							bKollision = true;
							bNeuesGefunden = true;
						} //testen ob ein passenderer stein gefunden wurde
						if (ZielRichtung == 1) {if ((*itS_Block).rctBlock.y > rctPos.y) {bNeuesGefunden = true;FAST_RECT(rctPos,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);}}
						else if (ZielRichtung == 2) {if ((*itS_Block).rctBlock.x < rctPos.x) {bNeuesGefunden = true;FAST_RECT(rctPos,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);}}
						else if (ZielRichtung == 3) {if ((*itS_Block).rctBlock.y < rctPos.y) {bNeuesGefunden = true;FAST_RECT(rctPos,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);}}
						else if (ZielRichtung == 4) {if ((*itS_Block).rctBlock.x > rctPos.x) {bNeuesGefunden = true;FAST_RECT(rctPos,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);}}
					}
				}
			}
			
			
			if (bKollision == true && ZielRichtung != 0 && bNeuesGefunden == true) { //abstaende messen
				bNeuesGefunden = false;
				if (ZielRichtung == 1) {*pAbstand = (*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h - rctPlayer.y;}
				else if (ZielRichtung == 2) {*pAbstand = rctPlayer.x + rctPlayer.w - (*itS_Block).rctBlock.x;}
				else if (ZielRichtung == 3) {
					*pAbstand = rctPlayer.y + rctPlayer.h - (*itS_Block).rctBlock.y;
					if ((*itS_Block).Art == 3) //damit es beim ziel richtig angepasst wird
						*pAbstand -= GOALH/2;
					if ((*itS_Block).Art == 7) {
						(*itS_Block).bStehtDrauf = true;
						if ((*itS_Block).iCloudRichtung == 4 || (*itS_Block).iCloudRichtung == 5 || (*itS_Block).iCloudRichtung == 6) {
							if (rctPlayer.y > (*itS_Block).rctBlock.y) {//bei hohen speeds gefaehrlich
								if (pImJump != NULL) {*pImJump = true;}
								*pAbstand = -((*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h - rctPlayer.y);
							}
						}
					} 
				}
				else if (ZielRichtung == 4) {*pAbstand = (*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w - rctPlayer.x;}
				else if (ZielRichtung == -1) {//horizontal wolke
					if ((*itS_Block).Art == 7) {
						if ((*itS_Block).iCloudRichtung==2 || (*itS_Block).iCloudRichtung==3 || (*itS_Block).iCloudRichtung==4) {
							if (rctPlayer.x > (*itS_Block).rctBlock.x)//bei hohen speeds gefaehrlich
								*pAbstand = (*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w - rctPlayer.x;
							else
								*pAbstand = -(rctPlayer.x + rctPlayer.w - (*itS_Block).rctBlock.x);
						}
						if ((*itS_Block).iCloudRichtung==6 || (*itS_Block).iCloudRichtung==7 || (*itS_Block).iCloudRichtung==8) {
							if (rctPlayer.x < (*itS_Block).rctBlock.x)//bei hohen speeds gefaehrlich
								*pAbstand = -(rctPlayer.x + rctPlayer.w - (*itS_Block).rctBlock.x);
							else
								*pAbstand = (*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w - rctPlayer.x;
						}	
					}
					
					if (rctPlayer.x > (*itS_Block).rctBlock.x) //bei hohen speeds gefaehrlich
						*pAbstand = (*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w - rctPlayer.x;
					else
						*pAbstand = -(rctPlayer.x + rctPlayer.w - (*itS_Block).rctBlock.x);
				}
				else if (ZielRichtung == -2) { //vertikal wolke
					if ((*itS_Block).Art == WOLKE) {
						if ((((*itS_Block).iCloudRichtung==8 || (*itS_Block).iCloudRichtung==1 || (*itS_Block).iCloudRichtung==2) && (*itS_Block).bRichtungWechsel == false) || ((*itS_Block).bRichtungWechsel == true && ((*itS_Block).iCloudRichtung==4 || (*itS_Block).iCloudRichtung==5 || (*itS_Block).iCloudRichtung==6)))
							*pAbstand = -(rctPlayer.y + rctPlayer.h - (*itS_Block).rctBlock.y);
						else if ((((*itS_Block).iCloudRichtung==4 || (*itS_Block).iCloudRichtung==5 || (*itS_Block).iCloudRichtung==6) && (*itS_Block).bRichtungWechsel == false) || ((*itS_Block).bRichtungWechsel == true && ((*itS_Block).iCloudRichtung==8 || (*itS_Block).iCloudRichtung==1 || (*itS_Block).iCloudRichtung==2)))
							*pAbstand = (*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h - rctPlayer.y;
					}
				}
			}
		}
	}
	
	return bKollision;
}

bool CBlock::Update(double CurTime, double dEllapsed, SDL_Rect *prctPlayer, bool bLinks , bool bRechts, double dJumpSteigung, int *pMinBlock, SDL_Rect &rctBlock1, bool bBeam, SDL_Rect *prctrelMouse) {
	if (bEditor == false) {//nur bei play mode
		bool bEinBlock = false;
		bool bErease = false;
		
		for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) {
			if ((*itS_Block).Art == TRANSPARENT) {
				if ((CurTime) - (*itS_Block).SetzZeitpunkt > 1.0) {
					if ((*itS_Block).bGeradeGesetzt == false) {
						if ((*itS_Block).rctBlock.x >= 0 && (*itS_Block).rctBlock.x < iAllW && (*itS_Block).rctBlock.y >= 0 && (*itS_Block).rctBlock.y < iAllH) {
							FILL((*itS_Block).rctBlock,ColBack);//bereich streichen
						}
						bErease = true;
					} 
					else if (!(KOLLISION((*itS_Block).rctBlock,(*prctPlayer)))) {//falls der player drin steht
						(*itS_Block).Art = 2;
						(*itS_Block).bGeradeGesetzt = false;
						if ((*itS_Block).rctBlock.x >= 0 && (*itS_Block).rctBlock.x < iAllW && (*itS_Block).rctBlock.y >= 0 && (*itS_Block).rctBlock.y < iAllH)
							BLIT (surBlock[(*itS_Block).Art], NULL, SDL_GetVideoSurface(), &((*itS_Block).rctBlock));
					}
				}
			}
			if ((*pMinBlock < (*itS_Block).rctBlock.y || bEinBlock == false) && (*itS_Block).Art == 0) {//die untersten ypos der bloecke ermitteln
				*pMinBlock = (*itS_Block).rctBlock.y;
				bEinBlock = true;
			}
			if (bErease == true) {
				bErease = false;
				SDL_Rect rctErease; FAST_RECT(rctErease,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);
				
				lBlock.erase(itS_Block);//geaetzten stein loeschen
				rctKolRender(rctErease);//angrenzende steine blitten
				itS_Block = lBlock.begin();//von vorne durchgehen
			}
		}
		
		//bloecke anpassen und abstand messen
		if (!((*prctPlayer).y > iGameRand && (*prctPlayer).y < iAllH-PLAYERH-iGameRand && (*prctPlayer).x > iGameRand && (*prctPlayer).x < iAllW-PLAYERW-iGameRand)) {
			SDL_Rect rctPlayer;
			FAST_RECT(rctPlayer,(*prctPlayer).x,(*prctPlayer).y,(*prctPlayer).w,(*prctPlayer).h)
			
			int iAbstand = 0;
			if ((*prctPlayer).y > iAllH-PLAYERH-iGameRand/* && (dJumpSteigung < 0 || bBeam == true)*/) {
				iAbstand = (*prctPlayer).y - (iAllH-PLAYERH-iGameRand);
				for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) {
					(*itS_Block).rctBlock.y -= iAbstand; (*itS_Block).iCloudStartY-=iAbstand; (*itS_Block).dCloudPosY=(*itS_Block).rctBlock.y;
				}
				rctPlayer.y -= iAbstand;
				rctLastCheckpoint.y -= iAbstand;
			}
			if ((*prctPlayer).y < iGameRand/* && (dJumpSteigung >= 0 || bBeam == true)*/) {
				iAbstand = iGameRand - (*prctPlayer).y;
				for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) {
					(*itS_Block).rctBlock.y += iAbstand; (*itS_Block).iCloudStartY+=iAbstand; (*itS_Block).dCloudPosY=(*itS_Block).rctBlock.y;
				}
				rctPlayer.y += iAbstand;
				rctLastCheckpoint.y += iAbstand;
			}
			if ((*prctPlayer).x < iGameRand/* && (bLinks == true || bBeam == true)*/) {
				iAbstand = iGameRand - (*prctPlayer).x;
				for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) {
					(*itS_Block).rctBlock.x += iAbstand; (*itS_Block).iCloudStartX+=iAbstand; (*itS_Block).dCloudPosX=(*itS_Block).rctBlock.x;
				}
				rctPlayer.x += iAbstand;
				rctLastCheckpoint.x += iAbstand;
			}
			if ((*prctPlayer).x > iAllW - PLAYERW - iGameRand/* && (bRechts == true || bBeam == true)*/) {
				iAbstand = (*prctPlayer).x - (iAllW - PLAYERW - iGameRand);
				for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) {
					(*itS_Block).rctBlock.x -= iAbstand; (*itS_Block).iCloudStartX-=iAbstand; (*itS_Block).dCloudPosX=(*itS_Block).rctBlock.x;
				}
				rctPlayer.x -= iAbstand;
				rctLastCheckpoint.x -= iAbstand;
			}
			
			FAST_RECT((*prctPlayer),rctPlayer.x,rctPlayer.y,rctPlayer.w,rctPlayer.h)
		}
		
		Streichen();//bloecke streichen
		
		return bBeimZiel;
	}
	
	else {//map verschieben beim editor
		int iAbstand = iMapSpeed * dEllapsed;
		
		if ((*prctPlayer).y+(CURSORH/2) > iAllH-iEditRand) {MapMovement(0,-iAbstand,&rctBlock1);(*prctrelMouse).y-=iAbstand;}
		if ((*prctPlayer).y-(CURSORH/2) < iEditRand) {MapMovement(0,iAbstand,&rctBlock1);(*prctrelMouse).y+=iAbstand;}
		if ((*prctPlayer).x-(CURSORW/2) < iEditRand) {MapMovement(iAbstand,0,&rctBlock1);(*prctrelMouse).x-=iAbstand;}
		if ((*prctPlayer).x+(CURSORW/2) > iAllW-iEditRand) {MapMovement(-iAbstand,0,&rctBlock1);(*prctrelMouse).x+=iAbstand;}
		
		Streichen();//bloecke streichen
				
		return false;
	}
}

void CBlock::Render(SDL_Surface *screen, SDL_Rect rctBlock1, SDL_Rect rctBlock2, bool bGesetzt1, bool bGesetzt2, int Art, int Anzahl, bool bAlles) {
	for (itRct = vrctReihe.begin(); itRct != vrctReihe.end(); itRct++) {if ((*itRct).x >= 0 && (*itRct).y >= 0 && (*itRct).x < iAllW && (*itRct).y < iAllH) {FILL((*itRct),ColBack);rctKolRender((*itRct));}}
	vrctReihe.clear();//die letzen bereiche fillen
	
	SDL_Rect rctStein;//rct fuer die steine
	FAST_RECT(rctStein,0,0,40,40)
	int AbstandX = rctBlock2.x - rctBlock1.x;//abstaende
	int AbstandY = rctBlock2.y - rctBlock1.y;
	//die reihe blitten , die nicht in der liste ist
	if (bGesetzt1 == true) {
		if (rctBlock1.x >= 0 && rctBlock1.y >= 0 && rctBlock1.x < iAllW && rctBlock1.y < iAllH) {rctKolRender(rctBlock1);BLIT (surBlock[Art], NULL, SDL_GetVideoSurface(), &(rctBlock1));}
		if (rctBlock2.x >= 0 && rctBlock2.y >= 0 && rctBlock2.x < iAllW && rctBlock2.y < iAllH) {rctKolRender(rctBlock2);BLIT (surBlock[Art], NULL, SDL_GetVideoSurface(), &(rctBlock2));}
		vrctReihe.push_back(rctBlock1); vrctReihe.push_back(rctBlock2);
		for (int i = 1; i < Anzahl+1; i++) {
			rctStein.x = rctBlock1.x + (i * (AbstandX / (Anzahl+1)));
			rctStein.y = rctBlock1.y + (i * (AbstandY / (Anzahl+1)));
			if (rctStein.x >= 0 && rctStein.y >= 0  && rctStein.x < iAllW && rctStein.y < iAllH)
				BLIT (surBlock[Art], NULL, screen, &rctStein);
			vrctReihe.push_back(rctStein);
		}
	}
	
	//normales rendern
	SDL_Rect rctBlockTeil;
	SDL_Rect rctBlockPos;
	for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) { 
		if (((*itS_Block).rctBlock.x >= 0 && (*itS_Block).rctBlock.x + (*itS_Block).rctBlock.w < iAllW && (*itS_Block).rctBlock.y >= 0 && (*itS_Block).rctBlock.y + (*itS_Block).rctBlock.h < iAllH) && (((*itS_Block).rctBlock.x != (*itS_Block).rctLastBlock.x || (*itS_Block).rctBlock.y != (*itS_Block).rctLastBlock.y) || (((*itS_Block).rctLastBlock.w == 0 && (*itS_Block).rctLastBlock.h == 0) || (*itS_Block).Art == 1 || bAlles == true))) { 
			BLIT (surBlock[(*itS_Block).Art], NULL, screen, &((*itS_Block).rctBlock));
		}
		//ansonsten teile davon
		else if (((*itS_Block).rctBlock.x >= 0-(*itS_Block).rctBlock.w && (*itS_Block).rctBlock.x < iAllW && (*itS_Block).rctBlock.y >= 0-(*itS_Block).rctBlock.h && (*itS_Block).rctBlock.y < iAllH) && (((*itS_Block).rctBlock.x != (*itS_Block).rctLastBlock.x || (*itS_Block).rctBlock.y != (*itS_Block).rctLastBlock.y) || (((*itS_Block).rctLastBlock.w == 0 && (*itS_Block).rctLastBlock.h == 0) || (*itS_Block).Art == 1 || bAlles == true))) {
			FAST_RECT(rctBlockPos,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);
			if ((*itS_Block).rctBlock.x < 0) {
				rctBlockTeil.x = -(*itS_Block).rctBlock.x;
				rctBlockTeil.w = (*itS_Block).rctBlock.w + (*itS_Block).rctBlock.x;
				rctBlockPos.x = 0;
			} else if ((*itS_Block).rctBlock.x + (*itS_Block).rctBlock.w > iAllW) {
				rctBlockTeil.x = 0;
				rctBlockTeil.w = iAllW - (*itS_Block).rctBlock.x;
			} else {
				rctBlockTeil.x = 0;
				rctBlockTeil.w = (*itS_Block).rctBlock.w;
			}
			if ((*itS_Block).rctBlock.y < 0) {
				rctBlockTeil.y = -(*itS_Block).rctBlock.y;
				rctBlockTeil.h = (*itS_Block).rctBlock.h + (*itS_Block).rctBlock.y;
				rctBlockPos.y = 0;
			} else if ((*itS_Block).rctBlock.y + (*itS_Block).rctBlock.h > iAllH) {
				rctBlockTeil.y = 0;
				rctBlockTeil.h = iAllH - (*itS_Block).rctBlock.y;
			} else {
				rctBlockTeil.y = 0;
				rctBlockTeil.h = (*itS_Block).rctBlock.h;
			}
			
			BLIT (surBlock[(*itS_Block).Art], &rctBlockTeil, screen, &rctBlockPos);
		}
		FAST_RECT((*itS_Block).rctLastBlock,(*itS_Block).rctBlock.x,(*itS_Block).rctBlock.y,(*itS_Block).rctBlock.w,(*itS_Block).rctBlock.h);
	}
}

void CBlock::CloudUpdate(double dEllapsed, int iSchritt, float *pPlayerX) { // die woken bewegen
	bool bWechsel = false; // richtungswechsel?
	int iVar = 0;
	for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) {
		if ((*itS_Block).Art == WOLKE) {//wolke
			bWechsel = false;
			if ((*itS_Block).bRichtungWechsel == true)//kein wechsel mehr
				(*itS_Block).bRichtungWechsel = false;
			if (((*itS_Block).iCloudRichtung == 1 || (*itS_Block).iCloudRichtung == 8  || (*itS_Block).iCloudRichtung == 2) && iSchritt == 2) {
				(*itS_Block).dCloudPosY -= (*itS_Block).iCloudSpeed * dEllapsed;
				if ((*itS_Block).dCloudPosY < (*itS_Block).iCloudStartY - (*itS_Block).iCloudL) {bWechsel = true;(*itS_Block).dCloudPosY += (*itS_Block).iCloudSpeed * dEllapsed;}
				(*itS_Block).rctBlock.y = static_cast<int> ((*itS_Block).dCloudPosY);
			}
			if (((*itS_Block).iCloudRichtung == 3 || (*itS_Block).iCloudRichtung == 2  || (*itS_Block).iCloudRichtung == 4) && iSchritt == 1) {
				(*itS_Block).dCloudPosX += (*itS_Block).iCloudSpeed * dEllapsed;
				if (pPlayerX!=NULL&&(*itS_Block).bStehtDrauf==true){(*itS_Block).bStehtDrauf = false;*pPlayerX += (*itS_Block).iCloudSpeed * dEllapsed;}
				if ((*itS_Block).dCloudPosX > (*itS_Block).iCloudStartX + (*itS_Block).iCloudL) {bWechsel = true;(*itS_Block).dCloudPosX -= (*itS_Block).iCloudSpeed * dEllapsed;}
				(*itS_Block).rctBlock.x = static_cast<int> ((*itS_Block).dCloudPosX);
			}
			if (((*itS_Block).iCloudRichtung == 5 || (*itS_Block).iCloudRichtung == 4  || (*itS_Block).iCloudRichtung == 6) && iSchritt == 2) {
				(*itS_Block).dCloudPosY += (*itS_Block).iCloudSpeed * dEllapsed;
				if ((*itS_Block).dCloudPosY > (*itS_Block).iCloudStartY + (*itS_Block).iCloudL) {bWechsel = true;(*itS_Block).dCloudPosY -= (*itS_Block).iCloudSpeed * dEllapsed;}
				(*itS_Block).rctBlock.y = static_cast<int> ((*itS_Block).dCloudPosY);
			}
			if (((*itS_Block).iCloudRichtung == 7 || (*itS_Block).iCloudRichtung == 6  || (*itS_Block).iCloudRichtung == 8) && iSchritt == 1) {
				(*itS_Block).dCloudPosX -= (*itS_Block).iCloudSpeed * dEllapsed;
				if (pPlayerX!=NULL&&(*itS_Block).bStehtDrauf==true){(*itS_Block).bStehtDrauf = false;*pPlayerX -= (*itS_Block).iCloudSpeed * dEllapsed;}
				if ((*itS_Block).dCloudPosX < (*itS_Block).iCloudStartX - (*itS_Block).iCloudL) {bWechsel = true;(*itS_Block).dCloudPosX += (*itS_Block).iCloudSpeed * dEllapsed;}
				(*itS_Block).rctBlock.x = static_cast<int> ((*itS_Block).dCloudPosX);
			}
			
			if (bWechsel == true) {
				if ((*itS_Block).iCloudRichtung <= 4) { (*itS_Block).iCloudRichtung += 4;}
				else {(*itS_Block).iCloudRichtung -= 4;}
				iVar = (*itS_Block).iCloudL;
				(*itS_Block).iCloudL = (*itS_Block).iCloudR;
				(*itS_Block).iCloudR = iVar;
				(*itS_Block).bRichtungWechsel = true;
			}
		}
	}
}

void CBlock::Streichen() { // die blockbereiche streichen
	if (bBack == false) {
		SDL_Rect rctBereich;
		for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) {
			if (((*itS_Block).rctLastBlock.x >= 0 && (*itS_Block).rctLastBlock.x + (*itS_Block).rctLastBlock.w < iAllW && (*itS_Block).rctLastBlock.y >= 0 && (*itS_Block).rctLastBlock.y + (*itS_Block).rctLastBlock.h < iAllH) && (((*itS_Block).rctBlock.x != (*itS_Block).rctLastBlock.x || (*itS_Block).rctBlock.y != (*itS_Block).rctLastBlock.y))) {
				FILL((*itS_Block).rctLastBlock,ColBack);
			}
			//ansonsten teilweise
			else if (((*itS_Block).rctLastBlock.x >= 0-BLOCKW && (*itS_Block).rctLastBlock.x < iAllW && (*itS_Block).rctLastBlock.y >= 0-BLOCKH && (*itS_Block).rctLastBlock.y < iAllH) && (((*itS_Block).rctBlock.x != (*itS_Block).rctLastBlock.x || (*itS_Block).rctBlock.y != (*itS_Block).rctLastBlock.y))) {
				FAST_RECT(rctBereich,(*itS_Block).rctLastBlock.x,(*itS_Block).rctLastBlock.y,(*itS_Block).rctLastBlock.w,(*itS_Block).rctLastBlock.h);
				
				if ((*itS_Block).rctLastBlock.x < 0) {
					rctBereich.w = (*itS_Block).rctLastBlock.w + (*itS_Block).rctLastBlock.x;
					rctBereich.x = 0;
				} else if ((*itS_Block).rctLastBlock.x + (*itS_Block).rctLastBlock.w > iAllW)
					rctBereich.w = iAllW - (*itS_Block).rctLastBlock.x;
				else	rctBereich.w = (*itS_Block).rctLastBlock.w;
				
				if ((*itS_Block).rctLastBlock.y < 0) {
					rctBereich.h = (*itS_Block).rctLastBlock.h + (*itS_Block).rctLastBlock.y;
					rctBereich.y = 0;
				} else if ((*itS_Block).rctLastBlock.y + (*itS_Block).rctLastBlock.h > iAllH)
					rctBereich.h = iAllH - (*itS_Block).rctLastBlock.y;
				else	rctBereich.h = (*itS_Block).rctLastBlock.h;
				
				FILL(rctBereich,ColBack);
			}
			
			if ((*itS_Block).Art == 7 && bEditor == false){ // oder bei wolke
				rctKolRender((*itS_Block).rctLastBlock);
			}
		}
	}
}

void CBlock::BlockRaster(SDL_Rect rctMouse) { // map an raster bestimmt am block anpassen
	bool bGefunden = false;
	int iAbstandX, iAbstandY;
	
	for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end() && bGefunden == false; itS_Block++) {
		if ( KOLLISION(rctMouse,(*itS_Block).rctBlock) ) {
			bGefunden = true;
			iAbstandX = -((*itS_Block).rctBlock.x % BLOCKW);
			iAbstandY = -((*itS_Block).rctBlock.y % BLOCKH);
			
			MapMovement (iAbstandX, iAbstandY);
		}
	}
}

void CBlock::NewBlock(int iArt, int XPos1, int YPos1, int iAnzahl, int XPos2, int YPos2, bool bGradGesetzt, int iRichtung, int iCloudL, int iCloudR, int iCloudSpeed) { // neuen block erstellen
	int AbstandX = XPos2 - XPos1;
	int AbstandY = YPos2 - YPos1;
	bool bGefunden = false;
	
	if (iAnzahl == -1 ) // modus 0
		NEWSTONE(iArt,XPos1,YPos1,bGradGesetzt,iRichtung,iCloudL,iCloudR,iCloudSpeed)
	else if (iAnzahl == -2) {//modus 2
		for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end() && bGefunden == false; itS_Block++) {
			if ((*itS_Block).rctBlock.x == XPos1 && (*itS_Block).rctBlock.y == YPos1)
				bGefunden = true;
		}
		if (bGefunden == false) {
			NEWSTONE(iArt,XPos1,YPos1,bGradGesetzt,iRichtung,iCloudL,iCloudR,iCloudSpeed)
		}
	}
	else { // modus 1
		NEWSTONE(iArt,XPos1,YPos1,bGradGesetzt,iRichtung,iCloudL,iCloudR,iCloudSpeed)
		if (XPos1 != XPos2 || YPos1 != YPos2) {
			NEWSTONE(iArt,XPos2,YPos2,bGradGesetzt,iRichtung,iCloudL,iCloudR,iCloudSpeed)
		}
		
		for (int i = 1; i < iAnzahl+1; i++) {
			NEWSTONE(iArt,XPos1 + (i * (AbstandX / (iAnzahl+1))),YPos1 + (i * (AbstandY / (iAnzahl+1))),bGradGesetzt,iRichtung,iCloudL,iCloudR,iCloudSpeed)
		}
	}
}

void CBlock::rctKolRender (SDL_Rect srcrct) { //alle kollidierenden bloecke rendern
	list<S_Block>::iterator itB;
	for (itB = lBlock.begin(); itB!=lBlock.end(); itB++) {
		if ((KOLLISION((*itB).rctBlock,srcrct)) && ((*itB).rctBlock.x >= 0 && (*itB).rctBlock.x < iAllW && (*itB).rctBlock.y >= 0 && (*itB).rctBlock.y < iAllH)) {
			FILL((*itB).rctBlock,ColBack);
			BLIT (surBlock[(*itB).Art], NULL, SDL_GetVideoSurface(), &((*itB).rctBlock));
		}
	}
}

void CBlock::MapMovement(int x, int y, SDL_Rect *rctBlock1) { // die karte bewegen
	for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) {
		(*itS_Block).rctBlock.x += x;
		(*itS_Block).rctBlock.y += y;
		(*itS_Block).dCloudPosX=(*itS_Block).rctBlock.x;(*itS_Block).dCloudPosY=(*itS_Block).rctBlock.y;
	}
	if (rctBlock1!=NULL) {(*rctBlock1).x += x; (*rctBlock1).y += y;}
}

void CBlock::Save(int iSaveLevel) { // abspeichern
	char chPfad[50];
	sprintf (chPfad, "Save/Level%i.txt", iSaveLevel);
	
	ofstream dat_aus;
	dat_aus.open(chPfad, ios_base::out);
	
	for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end(); itS_Block++) {
		dat_aus << (*itS_Block).Art << "," << (*itS_Block).rctBlock.x << "," << (*itS_Block).rctBlock.y << ",-1,";
		dat_aus << (*itS_Block).rctBlock.x << "," << (*itS_Block).rctBlock.y << ",";
		dat_aus << static_cast<int> ((*itS_Block).bGeradeGesetzt) << ",";
		dat_aus << (*itS_Block).iCloudRichtung << "," << (*itS_Block).iCloudL << ",";
		dat_aus << (*itS_Block).iCloudR << "," << (*itS_Block).iCloudSpeed << "\t##" << endl;
	} 
		
	dat_aus.close();
}

void CBlock::Load (int iLoadLevel) {//laden
	if (iLoadLevel > 0) {
		ifstream dat_ein;
		char chPfad[50];
		
		sprintf (chPfad, "Save/Level%i.txt", iLoadLevel);
		dat_ein.open(chPfad, ios_base::in);
		
		if ( !dat_ein.is_open()) { throw "Level-Datei konnte nicht geöffnet werden";}
		while (! dat_ein.eof() )
		{
			for (int i = 1; i < 12; i++) {
				if (i < 11)
	  	  			dat_ein.getline (cZeichen,198,',');
	  	  		if (i == 11)
	  	  			dat_ein.getline (cZeichen,198,'#');
	  	  		if (i == 1) {Art = strtoul(cZeichen,NULL,10);}
	  	  		if (i == 2) {XPos1 = strtoul(cZeichen,NULL,10);}
	  	  		if (i == 3) {YPos1 = strtoul(cZeichen,NULL,10);}
	  	  		if (i == 4) {Anzahl = strtoul(cZeichen,NULL,10);}
	  	  		if (i == 5) {XPos2 = strtoul(cZeichen,NULL,10);}
	  	  		if (i == 6) {YPos2 = strtoul(cZeichen,NULL,10);}
	  	  		if (i == 7) {iGesetzt = strtoul(cZeichen,NULL,10);}
	  	  		if (i == 8) {iCRi = strtoul(cZeichen,NULL,10);}
	  	  		if (i == 9) {iCL = strtoul(cZeichen,NULL,10);}
	  	  		if (i == 10) {iCR = strtoul(cZeichen,NULL,10);}
	  	  		if (i == 11) {iCS = strtoul(cZeichen,NULL,10);}
			}
			if (iGesetzt == 0) {NewBlock(Art, XPos1, YPos1, Anzahl, XPos2, YPos2, false, iCRi, iCL, iCR, iCS);}
	  	  	if (iGesetzt == 1) {NewBlock(Art, XPos1, YPos1, Anzahl, XPos2, YPos2, true, iCRi, iCL, iCR, iCS);}
	  	  	
	  	  	dat_ein.getline (cZeichen,198,'#');
		}
		
		dat_ein.close();
		
		lBlock.pop_back();//um den fehlerdtein in der ecke wegzubekommen
	}
}

void CBlock::Aetzen(SDL_Rect rctPlayer, double CurTime, bool links, bool rechts, bool up) {
	bool bSteinGefunden = false;
	int iAbstand = -1;
			
	if (links == true) { // zuerst links
		for (int i = 0; i < 2; i++) for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end() && bSteinGefunden == false; itS_Block++)
			if ((*itS_Block).Art == AETZBAR)
				if (rctPlayer.x < (*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w+1&&rctPlayer.x+rctPlayer.w>(*itS_Block).rctBlock.x&&
					rctPlayer.y < (*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h&&rctPlayer.y+rctPlayer.h>(*itS_Block).rctBlock.y) {//die 1 ist variable
					if(BETRAG(((*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h/2)-(rctPlayer.y+rctPlayer.h/2)) < iAbstand || iAbstand==-1) iAbstand = BETRAG(((*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h/2)-(rctPlayer.y+rctPlayer.h/2));
					if (i == 1 && BETRAG(((*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h/2)-(rctPlayer.y+rctPlayer.h/2)) == iAbstand) bSteinGefunden = true;
				}	
	}
	else if (rechts == true) { // rechts
		for (int i = 0; i < 2; i++) for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end() && bSteinGefunden == false; itS_Block++)
			if ((*itS_Block).Art == AETZBAR)
				if (rctPlayer.x < (*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w&&rctPlayer.x+rctPlayer.w>(*itS_Block).rctBlock.x-1&&
					rctPlayer.y < (*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h&&rctPlayer.y+rctPlayer.h>(*itS_Block).rctBlock.y) {//die 1 ist variable
					if(BETRAG(((*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h/2)-(rctPlayer.y+rctPlayer.h/2)) < iAbstand || iAbstand==-1) iAbstand = BETRAG(((*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h/2)-(rctPlayer.y+rctPlayer.h/2));
					if (i == 1 && BETRAG(((*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h/2)-(rctPlayer.y+rctPlayer.h/2)) == iAbstand) bSteinGefunden = true;
				}	
	}
	if (bSteinGefunden == false && up == true) {//sonst oben vielleicht
		for (int i = 0; i < 2; i++) for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end() && bSteinGefunden == false; itS_Block++)
			if ((*itS_Block).Art == AETZBAR)
				if (rctPlayer.x < (*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w&&rctPlayer.x+rctPlayer.w>(*itS_Block).rctBlock.x&&
					rctPlayer.y < (*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h+5&&rctPlayer.y+rctPlayer.h>(*itS_Block).rctBlock.y) {//die 5 ist variable
					if(BETRAG(((*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w/2)-(rctPlayer.x+rctPlayer.w/2)) < iAbstand || iAbstand==-1) iAbstand = BETRAG(((*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w/2)-(rctPlayer.x+rctPlayer.w/2));
					if (i == 1 && BETRAG(((*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w/2)-(rctPlayer.x+rctPlayer.w/2)) == iAbstand) bSteinGefunden = true;
				}	
	}
	else if (bSteinGefunden == false) { // ansonsten unten
		for (int i = 0; i < 2; i++) for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end() && bSteinGefunden == false; itS_Block++)
			if ((*itS_Block).Art == AETZBAR)
			 	if((rctPlayer.x < (*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w&&rctPlayer.x+rctPlayer.w>(*itS_Block).rctBlock.x&&
					rctPlayer.y < (*itS_Block).rctBlock.y+(*itS_Block).rctBlock.h&&rctPlayer.y+rctPlayer.h>(*itS_Block).rctBlock.y-1)) {//die 1 ist variable
					if(BETRAG(((*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w/2)-(rctPlayer.x+rctPlayer.w/2)) < iAbstand || iAbstand==-1) iAbstand = BETRAG(((*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w/2)-(rctPlayer.x+rctPlayer.w/2));
					if (i == 1 && BETRAG(((*itS_Block).rctBlock.x+(*itS_Block).rctBlock.w/2)-(rctPlayer.x+rctPlayer.w/2)) == iAbstand) bSteinGefunden = true;
				}	
	}
	
	if (bSteinGefunden == true) {
		itS_Block--;
		(*itS_Block).Art = 1;
		(*itS_Block).SetzZeitpunkt = CurTime;
		(*itS_Block).bGeradeGesetzt = false;
		
		VerbrauchteSteine--;
		
		if ((*itS_Block).rctBlock.x >= 0 && (*itS_Block).rctBlock.x < iAllW && (*itS_Block).rctBlock.y >= 0 && (*itS_Block).rctBlock.y < iAllH) {
			SDL_FillRect (SDL_GetVideoSurface(), &((*itS_Block).rctBlock), SDL_MapRGB (SDL_GetVideoSurface()->format, ColBack.r, ColBack.g, ColBack.b));
			BLIT (surBlock[(*itS_Block).Art], NULL, SDL_GetVideoSurface(), &((*itS_Block).rctBlock));
		}
	}
}

bool CBlock::GetZeigerPos(SDL_Rect *rct, int stelle) {
	bool bret = false;
	float f = 0.0f, f2 = 0.0f, f3 = 0.0f;
	(*rct).x = -1;
	
	for (itS_Block = lBlock.begin(); itS_Block!=lBlock.end() && bret == false; itS_Block++)
		if ((*itS_Block).Art == ZIEL) {
			stelle--;
			if(stelle == 0) {
				bret = true;
				
				//nur wenn ausserhalb des feldes
				if (((*itS_Block).rctBlock.x+(GOALW/2) < 0 || (*itS_Block).rctBlock.x+(GOALW/2) >= iAllW || (*itS_Block).rctBlock.y+(GOALH/4) < 0 || (*itS_Block).rctBlock.y+(GOALH/4) >= iAllH)) {
				
					f = ((*itS_Block).rctBlock.x+(GOALW/2))-(iAllW/2);
					f2 = BETRAG(((*itS_Block).rctBlock.y+(GOALH/4))-(iAllH/2));
					if( f2 != 0.0f) {
						f3 = (f / f2 * (iAllH/2)) + iAllW/2;
						(*rct).x = static_cast<int> (f3);
					}
					
					if ((*rct).x >= 0 && (*rct).x < iAllW-ZEIGERBALLW && f2 != 0.0f) { 
						//y anpassen
						if ((*itS_Block).rctBlock.y < 0) (*rct).y = 0;
						else (*rct).y = iAllH - ZEIGERBALLH;
					} else { //wenn x nicht passt dann linken/rechten rand
						f = (((*itS_Block).rctBlock.y+(GOALH/4))-(iAllH/2));
						f2 = BETRAG(((*itS_Block).rctBlock.x+(GOALW/2))-(iAllW/2));
						f3 = (f / f2 * (iAllW/2)) + iAllH/2;
						(*rct).y = static_cast<int> (f3);
						//x anpassen
						if ((*itS_Block).rctBlock.x < 0) (*rct).x = 0;
						else (*rct).x = iAllW - ZEIGERBALLW;
					}
				}
			}
		}
			
	return bret;
}

#ifndef CPLAYER_H_
#define CPLAYER_H_
class CPlayer : public TSingleton<CPlayer> , public CTimer
{
public:
	CPlayer();
	~CPlayer();
	
	void Init(int l = 200, int r = 200, int h = 400, double a = 2.0);
	void Quit();
	
	void Bewegung(double dEllapsed, bool bLinks, bool bRechts, bool bJump, bool bUp, bool *pRestart = NULL);//links rechts und springen
	void SteinSetzen(double CurTime){CBlock::Get()->SteinSetzen(rctPlayer, CurTime);}//einen stein setzen
	void Aetzen(double CurTime){CBlock::Get()->Aetzen(rctPlayer,CurTime,bImLinks,bImRechts,bImUp);}//stein wieder wegnehmen
	void Render(SDL_Surface *screen, SDL_Rect rctBlock1, SDL_Rect rctBlock2, bool bGesetzt1, bool bGesetzt2, int Art, int Anzahl, int modus, bool bAlles = false);//rendern
	bool Update(double CurTime, double dEllapsed, int *pModus, int *BackgroundX, int *BackgroundY, SDL_Rect rctMouse, SDL_Rect *prctrelMouse, SDL_Rect &rctBlock1);//update und schauen ob man das level geschafft hat
	void SetOptions(int l = 200, int r = 200, int h = 400, double a = 2.0) {iPlayerSpeedL = l; iPlayerSpeedR = r; iPlayerSpeedH = h; dJumpSteigungMinus = a;}
	void SetScreen(int W, int H, int BW, int BH, bool BB, int MS, int GR, int ER, SDL_Color col) {iAllW = W; iAllH = H; iBackgroundW = BW, iBackgroundH = BH; bBack = BB; iGameRand = GR; iEditRand = ER; FAST_COLOR(ColBack,col.r,col.g,col.b);CBlock::Get()->SetScreen(W,H,BW,BH,BB,MS,GR,ER,col);}
	bool GetImJump(){return bImJump;}
	int GetDirection(int XPos) {if (XPos < rctPlayer.x) return 1; if (XPos > rctPlayer.x + rctPlayer.w) return 2; return 3;}
	void SetEditor(bool bEdit2) {bEditor = bEdit2;CBlock::Get()->SetEditor(bEdit2);}
	void ZumCheckpoint(bool bZwingend = false) {if(CBlock::Get()->GetTot() == true || bZwingend == true) {bBeam = true;FILL(rctPlayer,ColBack);CBlock::Get()->rctKolRender(rctPlayer);CBlock::Get()->SetTot(false);rctCheck = CBlock::Get()->GetCheckRect();if (rctCheck.w!=0) {FAST_RECT(rctPlayer,rctCheck.x,rctCheck.y,rctCheck.w,rctCheck.h);FILL(rctLastPlayer,ColBack);FAST_RECT(rctLastPlayer,0,0,0,0);} else{CBlock::Get()->SetRestart(true);}fMoveX = rctPlayer.x; fMoveY = rctPlayer.y;if(CBlock::Get()->bKollisionscheck(rctPlayer)==true){CBlock::Get()->SetRestart(true);}} else {bBeam = false;}}
	bool GetbFlip(){return bFlip;}
	void SetbFlip(bool B){bFlip = B;}
	
private:
	SDL_Surface *surPlayer[2];//die 2 player surfaces
	SDL_Rect rctPlayer;//rct fuer player
	SDL_RWops *rwopPlayer;
	SDL_Color ColBack;
	
	double dJumpSteigung;//die steigung beim jump
	bool bImJump;//ist man im jump
	float fMoveX;//um die bewegung ruckelfrei zu machen
	float fMoveY;//um die bewegung ruckelfrei zu machen
	
	//fuer block update
	bool bImLinks, bImRechts, bImUp;
	
	bool bEditor;//editmode?
	bool bBack;//hintergrund an?
	bool bBeam;//irgendwo hin gebeamed?
	bool bImRand[4];//fuer die wolken kollision
	bool bFlip;//flippen?
	
	int iPlayerSpeedL,iPlayerSpeedR,iPlayerSpeedH;
	double dJumpSteigungMinus;
	
	//screen daten
	int iAllW,iAllH,iBackgroundW,iBackgroundH,iGameRand,iEditRand;
	
	int iTest;
	
	int iLastModus;
	
	SDL_Rect rctCheck, rctLastPlayer;
};
#endif /*CPLAYER_H_*/

CPlayer::CPlayer()
{
	surPlayer[0] = NULL;
	surPlayer[1] = NULL;
	
	rwopPlayer = NULL;
	rwopPlayer = SDL_RWFromFile ("media/PlayerJaR1.png", "rb");
	surPlayer[0] = IMG_LoadPNG_RW(rwopPlayer);//playergrafik laden
	rwopPlayer = NULL;
	rwopPlayer = SDL_RWFromFile ("media/PlayerJaR2.png", "rb");
	surPlayer[1] = IMG_LoadPNG_RW(rwopPlayer);//playergrafik2 laden
	if (surPlayer[0] == 0 || surPlayer[1] == 0) {throw "Playergrafik konnte nicht geladen  werden";}
		
	iTest = 0;
	
	Init();
}
CPlayer::~CPlayer()
{
	for (int i = 0; i < 2; i++) {
		SAFE_FREEN(surPlayer[i])
	}
	Quit();
}
void CPlayer::Init(int l, int r, int h, double a) {
	SetOptions(l,r,h,a);
	FAST_RECT(rctPlayer,(800-PLAYERW)/2,(600-PLAYERH)/2,PLAYERW,PLAYERH)//600 und 800 wegen variabilitaet fest
	FAST_RECT(rctLastPlayer,rctPlayer.x,rctPlayer.y,rctPlayer.w,rctPlayer.h);
	
	for (int i = 0; i < 4; i++) {bImRand[i] = false;} 
	dJumpSteigung = 1.0;
	bImJump = false;
	fMoveX = rctPlayer.x;
	fMoveY = rctPlayer.y;
	bBeam = false;
	iLastModus = 0;
	bFlip = true;
}
void CPlayer::Quit() {
	
}
void CPlayer::Bewegung(double dEllapsed, bool bLinks, bool bRechts, bool bJump, bool bUp, bool *pRestart) {
	CBlock::Get()->CloudUpdate(dEllapsed,1,&fMoveX); //wolken updaten X
	
	bImLinks = bLinks;	bImRechts = bRechts;	bImUp = bUp;
	int Abstand = 0;
	
	//falls die wolke in einene reingelaufen ist
	rctPlayer.x = static_cast<int> (fMoveX);//position aktualisieren
	if (CBlock::Get()->bKollisionscheck(rctPlayer,-1,&Abstand) == true) {//s.o.
		fMoveX += Abstand;		rctPlayer.x = static_cast<int> (fMoveX);//position aktualisieren
	}
	
	if (bLinks == true) {
		fMoveX -= iPlayerSpeedL * dEllapsed;  	rctPlayer.x = static_cast<int> (fMoveX);
		if (CBlock::Get()->bKollisionscheck(rctPlayer,4,&Abstand) == true)//am block?
			fMoveX += Abstand;		rctPlayer.x = static_cast<int> (fMoveX);//position aktualisieren
	}	
	else if (bRechts == true) {
		fMoveX += iPlayerSpeedR * dEllapsed;		rctPlayer.x = static_cast<int> (fMoveX);
		if (CBlock::Get()->bKollisionscheck(rctPlayer,2,&Abstand) == true)//s.o.
			fMoveX -= Abstand;		rctPlayer.x = static_cast<int> (fMoveX);//position aktualisieren
	}
	
	if (CBlock::Get()->bKollisionscheck(rctPlayer) == true) //zerquetscht X
		CBlock::Get()->SetTot(true);
	
	CBlock::Get()->CloudUpdate(dEllapsed,2); //wolken updaten Y
	
	if (bJump == true) {if (bImJump == false) {bImJump = true;dJumpSteigung = 1;}}
	
	if (bImJump == true) {
		fMoveY -= iPlayerSpeedH * dEllapsed * dJumpSteigung;  		rctPlayer.y = static_cast<int> (fMoveY);
		if (CBlock::Get()->bKollisionscheck(rctPlayer) == true) {
			if (dJumpSteigung > 0) { 	//steigung = 0
				dJumpSteigung = 0;
				if (CBlock::Get()->bKollisionscheck(rctPlayer,1,&Abstand) == true) {
					fMoveY += Abstand; 		rctPlayer.y = static_cast<int> (fMoveY);
				}
			}
			else {					//kollision,also kein sprung mehr
				bImJump = false;
				if (CBlock::Get()->bKollisionscheck(rctPlayer,3,&Abstand,&bImJump) == true) { //player aus block(speziel wolke) ziehen
					fMoveY -= Abstand;		rctPlayer.y = static_cast<int> (fMoveY);
				}
			}
		}
		dJumpSteigung -= dJumpSteigungMinus * dEllapsed;//steigung verringern
		if(bJump == false && dJumpSteigung > 0) {dJumpSteigung -= dJumpSteigungMinus * dEllapsed;}//steigung verringern 2.mal
		
		if (dJumpSteigung < -2)
			dJumpSteigung = -2;
	}
	else {
		rctPlayer.y = static_cast<int> (fMoveY);//position aktualisieren
		if (CBlock::Get()->bKollisionscheck(rctPlayer,-2,&Abstand) == true)
			fMoveY += Abstand;
	}
	rctPlayer.x = static_cast<int> (fMoveX);//position aktualisieren
	rctPlayer.y = static_cast<int> (fMoveY);//genauso
	
	if (CBlock::Get()->bKollisionscheck(rctPlayer) == true) //zerquetscht Y
		CBlock::Get()->SetTot(true);
	
	for (int i = 0; i < 4; i++) {bImRand[i] = false;}
}

bool CPlayer::Update(double CurTime, double dEllapsed, int *pModus, int *BackgroundX, int *BackgroundY, SDL_Rect rctMouse, SDL_Rect *prctrelMouse, SDL_Rect &rctBlock1) {
	bool bGeschafft = false;
	int iMinBlock = 0;
	int ibla = 0;
	
	if (bEditor == false) {	//nur im play mode
		rctPlayer.x = static_cast<int> (fMoveX);//position aktualisieren
		rctPlayer.y = static_cast<int> (fMoveY);//genauso
		
		ZumCheckpoint();//vielleicht zum checkpoint
		
		if (bImJump == false) {//bImJump updaten
			//testen ob man in freien raum GEGANGEN ist
			rctPlayer.y += 1;
			if (CBlock::Get()->bKollisionscheck(rctPlayer) == false) {
				bImJump = true;
				dJumpSteigung = 0;
			}
			rctPlayer.y -= 1;
		}
		
		if (CBlock::Get()->Update(CurTime, dEllapsed, &rctPlayer, bImLinks, bImRechts, dJumpSteigung, &iMinBlock, rctBlock1, bBeam) == true)
			*pModus = 1; // beim haus
		
		if (*pModus == 1 && rctPlayer.y + rctPlayer.h >= iMinBlock-1)//1 ist variable
			bGeschafft = true;  // auf dem boden
		
		//player aus dem rand ziehen
		if (fMoveX > iAllW-PLAYERW-iGameRand) {
			*BackgroundX += fMoveX - (iAllW-PLAYERW-iGameRand);
			if (*BackgroundX > iBackgroundW-iAllW)
				*BackgroundX = iBackgroundW -iAllW;
			rctPlayer.x = iAllW-PLAYERW-iGameRand;		fMoveX = rctPlayer.x;//position aktualisieren
			bImRand[1] = true;
		}
		if (fMoveX < iGameRand) {
			*BackgroundX -= iGameRand - fMoveX;
			if (*BackgroundX < 0)
				*BackgroundX = 0;
			rctPlayer.x = iGameRand;					fMoveX = rctPlayer.x;//position aktualisieren
			bImRand[3] = true;
		}
		if (fMoveY > iAllH-PLAYERH-iGameRand) {
			*BackgroundY += fMoveY - (iAllH-PLAYERH-iGameRand);
			if (*BackgroundY > iBackgroundH-iAllH)
				*BackgroundY = iBackgroundH - iAllH;
			rctPlayer.y = iAllH-PLAYERH-iGameRand;		fMoveY = rctPlayer.y;//position aktualisieren
			bImRand[2] = true;
		}
		if (fMoveY < iGameRand) {
			*BackgroundY -= iGameRand - fMoveY;
			if (*BackgroundY < 0)
				*BackgroundY = 0;
			ibla = iGameRand - rctPlayer.y;
			rctPlayer.y = iGameRand;					fMoveY = rctPlayer.y;//position aktualisieren
			bImRand[0] = true;
		}
	}
	else {
		CBlock::Get()->Update(CurTime, dEllapsed, &rctMouse, bImLinks, bImRechts, dJumpSteigung, &iMinBlock, rctBlock1, false, prctrelMouse);
	}
	
	return bGeschafft;
}

void CPlayer::Render(SDL_Surface *screen, SDL_Rect rctBlock1, SDL_Rect rctBlock2, bool bGesetzt1, bool bGesetzt2, int Art, int Anzahl, int modus, bool bAlles) {
	CBlock::Get()->Render(screen, rctBlock1, rctBlock2, bGesetzt1, bGesetzt2, Art, Anzahl, bAlles);
	
	if (rctPlayer.x >= 0 && rctPlayer.x < iAllW && rctPlayer.y >= 0 && rctPlayer.y < iAllH && (rctPlayer.x!=rctLastPlayer.x || rctPlayer.y != rctLastPlayer.y || bEditor == true || CBlock::Get()->GetImZiel() == true || rctLastPlayer.w==0 || modus != iLastModus || bAlles == true || CBlock::Get()->GetImStein1(rctPlayer) == true)) {
		if (bBack == false && rctLastPlayer.w!=0) {
			FILL(rctLastPlayer,ColBack);
			CBlock::Get()->rctKolRender(rctLastPlayer);
		}/*Player streichen*/
		BLIT (surPlayer[modus], NULL, screen, &rctPlayer);
		CBlock::Get()->SetImZiel(false);
	}
	
	//last player updaten
	FAST_RECT(rctLastPlayer,rctPlayer.x,rctPlayer.y,rctPlayer.w,rctPlayer.h);
	
	iLastModus = modus;
}

#ifndef CGAME_H_
#define CGAME_H_
class CGame : public TSingleton<CGame> , public CTimer
{
	public:
		CGame();
		~CGame();
		void Init();
		void Quit();
		
		void Run();//hauptfunktion
		void Eingabe(int iTyp = 1);//eingaben abfangen
		void Hacken(); //textbox verwenden
		void Render(bool bSave = false, bool bAlles = false);//rendern
		void ZeigerBallRender();
		void Update();//updaten
		void Flip2();//flippen
	
		SDL_Surface *screen;
		
	
	private:
		void Toggle_Fullscreen() {if (bFullScreen == false){bFullScreen=true;screen = SDL_SetVideoMode (iAllW+BOXW, iAllH, 24, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);} else {bFullScreen=false;screen = SDL_SetVideoMode (iAllW+BOXW, iAllH, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);}} 
		
		SDL_Surface *surBackground;//hinergrund
		SDL_Surface *surMenue;//menue bei escape
		SDL_Surface *ttfsurInfo[5];
		SDL_Surface *surZielZeiger;//ball am rand
		SDL_Rect rctZielZeiger;
		CTextBox TextBox;//textbox
		SDL_Rect rctBoxBereich;
		vector<SDL_Rect> vrctLastZeiger;//zum fillen
		vector<SDL_Rect>::iterator itrct;
		SDL_Rect rctInfo[5];//4 texte
		SDL_Rect rctScreen;//nur playbereich
		SDL_Rect rctBackground;//position
		SDL_Rect rctBackground2;//ausschnitt
		SDL_Cursor *curCursor1;//mauszeiger
		SDL_Cursor *curCursor2;//mauszeiger
		
		SDL_Event event;//event
		SDL_RWops *rwopBackground;//hintergrund wop
		SDL_Color ColBack; //farbe
		
		TTF_Font *pFont[2]; //schriftgroesse 24
		
		Mix_Music *Karuna;//hintergrundmusik
		
		bool bLinks;//links gedrueckt?
		bool bRechts;//rechts gedrueckt?
		bool bJump;//springen?
		bool bUp;//nach oben (aetzen)?
		bool bStein;//stein legen
		bool bAetzen;//stein wegaetzen
		bool bPause;//enter gedrueckt?
		bool bEscape;//escape gedrueckt?
		bool bGameOver;//gestorben?
		bool bNeuesSpiel;//neus spiel gewollt?
		bool bWeiter;//taste gedrueckt?
		bool bRestart;//level wiederholen?
		bool bQuit;//beenden?
		bool bLevelGeschafft;//level geschafft?
		bool bFullScreen;//vollbilbmodus
		bool bLastLevel;//ein level zurueck
		bool bNextLevel;//ein level vor
		bool bRender;//wird gerendert?
		bool bBackgroundAn;//an?
		bool bEditor;//editor an?
		bool bBlit;//muss geblittet werden?
		bool bTextBlit;//muss text geblittet werden?
		bool bTot;//zum letzten checkpoint?
		bool bSoundAn;//sound an?
		bool bExtraModus;//bestimmte knoepfe aktivierbar
		bool bActButton[3];//knoepfe zum aktiwieren
		bool bMBL;//maus links
		bool bFlip;//flippen?
		
		struct S_Edit {//stuktur fuer editor
			int iModus;//0 = einzeln, 1 = reihe
			SDL_Rect rctMouse;//mausposition
			SDL_Rect rctrelMouse;//insgesamte mausposition
			SDL_Rect rctStein[2];//beide steine position
			int iLoadLevel;//geladenes level
			bool bLoad;//soll geladen werden?
			int iSaveLevel;//speicher level
			bool bSave;//speichern?
			char chPfad[50];//save und load pfad
			int iAnzahl;//anzahl der bloecke bei reihe
			int iArt;//art des blocks
			int iCloudRichtung;//1 = Norden
			int iCloudL;//abstand nach links
			int iCloudR;//abstand nach rechts
			int iCloudSpeed;//geschwindigkeit
			bool bSteinGesetzt[2];//gerade im setzvorgang?
			bool bML;//maus links
			bool bMR;//maus rechts
			bool bJ;//jKey
			bool bL;//lKey
			bool bU;//uKey
			bool bO;//oKey
			bool b7;//7key
			bool b8;//8key
		} Editor;//editor
		
		struct S_Hacken {//stuktur fuers hacken
			int iAuswahl;
			string sText;
		} Hack;
		
		//fuer stopp
		int Zeit1;
		int Zeit2;
		int Zeit3;
		
		float fAktZeit;//aktuelle zeit
		float fHighscore;//highscore
		int iLevel;//aktuelles level, 0 = 1. level
		int iAnzLevel;//anzahl
		int iModus;//0 = auf; 1 = ab
		int iSchriftgroesse[2];
		
		vector<int> vMaxSteine;//die maximale anzahl steine pro level
		vector<int> vL;//linsspeed vektor
		vector<int> vR;//rechtsspeed vektor
		vector<int> vH;//hochspeed vektor
		struct S_Title {
			char chTitle[400];
			char chTipp[400];
		}Title;
		vector<S_Title> vLevelName;//name fuer die levels
		vector<S_Title>::iterator itvch;
		vector<int>::iterator itv;
		vector<double> vA;//steigung vektor
		
		//screen daten
		int iAllW, iAllH;
		
		int iBackgroundY, iBackgroundX;//y/x pos fuer background
		
		int iBackgroundW, iBackgroundH;
		
		char cText[50]; //text
		
		int iAnzSlow;//anzahl von zu slow
		double dFPS, dFPS2;//fps durchschnitt
		int iAnzFPS;
		int iCloudChange;//wert l und r zu veraendern
		
		int iBestLevel;//weiteste level
};
#endif /*CGAME_H_*/

CGame::CGame() {
	time_t zeit;//zeit
	time (&zeit);
	srand((unsigned)zeit);//zufallsgenerator initialisieren
	
	//konfigurationen lesen
	ifstream dat_ein;
	
	char cZeichen[400];
	int iZeile = 0, iBack = 2, iMapSpeed = 0, iGameRand = 0, iEditRand = 0, iSound = 2;
	
	dat_ein.open("Save/konfiguration.txt", ios_base::in);
	if ( !dat_ein.is_open()) { throw "Konfigurations-Datei konnte nicht geöffnet werden";}
	while (! dat_ein.eof() )
	{
  	  iZeile++;
  	  
  	  if (iZeile <= 12 && iZeile != 4 && iZeile != 7 && iZeile != 11)
  	  	dat_ein.getline (cZeichen,398,'#');
  	  
  	  if (iZeile == 1) {if (strtod(cZeichen,NULL) < AKTVERSION) throw "die konfigurationsdatei ist zu alt"; else if (strtod(cZeichen,NULL) > AKTVERSION) throw "die konfigurationsdatei ist zu neu";}
  	  else if (iZeile == 2) {iAllW = strtoul(cZeichen,NULL,10);}
  	  else if (iZeile == 3) {iAllH = strtoul(cZeichen,NULL,10);}
  	  else if (iZeile == 4) {//hintergrund
  	  	for (int i = 1; i < 5; i++) {
  	  		if (i == 4) {dat_ein.getline (cZeichen,398,'#');}
  	  		else {dat_ein.getline (cZeichen,398,',');}
  	  			
  	  		if (i == 1) {iBack = strtoul(cZeichen,NULL,10);}	
  	  		else if (i == 2) {ColBack.r = strtoul(cZeichen,NULL,10);}
  	  		else if (i == 3) {ColBack.g = strtoul(cZeichen,NULL,10);}
  	  		else if (i == 4) {ColBack.b = strtoul(cZeichen,NULL,10);}
  	  	}
	  }
  	  else if (iZeile == 5) {iBackgroundW = strtoul(cZeichen,NULL,10);}
  	  else if (iZeile == 6) {iBackgroundH = strtoul(cZeichen,NULL,10);}
  	  else if (iZeile == 7) {
	  	   for (int i = 1; i < 3; i++) {
	  	  		if (i == 2) {dat_ein.getline (cZeichen,398,'#');}
	  	  		else {dat_ein.getline (cZeichen,398,',');}
	  	  			
	  	  		if (i == 1) {iGameRand = strtoul(cZeichen,NULL,10);}	
	  	  		else if (i == 2) {iEditRand = strtoul(cZeichen,NULL,10);}
	  	   }
  	  }
  	  else if (iZeile == 8) {iMapSpeed = strtoul(cZeichen,NULL,10);}
  	  else if (iZeile == 9) {iSound = strtoul(cZeichen,NULL,10);}
  	  else if (iZeile == 10) {iCloudChange = strtoul(cZeichen,NULL,10);}
  	  else if (iZeile == 11) {
  	  	for (int i = 1; i < 3; i++) {
  	  		if (i == 2) {dat_ein.getline (cZeichen,398,'#');}
  	  		else {dat_ein.getline (cZeichen,398,',');}
  	  			
  	  		if (i == 1) {iSchriftgroesse[0] = strtoul(cZeichen,NULL,10);}	
  	  		else if (i == 2) {iSchriftgroesse[1] = strtoul(cZeichen,NULL,10);}
	  	}
  	  }
  	  else if (iZeile == 12) {iAnzLevel = strtoul(cZeichen,NULL,10);}
  	  else {
  	  	for (int i = 1; i < 8; i++) {
  	  		if (i == 7)
  	  			dat_ein.getline (cZeichen,398,'|');
  	  		else
  	  			dat_ein.getline (cZeichen,398,',');
  	  			
  	  		if (i == 1) {vMaxSteine.push_back(strtoul(cZeichen,NULL,10));}	
  	  		else if (i == 2) {vL.push_back(strtoul(cZeichen,NULL,10));}
  	  		else if (i == 3) {vR.push_back(strtoul(cZeichen,NULL,10));}
  	  		else if (i == 4) {vH.push_back(strtoul(cZeichen,NULL,10));}
  	  		else if (i == 5) {vA.push_back(strtod(cZeichen,NULL));}
  	  		else if (i == 6) {sprintf(Title.chTitle,"%s",cZeichen);}
  	  		else if (i == 7) {sprintf(Title.chTipp,"%s",cZeichen);vLevelName.push_back(Title);dat_ein.getline (cZeichen,398,'#');}
  	  	}
  	  }
  	  dat_ein.getline (cZeichen,398,'#');
	}
	vL.pop_back(); vR.pop_back(); vH.pop_back(); vA.pop_back(); vLevelName.pop_back();//den lesefehler korrigieren
	if (iBack == 0) {bBackgroundAn = false;} if (iBack == 1) {bBackgroundAn = true;}
	if (iSound == 0) {bSoundAn = false;} if (iSound == 1) {bSoundAn = true;}
	
	//optinen bei den anderen klassen setzen
	CPlayer::Get()->SetScreen(iAllW,iAllH,iBackgroundW,iBackgroundH,bBackgroundAn,iMapSpeed,iGameRand,iEditRand,ColBack);
	
	dat_ein.close();
	//konfiguration zu ende
	
	SDL_RWops *rwopIcon = NULL	;rwopIcon=SDL_RWFromFile ("media/PlayerJaR1.png", "rb");
	SDL_Surface *surIcon = NULL;surIcon = IMG_LoadPNG_RW(rwopIcon);
	SDL_WM_SetIcon(surIcon, NULL);//Icon
	
	//fenster
	screen = NULL; screen = SDL_SetVideoMode (iAllW +BOXW, iAllH, 24, SDL_HWSURFACE | SDL_DOUBLEBUF); // Das gesamte Fenster
	//fenstername
	SDL_WM_SetCaption("JaR | 2008 by Eric (C.)", "JaR");//titel
	
	//grafiken
	rwopBackground = SDL_RWFromFile ("media/Background.png", "rb");
	surBackground = NULL;surBackground = IMG_LoadPNG_RW(rwopBackground);if (surBackground == NULL){throw "backgroundgrafik konnte nicht geladen werden";}
	rwopBackground = SDL_RWFromFile ("media/Menue.png", "rb");
	surMenue = NULL;surMenue = IMG_LoadPNG_RW(rwopBackground);if(surMenue==NULL){throw "Menuegrafik konnte nicht geladen werden";}
	rwopBackground = SDL_RWFromFile ("media/ZeigerBallJaR.png", "rb");
	surZielZeiger = NULL;surZielZeiger = IMG_LoadPNG_RW(rwopBackground);if(surZielZeiger==NULL){throw "ZeigerBall-grafik konnte nicht geladen werden";}
	
	/*schriftart*/
	for (int i = 0; i < 2; i++) {pFont[i] = NULL; pFont[i] = TTF_OpenFont ("media/FreeSans.ttf", iSchriftgroesse[i]);}
	
	if (bSoundAn == true) {//musik
		Karuna = Mix_LoadMUS("media/Karuna-Stellamara.ogg");
		Mix_VolumeMusic(95);
		Mix_PlayMusic(Karuna, -1);
	} else Karuna = NULL;
	
	//mauszeiger
	const char *arrow[] = {
	  /* width height num_colors chars_per_pixel */
	  "    32    32        3            1",
	  /* colors */
	  "X c #000000",
	  ". c #ffffff",
	  "  c None",
	  /* pixels */
	  "            DDDDDDDD            ",
	  "         KKD........DKK         ",
	  "      XXXXM..........MXXXX      ",
	  "     XSSSSM..........MSSSSX     ",
	  "    XSSSSSSM........MSSSSSSX    ",
	  "   XSSSSSSSM........MSSSSSSSX   ",
	  "  XSSSSSSSSSM......MSSSSSSSSSX  ",
	  "  XSSSSSSSSSM......MSSSSSSSSSX  ",
	  "  XSSSSSSSSSSM....MSSSSSSSSSSX  ",
	  " KXSSSSSSSSSSM....MSSSSSSSSSSXK ",
	  " KMMSSSSSSSSSSM..MSSSSSSSSSSMMK ",
	  " D..MMSSSSSSSSSMMSSSSSSSSSMM..D ",
	  "D.....MMSSSSSSSSSSSSSSSSMM.....D",
	  "D.......MMSSSSSSSSSSSSMM.......D",
	  "D.........MSSSSSSSSSSM.........D",
	  "D..........MSSSSSSSSM..........D",
	  "D..........MSSSSSSSSM..........D",
	  "D.........MSSSSSSSSSSM.........D",
	  "D.......MMSSSSSSSSSSSSMM.......D",
	  "D.....MMSSSSSSSSSSSSSSSSMM.....D",
	  " D..MMSSSSSSSSSMMSSSSSSSSSMM..D ",
	  " KMMSSSSSSSSSSM..MSSSSSSSSSSMMK ",
	  " KXSSSSSSSSSSM....MSSSSSSSSSSXK ",
	  "  XSSSSSSSSSSM....MSSSSSSSSSSX  ",
	  "  XSSSSSSSSSM......MSSSSSSSSSX  ",
	  "  XSSSSSSSSSM......MSSSSSSSSSX  ",
	  "   XSSSSSSSM........MSSSSSSSX   ",
	  "    XSSSSSSM........MSSSSSSX    ",
	  "     XSSSSM..........MSSSSX     ",
	  "      XXXXM..........MXXXX      ",
	  "         KKD........DKK         ",
	  "            DDDDDDDD            ",
	  "0,0"
	};

	
	int i, row, col;
	Uint8 data[4*CURSORW];
	Uint8 mask[4*CURSORW];
	
	for (int j = 0; j < 2; j++) {
		i = -1;
		for ( row=0; row<CURSORH; ++row ) {
			for ( col=0; col<CURSORW; ++col ) {
		     	if ( col % 8 ) {
		    		data[i] <<= 1;
		        	mask[i] <<= 1;
		      	} else {
		        	++i;
		        	data[i] = mask[i] = 0;
		      	}
		      	switch (arrow[4+row][col]) {
		        	case '.':
		          		if(j==0) {data[i] |= 0x01;mask[i] |= 0x01;} //schwarz
		          		else {}//transparent
		          			break;	
		        	case 'M': mask[i] |= 0x01; break;//weiss
		          	case 'S':
		          		if(j==0) {}
		          		else {data[i] |= 0x01;mask[i] |= 0x01;}
		          			break;
		          	case 'X': if(j==0){} else {mask[i] |= 0x01;}break;
		          	case 'D': if(j==0) {mask[i] |= 0x01;} break;
		        	case ' ': break;
		      	}
		  	}
		}
		if(j==0) curCursor1 = SDL_CreateCursor(data, mask, CURSORW, CURSORH, 0, 0);
		else curCursor2 = SDL_CreateCursor(data, mask, CURSORW, CURSORH, 0, 0);
	}
	
	SDL_SetCursor(curCursor1);
	//mauszeige ende
	
	FAST_RECT(rctScreen,0,0,iAllW,iAllH)
	FAST_RECT(rctBoxBereich,iAllW,0,BOXW,BOXH)
	FAST_RECT(rctBackground,0,0,iAllW,iAllH)
	FAST_RECT(rctBackground2,0,0,iAllW,iAllH)
	FAST_RECT(rctInfo[0],0,0,350,iSchriftgroesse[0])
	FAST_RECT(rctInfo[1],iAllW-250,0,250,iSchriftgroesse[0])
	FAST_RECT(rctInfo[2],0,iAllH-iSchriftgroesse[0],500,iSchriftgroesse[0])
	FAST_RECT(rctInfo[3],iAllW-250,iAllH-iSchriftgroesse[0],250,iSchriftgroesse[0])
	FAST_RECT(rctInfo[4],iAllW/2-100,0,200,iSchriftgroesse[1])
	for (int i = 0; i < 5; i++)
		ttfsurInfo[i] = NULL;
	
	
	iLevel = 0;
	iBestLevel = 0;
	bFullScreen = false;
	bEditor = false;
	bExtraModus = false;
	for (int i = 0; i < 3; i++) {bActButton[i] = false;}
	CPlayer::Get()->SetEditor(bEditor);
	iAnzSlow = 0;
	dFPS = 0;
	iAnzFPS = 0;
	
	//editorinstanz initialisieren
	Editor.iModus = 0;
	FAST_RECT(Editor.rctMouse,iBackgroundW/2,iBackgroundH/2,0,0)
	FAST_RECT(Editor.rctStein[0],0,0,BLOCKW,BLOCKH)
	FAST_RECT(Editor.rctStein[1],0,0,BLOCKW,BLOCKH)
	Editor.iLoadLevel = 1;
	Editor.iSaveLevel = 1;
	sprintf(Editor.chPfad, "Save/Level%i.txt", Editor.iLoadLevel);
	Editor.iAnzahl = 0;
	Editor.iArt = 0;
	Editor.bSteinGesetzt[0] = false;
	Editor.bSteinGesetzt[1] = false;
	Editor.bML = false;
	Editor.bMR = false;
	Editor.iCloudRichtung = 1;
	Editor.iCloudL = 100;
	Editor.iCloudR = 100;
	Editor.iCloudSpeed = 100;
	Editor.bJ = false;
	Editor.bL = false;
	Editor.bU = false;
	Editor.bO = false;
	Editor.b7 = false;
	Editor.b8 = false;
	
	//textbox
	TextBox.init(&screen,rctBoxBereich,10,12,10,colWhite);
	TextBox.open();
	TextBox.load("Save/Logfile.txt");
	TextBox.up(4);
	TextBox.write("Spiel gestartet",colPurple);
	TextBox.up(2);
	
	Init();
	Quit();
}

CGame::~CGame() {
	SAFE_FREEN(surBackground)
	SAFE_FREEN(surMenue)
	SAFE_FREEN(surZielZeiger)
	for (int i = 0; i < 5; i++) {
		SAFE_FREEN(ttfsurInfo[i])
	}
	if(Karuna!=NULL) {Mix_FreeMusic(Karuna);Karuna = NULL;}
	if (curCursor1!=NULL){SDL_FreeCursor(curCursor1);curCursor1=NULL;}
	if (curCursor2!=NULL){SDL_FreeCursor(curCursor2);curCursor2=NULL;}
	
	TextBox.save("Save/Logfile.txt");
	TextBox.close();
	TextBox.quit();
}

void CGame::Init() {
	TimerInit();
	FAST_RECT(rctZielZeiger,0,0,0,0);
	FAST_RECT(Editor.rctrelMouse, 0, 0, 0, 0);
	
	bLinks = false;
	bRechts = false;
	bJump = false;
	bUp = false;
	bStein = false;
	bAetzen = false;
	bPause = false;
	bEscape = false;
	bGameOver = false;
	bNeuesSpiel = true;
	bWeiter = false;
	bQuit = false;
	bRestart = false;
	bLevelGeschafft = false;
	bLastLevel = false;
	bNextLevel = false;
	bRender = true;
	bTextBlit = true;
	bTot = false;
	bMBL = false;
	
	
	Editor.bSteinGesetzt[0] = false;
	Editor.bSteinGesetzt[1] = false;
	
	
	Zeit1 = 0;
	Zeit2 = 0;
	Zeit3 = 0;
	
	fAktZeit = 0;
	fHighscore = 0;
	iModus = 0;
	
	if (iLevel >= 0) {
		CBlock::Get()->SetLevel(iLevel);
		CBlock::Get()->SetMaxSteine(vMaxSteine[iLevel]);
		
		iBackgroundX = 0;
		iBackgroundY = iBackgroundH/2;
		FAST_RECT(rctBackground2,iBackgroundX,iBackgroundY,iAllW,iAllH)
	}
	
	CPlayer::Get()->Init(vL[iLevel],vR[iLevel],vH[iLevel],vA[iLevel]);
	CBlock::Get()->Init(CurTime);//die initialisierungen
	
	TimerUpdate();
}

void CGame::Quit() {
	CBlock::Get()->Quit();
	CPlayer::Get()->Quit();
	TimerUpdate();
}

void CGame::Run() {
	while (bQuit == false && iLevel < iAnzLevel) {
		Init();
		FILL(rctScreen,ColBack);//Fenster streichen
		CPlayer::Get()->Bewegung (AllTime.Ellapsed, bLinks, bRechts, bJump, bUp); // damit der player nicht in der luft klebt
		
		while (bLevelGeschafft == false && bRestart == false && bQuit == false && bLastLevel == false && bNextLevel == false) {
			TimerUpdate();
			if (bEscape == true) { // menue
				Eingabe(2);
				if (bWeiter == false) {
					BLIT (surMenue, NULL, screen, NULL);
					SDL_Flip(screen);
					bWeiter = true;
					Zeit1=SDL_GetTicks();
				}
				if(bEscape == false) {Zeit2=SDL_GetTicks();Zeit3+=Zeit2-Zeit1;}
			}
			else if (bEditor == false) { //gameplay
				bWeiter = false;
				
				if (EingabeTime.Ellapsed > 0.005) {
					EingabeTime.LastTime = CurTime;
					Eingabe();
				}
			
				if (bPause == false) {
					if (iModus == 1) {//falls downmodus jump und stein ausschalten
						bJump = false;
						bStein = false;
					}
					
//					if (bLinks == true || bRechts == true || bJump == true || CPlayer::Get()->GetImJump() == true)
					CPlayer::Get()->Bewegung (AllTime.Ellapsed, bLinks, bRechts, bJump, bUp, &bRestart);
					
					Update();//updates
					
					if (bStein == true) {bStein = false;CPlayer::Get()->SteinSetzen(CurTime);}
					if (bAetzen == true) {bAetzen = false;CPlayer::Get()->Aetzen(CurTime);}
					
					Render();//rendern
				}
			}
			
			else { //editor
				Eingabe(3);
				if (Editor.bLoad == true) {Editor.bLoad = false;Quit();CBlock::Get()->Load (Editor.iLoadLevel);}//laden
				if (Editor.bSave == true) {Editor.bSave = false;CBlock::Get()->Save (Editor.iSaveLevel);}//speichern
					
				Update ();
					
				Render();
			}
		}
		if (bLevelGeschafft == true) {iLevel++;bLevelGeschafft = false;}
		else if (bNextLevel == true && iLevel < iAnzLevel-1) {iLevel++;}
		else if (bLastLevel == true && iLevel > 0) {iLevel--;bLastLevel = false;}
		Editor.iLoadLevel = iLevel+1; Editor.iSaveLevel = iLevel+1; // edit level anpassungen
		if(iLevel > iBestLevel) iBestLevel = iLevel;
		
		Quit();//die quits
	}
	
}

void CGame::Hacken() {
	TextBox.up(2); TextBox.write("Eingabe Start"); TextBox.up();
	
	bool bExit = false;
	char ch[300];
	int i = 0;
	
	while (bExit == false) {
		Hack.iAuswahl = 0;
		TextBox.getStr(&Hack.sText, "Befehl:");
		
		if (Hack.sText == "menue" || Hack.sText == "Menue" || Hack.sText == "menu" || Hack.sText == "help") {
			while (Hack.iAuswahl == 0) {
				TextBox.up(2);
				TextBox.write("Continue[1]",colBlack);
				TextBox.write("Level waehlen[2]",colBrown);
				TextBox.write("Tipp[3]",colGrey);
				TextBox.write("Quit Game[4]",colBlue);
				TextBox.up();
				Hack.iAuswahl = TextBox.getInt("Wahl:");
				TextBox.up();
				switch (Hack.iAuswahl) {
					case 1: bExit = true; break;
					case 2: TextBox.up();TextBox.write("alle level: ",colBlack);TextBox.up();i=0;
						for(itvch = vLevelName.begin(); itvch != vLevelName.end(); itvch++) {
							i++;
							sprintf(ch,"[%i] %s", i, (*itvch).chTitle); 
							TextBox.write(ch, colBlack);
						} TextBox.up();
						TextBox.write("Back[0]",colBlack);
						Hack.iAuswahl = TextBox.getInt("Wahl:");
						if (Hack.iAuswahl == 0) {}
						else if (Hack.iAuswahl < iAnzLevel) {iLevel = Hack.iAuswahl-1; bRestart = true; bExit = true;}
						else TextBox.write("falsche eingabe",colRed);
					
						 break;
					case 3: TextBox.up();sprintf(ch,"Tipp: %s",vLevelName[iLevel].chTipp);TextBox.write(ch,colBrown);TextBox.up();bExit=true; break;
					case 4: bQuit = true; bExit = true; break;
					default: TextBox.write("falsche eingabe",colRed); Hack.iAuswahl = 0; break;
				}
			}
		}
		else {
			TextBox.write("falsche eingabe",colRed);
			bExit = true;
		}  
	}
	TextBox.up(); TextBox.write("Eingabe zu Ende"); TextBox.up();
	TimerUpdate();TimerUpdate();
}

void CGame::Eingabe(int iTyp) {
	if ((event.motion.x > 0 || (event.motion.x == 0 && event.motion.y > 0)) && event.motion.x <= iAllW+BOXW) {Editor.rctMouse.x = event.motion.x + CURSORW/2;}//XMausposition updaten
	if ((event.motion.y > 0 || (event.motion.y == 0 && event.motion.x > 0)) && event.motion.y <= iAllH) {Editor.rctMouse.y = event.motion.y + CURSORH/2;}//YMausposition updaten
	
	bRestart = CBlock::Get()->GetRestart(); //bRestart bekommen
	
	while(SDL_PollEvent(&event)) {//EingabeAbfrage
		switch(event.type){
			case SDL_KEYDOWN: {//Tast unten?
				if (iTyp == 1) {
					bTextBlit = true;
					switch(event.key.keysym.sym){
						case SDLK_b:{bJump = true;}break;
						case SDLK_n:{bStein = true;}break;
						case SDLK_m:{bAetzen = true;} break;
						case SDLK_v:{if (bExtraModus == true) {if(iModus==0)iModus=1;else {iModus = 0;CBlock::Get()->SetBeimZiel(false);}}} break;
						case SDLK_c:{if (bExtraModus==true || iLevel < iBestLevel){bNextLevel = true;}}break;
						case SDLK_x:{bLastLevel = true;} break;
						case SDLK_t:{CPlayer::Get()->ZumCheckpoint(true);TimerUpdate();} break;
						case SDLK_RETURN:{if (bPause == false){bPause = true;Zeit1=SDL_GetTicks();} else {bPause = false;Zeit2=SDL_GetTicks();Zeit3+=Zeit2-Zeit1;}} break;
						case SDLK_LEFT:{bLinks = true;} break;
						case SDLK_RIGHT:{bRechts = true;} break;
						case SDLK_UP:{bUp = true;} break;
						case SDLK_ESCAPE:{bEscape = true;} break;
						case SDLK_KP5:{SWITCH(bActButton[0]);} break;
						case SDLK_KP_DIVIDE:{SWITCH(bActButton[1]);} break;
						case SDLK_KP_MINUS:{SWITCH(bActButton[2]);} break;
						case SDLK_KP0: if(bExtraModus==true) Hacken(); break;
						default:{bTextBlit = false;}break;
					}
					//extramodus aus/an?
					if (bActButton[0] == true && bActButton[1] == true && bActButton[2] == true) {bExtraModus = true;}
					else {bExtraModus = false;}
				}
				if (iTyp == 2) {
					switch(event.key.keysym.sym){
						case SDLK_ESCAPE:{bQuit = true;}break;
						case SDLK_r:{bRestart = true;TimerUpdate();}break;
						case SDLK_RETURN:{bEscape = false;bTextBlit = true;FILL(rctScreen,ColBack);CPlayer::Get()->Render(screen, Editor.rctStein[0], Editor.rctStein[1], Editor.bSteinGesetzt[0], Editor.bSteinGesetzt[1], Editor.iArt, Editor.iAnzahl, iModus, true);}break;
						default:{}break;
					}
				}
				if (iTyp == 3) {
					bTextBlit = true;
					switch(event.key.keysym.sym){
						case SDLK_q:{Editor.iModus++;}break;
						case SDLK_w:{Editor.iLoadLevel++; Editor.bLoad=true;}break;
						case SDLK_s:{Editor.iLoadLevel--; Editor.bLoad=true;FILL(rctScreen,ColBack)}break;
						case SDLK_a:{Editor.iSaveLevel--;}break;
						case SDLK_d:{Editor.iSaveLevel++;}break;
						case SDLK_i:{Editor.iCloudRichtung++;}break;
						case SDLK_k:{Editor.iCloudRichtung--;}break;
						case SDLK_u:{Editor.bU = true;}break;
						case SDLK_o:{Editor.bO = true;}break;
						case SDLK_j:{Editor.bJ = true;}break;
						case SDLK_l:{Editor.bL = true;}break;
						case SDLK_7:{Editor.b7 = true;}break;
						case SDLK_8:{Editor.b8 = true;}break;
						case SDLK_1: {Editor.iLoadLevel-=10;Editor.bLoad=true;FILL(rctScreen,ColBack);} break;
						case SDLK_2: {Editor.iLoadLevel+=5;Editor.bLoad=true;} break;
						case SDLK_3: {Editor.iLoadLevel+=10;Editor.bLoad=true;} break;
						case SDLK_4: {Editor.iSaveLevel-=10;} break;
						case SDLK_5: {Editor.iSaveLevel+=5;} break;
						case SDLK_6: {Editor.iSaveLevel+=10;} break;
						case SDLK_z:{CBlock::Get()->Back();Render(false,true);}break;
						case SDLK_g:{Editor.bSave = true; Render(true);}break;
						case SDLK_UP:{Editor.iAnzahl++;}break;
						case SDLK_DOWN:{Editor.iAnzahl--;}break;
						case SDLK_LEFT:{Editor.iArt--;}break;
						case SDLK_RIGHT:{Editor.iArt++;}break;
						case SDLK_KP9:{CBlock::Get()->MapMovement(-BLOCKW,BLOCKH, &Editor.rctStein[0]);Editor.rctrelMouse.x+=40;Editor.rctrelMouse.y+=40;}break;
						case SDLK_KP6:{CBlock::Get()->MapMovement(-BLOCKW,0, &Editor.rctStein[0]);Editor.rctrelMouse.x+=40;}break;
						case SDLK_KP3:{CBlock::Get()->MapMovement(-BLOCKW,-BLOCKH, &Editor.rctStein[0]);Editor.rctrelMouse.x+=40;Editor.rctrelMouse.y-=40;}break;
						case SDLK_KP2:{CBlock::Get()->MapMovement(0,-BLOCKH, &Editor.rctStein[0]);Editor.rctrelMouse.y-=40;}break;
						case SDLK_KP1:{CBlock::Get()->MapMovement(BLOCKW,-BLOCKH, &Editor.rctStein[0]);Editor.rctrelMouse.x-=40;Editor.rctrelMouse.y-=40;}break;
						case SDLK_KP4:{CBlock::Get()->MapMovement(BLOCKW,0, &Editor.rctStein[0]);Editor.rctrelMouse.x-=40;}break;
						case SDLK_KP7:{CBlock::Get()->MapMovement(BLOCKW,BLOCKH, &Editor.rctStein[0]);Editor.rctrelMouse.x-=40;Editor.rctrelMouse.y+=40;}break;
						case SDLK_KP8:{CBlock::Get()->MapMovement(0,BLOCKH, &Editor.rctStein[0]);Editor.rctrelMouse.y+=40;}break;
						case SDLK_ESCAPE:{if(bEditor==true){bEditor=false;Quit();Init();}else{bEditor=true;Quit();Editor.bLoad=true;}CPlayer::Get()->SetEditor(bEditor);FILL(rctScreen,ColBack)} break;
						case SDLK_KP0: if(bExtraModus==true) Hacken(); break;
						default:{bTextBlit = false;}break;
					}
					//anpassungen
					if (Editor.iModus > 2) { Editor.iModus = 0;}
					if (Editor.iLoadLevel > iAnzLevel) { Editor.iLoadLevel = iAnzLevel; Editor.bLoad = false;} else if(Editor.bLoad == true) {FILL(rctScreen,ColBack)}
					if (Editor.iLoadLevel < 0) { Editor.iLoadLevel = 0;}
					if (Editor.iSaveLevel < 1) { Editor.iSaveLevel = 1;}
					if (Editor.iAnzahl < 0) { Editor.iAnzahl = 0;}
					if (Editor.iArt < 0) { Editor.iArt = 7;}
					if (Editor.iArt > 7) { Editor.iArt = 0;}
					if (Editor.iCloudRichtung > 8) { Editor.iCloudRichtung = 1;}
					if (Editor.iCloudRichtung < 1) { Editor.iCloudRichtung = 8;}
					iLevel = Editor.iLoadLevel-1;//level anpassen fuer titel
				}
			} break;
			case SDL_KEYUP: {//taste oben?
				CBlock::Get()->SetCloud(Editor.iCloudRichtung,Editor.iCloudL,Editor.iCloudR);
				switch(event.key.keysym.sym){
					case SDLK_b:{bJump = false;}break;
					case SDLK_n:{bStein = false;}break;
					case SDLK_m:{bAetzen = false;} break;
					case SDLK_UP:{bUp = false;} break;
					case SDLK_e:{if (bExtraModus==true){if(bEscape == false){if(bEditor==true){bEditor=false;Quit();Init();TimerUpdate();}else{bEditor=true;Quit();Init();Quit();Editor.bLoad=true;}CPlayer::Get()->SetEditor(bEditor);FILL(rctScreen,ColBack)} SDL_WarpMouse((iAllW-CURSORW)/2,(iAllH-CURSORH)/2);}} break;
					case SDLK_f:{Toggle_Fullscreen();bTextBlit = true;FILL(rctScreen,ColBack);CPlayer::Get()->Render(screen, Editor.rctStein[0], Editor.rctStein[1], Editor.bSteinGesetzt[0], Editor.bSteinGesetzt[1], Editor.iArt, Editor.iAnzahl, iModus, true);} break;
					case SDLK_LEFT:{bLinks = false;} break;
					case SDLK_RIGHT:{bRechts = false;} break;
					case SDLK_u:{Editor.bU = false;}break;
					case SDLK_o:{Editor.bO = false;}break;
					case SDLK_j:{Editor.bJ = false;}break;
					case SDLK_l:{Editor.bL = false;}break;
					case SDLK_7:{Editor.b7 = false;}break;
					case SDLK_8:{Editor.b8 = false;}break;
					case SDLK_h:{FILL(rctScreen,ColBack);Render(false,true);} break;//alles blitten
					default:{}break;
				}
			} break;
			
			case SDL_MOUSEBUTTONDOWN: {//maustast gedrueckt?
				if (iTyp == 3) {
					switch(event.button.button) {
						case SDL_BUTTON_LEFT: {//linke taste?	 
							Editor.bML = true;
							if (Editor.iModus == 0)
								CBlock::Get()->NewBlock(Editor.iArt,Editor.rctMouse.x-(BLOCKW/2),Editor.rctMouse.y-(BLOCKH/2), -1,Editor.rctMouse.x-(BLOCKW/2),Editor.rctMouse.y-(BLOCKH/2),0, Editor.iCloudRichtung, Editor.iCloudL, Editor.iCloudR, Editor.iCloudSpeed);
							else if (Editor.iModus == 1) {
								if (Editor.bSteinGesetzt[0] == false) {
									 Editor.bSteinGesetzt[0] = true;
									 Editor.rctStein[0].x =  Editor.rctMouse.x-(BLOCKW/2);
									 Editor.rctStein[0].y =  Editor.rctMouse.y-(BLOCKH/2);
									 Editor.rctStein[1].x =  Editor.rctMouse.x-(BLOCKW/2);
									 Editor.rctStein[1].y =  Editor.rctMouse.y-(BLOCKH/2);
								}
								else if (Editor.bSteinGesetzt[1] == false)
									Editor.bSteinGesetzt[1] = true;
									
								if (Editor.bSteinGesetzt[0] == true && Editor.bSteinGesetzt[1] == true) {
									Editor.bSteinGesetzt[0] = false;
									Editor.bSteinGesetzt[1] = false;
									CBlock::Get()->NewBlock(Editor.iArt,Editor.rctStein[0].x,Editor.rctStein[0].y, Editor.iAnzahl,Editor.rctStein[1].x,Editor.rctStein[1].y,0, Editor.iCloudRichtung, Editor.iCloudL, Editor.iCloudR, Editor.iCloudSpeed);
								} 
							}
							else if (Editor.iModus == 2) {
								CBlock::Get()->NewBlock(Editor.iArt,Editor.rctMouse.x-(Editor.rctMouse.x%BLOCKW),Editor.rctMouse.y-(Editor.rctMouse.y%BLOCKH), -2,Editor.rctMouse.x-(Editor.rctMouse.x%BLOCKW),Editor.rctMouse.y-(Editor.rctMouse.y%BLOCKH),0, Editor.iCloudRichtung, Editor.iCloudL, Editor.iCloudR, Editor.iCloudSpeed);
							}
						} break;
						
						case SDL_BUTTON_RIGHT: {//rechte taste?
							SDL_SetCursor(curCursor2);
							if (Editor.bSteinGesetzt[0] == true) {
								Editor.bSteinGesetzt[0] = false;
								Editor.bSteinGesetzt[1] = false;
							}
							else 
								Editor.bMR = true;	
						} break;
						
						case SDL_BUTTON_WHEELUP: {if(KOLLISION(Editor.rctMouse,rctScreen)){Editor.iArt++;if (Editor.iArt > 7)Editor.iArt=0;} else TextBox.down();} break;
						case SDL_BUTTON_WHEELDOWN: {if(KOLLISION(Editor.rctMouse,rctScreen)){Editor.iArt--;if (Editor.iArt < 0)Editor.iArt=7;} else TextBox.up();} break;
						case SDL_BUTTON_MIDDLE: {CBlock::Get()->BlockRaster(Editor.rctMouse);} break;
							
						default: {} break;
					}
				}
				else if (iTyp == 1) {
					switch(event.button.button) {
						case SDL_BUTTON_LEFT: bMBL = true; break;
						case SDL_BUTTON_RIGHT: bJump = true; break;
						case SDL_BUTTON_WHEELUP: {if(KOLLISION(Editor.rctMouse,rctScreen)){} else TextBox.down();} break;
						case SDL_BUTTON_WHEELDOWN: {if(KOLLISION(Editor.rctMouse,rctScreen)){} else TextBox.up();} break;
					}	 
				}		 
			}break;
			case SDL_MOUSEBUTTONUP: {//maustaste hoch?
				if (iTyp == 3) {
					switch(event.button.button) {
						case SDL_BUTTON_LEFT: {Editor.bML = false;} break;
						case SDL_BUTTON_RIGHT: {Editor.bMR = false;SDL_SetCursor(curCursor1);} break;
						default: {} break;
					}
				}
				else if (iTyp == 1) {
					switch(event.button.button) {
						case SDL_BUTTON_LEFT: {bMBL = false;bLinks = false; bRechts = false;} break;
						case SDL_BUTTON_RIGHT: {bJump = false;} break;
						default: {} break;
					}
				}
			} break;
			
			case SDL_QUIT:{if (bEscape==true){bQuit=true;} else {bEscape = true;}} break;
			
			default: {} break;
		}
	}
	if (Editor.bMR == true) {CBlock::Get()->Erease(Editor.rctMouse);}
	if (Editor.bML == true && Editor.iModus == 2) {CBlock::Get()->NewBlock(Editor.iArt,Editor.rctMouse.x-(Editor.rctMouse.x%BLOCKW),Editor.rctMouse.y-(Editor.rctMouse.y%BLOCKH), -2,Editor.rctMouse.x-(Editor.rctMouse.x%BLOCKW),Editor.rctMouse.y-(Editor.rctMouse.y%BLOCKH),0, Editor.iCloudRichtung, Editor.iCloudL, Editor.iCloudR, Editor.iCloudSpeed);}
	if (Editor.bSteinGesetzt[0] == true) {Editor.rctStein[1].x =  Editor.rctMouse.x-(BLOCKW/2); Editor.rctStein[1].y =  Editor.rctMouse.y-(BLOCKH/2);}
	
	//updates
	if (Editor.bU == true) {Editor.iCloudL -= iCloudChange * AllTime.Ellapsed;if (Editor.iCloudL < 0){Editor.iCloudL=0;}bTextBlit=true;}
	if (Editor.bO == true) {Editor.iCloudL += iCloudChange * AllTime.Ellapsed;bTextBlit=true;}
	if (Editor.bJ == true) {Editor.iCloudR -= iCloudChange * AllTime.Ellapsed;if (Editor.iCloudR < 0){Editor.iCloudR=0;}bTextBlit=true;}
	if (Editor.bL == true) {Editor.iCloudR += iCloudChange * AllTime.Ellapsed;bTextBlit=true;}
	if (Editor.b7 == true) {Editor.iCloudSpeed -= iCloudChange * AllTime.Ellapsed;if (Editor.iCloudSpeed < 0){Editor.iCloudSpeed=0;}bTextBlit=true;}
	if (Editor.b8 == true) {Editor.iCloudSpeed += iCloudChange * AllTime.Ellapsed;bTextBlit=true;}
	
	//maus bei typ 1
	if (bMBL == true && iTyp == 1) {if (CPlayer::Get()->GetDirection(Editor.rctMouse.x) == 1) {bLinks = true;bRechts = false;} else if (CPlayer::Get()->GetDirection(Editor.rctMouse.x) == 2) {bRechts = true;bLinks=false;} else {bLinks = false; bRechts=false;}}
}

void CGame::Render(bool bSave, bool bAlles) {
	int j = 0;//fuer vector
	if (BlitTime.Ellapsed > 2.00) {//frames fuer resourcen festsetzen ( ~0.5 mal pro sekunde)	
		BlitTime.LastTime = CurTime;//damit die framerate stimmt
//		FILLRECT; bAlles = true; // momentan nicht gebraucht
	}
	if(bAlles == true) {FILL(rctScreen,ColBack);}//streichen
	
	rctBackground2.y = iBackgroundY; rctBackground2.x = iBackgroundX;//hintergrund anpassen
	
	if (bBackgroundAn == true) {BLIT (surBackground, &rctBackground2, screen, &rctBackground);}
	
	CPlayer::Get()->Render(screen, Editor.rctStein[0], Editor.rctStein[1], Editor.bSteinGesetzt[0], Editor.bSteinGesetzt[1], Editor.iArt, Editor.iAnzahl, iModus, bAlles);
	
	if (CPlayer::Get()->GetbFlip() == true || CBlock::Get()->GetbFlip() == true) bTextBlit = true;//text blitten ?
	if ((bTextBlit == true || bAlles == true) && bBackgroundAn == false) {for (int i = 0; i < 5; i++) {FILL(rctInfo[i],ColBack);CBlock::Get()->rctKolRender(rctInfo[i]);}}//infobereiche streichen
	
	ZeigerBallRender();//die kreise am rand rendern
	
	if (bTextBlit == true || bAlles == true) {
		bTextBlit = false;
		for (int i = 0; i < 5; i++) {
			if (i==0 && bEditor == false && bExtraModus == false){sprintf (cText,"Level %i", iLevel+1);}
			if (i==0 && bEditor == false && bExtraModus == true){sprintf (cText,"Level %i   Extramode", iLevel+1);}
			if (i==0 && bEditor == true){
				if (bSave == true) {sprintf (cText,"Speichern");}
				else {sprintf (cText,"Editor; Load:%i;Save:%i", Editor.iLoadLevel, Editor.iSaveLevel);}
			}
			if (i==1 && bEditor == false){sprintf (cText,"Gegebene Bloecke: %i", vMaxSteine[iLevel]);}
			if (i==1 && bEditor == true){sprintf (cText,"Modus: %i;Cursor:(%i|%i);(%i|%i)", Editor.iModus, Editor.rctMouse.x, Editor.rctMouse.y, Editor.rctrelMouse.x+(Editor.rctMouse.x-(iAllW/2)), Editor.rctrelMouse.y-(Editor.rctMouse.y-(iAllH/2)));}
			if (i==2 && bEditor == false){sprintf (cText,"%.2f FPS", dFPS2);}
			if (i==2 && bEditor == true){sprintf (cText,"Blockart:%i;Richtung:%i;S:%i;L:%i;R:%i", Editor.iArt, Editor.iCloudRichtung, Editor.iCloudSpeed, Editor.iCloudL, Editor.iCloudR);}
			if (i==3 && bEditor == false){sprintf (cText,"Restliche Bloecke: %i", vMaxSteine[iLevel] - CBlock::Get()->GetVerbrauchteSteine());}
			if (i==3 && bEditor == true){sprintf (cText,"Anzahl Steine: %i", Editor.iAnzahl);}
			
			SAFE_FREEN(ttfsurInfo[i]);//alte surface freen
			if (i < 4)
				ttfsurInfo[i] = TTF_RenderUTF8_Blended(pFont[0], cText, colWhite);
			else { // titel
				for (itvch = vLevelName.begin(); itvch != vLevelName.end(); itvch++) {
					if (j == iLevel)
						ttfsurInfo[i] = TTF_RenderUTF8_Blended(pFont[1], (*itvch).chTitle, colRed);
					j++;
				}
			}
			BLIT (ttfsurInfo[i], NULL, screen, &rctInfo[i]);
		}
	}
	
	Flip2 ();//veraenderungen anzeigen
	if (bSave == true) {SDL_Delay(1000);}//bei speichern warten
}

void CGame::Update() {
	if (AllTime.Ellapsed > 0.12){//programm abbrechen, falls zu langsam
		iAnzSlow++;
		if (iAnzSlow > 5)
			throw "Wiederholungsrate ist zu gering";
	}
	if (AllTime.Ellapsed < 0.12) {iAnzSlow = 0;}//damit es auch richtig funktioniert
	
	
	if (UpdateTime.Ellapsed > 0.005) {//frames fuer resourcen festsetzen ( ~20 mal pro sekunde)	
		bLevelGeschafft = CPlayer::Get()->Update (CurTime, AllTime.Ellapsed, &iModus, &iBackgroundX, &iBackgroundY, Editor.rctMouse, &Editor.rctrelMouse, Editor.rctStein[0]);
		UpdateTime.LastTime = CurTime;//damit die framerate stimmt
	}
	
	
	//framerate berechnen
	if (iAnzFPS > 100) {
		dFPS /= iAnzFPS;
		if (dFPS < 500.0 && dFPS > 0) {dFPS2 = dFPS; bTextBlit = true;}
		dFPS = 0;
		iAnzFPS = 0;
	}	
	dFPS += 1/AllTime.Ellapsed;
	iAnzFPS++;
	
	TextBox.update();
}

void CGame::ZeigerBallRender() {
	for (itrct = vrctLastZeiger.begin(); itrct != vrctLastZeiger.end(); itrct++) { //alte zeigerpunkte fillen
		FILL((*itrct),ColBack);
		CBlock::Get()->rctKolRender((*itrct));
	}
	vrctLastZeiger.clear();//vektor leeren
	
	for (int i = 1; CBlock::Get()->GetZeigerPos(&rctZielZeiger,i) == true; i++) { // neue zeigerpunkte blitten
		if (rctZielZeiger.x != -1) {
			BLIT (surZielZeiger, NULL, screen, &rctZielZeiger);
			vrctLastZeiger.push_back(rctZielZeiger);
		}
	}	
}

void CGame::Flip2() {
	if (bFlip == true || CPlayer::Get()->GetbFlip() == true || CBlock::Get()->GetbFlip() == true) {
		CPlayer::Get()->SetbFlip(false); CBlock::Get()->SetbFlip(false); bFlip = false;
		SDL_Flip(screen);
	}
}

// main
int main(int argc, char **argv)
{
	try {
		if(SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) == -1) //SDL Initialisierung
			throw "SDL konnte nicht initialisiert werden"; 
		TTF_Init ();//Text
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);//musik init
		Mix_AllocateChannels(20);
		
		CGame::Get();
		CPlayer::Get();
		CBlock::Get();
		
		CGame::Get()->Run();
	}
	
	catch (const char *Fehlerstring) {
		cout << "Ausnahme aufgetrten: " << Fehlerstring << "\nSpeicher wird freigegeben und das Programm sauber heruntergefahren\n";
	}
	
	CBlock::Del();
	CPlayer::Del();
	CGame::Del();
	
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
	
	return 0;
}
