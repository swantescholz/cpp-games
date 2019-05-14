#include "RoeTextfile.h"
#include "RoeException.h"
#include "RoeCommon.h"
#include <fstream>
#include <stack>
#include <iostream>

namespace Roe {
	
	const int Textfile::s_iMaxLines         = 1000;     // a maximum of 1000 lines (unused)
	const int Textfile::s_iMaxCharsPerLine  = 500;      // 500 characters per line should be enough
	
	Textfile::Textfile()
	{
		m_sName     = "";
		m_iNumLines = 0;
	}
	Textfile::Textfile(Textfile& rhs)
	{
		m_sName         = rhs.m_sName;
		m_iNumLines     = rhs.m_iNumLines;
		for (std::list<std::string>::iterator it1 = rhs.m_lsLines.begin(); it1 != rhs.m_lsLines.end(); it1++) {
			m_lsLines.push_back( (*it1) );
		}
	}
	Textfile::Textfile(const std::string& psFileName)
	{
		m_sName     = psFileName;
		m_iNumLines = 0;
	}
	Textfile::~Textfile()
	{
		clear();
	}
	
	void Textfile::operator = (Textfile& rhs) {
		m_sName        = rhs.m_sName;
		m_iNumLines    = rhs.m_iNumLines;
		m_lsLines.clear();
		for (std::list<std::string>::iterator it1 = rhs.m_lsLines.begin(); it1 != rhs.m_lsLines.end(); it1++) {
			m_lsLines.push_back( (*it1) );
		}
	}
	void Textfile::operator+= (Textfile& rhs) {
		m_iNumLines += rhs.m_iNumLines;
		for (std::list<std::string>::iterator it1 = rhs.m_lsLines.begin(); it1 != rhs.m_lsLines.end(); it1++) {
			m_lsLines.push_back( (*it1) );
		}
	}
	
	void Textfile::readFile (bool append) {
		if (append == false) { // truncate
			clear();
		}
		std::ifstream fin;
		fin.open(m_sName.c_str(), std::ios_base::in);
		if (!fin) {
			throw Exception("file '" + m_sName + "' does not exist", "readFile", "RoeTextfile.cpp", ROE_LINE);
		}
		fin.seekg(0, std::ios_base::end);
		const unsigned long numBytes = fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		std::string sLine = "";
		char chBuf;
		for (unsigned long i = 0; i < numBytes; i++) {
			fin.read(&chBuf, 1);
			if (chBuf == '\n') {
				m_lsLines.push_back(sLine);
				sLine.clear();
			} else {
				sLine += chBuf;
			}
		}
		if (chBuf != '\n')
			m_lsLines.push_back(sLine); //last line also (if there was a newline)
		m_iNumLines = static_cast<int>(m_lsLines.size());
		fin.close();
	}
	
	void Textfile::saveFile (bool append) {
		std::ofstream fout;
		if (append == false) {      // truncate 
			fout.open(m_sName.c_str(), std::ios_base::out | std::ios_base::trunc );
		} else {                    // append
			fout.open(m_sName.c_str(), std::ios_base::out | std::ios_base::app   );
		}
		if (!fout) {
			throw Exception("file '" + m_sName + "' can not be opened/created", "saveFile", "RoeTextfile.cpp", ROE_LINE);
		}
		std::list<std::string>::iterator it1 = m_lsLines.begin();
		if (it1 == m_lsLines.end()) return;
		fout << (*it1); //first line (if any)
		for (it1++; it1 != m_lsLines.end(); it1++) {
			fout << "\n" << (*it1);
		}
		
		fout.close();
	}
	
	void Textfile::readFileAt (const std::string& path, bool append) {
		std::string oldName = m_sName;
		m_sName = path;
		readFile(append);
		m_sName = oldName;
	}
	void Textfile::saveFileAt (const std::string& path, bool append) {
		std::string oldName = m_sName;
		m_sName = path;
		saveFile(append);
		m_sName = oldName;
	}
	
	void Textfile::setText(const std::string& str) {
		clear();
		pushBack(str);
	}
	std::string Textfile::getText (const std::string& delimiter) {
		std::string sReturn = "";
		std::list<std::string>::iterator it1 = m_lsLines.begin();
		if(it1 == m_lsLines.end()) return ""; //empty
		sReturn += (*it1);
		int iLine = 1;
		for (it1++; it1 != m_lsLines.end(); it1++) {
			sReturn += delimiter + (*it1);
			iLine++;
		}
		return sReturn;
	}
	void Textfile::getTextAsArray (std::string *buffer) {
		std::list<std::string>::iterator it1 = m_lsLines.begin();
		if(it1 == m_lsLines.end()) return; //empty
		buffer[0] = (*it1);
		int iLineIndex = 1;
		for (it1++; it1 != m_lsLines.end(); it1++) {
			buffer[iLineIndex++] = (*it1);
		}
	}
	std::string Textfile::getLine (int index) {
		static int s_iLine = 0;
		if (index >= 0) s_iLine = index;
		if (s_iLine >= m_iNumLines) {
			throw Exception("index out of bounds", "getLine", "RoeTextfile.cpp", ROE_LINE);
		}
		std::string sReturnLine;
		index = 0;
		for (std::list<std::string>::iterator it1 = m_lsLines.begin(); it1 != m_lsLines.end(); it1++) {
			if (index == s_iLine) {
				sReturnLine = (*it1);
				break;
			}
			index++;
		}
		
		s_iLine++;
		return sReturnLine;
	}
	void Textfile::setLine (const std::string& psNewLine, int index) {
		static int s_iLine = 0;
		if (index >= 0) s_iLine = index;
		if (s_iLine >= m_iNumLines) {
			throw Exception("index out of bounds", "setLine", "RoeTextfile.cpp", ROE_LINE);
		}
		index = 0;
		for (std::list<std::string>::iterator it1 = m_lsLines.begin(); it1 != m_lsLines.end(); it1++) {
			if (index == s_iLine) {
				(*it1) = psNewLine;
				break;
			}
			index++;
		}
		
		s_iLine++;
	}
	
	int  Textfile::length() {
		return static_cast<int>(m_lsLines.size());
	}
	void Textfile::clear() {
		m_lsLines.clear();
		m_iNumLines = 0;
	}
	void Textfile::erase (int pos) {
		int i = 0;
		std::list<std::string>::iterator it1;
		for (it1 = m_lsLines.begin(); it1 != m_lsLines.end(); it1++) {
			if (i == pos) break;
			i++;
		}
		m_lsLines.erase (it1);
		m_iNumLines--;
	}
	void Textfile::insert(int pos, const std::string& psLine) {
		int i = 0;
		std::list<std::string>::iterator it1;
		std::string s = psLine;
		std::string sTmp;
		for (it1 = m_lsLines.begin(); it1 != m_lsLines.end(); it1++) {
			if (i == pos) break;
			i++;
		}
		do {
			sTmp = s;
			s.erase(0, s.find_first_of("\n"));
			it1 = m_lsLines.insert(it1, sTmp.substr(0,(sTmp.length() - s.length()) ));
			m_iNumLines++;
			it1++;
			s.erase(0,1);
		} while (s.length() > 0);
		if (sTmp.length() > 0) {if (sTmp[sTmp.length()-1] == '\n') {m_lsLines.insert(it1, std::string("")); m_iNumLines++;}}
	}
	void Textfile::pushFront(const std::string& psLine) {
		std::string s = psLine;
		std::string sTmp;
		std::stack<std::string> st;
		do {
			sTmp = s;
			s.erase(0, s.find_first_of("\n"));
			st.push(sTmp.substr(0,(sTmp.length() - s.length()) ));
			m_iNumLines++;
			s.erase(0,1);
		} while (s.length() > 0);
		while (st.empty() == false) {
			m_lsLines.push_front(st.top());
			st.pop();
		}
	}
	void Textfile::pushBack (const std::string& psLine) {
		std::string s = psLine;
		std::string sTmp;
		do {
			sTmp = s;
			s.erase(0, s.find_first_of("\n"));
			m_lsLines.push_back(sTmp.substr(0,(sTmp.length() - s.length()) ));
			m_iNumLines++;
			s.erase(0,1);
		} while (s.length() > 0);
	}
	void Textfile::popFront() {
		m_lsLines.pop_front();
		m_iNumLines--;
	}
	void Textfile::popBack() {
		m_lsLines.pop_back();
		m_iNumLines--;
	}
	
	
	
} // namespace Roe
