#include "RoeTokenizer.h"
#include "RoeUtil.h"
#include "RoeException.h"

namespace Roe {

	Tokenizer::Tokenizer(std::string text, char stringDelim) {	
		m_sText = text; 
		m_cStringDelim = stringDelim;
	}
	
	Tokenizer::~Tokenizer() {}
	
	bool Tokenizer::readBool(bool skipline) {
		if (Util::getNumberOfBoolsInString(m_sText) == 0)
			throw Exception("no bool found", "readBool", "RoeTokenizer.cpp", ROE_LINE);
		bool b = Util::getFirstBoolOfStringAndErase(m_sText);
		if (skipline) skipLine(1);
		return b;
	}
	int Tokenizer::readInt(bool skipline) {
		if (Util::getNumberOfNumbersInString(m_sText) == 0)
			throw Exception("no ints found", "readInt", "RoeTokenizer.cpp", ROE_LINE);
		int i = Util::parseInt(Util::getFirstNumberOfStringAndErase(m_sText));
		if (skipline) skipLine(1);
		return i;
	}
	float Tokenizer::readFloat(bool skipline) {
		if (Util::getNumberOfNumbersInString(m_sText) == 0)
			throw Exception("no doubles found", "readDouble", "RoeTokenizer.cpp", ROE_LINE);
		float f = Util::parseFloat(Util::getFirstNumberOfStringAndErase(m_sText));
		if (skipline) skipLine(1);
		return f;
	}
	double Tokenizer::readDouble(bool skipline) {
		if (Util::getNumberOfNumbersInString(m_sText) == 0)
			throw Exception("no doubles found", "readDouble", "RoeTokenizer.cpp", ROE_LINE);
		double d = Util::parseDouble(Util::getFirstNumberOfStringAndErase(m_sText));
		if (skipline) skipLine(1);
		return d;
	}
	std::string Tokenizer::readString(bool skipline) {
		if (Util::getNumberOfStringsInString(m_sText, m_cStringDelim) == 0)
			throw Exception("no strings found", "readString", "RoeTokenizer.cpp", ROE_LINE);
		std::string s = Util::getFirstStringOfStringAndErase(m_sText, m_cStringDelim);
		if (skipline) skipLine(1);
		return s;
	}
	
	void Tokenizer::readBool(bool& b, bool skipline) {
		if (Util::getNumberOfBoolsInString(m_sText) != 0)
			b = Util::getFirstBoolOfStringAndErase(m_sText);
		if (skipline) skipLine(1);
	}
	void Tokenizer::readInt(int& i, bool skipline) {
		if (Util::getNumberOfNumbersInString(m_sText) != 0)
			i = Util::parseInt(Util::getFirstNumberOfStringAndErase(m_sText));
		if (skipline) skipLine(1);
	}
	void Tokenizer::readFloat(float& f, bool skipline) {
		if (Util::getNumberOfNumbersInString(m_sText) != 0)
			f = Util::parseFloat(Util::getFirstNumberOfStringAndErase(m_sText));
		if (skipline) skipLine(1);
	}
	void Tokenizer::readDouble(double& d, bool skipline) {
		if (Util::getNumberOfNumbersInString(m_sText) != 0)
			d = Util::parseDouble(Util::getFirstNumberOfStringAndErase(m_sText));
		if (skipline) skipLine(1);
	}
	void Tokenizer::readString(std::string& s, bool skipline) {
		if (Util::getNumberOfStringsInString(m_sText, m_cStringDelim) != 0)
			s = Util::getFirstStringOfStringAndErase(m_sText, m_cStringDelim);
		if (skipline) skipLine(1);
	}
	
	void Tokenizer::skipLine(int num) {
		int r = 0;
		for (int i = 0; i < num; i++) {
			r = m_sText.find("\n");
			if (r < 0) return;
			m_sText.erase(0, r+1 );
		}
	}
}
