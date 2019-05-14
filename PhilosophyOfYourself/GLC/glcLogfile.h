#ifndef GLCLOGFILE_H_
#define GLCLOGFILE_H_


// Includes
//
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include "Singleton.h"
#include "glcException.h"

// Defines
#define MAX_BUFFER 1024             // Maximale Größe für den Buffer
#define L_FAIL false                // Funktion war erfolgreich
#define L_OK   true                 // Funktion ist fehlgeschlagen
#define g_pLogfile glcLogfile::Get () // Makro zur einfachen Verwendung
#define g_Logfile (*glcLogfile::Get())

// Farben für den Text
enum FONTCOLORS
{
	BLACK,
	RED,
	GREEN,
	BLUE,
	PURPLE
};

// Klassendeklaration
//
class glcLogfile : public TSingleton<glcLogfile>
{
	// Memberfunktionen
	public:

		glcLogfile       ();
		~glcLogfile      ();
		void CreateLogfile  (const char *LogName);
		void WriteTopic     (const char *Topic, int Size);
		void Textout        (const char *Text);
		void Textout        (int Color, const char *Text);
		void Textout        (int Color, bool List, const char *Text);
		void fTextout       (const char *Text, ...);
		void fTextout       (int Color, const char *Text, ...);
		void fTextout       (int Color, bool List, const char *Text, ...);
		void FunctionResult (const char *Name, bool Result);
		
		//self-made
		void operator << (string s) 	{Textout(m_LastCol, s.data());}
		void operator << (FONTCOLORS f)		{m_LastCol = f;}
		void operator << (const glcException &ex) 	{
			Textout(RED, "Exception aufgetreten: ");
			char c[200]; sprintf(c,"Fehlerstring:\t %s", ex.GetErrStr().data());
			Textout(RED, c);
			sprintf(c,"Delaytion:\t%ims", ex.GetDelay());Textout(RED, c);
		}
		
		// Membervariablen
	private:
		FONTCOLORS m_LastCol;	//letzte farbe	
		
		FILE *m_Logfile;

};

#endif /*GLCLOGFILE_H_*/
