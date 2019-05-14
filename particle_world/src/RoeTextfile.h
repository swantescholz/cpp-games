#ifndef ROETEXTFILE_H_
#define ROETEXTFILE_H_

#include <list>
#include <string>

namespace Roe {
	
	
	//class Textfile
	//for loading, reading, and saving textfiles
	//access just like arrays, lines start at index = 0 and end at numLines-1
	class Textfile
	{
		
	public:
		Textfile();
		Textfile(Textfile& rhs);
		Textfile(const std::string& psFilename);
		~Textfile();
		
		void operator=  (Textfile& rhs);
		void operator+= (Textfile& rhs);
		void operator=  (std::string s) {clear(); pushBack(s);}
		void operator+= (std::string s) {         pushBack(s);}
		void operator<< (std::string s) {         pushBack(s);}
		
		void        setFilename (const std::string& psFilename) {m_sName = psFilename;}
		std::string getFilename () const {return m_sName;}
		
		void        readFile    (bool append = false);  // read a complete textfile(m_sName) into m_lsLines
		void        saveFile    (bool append = false);  // saves the complete text in a textfile(m_sName)
		void        readFileAt  (const std::string& path, bool append = false); // read a other complete textfile into m_lsLines
		void        saveFileAt  (const std::string& path, bool append = false); // saves the complete text in an other textfile
		void        setText     (const std::string& str);                       //sets all of the text
		std::string getText     (const std::string& delimiter = "\n");          // return the complete text in one string
		void        getTextAsArray (std::string *buffer);                   // return the complete text in an array of strings
		std::string getLine     (int index = -1);                               // -1 -> auto
		void        setLine     (const std::string& psLine, int index = -1);    // -1 -> auto
		void        setLine     (int index, const std::string& psLine) {setLine(psLine, index);}
		int         getNumLines () const {return m_iNumLines;}     //should always be equivalent to length()
		
		//simple functions, just like std::list methods
		int         length      ();
		void        clear       ();
		void        erase       (int pos);                              //deletes entry on pos
		void        insert      (int pos, const std::string& psLine);   //new pos will be on pos, old entry on pos will be pushed forward
		void        pushFront   (const std::string& psLine);
		void        pushBack    (const std::string& psLine);
		void        popFront    ();
		void        popBack     ();
		
		static const int        s_iMaxLines;            //static maximum number of lines per file
		static const int        s_iMaxCharsPerLine;     //static maximum of chars per line
	private:
		
		std::string            m_sName;        // the name(path) of the file
		std::list<std::string> m_lsLines;      // all text lines
		int                    m_iNumLines;    // number of lines
	};
	
	
} // namespace Roe

#endif /*ROETEXTFILE_H_*/
