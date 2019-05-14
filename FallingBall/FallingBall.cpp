//FallingBall

//variable defines
#define TEXTSPEED 1000.0f
#define BALLSPEEDW 800.0f
#define BALLSPEEDDOWN 500.0f
#define SPEEDUP 150.0f
#define ANZPITS 6
#define ANZROWS 6
#define TIMERENDER 0.05f
#define TIMESPEED 15.0f
#define TIMEPIT 10.0f
//wichtige kleine konstante defines
//~ #define PI 3.14159265358979
#define ALLW 800
#define ALLH 600
#define BALLW 40
#define BALLH 40
#define BALKENW 50
#define BALKENH 10
#define TEXTW 140
#define TEXTH 24
#define BALLZOOM 1.0
#define BALLSMOOTH 0
#define ANZBALKEN 16
//~ #define BALLANGLE (360*(BALLSPEEDW/(BALLW*PI)))

//praktische lange defines
#define SAFE_FREEN(X) {if(X!=NULL){SDL_FreeSurface(X); X = NULL;}}
#define FAST_RECT(X,A,B,C,D) {X.x = A; X.y = B; X.w = C; X.h = D;}
#define NEW_ROW(X) {for(int i=0;i<ANZBALKEN;i++){if(!bvectorCheck(i+1)){FAST_RECT(rctBla,i*BALKENW,X,BALKENW,BALKENH);lrctBalken.push_back(rctBla);}}}

#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <time.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
//~ #include "SDL_rotozoom.h"
using namespace std; 


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



class CTimer
{
public:
	CTimer();
protected:
	double TimerUpdate();//timer aktualisieren
	void SetStopTime (double StopTime) {Zeit3 = StopTime;}
	double GetCurTime () {return CurTime;}
	void TimerInit();
	
	double Zeit3;//stopzeit
	double CurTime;//vergangene zeit
	double newTime;//Zeit bei neustart
	
	struct S_Times//struktur f¸r die verschiedenenereignisse
	{
		double Ellapsed;
		double LastTime;
	};
	S_Times AllTime;//insgesamt
	S_Times BlitTime;//fuer rendern
	S_Times SpeedTime;//speed erhoehen
	S_Times PitTime;//pits verringern
};

CTimer::CTimer()
{
}

double CTimer :: TimerUpdate (){//timer aktualisieren
	CurTime = ((SDL_GetTicks()-Zeit3) / 1000.0f) - newTime;//curtime - gestoppte zeit und neue zeit
	
	AllTime.Ellapsed = CurTime - AllTime.LastTime;
	AllTime.LastTime = CurTime;
	
	BlitTime.Ellapsed = CurTime - BlitTime.LastTime;
	SpeedTime.Ellapsed = CurTime - SpeedTime.LastTime;
	PitTime.Ellapsed = CurTime - PitTime.LastTime;
	
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
	SpeedTime.Ellapsed = 0.0;
	SpeedTime.LastTime = 0.0;
	PitTime.Ellapsed = 0.0;
	PitTime.LastTime = 0.0;
	
	CurTime = SDL_GetTicks() / 1000;//insgesamt zeit
}


class CBalken : public TSingleton<CBalken>
{
	friend class CRender;
public:
	CBalken();
	~CBalken();
	void Init();
	void Quit();
	
	void newBalken(int X = 600){for(itervRandoms=vRandoms.begin();itervRandoms!=vRandoms.end();itervRandoms++){(*itervRandoms)=Zufall();}NEW_ROW(X);Stufe++;}//neue reihe balken
	void Update(double Ellapsed, float fSchwierigkeitsgrad);//pdaten
	void Rendern();//rendern
	void minPits(){if(static_cast<int>(vRandoms.size())>2){vRandoms.pop_back();}}//pits verringern
	
	bool bKollisionscheck(SDL_Rect &Rect);//kollisionscheck
	bool bvectorCheck(int X);//fuer die pits
	
	int Zufall(int Bereich = ANZBALKEN){return (rand()%Bereich+1);}//zufallszahl bekommen
	int GetStufe(){return Stufe;}//anzahl der stufen bekommen
private:
	SDL_RWops *rwopBalken;//fuer den Balken (image)
	SDL_Surface *surBalken;//balken surface
	SDL_Rect rctBla;//rect fuer neue balken
	
	list<SDL_Rect> lrctBalken;//liste fuer die balken
	list<SDL_Rect>::iterator iterrctBalken;
	vector<int> vRandoms;//vektor fuer die pits
	vector<int>::iterator itervRandoms;
	
	bool bnewBalken;//kommt neuer balken?
	
	int Stufe;//anzahl der stufen
	
	float m_fXPos;//fuer die balken
	float m_fYPos;//fuer die balken
};

CBalken::CBalken(){//konstruktor
	time_t zeit;//zeit
	time (&zeit);
	srand((unsigned)zeit);//zufallsgenerator initialisieren
	
	rwopBalken = NULL;
	rwopBalken = SDL_RWFromFile ("media/Balken.png", "rb");
	surBalken = NULL;
	surBalken = IMG_LoadPNG_RW(rwopBalken);//ballgrafik laden
}

CBalken::~CBalken(){//destruktor
	SAFE_FREEN(surBalken);//ballgrafik freen
	
	Quit();
	
	lrctBalken.clear();//liste loeschen
}

void CBalken :: Init(){//initialisieren
	bnewBalken = false;
	
	for (int i = 0; i < ANZPITS; i++)//zufaelle fuer die pits
		vRandoms.push_back(Zufall());
	
	Stufe = 0;//0 Stufen
	
	for (int i = 0; i < ANZROWS; i++)//die rows
		newBalken((i+1)*(ALLH/ANZROWS));
}

void CBalken :: Quit(){
	//alle listen/vektoren leeren
	int Anz = 0;
	for(itervRandoms=vRandoms.begin();itervRandoms!=vRandoms.end();itervRandoms++)
		Anz++;
	for(int i = Anz; i > 0; i--)
		vRandoms.pop_back();
	Anz = 0;
	for(iterrctBalken=lrctBalken.begin();iterrctBalken!=lrctBalken.end();iterrctBalken++)
		Anz++;
	for(int i = Anz; i > 0; i--)
		lrctBalken.pop_back();
}

void CBalken :: Update(double Ellapsed, float fSchwierigkeitsgrad){
	for (iterrctBalken=lrctBalken.begin(); iterrctBalken!=lrctBalken.end(); iterrctBalken++){//alle Balken updaten 
		m_fYPos = (SPEEDUP * Ellapsed) * fSchwierigkeitsgrad;
		(*iterrctBalken).y -= static_cast<int> (m_fYPos);
		if ((*iterrctBalken).y < 1){//wenn noetig loeschen
			lrctBalken.pop_front();
			iterrctBalken--;
			bnewBalken = true;//wenn geloescht spawnen
		}
	}	
	
	if(bnewBalken == true){//spawnen
		bnewBalken = false;
		newBalken();
	}
}

bool CBalken :: bKollisionscheck(SDL_Rect &Rect){//kollisioin mit ball ueberpruefen
	bool bret = false;
	for (iterrctBalken = lrctBalken.begin(); iterrctBalken != lrctBalken.end(); iterrctBalken++){
		if(Rect.x < (*iterrctBalken).x + (*iterrctBalken).w && Rect.x + Rect.w > (*iterrctBalken).x &&
						Rect.y < (*iterrctBalken).y + (*iterrctBalken).h && Rect.y + Rect.h > (*iterrctBalken).y) {
			bret = true;
			while (Rect.y + Rect.h > (*iterrctBalken).y)
				Rect.y -= 1;//Ball zuruecksetzen
			Rect.y += 2;//ball ein wenig hereinsetzen
		}
	}
	return bret;
}

bool CBalken :: bvectorCheck(int X) {//fuer die pits
	bool bCheck = false;
	for(itervRandoms=vRandoms.begin();itervRandoms!=vRandoms.end();itervRandoms++){
		if((*itervRandoms)==X)
			bCheck = true;
	}
	return bCheck;
}	

void CBalken :: Rendern(){//rendern
	SDL_Surface *Screen = SDL_GetVideoSurface();
	for (iterrctBalken = lrctBalken.begin(); iterrctBalken != lrctBalken.end(); iterrctBalken++)//alle balken durchlaufen und rendern
		SDL_BlitSurface (surBalken, NULL, Screen, &(*iterrctBalken));
} 



class CBall : public TSingleton<CBall>
{
	friend class CRender;
	
public:
	CBall();
	~CBall();
	void Init();
	void Quit();
	
	void SetbImFallen (bool JA){bImFallen = JA;}//bool setten
	bool GetbImFallen (){return bImFallen;}// bool bekommen
	
	bool Update(double Ellapsed, float fSchwierigkeitsgrad, bool bLinks);//updaten
	void Bewegung(bool blinks, double Ellapsed);//horizontale bewegung
	void Rendern() {SDL_Surface *Screen=SDL_GetVideoSurface();SDL_BlitSurface (surBall2, &rctBall2, Screen, &rctBall);}//rendern
	
private:
	SDL_Rect rctBall;//rect fuer ball
	SDL_Rect rctBall2;//zeiter rect
	SDL_Surface *surBall;//surface fuer ball
	SDL_Surface *surBall2;
	SDL_RWops *rwopBall;//fuer den Ball (image)
	SDL_RWops *rwopBall2;//fuer den Ball (image)
	bool bImFallen;//faellt der ball?
	bool bGameOver;//ist der ball gameover?
	float m_fXPos;//fuer bewegung
	float m_fXPos2;//fuer das passende hereinfallen bei den pits
	float m_fYPos;//fuer bewegung
	//~ double m_dAngle;
	
};

CBall::CBall(){
	rwopBall = SDL_RWFromFile ("media/Ball.png", "rb");
	rwopBall2 = SDL_RWFromFile ("media/Ball.png", "rb");
	surBall = NULL;
	surBall = IMG_LoadPNG_RW(rwopBall);
	surBall2 = NULL;
	surBall2 = IMG_LoadPNG_RW(rwopBall2);
}

CBall :: ~CBall(){
	SAFE_FREEN(surBall)
	SAFE_FREEN(surBall2)
	
	Quit();
}

void CBall :: Init (){
	bGameOver = false;
	FAST_RECT (rctBall,50,200,BALLW,BALLH);
	FAST_RECT (rctBall2,0,0,BALLW,BALLH);
	
	m_fXPos = 0.0f;
	m_fXPos2 = 0.0f;
	m_fYPos = 0.0f;
	//~ m_dAngle = 0.0f;
}

void CBall :: Quit (){
}

bool CBall :: Update(double Ellapsed, float fSchwierigkeitsgrad, bool bLinks){	
	m_fXPos=rctBall.x%(BALKENW);
	m_fXPos2 = (rctBall.x + rctBall.w)%BALKENW;
	if(bLinks&&!bImFallen) {rctBall.x -= m_fXPos2; rctBall.x += rctBall.w; rctBall.x += (BALKENW-BALLW);}//an den rechten rand pressen
	else if(!bLinks&&!bImFallen){rctBall.x -= m_fXPos;}//an den linken rand pressen
	
	if(CBalken::Get()->bKollisionscheck(rctBall) == false)//faellt der ball
		bImFallen = true;
	else {
		if(bLinks&&!bImFallen){rctBall.x -= (BALKENW - BALLW); rctBall.x -= rctBall.w; rctBall.x += m_fXPos2;}//zurueckstellen
		else if (!bImFallen){rctBall.x += m_fXPos;}//zurueckstellen
		bImFallen = false;
	}
		
	if(bImFallen == false){//nach oben
		m_fYPos = (SPEEDUP * Ellapsed) * fSchwierigkeitsgrad;
		rctBall.y -= static_cast<int> (m_fYPos);//anpassen
	}
	else{//fallen
		m_fYPos = (BALLSPEEDDOWN * Ellapsed);
		rctBall.y += static_cast<int> (m_fYPos);//anpassen
	}
	
	if(rctBall.y < 1)//tot?
		bGameOver = true;
	if(rctBall.y > ALLH-BALLH-BALKENH-5)//unterer rand
		rctBall.y = ALLH-BALLH-BALKENH-5;
	
	return bGameOver;
}

void CBall :: Bewegung(bool blinks, double Ellapsed){//bewegen
	//~ int w, h;
	//~ SDL_Rect tmpRect = surBall->clip_rect;
	if (bImFallen == false){//wenn er nicht faellt
		if (blinks == true && rctBall.x > 0){//links
			//~ m_dAngle += BALLANGLE * Ellapsed;//winkel aendern
			//~ if(m_dAngle > 360.0f) {m_dAngle = 0.0f;}//bereichgrenze
			m_fXPos = BALLSPEEDW * Ellapsed;
			rctBall.x -= static_cast<int> (m_fXPos);//anpassen
			//~ rotozoomSurfaceSize (tmpRect.w, tmpRect.h, m_dAngle, BALLZOOM, &w, &h);//entstehende groesse berechnen
			//~ SAFE_FREEN(surBall2)//alte surface befreien
			//~ surBall2 = (rotozoomSurface (surBall, m_dAngle, BALLZOOM, BALLSMOOTH));//ball nach links drehen
			//~ rctBall2.x = (w - BALLW) / 2;//neue groesse anpassen
			//~ rctBall2.y = (h - BALLH) / 2;//neue groesse anpassen
		}
		else if (blinks == false && rctBall.x < ALLW-BALLW){//rechts
			//~ m_dAngle -= BALLANGLE * Ellapsed;//winkel aendern
			//~ if(m_dAngle < 0.0f) {m_dAngle = 360.0f;}//bereichgrenze
			m_fXPos = BALLSPEEDW * Ellapsed;
			rctBall.x += static_cast<int> (m_fXPos);//anpassen
			//~ rotozoomSurfaceSize (tmpRect.w, tmpRect.h, (m_dAngle), BALLZOOM, &w, &h);//entstehende groesse berechnen
			//~ SAFE_FREEN(surBall2)//alte surface befreien
			//~ surBall2 = rotozoomSurface (surBall, m_dAngle, BALLZOOM, BALLSMOOTH);//ball nach rechts drehen
			//~ rctBall2.x = (w - BALLW) / 2;//neue groesse anpassen
			//~ rctBall2.y = (h - BALLH) / 2;//neue groesse anpassen
		}
		
		if(rctBall.x < 0){rctBall.x = 0;}//linker rand
		if(rctBall.x > ALLW-BALLW){rctBall.x = ALLW-BALLW;}//rechter rand
	}
}


class CRender : public TSingleton<CRender> , public CTimer
{
public:
	CRender();
	~CRender();
	void Init();
	void Quit();
	void allesRendern(float fAktZeit, float fHighscore, float fSchwierigkeitsgrad, int Stufe);//alles rendern
	void Update(float fAktZeit, float fHighscore, float fSchwierigkeitsgrad, int Stufe);//zeiten und alles updaten
	void Fillrects(){SDL_FillRect (Fenster, NULL, SDL_MapRGB (Fenster->format, 0, 0, 0));}//Fenster weiﬂ streichen
	void Spielende (bool Escape,float fAktZeit, float fHighscore, float fSchwierigkeitsgrad, int Stufe);//grafikzauber am ende
	
private:
	void SwitchingRect (SDL_Surface **Surface, int Richtung, int XPos, int YPos);//text von der seite
	
	TTF_Font *pFont24;//schriftgrˆﬂe
	SDL_Color Color;//Farbe weiﬂ
	
	//die rects fuer die ecken
	SDL_Rect rctAktZeit;
	SDL_Rect rctHighscore;
	SDL_Rect rctSchwierigkeitsgrad;
	SDL_Rect rctStufe;
	
	SDL_Surface *Fenster;//das gesamtfenster
	SDL_Surface *surIcon;//das Icon
	SDL_RWops *rwopIcon;//rwop
	SDL_Surface *surBackground;//hintergrund surface
	SDL_RWops *rwopBackground;//hintergrund
	SDL_Surface *surAktZeit;//aktuelle zeit surface
	SDL_Surface *surHighscore;//highscore surface
	SDL_Surface *surSchwierigkeitsgrad;//speedsurface
	SDL_Surface *surStufe;//stufen surface
	
	//die strings
	char cAktZeit[50];
	char cHighscore[20];
	char cSchwierigkeitsgrad[20];
	char cStufe[20];
};

CRender::CRender(){
	rwopIcon = SDL_RWFromFile ("media/Ball.png", "rb");
	surIcon = NULL;
	surIcon = IMG_LoadPNG_RW(rwopIcon);
	SDL_WM_SetIcon(surIcon, NULL);//Icon
	
	Fenster = NULL;
	Fenster = SDL_SetVideoMode (ALLW, ALLH, 24, SDL_HWSURFACE | SDL_DOUBLEBUF); // Das gesamte Fenster
	rwopBackground = SDL_RWFromFile ("media/Background.png", "rb");
	surBackground = NULL;
	surBackground = IMG_LoadPNG_RW(rwopBackground);
	
	SDL_WM_SetCaption("Falling_Ball | 2008 by S.Scholz", "Falling_Ball");//titel
	
	pFont24 = NULL;
	pFont24 = TTF_OpenFont ("media/FreeSans.ttf", 24);//schriftart
	//farbe
	Color.r = 255;
	Color.g = 255;
	Color.b = 255;
}
CRender::~CRender(){
	//alles freen
	SAFE_FREEN(surAktZeit)
	SAFE_FREEN(surHighscore)
	SAFE_FREEN(surSchwierigkeitsgrad)
	SAFE_FREEN(surStufe)
	SAFE_FREEN(surIcon)
	SAFE_FREEN(Fenster)
}

void CRender :: Init(){
	//die rects erneuern
	FAST_RECT(rctAktZeit,0,0,TEXTW,TEXTH)
	FAST_RECT(rctHighscore,ALLW-TEXTW,0,TEXTW,TEXTH)
	FAST_RECT(rctSchwierigkeitsgrad,ALLW-TEXTW,ALLH-TEXTH,TEXTW,TEXTH)
	FAST_RECT(rctStufe,0,ALLH-TEXTH,TEXTW,TEXTH)
	TimerInit();
}
void CRender :: Quit() {
}

void CRender :: allesRendern(float fAktZeit, float fHighscore, float fSchwierigkeitsgrad, int Stufe) {//alles rendern
	Fillrects();
	SDL_BlitSurface (surBackground, NULL, Fenster, NULL);
	
	CBall::Get()->Rendern();//ball
	CBalken::Get()->Rendern();//balken
	
	Update(fAktZeit, fHighscore, fSchwierigkeitsgrad, Stufe);
	
	SDL_BlitSurface (surAktZeit, NULL, Fenster, &rctAktZeit);
	SDL_BlitSurface (surHighscore, NULL, Fenster, &rctHighscore);
	SDL_BlitSurface (surSchwierigkeitsgrad, NULL, Fenster, &rctSchwierigkeitsgrad);
	SDL_BlitSurface (surStufe, NULL, Fenster, &rctStufe);
	
	SDL_Flip(Fenster);
}

void CRender :: Update(float fAktZeit, float fHighscore, float fSchwierigkeitsgrad, int Stufe){//zeiten updaten
	sprintf(cAktZeit, "Now: %.2fs", fAktZeit);
	sprintf(cHighscore, "Best: %.2fs", fHighscore);
	sprintf(cSchwierigkeitsgrad, "Speed: %.1f", fSchwierigkeitsgrad);
	sprintf(cStufe, "Stufe: %i", Stufe);
	
	surAktZeit = TTF_RenderUTF8_Blended(pFont24, cAktZeit, Color);
	surHighscore = TTF_RenderUTF8_Blended(pFont24,cHighscore, Color);
	surSchwierigkeitsgrad = TTF_RenderUTF8_Blended(pFont24, cSchwierigkeitsgrad, Color);
	surStufe = TTF_RenderUTF8_Blended(pFont24,cStufe, Color);
}

void CRender :: Spielende(bool Escape,float fAktZeit, float fHighscore, float fSchwierigkeitsgrad, int Stufe){//grafikspektakel am ende
	Fillrects();//weiss
	//Escape gedrueckt oder gameOver?
	if(Escape == true){sprintf(cAktZeit, "Spiel beendet");}
	else{sprintf(cAktZeit, "GameOver");}
	surAktZeit = TTF_RenderUTF8_Blended(pFont24, cAktZeit, Color);
	SwitchingRect(&surAktZeit, 2, (ALLW-TEXTW)/2, 0);
	
	Update(fAktZeit, fHighscore, fSchwierigkeitsgrad, Stufe);//updaten
	
	//die ecken nach und nach erscheinen lassen
	SwitchingRect(&surAktZeit, 3, 0, 0);
	SwitchingRect(&surHighscore, 3, ALLW-TEXTW, 0);
	SwitchingRect(&surSchwierigkeitsgrad, 4, ALLW-TEXTW, ALLH-TEXTH);
	SwitchingRect(&surStufe, 3, 0, ALLH-TEXTH);
	
	//neuer highscore?
	if(fAktZeit > fHighscore){sprintf(cAktZeit, "Neuer Highscore!");}
	else{sprintf(cAktZeit, "Leider kein neuer Highscore!");}
	surAktZeit = TTF_RenderUTF8_Blended(pFont24, cAktZeit, Color);
	SwitchingRect(&surAktZeit, 3, (ALLW-TEXTW)/2, (ALLH-TEXTH)/2);
	sprintf(cAktZeit, "Neues Spiel (j/n)");
	surAktZeit = TTF_RenderUTF8_Blended(pFont24, cAktZeit, Color);
	SwitchingRect(&surAktZeit, 2, (ALLW-TEXTW)/2, (ALLH-TEXTH)/4*3);
	SDL_Flip(Fenster);
}

void CRender :: SwitchingRect(SDL_Surface **Surface, int Richtung, int XPos, int YPos){//texte von der seite reinschweben lassen
	SDL_Rect Rect;
	float Var = 0.0f;
	if(Richtung == 1){//von oben
		Var = 0.0f;
		Rect.x = XPos;
		Rect.y = 0;
		while (Rect.y < YPos){
			TimerUpdate();
			SDL_FillRect (Fenster, &Rect, SDL_MapRGB (Fenster->format, 0, 0, 0));//Fenster weiﬂ streichen
			Var += static_cast<float> (TEXTSPEED*AllTime.Ellapsed);
			Rect.y = static_cast<int> (Var);
			if(Rect.y >= YPos){Rect.y = YPos;}
			SDL_BlitSurface (*Surface, NULL, Fenster, &Rect);
			SDL_Flip(Fenster);
		}
	}
	else if(Richtung == 2){//von rechts
		Var = ALLW;
		Rect.x = ALLW;
		Rect.y = YPos;
		while (Rect.x > XPos){
			TimerUpdate();
			SDL_FillRect (Fenster, &Rect, SDL_MapRGB (Fenster->format, 0, 0, 0));//Fenster weiﬂ streichen
			Var -= static_cast<float> (TEXTSPEED*AllTime.Ellapsed);
			Rect.x = static_cast<int> (Var);
			if(Rect.x <= XPos){Rect.x = XPos;}
			SDL_BlitSurface (*Surface, NULL, Fenster, &Rect);
			SDL_Flip(Fenster);
		}
	}
	else if(Richtung == 3){//von unten
		Var = ALLH;
		Rect.x = XPos;
		Rect.y = ALLH;
		while (Rect.y > YPos){
			TimerUpdate();
			SDL_FillRect (Fenster, &Rect, SDL_MapRGB (Fenster->format, 0, 0, 0));//Fenster weiﬂ streichen
			Var -= static_cast<float> (TEXTSPEED*AllTime.Ellapsed);
			Rect.y = static_cast<int> (Var);
			if(Rect.y <= YPos){Rect.y = YPos;}
			SDL_BlitSurface (*Surface, NULL, Fenster, &Rect);
			SDL_Flip(Fenster);
		}
	}
	else if(Richtung == 4){//von links
		Var = 0.0f;
		Rect.x = 0;
		Rect.y = YPos;
		while (Rect.x < XPos){
			TimerUpdate();
			SDL_FillRect (Fenster, &Rect, SDL_MapRGB (Fenster->format, 0, 0, 0));//Fenster weiﬂ streichen
			Var += static_cast<float> (TEXTSPEED*AllTime.Ellapsed);
			Rect.x = static_cast<int> (Var);
			if(Rect.x >= XPos){Rect.x = XPos;}
			SDL_BlitSurface (*Surface, NULL, Fenster, &Rect);
			SDL_Flip(Fenster);
		}
	}
}


class CGame : public TSingleton<CGame> , public CTimer
{
	public:
		CGame();
		~CGame();
		void Init();
		void Quit();
		
		void Run();//hauptfunktion
		void Eingabe(int Typ = 1);//eingaben abfangen
	
	private:
		
		SDL_Event event;//event
		
		bool bLinks;//links gedrueckt?
		bool bRechts;//rechts gedrueckt?
		bool bPause;//enter gedrueckt?
		bool bEscape;//escape gedrueckt?
		bool bGameOver;//gestorben?
		bool bNeuesSpiel;//neus spiel gewollt?
		bool bWeiter;//taste gedrueckt?
		
		//fuer stopp
		int Zeit1;
		int Zeit2;
		int Zeit3;
		
		float fAktZeit;//aktuelle zeit
		float fHighscore;//highscore
		float fSchwierigkeitsgrad;//auch speed
};

CGame :: CGame(){
	Init();
}

CGame :: ~CGame(){	
}

void CGame :: Init(){
	fHighscore = 0.0f;
	
	ifstream Input ("Save/Highscore.hsc", ios::binary);
	Input.read((char*) &fHighscore, sizeof(fHighscore));//highscore laden
	Input.close();
	
	bLinks = false;
	bRechts = false;
	bPause = false;
	bEscape = false;
	bGameOver = false;
	bNeuesSpiel = true;
	bWeiter = false;
	
	fAktZeit = 0.0f;
	fSchwierigkeitsgrad = 1.0f;
	
	Zeit1 = 0;
	Zeit2 = 0;
	Zeit3 = 0;
	
	TimerInit();
}

void CGame :: Quit(){
}

void CGame :: Run(){//haupsfunktion
	while(bNeuesSpiel == true){//bis man kein neues spiel will
		//alles initialisieren
		Init();
		CRender::Get()->Init();
		CBall::Get()->Init();
		CBalken::Get()->Init();
		
		while (bEscape == false && bGameOver == false){//hauptschleife
			Eingabe();//eingabe checken
			
			if(bPause == false){
				SetStopTime (Zeit3);
				TimerUpdate();//timer aktualisieren
				fAktZeit = GetCurTime();
				if(bLinks == true)//bewegung links
					CBall::Get()->Bewegung (true, AllTime.Ellapsed);
				if(bRechts == true)//bewegung rechts
					CBall::Get()->Bewegung (false, AllTime.Ellapsed);
				//Updates
				if(CBall::Get()->Update(AllTime.Ellapsed, fSchwierigkeitsgrad, bLinks) == true)
					bGameOver = true;//ball tot?
				CBalken::Get()->Update(AllTime.Ellapsed, fSchwierigkeitsgrad);
						
				if (BlitTime.Ellapsed > TIMERENDER){//resourcen schonend
					BlitTime.LastTime = CurTime;
					CRender::Get()->allesRendern(fAktZeit, fHighscore, fSchwierigkeitsgrad, CBalken::Get()->GetStufe());//alles rendern
				}
				if (SpeedTime.Ellapsed > TIMESPEED){//Speed(Schwierigkeitsgrad) erhoehen
					SpeedTime.LastTime = CurTime;
					fSchwierigkeitsgrad += 0.1;//um 10%
				}
				if (PitTime.Ellapsed > TIMEPIT){PitTime.LastTime = CurTime;CBalken::Get()->minPits();} //Pits verringern
			}
		}
		CRender::Get()->Spielende(bEscape,fAktZeit,fHighscore,fSchwierigkeitsgrad,CBalken::Get()->GetStufe());//ende grafik abfolge
		
		if (fHighscore < fAktZeit){//neuer highscore zum abspeichern
			ofstream Output ("Save/Highscore.hsc", ios::binary);
			Output.write((char*) &fAktZeit, sizeof(fAktZeit));
			Output.close();
		}
		
		while (bWeiter == false){//neues spiel?
			Eingabe(2);
		}
		//sauber beenden
		CBalken::Get()->Quit();
		CBall::Get()->Quit();
		CRender::Get()->Quit();
	}
}
	
void CGame :: Eingabe(int Typ){
	while(SDL_PollEvent(&event)){//EingabeAbfrage
		switch(event.type){
			case SDL_KEYDOWN:{//Tast unten?
				if (Typ == 1) {
					switch(event.key.keysym.sym){
						case SDLK_ESCAPE:{bEscape = true;}break;
						case SDLK_LEFT:{bLinks = true;}break;
						case SDLK_RIGHT:{bRechts = true;} break;
						case SDLK_RETURN:{if(bPause==true){bPause=false;Zeit2=SDL_GetTicks();Zeit3+=Zeit2-Zeit1;}else{bPause=true;Zeit1=SDL_GetTicks();}} break;
						default:{}
					}
				}
				else if (Typ == 2) {
					switch(event.key.keysym.sym){
						case SDLK_j:{bNeuesSpiel = true;bWeiter = true;}break;
						case SDLK_n:{bNeuesSpiel = false;bWeiter = true;}break;
						case SDLK_ESCAPE:{bNeuesSpiel = false;bWeiter = true;}break;
						default:{}
					}
				}
			}break;
			case SDL_KEYUP:{//taste wieder oben?
				if (Typ == 1) {
					switch(event.key.keysym.sym){
						case SDLK_LEFT:{bLinks = false;}break;
						case SDLK_RIGHT:{bRechts = false;} break;
						default:{}
					}
				}
			}break;
		}
	}
}

// main
int main(int argc, char **argv)
{
	SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER);//SDL Initialisierung
	TTF_Init ();//Text
	
	CGame::Get();
	CRender::Get();
	CBall::Get();
	CBalken::Get();
	
	CGame::Get()->Run();
	
	CBalken::Del();
	CBall::Del();
	CRender::Del();
	CGame::Del();
	
	TTF_Quit();
	SDL_Quit();
	
	return 0;
}