#ifndef ROELOGFILE_H_
#define ROELOGFILE_H_

#include <string>
#include "RoeSingleton.h"
#include "RoeException.h"
#include "RoeColor.h"
#include "RoeTextfile.h"

#define roe_log Roe::Logfile::getSingleton()

namespace Roe {
	
	class Logfile : public Singleton<Logfile>
	{
	public:
		
		Logfile  ();
		~Logfile ();
		void createLogfile  (const std::string& sLogName);
		void writeTopic     (const std::string& sTopic, int size); //the bigger size -> smaller text
		
		void setColor       (const Color& col)     {m_colLast = col;}
		void write          (const std::string& s) {write(s, m_colLast);}
		void write          (const std::string& s, Color col);
		void addLine        ();
		Logfile& operator<< (const std::string& s) {write(s); return *this;}
		Logfile& operator<< (const Color& col)     {m_colLast = col; return *this;}
		Logfile& operator<< (const Exception& e)   {write(e.toString(), Color::RED); return *this;}
		
	private:
		
		Textfile m_tf;
		bool     m_bCreated;
		Color    m_colLast;
	};
	
} // namespace Roe

#endif /*ROELOGFILE_H_*/
