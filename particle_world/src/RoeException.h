#ifndef ROEEXCEPTION_H_
#define ROEEXCEPTION_H_

#include <string>
#include <cstdio>
/*
throw Exception("", "", ".cpp", ROE_LINE);
throw Roe::Exception("", "", ".cpp", ROE_LINE);
*/

#define ROE_EXCEPTION(x,y,z) (Roe::Exception(x,y,z,__LINE__))
#define ROE_LINE __LINE__

namespace Roe {

	class Exception
	{
	public:
		Exception(const std::string& psError, const std::string& sFunction = "-", const std::string sFile = "-", int line = 0)
			: m_sErrorMessage(psError), m_sFunction(sFunction), m_sFile(sFile), m_iLine(line)
		{
		}
		virtual ~Exception() {}
		virtual std::string toString() const {
			std::string tmp("Exception thrown in function ");
			if (m_sFunction != "-") tmp += m_sFunction + "() ";
			if (m_sFile     != "-") tmp += "of " + m_sFile;
			char ch[500];
			if (m_iLine      >   0) {
				tmp += " in line ";
				sprintf(ch,"%s%i: \"%s\"", tmp.c_str(), m_iLine, m_sErrorMessage.c_str());
			}
			else {
				sprintf(ch,"%s: \"%s\""  , tmp.c_str(), m_sErrorMessage.c_str());
			}
			return std::string(ch);
		}
		
	private:
		std::string m_sErrorMessage;
		std::string m_sFunction;
		std::string m_sFile;
		int         m_iLine;
	};
	
} // namespace Roe

#endif /* ROEEXCEPTION_H_ */
