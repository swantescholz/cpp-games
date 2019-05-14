#include "RoeBinFile.h"
#include "RoeException.h"

namespace Roe {
	
	BinFile::BinFile()
	{
	}
	
	BinFile::~BinFile()
	{
		if (m_fin.is_open ()) m_fin.close ();
		if (m_fout.is_open()) m_fout.close();
	}
	
	
	void BinFile::openIn (const std::string& pathIn ) {
		m_sNameIn  = pathIn;
		m_fin.open (m_sNameIn.c_str() , std::ios_base::in  | std::ios_base::binary);
		if (m_fin.fail())
			throw Exception("could not open binfile input '" + pathIn + "'", "openIn", "RoeBinFile.cpp", ROE_LINE);
	}
	void BinFile::openOut(const std::string& pathOut, bool append) {
		m_sNameOut = pathOut;
		if (append) m_fout.open(m_sNameOut.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::app);
		else        m_fout.open(m_sNameOut.c_str(), std::ios_base::out | std::ios_base::binary);
		if (m_fout.fail())
			throw Exception("could not open binfile output '" + pathOut + "'", "openOut", "RoeBinFile.cpp", ROE_LINE);
	}
	void BinFile::closeIn () {
		m_fin.close();
	}
	void BinFile::closeOut() {
		m_fout.close();
	}
	
	std::ifstream& BinFile::getIfstream() {return m_fin;}
	std::ofstream& BinFile::getOfstream() {return m_fout;}
	std::string    BinFile::getNameIn  () {return m_sNameIn ;}
	std::string    BinFile::getNameOut () {return m_sNameOut;}
	
	bool BinFile::isOpenIn   () {return m_fin.is_open ();}
	bool BinFile::isOpenOut  () {return m_fout.is_open();}
	bool BinFile::eofIn      () {return m_fin.eof     ();}
	bool BinFile::eofOut     () {return m_fout.eof    ();}
	bool BinFile::goodIn     () {return m_fin.good    ();}
	bool BinFile::goodOut    () {return m_fout.good   ();}
	unsigned long int BinFile::sizeIn  () {
		unsigned long int pos1, pos2 = m_fin.tellg();
		m_fin.seekg(0, std::ios_base::end);
		pos1 = m_fin.tellg();
		m_fin.seekg(pos2, std::ios_base::beg);
		return pos1;
	}
	unsigned long int BinFile::sizeOut  () {
		unsigned long int pos1, pos2 = m_fout.tellp();
		m_fout.seekp(0, std::ios_base::end);
		pos1 = m_fout.tellp();
		m_fout.seekp(pos2, std::ios_base::beg);
		return pos1;
	}
	void BinFile::seekIn  (std::streamoff off, std::ios_base::seekdir dir)  {m_fin.seekg (off, dir);}
	void BinFile::seekOut (std::streamoff off, std::ios_base::seekdir dir)  {m_fout.seekp(off, dir);}
	void BinFile::movePointerIn  (int offBytes, int startPos) {
		if(startPos <= -1) {seekIn(offBytes, std::ios_base::beg); return;}
		if(startPos >=  1) {seekIn(offBytes, std::ios_base::end); return;}
		seekIn(offBytes, std::ios_base::cur);
	}
	void BinFile::movePointerOut (int offBytes, int startPos) {
		if(startPos <= -1) {seekOut(offBytes, std::ios_base::beg); return;}
		if(startPos >=  1) {seekOut(offBytes, std::ios_base::end); return;}
		seekOut(offBytes, std::ios_base::cur);
	}
	long int BinFile::getPointerPosIn () {return (long int)m_fin.tellg ();}
	long int BinFile::getPointerPosOut() {return (long int)m_fout.tellp();}
	
} // namespace Roe
