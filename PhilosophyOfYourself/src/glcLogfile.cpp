#include "glcLogfile.h"


// Konstruktor
//
// Aufgabe: Bisher noch keine
//
glcLogfile::glcLogfile ()
{

} // Konstruktor

// Destruktor
//
// Aufgabe: Gibt Ende-Meldung aus und schließ das Logfile
//
glcLogfile::~glcLogfile ()
{
	// Logfile-Ende schreiben und Datei schließen
	Textout ("<br><br>End of logfile</font></body></html>");
	fclose (m_Logfile);

} // Destruktor

// CreateLogfile
//
// Aufgabe: Logfile erstellen und Kopf schreiben
//
void glcLogfile::CreateLogfile (const char *LogName)
{
	// Logfile leeren und Kopf schreiben
	m_Logfile = fopen (LogName, "w");
	Textout ("<html><head><title>Logfile</title></head>");
	Textout ("<body><font face='courier new'>");
	WriteTopic ("Logfile", 3);

	// Aktuelle Build-Konfiguration ausgeben
	#ifdef _DEBUG
	Textout ("BUILD: DEBUG<br>");
	#else
	Textout ("BUILD: RELEASE<br>");
	#endif

	// Link für E-Mail-Adresse schreiben
	//Textout ("<a href='mailto:support@meineURL.de?subject=Logfile'>");
	//Textout ("Send E-Mail to me</a><br><br>");

	// Logfile schließen und mit append wieder öffnen
	fclose (m_Logfile);
	m_Logfile = fopen (LogName, "a");
	
	m_LastCol = BLACK;
} // CreateLogfile

// WriteTopic
//
// Aufgabe: Überschrift erzeugen
//
void glcLogfile::WriteTopic (const char *Topic, int Size)
{
	// Überschrift schreiben und flushen
	Textout ("<table cellspacing='0' cellpadding='0' width='100%%' ");
	Textout ("bgcolor='#DFDFE5'>\n<tr>\n<td>\n<font face='arial' ");
	fTextout ("size='+%i'>\n", Size);
	Textout (Topic);
	Textout ("</font>\n</td>\n</tr>\n</table>\n<br>");
	fflush (m_Logfile);

} // WriteTopic

// Textout
//
// Aufgabe: Text ins Logfile schreiben (schwarz)
//
void glcLogfile::Textout (const char *Text)
{
	// Text schreiben und flushen
	fprintf (m_Logfile, Text);
	fflush (m_Logfile);

} // Textout (schwarz)

// Textout
//
// Aufgabe: Text ins Logfile schreiben (farbig)
//
void glcLogfile::Textout (int Color, const char *Text)
{
	Textout (Color, false, Text);

} // Textout (farbig)

// Textout
//
// Aufgabe: Text ins Logfile schreiben (farbig, Liste)
//
void glcLogfile::Textout (int Color, bool List, const char *Text)
{
	// Listen-Tag schreiben
	if (List == true)
	Textout ("<li>");

	// Farbtag schreiben
	switch (Color)
	{
		case BLACK:
			Textout ("<font color=black>");  break;
		case RED:
			Textout ("<font color=red>");    break;
		case GREEN:
			Textout ("<font color=green>");  break;
		case BLUE:
			Textout ("<font color=blue>");   break;
		case PURPLE:
			Textout ("<font color=purple>"); break;
	};

	// Text schreiben
	Textout (Text);
	Textout ("</font>");

	if (List == false)
		Textout ("<br>");
	else
		Textout ("</li>");

} // Textout (farbig, liste)

// fTextout
//
// Aufgabe: formatierten Text ins Logfile schreiben (schwarz)
//
void glcLogfile::fTextout (const char *Text, ...)
{
	char buffer[MAX_BUFFER];  // char-Buffer
	va_list pArgList;          // Liste der übergebenen Argumente

	// String aus den Argumenten erstellen
	va_start (pArgList, Text);
	vsprintf (buffer, Text, pArgList);
	va_end (pArgList);

	// Erzeugten String schreiben
	Textout (buffer);

} // fTextout (schwarz)

// fTextout
//
// Aufgabe: formatierten Text ins Logfile schreiben (farbig)
//
void glcLogfile::fTextout (int Color, const char *Text, ...)
{
	char buffer[MAX_BUFFER];  // char-Buffer
	va_list pArgList;          // Liste der übergebenen Argumente

	// String aus den Argumenten erstellen
	va_start (pArgList, Text);
	vsprintf (buffer, Text, pArgList);
	va_end (pArgList);

	// Erzeugten String schreiben
	Textout (Color, buffer);

} // fTextout (farbig)

// fTextout
//
// Aufgabe: formatierten Text ins Logfile schreiben (farbig, Liste)
//
void glcLogfile::fTextout (int Color, bool List,const char *Text, ...)
{
	char buffer[MAX_BUFFER];  // char-Buffer
	va_list pArgList;          // Liste der übergebenen Argumente

	// String aus den Argumenten erstellen
	va_start (pArgList, Text);
	vsprintf (buffer, Text, pArgList);
	va_end (pArgList);

	// Erzeugten String schreiben
	Textout (Color, List, buffer);

} // fTextout (farbig, Liste)

// FunctionResult
//
// Aufgabe: OK oder ERROR für Funktionsaufruf ausgeben
//
void glcLogfile::FunctionResult (const char *Name, bool Result)
{
	if (L_OK == Result)
	{
		Textout("<table width='100%%' cellSpacing='1' cellPadding='5'");
		Textout(" border='0' bgcolor='#C0C0C0'><tr><td bgcolor=");
		fTextout("'#FFFFFF' width='35%%'>%s</TD>", Name);
		Textout("<td bgcolor='#FFFFFF' width='30%%'><font color =");
		Textout("'green'>OK</FONT></TD><td bgcolor='#FFFFFF' ");
		Textout("width='35%%'>-/-</TD></tr></table>");
	}
	else
	{
		Textout("<table width='100%%' cellSpacing='1' cellPadding='5'");
		Textout(" border='0' bgcolor='#C0C0C0'><tr><td bgcolor=");
		fTextout("'#FFFFFF' width='35%%'>%s</TD>", Name);
		Textout("<td bgcolor='#FFFFFF' width='30%%'><font color =");
		Textout("'red'>ERROR</FONT></TD><td bgcolor='#FFFFFF' ");
		Textout("width='35%%'>-/-</TD></tr></table>");
	}

} // FunctionResult

