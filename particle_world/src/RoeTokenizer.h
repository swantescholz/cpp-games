#ifndef ROETOKENIZER_H_
#define ROETOKENIZER_H_

#include <string>

namespace Roe {
	
	//a class to read numbers and strings from long string
	class Tokenizer {
			
	public:
		Tokenizer(std::string text = "", char stringDelim = '\"');
		~Tokenizer();
		
		void        setText   (const std::string& s) {m_sText = s;}
		std::string getText   () const {return m_sText;}
		bool        readBool  (bool skipLine = false);
		int         readInt   (bool skipLine = false);
		float       readFloat (bool skipLine = false);
		double      readDouble(bool skipLine = false);
		std::string readString(bool skipLine = false);
		
		void readBool  (bool       & b, bool skipLine = false);
		void readInt   (int        & i, bool skipLine = false);
		void readFloat (float      & f, bool skipLine = false);
		void readDouble(double     & d, bool skipLine = false);
		void readString(std::string& s, bool skipLine = false);
		
		void skipLine(int num = 1);
		
	private:
		
		std::string m_sText;
		char m_cStringDelim;
	};

} //namespace Roe

#endif /* ROETOKENIZER_H_ */
