#include "RoeLogfile.h"
#include "RoeUtil.h"

namespace Roe {

	Logfile::Logfile ()
		: m_bCreated(false)
		, m_colLast(0,0,0,1)
	{
	}
	
	Logfile::~Logfile ()
	{
		if (m_bCreated) {
			addLine();
			write("CORRECT QUITTING", Color::BLUE);
			m_tf.pushBack("<br><br>End of logfile</font></body></html>");
			m_tf.saveFile();
		}
	}
	
	void Logfile::createLogfile (const std::string& sLogName) {
		m_tf.setFilename(sLogName);
		m_tf.pushBack("<html><head><title>Logfile "+sLogName+"</title></head>");
		m_tf.pushBack("<body><font face='courier new'>");
		
		m_bCreated = true;
		
		writeTopic ("Logfile", 1);
		
		#ifdef _DEBUG
		m_tf.pushBack("<p>BUILD: DEBUG</p>");
		#else
		m_tf.pushBack("<p>BUILD: RELEASE</p>");
		#endif
		
		addLine();
	}
	
	void Logfile::writeTopic (const std::string& sTopic, int size) {
		if (m_bCreated == false) return;
		m_tf.pushBack("<h"+Util::toString(size)+">"+sTopic+"</h"+Util::toString(size)+">");
		m_tf.saveFile();
	}
	
	void Logfile::write (const std::string& s, Color col) {
		if (m_bCreated == false) return;
		std::string sNum[3];
		sNum[0] = Util::toStringHexNumber(static_cast<int>(col.r * 255.0));
		sNum[1] = Util::toStringHexNumber(static_cast<int>(col.g * 255.0));
		sNum[2] = Util::toStringHexNumber(static_cast<int>(col.b * 255.0));
		for (int i = 0; i < 3; ++i) {
			if (sNum[i].length() == 1) {
				sNum[i] = "0" + sNum[i];
			}
		}
		m_tf.pushBack("<p style=\"color:#"+sNum[0]+sNum[1]+sNum[2]+"\">"+s+"</p>");
		m_tf.saveFile();
	}
	
	void Logfile::addLine() {m_tf.pushBack("<hr></hr>");}
	
} // namespace Roe

