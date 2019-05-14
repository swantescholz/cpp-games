#include "RoeUtil.h"

#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <GL/gl.h>
#include "RoeBinFile.h"
#include "RoeException.h"
#include "RoeMatrix.h"

namespace Roe {
	
	std::string operator+= (      std::string &s, bool   b) {if (b) return s += "true"; s += "false"; return s;}
	std::string operator+  (const std::string &s, bool   b) {if (b) return s +  "true";   return (s + "false");}
	//std::string operator+= (      std::string &s, char   c) {char a[2]; a[0] = c; a[1] = '\0'; return std::string(a);}
	//std::string operator+  (const std::string &s, char   c) {char a[2]; a[0] = c; a[1] = '\0'; return std::string(a);}
	std::string operator+= (      std::string &s, int    i) {char c[40]; sprintf(c, "%i", i); s+=c; return s;}
	std::string operator+  (const std::string &s, int    i) {char c[40]; sprintf(c, "%i", i); return (s + c);}
	std::string operator+= (      std::string &s, float  f) {char c[40]; sprintf(c, "%f", f); s+=c; return s;} //<- here possibly specify precision
	std::string operator+  (const std::string &s, float  f) {char c[40]; sprintf(c, "%f", f); return (s + c);} //<- here possibly specify precision
	std::string operator+= (      std::string &s, double d) {char c[40]; sprintf(c, "%g", d); s+=c; return s;} //<- here possibly specify precision
	std::string operator+  (const std::string &s, double d) {char c[40]; sprintf(c, "%g", d); return (s + c);} //<- here possibly specify precision
	
	std::string operator+  (bool   b, const std::string &s) {if (b) return "true" +  s; return "false" +  s;}
	std::string operator+  (char   c, const std::string &s) {char a[2]; a[0] = c; a[1] = '\0'; return std::string(a);}
	std::string operator+  (int    i, const std::string &s) {char c[40]; sprintf(c, "%i", i); return c +  s;}
	std::string operator+  (float  f, const std::string &s) {char c[40]; sprintf(c, "%f", f); return c +  s;} //<- here possibly specify precision
	std::string operator+  (double d, const std::string &s) {char c[40]; sprintf(c, "%g", d); return c +  s;} //<- here possibly specify precision
	
	namespace Util {
		
		void delay(double timeInSec) {
			/*if (clock() != -1) { // clock is available, therefore floats
				clock_t start_time;
				start_time = clock();
				while((clock() - start_time) < timeInSec * CLOCKS_PER_SEC) {}
			} else {             //only full seconds are available
				time_t start_time, cur_time;
				time(&start_time);
				int iSec = static_cast<int>(timeInSec);
				do {
					time(&cur_time);
				} while((cur_time - start_time) < iSec);
			}*/
			auto t1 = std::chrono::monotonic_clock::now();
			while (std::chrono::duration<double>(std::chrono::monotonic_clock::now()-t1).count() < timeInSec) {}
		}
		
		bool isTimePassed(int gap, double frameTime) {
			static double     dTime = 0.0;
			static int        iTime = 0;
			dTime += frameTime;
			if (iTime  + gap < dTime) {
				iTime += gap;
				return true;
			}
			return false;
		}
		double timeDiff() {
			static std::chrono::monotonic_clock::time_point last = std::chrono::monotonic_clock::now();
			double d = std::chrono::duration<double>(std::chrono::monotonic_clock::now() - last).count();
			last = std::chrono::monotonic_clock::now();
			return d;
		}
		int getNextNumber(int pNum) {
			static int sCounter = 0;
			if (pNum >= 0) sCounter = pNum;
			sCounter++;
			return sCounter-1;
		}
		
		bool  isNaN(float  f) {return f != f;}
		bool  isNaN(double d) {return d != d;}
		bool  isPowerOfTwo      (unsigned int i) {return i > 0 && (i & (i-1) ) == 0;}
		int   nextPowerOfTwo    (unsigned int i) {unsigned int j; for(j=1; j<=i; j*=2); return j  ;}
		int   previousPowerOfTwo(unsigned int i) {if (i<=1) return 1; unsigned int j; for(j=1; j< i; j*=2); return j/2;}
		float  floor(float  f) {return (int)f;}
		double floor(double d) {return (int)d;}
		float  ceil (float  f) {return (int)f+1;}
		double ceil (double d) {return (int)d+1;}
		bool  isNearlyZero(float  f) {return f > -ROE_REAL_TOLERANCE && f < ROE_REAL_TOLERANCE;}
		bool  isNearlyZero(double d) {return d > -ROE_REAL_TOLERANCE && d < ROE_REAL_TOLERANCE;}
		int    round         (double d)               {
			if (d - static_cast<int>(d) < static_cast<int>(d)+1 - d)
				return static_cast<int>(d);
			return static_cast<int>(d)+1;
		}
		
		std::string replaceAnyWith(std::string str, const std::string& any, const std::string& with) {
			size_t pos = 0;
			size_t len1 = any.length(), len2 = with.length();
			for (;;) {
				pos = str.find(any, pos);
				if (pos == std::string::npos) return str;
				str.replace(pos, len1, with);
				pos += len2;
			};
		}
		std::string deleteLetters (std::string ps, std::string delims, bool atFront, bool inMiddle,  bool atEnd) {
			std::string::size_type notDelimPos;
			if (atFront == true) { // trim leading whitespace
				notDelimPos = ps.find_first_not_of(delims.c_str());
				ps.erase(0, notDelimPos);
			}
			if (atEnd == true) {   // trim trailing whitespace
				notDelimPos = ps.find_last_not_of (delims.c_str());
				ps.erase(notDelimPos+1);
			}
			if (inMiddle == true) {
				std::string tmp = ps;
				ps = "";
				for (unsigned int i = 0; i < tmp.length(); i++) {
					if (delims.find( tmp.at(i)) == std::string::npos) {
						ps += tmp.at(i);
					}
				}
			}
			return ps;
		}
		std::string deleteOtherLettersThan (std::string ps, std::string delims, bool atFront, bool inMiddle,  bool atEnd) {
			std::string::size_type delimPos;
			if (atFront == true) { // trim leading whitespace
				delimPos = ps.find_first_of(delims.c_str());
				ps.erase(0, delimPos);
			}
			if (atEnd == true) {   // trim trailing whitespace
				delimPos = ps.find_last_of (delims.c_str());
				ps.erase(delimPos+1);
			}
			if (inMiddle == true) {
				std::string tmp = ps;
				ps = "";
				for (unsigned int i = 0; i < tmp.length(); i++) {
					if (delims.find( tmp.at(i)) != std::string::npos) {
						ps += tmp.at(i);
					}
				}
			}
			return ps;
		}
		std::string reduceWhitespaces (std::string ps) {
			if (ps.length() <= 1) return ps; 
			std::string tmp;
			for (unsigned int i = 0; i < ps.length(); i++) {
				if (i != 0) {
					if (ps[i] == ' ' && ps[i-1] == ' ') {
						continue;
					}
				}
				tmp += ps[i];
			}
			return tmp;
		}
		std::string getFirstTokenOfStringAndErase(std::string& ps, std::string delim, bool delDelim) {
			std::string s = ps;
			ps = Util::deleteOtherLettersThan(ps, delim, true, false, false);
			s = s.substr(0, (s.length() - ps.length()) );
			if (delDelim) ps.erase(0,1);
			return s;
		}
		std::string getFirstNumberOfStringAndErase(std::string &ps) {
			ps = deleteOtherLettersThan(ps,"1234567890.-",true,false,false);
			std::string tmp = ps.substr(0,ps.find_first_not_of("1234567890.-"));
			ps = deleteLetters(ps,"1234567890.-",true,false,false);
			if (tmp.length() == 0) {throw Exception("no number found", "getFirstNumberOfStringAndErase", "Util.cpp", ROE_LINE);}
			return tmp;
		}
		std::string getFirstStringOfStringAndErase(std::string &ps, char delim) {
			std::string tmp = "";
			size_t pos = ps.find_first_of(delim);
			if (pos == std::string::npos) {throw Exception("no string found", "getFirstStringOfStringAndErase", "Util.cpp", ROE_LINE);}
			ps.erase(0,pos+1); //also deleting first '"'
			pos = ps.find(delim,0);
			if (pos == std::string::npos) {throw Exception("no string found", "getFirstStringOfStringAndErase", "Util.cpp", ROE_LINE);}
			tmp = ps.substr(0,pos);
			ps.erase(0,pos+1);
			return tmp;
		}
		std::string removeFoldersOfString(std::string  ps) {
			size_t pos = ps.rfind("/");
			if (pos == std::string::npos) return ps;
			ps.erase(0,pos+1);
			return ps;
		}
		std::string removeFileExtensionOfString(std::string  ps) {
			size_t pos = ps.rfind(".");
			if (pos == std::string::npos) return ps;
			ps.erase(pos);
			return ps;
		}
		bool getFirstBoolOfStringAndErase(std::string& ps) {
			size_t pos1 = ps.find("true");
			size_t pos2 = ps.find("false");
			if (pos1 == std::string::npos && pos2 == std::string::npos) {      //neither true nor false exist
				throw Exception("no true or false found ('"+ps+"')", "getFirstBooleanOfStringAndErase", "RoeUtil.cpp", ROE_LINE);
			}
			else if (pos1 != std::string::npos && pos2 != std::string::npos) { //both true and false exitst
				if (pos1 <= pos2) {
					ps.erase(0,pos1+4);
					return true;
				}
				else {
					ps.erase(0,pos2+5);
					return false;
				}
			}
			else if (pos1 != std::string::npos) {  //only true exists 
				ps.erase(0,pos1+4);
				return true;
			}
			else {                                 //only false exists
				ps.erase(0,pos1+5);
				return false;
			}
		}
		int getNumberOfNumbersInString (std::string ps) {
			int iNum = 0;
			ps = deleteOtherLettersThan(ps,"1234567890",true,false,false);
			while (ps.length() > 0) {
				ps = deleteLetters         (ps,"1234567890.",true,false,false);
				ps = deleteOtherLettersThan(ps,"1234567890" ,true,false,false);
				iNum++;
			}
			return iNum;
		}
		int getNumberOfStringsInString (std::string ps, char delim) {
			int         iNum = -1;
			std::string tmp  = "";
			size_t      pos  = -1;
			do {
				pos = ps.find_first_of(delim, pos+1);
				iNum++;
			} while (pos != std::string::npos);
			return static_cast<int>(iNum / 2);
		}
		int getNumberOfBoolsInString (std::string ps) {
			int iNum   = -2;
			size_t pos = -1;
			
			do {
				pos = ps.find("true", pos+1);
				iNum++;
			} while (pos != std::string::npos);
			pos = -1;
			do {
				pos = ps.find("false", pos+1);
				iNum++;
			} while (pos != std::string::npos);
			
			return iNum;
		}
		
		bool stringBeginsWith(const std::string& s1, const std::string& s2) {
			if(s2.length() > s1.length()) return false;
			if(s2 == s1) return true;
			for(unsigned int i = 0; i< s2.length(); i++) {
				if (s1[i] != s2[i]) return false;
			}
			return true;
		}
		bool stringEndsWith(const std::string& s1, const std::string& s2) {
			if(s2.length() > s1.length()) return false;
			if(s2 == s1) return true;
			for(unsigned int i = 0; i< s2.length(); i++) {
				if (s1[s1.length()-1-i] != s2[s2.length()-1-i]) return false;
			}
			return true;
		}
		bool isStringWhitespace(const std::string& str) {
			const unsigned long length = str.length();
			for (unsigned long i = 0; i < length; ++i) {
				if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\v' && str[i] != '\f' && str[i] != '\r')
					return false;
			}
			return true;
		}
		std::string deleteComments(std::string str, const std::string& com_line,
		                           const std::string& com_block1,
		                           const std::string& com_block2, bool deleteWhiteLines) {
			size_t pos1 = 0, pos2, tmp1, tmp2;
			
			for(;;) {
				pos1 = str.find(com_line, pos1);
				if (pos1 == std::string::npos) break;
				pos2 = str.find("\n", pos1);
				if (pos2 == std::string::npos)
					str.erase(pos1);
				else
					str.erase(pos1, pos2-pos1);
				if (deleteWhiteLines) {
					//whitespaces?
					tmp1 = str.rfind("\n", pos1-1);
					if (tmp1 == std::string::npos) tmp1 = 0;
					tmp2 = str.find ("\n", pos1);
					if (tmp2 == std::string::npos) tmp2 = str.length()-1;
					if (Util::isStringWhitespace(str.substr(tmp1, tmp2-tmp1))) {
						str.erase(tmp1, tmp2-tmp1);
					}
				}
			}
			pos1 = 0;
			for(;;) {
				pos1 = str.find(com_block1, pos1);
				if (pos1 == std::string::npos) break;
				pos2 = str.find(com_block2, pos1);
				if (pos2 == std::string::npos) { //end reached
					str.erase(pos1);
					break;
				}
				else
					str.erase(pos1, pos2-pos1+com_block2.length());
			}
			return str;
		}
		
		//parsing functions
		bool   parseBool   (std::string ps)   {return ps.find("true") < ps.find("false");}
		int    parseInt    (std::string ps)   {return strtol(ps.c_str(), NULL, 10);}
		int    parseUInt   (std::string ps)   {return strtoul(ps.c_str(), NULL, 10);}
		float  parseFloat  (std::string ps)   {return static_cast<float>(strtod(ps.c_str(), NULL) );}
		double parseDouble (std::string ps)   {return strtod(ps.c_str(), NULL);}
		
		//zero function
		std::string addZeros(std::string  ps, int newSize) {
			for (int i = ps.length(); i < newSize; i++) {
				ps = "0" + ps;
			}
			return ps;
		}
		
		//to string functions
		std::string toString  (bool   b) {if(b) return std::string("true"); return std::string("false");}
		std::string toString  (char   c) { return std::string(1,c);}
		std::string toString  (char*  c) { return std::string(c);}
		std::string toString  (int    i) { char c[40]; sprintf(c, "%i", i); return std::string(c);}
		std::string toString  (float  f) { char c[40]; sprintf(c, "%f", f); return std::string(c);}
		std::string toString  (double d) { char c[40]; sprintf(c, "%g", d); return std::string(c);}
		std::string toStringHexNumber (int i) {
			char cTmp[15];
			sprintf(cTmp, "%X", i);
			return std::string(cTmp);
		}
		void toStringArray(int argc, char **argv, std::string *buffer) {
			if(buffer == NULL || argc == 0 || argv == NULL) return;
			for (int i = 0; i < argc; i++) {
				buffer[i] = argv[i];
			}
		}
		std::string toString  (Roe::EKeyCode kc, bool pbShiftDown) {
			switch (kc) {
				case KC_SPACE: return " ";
				case KC_COMMA:  if (pbShiftDown) return ";"; return ",";
				case KC_PERIOD: if (pbShiftDown) return ":"; return ".";
				
				case KC_A: if(pbShiftDown) return "A"; return "a";
				case KC_B: if(pbShiftDown) return "B"; return "b";
				case KC_C: if(pbShiftDown) return "C"; return "c";
				case KC_D: if(pbShiftDown) return "D"; return "d";
				case KC_E: if(pbShiftDown) return "E"; return "e";
				case KC_F: if(pbShiftDown) return "F"; return "f";
				case KC_G: if(pbShiftDown) return "G"; return "g";
				case KC_H: if(pbShiftDown) return "H"; return "h";
				case KC_I: if(pbShiftDown) return "I"; return "i";
				case KC_J: if(pbShiftDown) return "J"; return "j";
				case KC_K: if(pbShiftDown) return "K"; return "k";
				case KC_L: if(pbShiftDown) return "L"; return "l";
				case KC_M: if(pbShiftDown) return "M"; return "m";
				case KC_N: if(pbShiftDown) return "N"; return "n";
				case KC_O: if(pbShiftDown) return "O"; return "o";
				case KC_P: if(pbShiftDown) return "P"; return "p";
				case KC_Q: if(pbShiftDown) return "Q"; return "q";
				case KC_R: if(pbShiftDown) return "R"; return "r";
				case KC_S: if(pbShiftDown) return "S"; return "s";
				case KC_T: if(pbShiftDown) return "T"; return "t";
				case KC_U: if(pbShiftDown) return "U"; return "u";
				case KC_V: if(pbShiftDown) return "V"; return "v";
				case KC_W: if(pbShiftDown) return "W"; return "w";
				case KC_X: if(pbShiftDown) return "X"; return "x";
				case KC_Y: if(pbShiftDown) return "Y"; return "y";
				case KC_Z: if(pbShiftDown) return "Z"; return "z";
				
				case KC_0: if(pbShiftDown) return "="; return "0";
				case KC_1: if(pbShiftDown) return "!"; return "1";
				case KC_2: if(pbShiftDown) return "\"";return "2";
				case KC_3: if(pbShiftDown) return "";  return "3";
				case KC_4: if(pbShiftDown) return "$"; return "4";
				case KC_5: if(pbShiftDown) return "%"; return "5";
				case KC_6: if(pbShiftDown) return "&"; return "6";
				case KC_7: if(pbShiftDown) return "/"; return "7";
				case KC_8: if(pbShiftDown) return "("; return "8";
				case KC_9: if(pbShiftDown) return ")"; return "9";
				
				case KC_KP_0: return "0";
				case KC_KP_1: return "1";
				case KC_KP_2: return "2";
				case KC_KP_3: return "3";
				case KC_KP_4: return "4";
				case KC_KP_5: return "5";
				case KC_KP_6: return "6";
				case KC_KP_7: return "7";
				case KC_KP_8: return "8";
				case KC_KP_9: return "9";
				
				case KC_KP_PLUS    : return "+";
				case KC_KP_MINUS   : return "-";
				case KC_KP_MULTIPLY: return "*";
				case KC_KP_DIVIDE  : return "/";
				
				default: return "";
			}
		}
		
		bool isPartOfRoeKeyCodes (int kc) {
			switch (kc) {
				case KC_ESCAPE    :
				case KC_TAB       :
				case KC_RETURN    :
				case KC_BACKSPACE :
				case KC_SPACE     :
				case KC_RSHIFT    :
				case KC_LSHIFT    :
				case KC_RCTRL     :
				case KC_LCTRL     :
				case KC_RALT      :
				case KC_LALT      :
				case KC_COMMA     :
				case KC_PERIOD    :
				
				case KC_A:case KC_B:case KC_C:case KC_D:case KC_E:case KC_F:
				case KC_G:case KC_H:case KC_I:case KC_J:case KC_K:case KC_L:
				case KC_M:case KC_N:case KC_O:case KC_P:case KC_Q:case KC_R:
				case KC_S:case KC_T:case KC_U:case KC_V:case KC_W:case KC_X:
				case KC_Y:case KC_Z:
				
				case KC_0:case KC_1:case KC_2:case KC_3:case KC_4:
				case KC_5:case KC_6:case KC_7:case KC_8:case KC_9:
				
				case KC_F1: case KC_F2: case KC_F3: case KC_F4: case KC_F5:
				case KC_F6: case KC_F7: case KC_F8: case KC_F9: case KC_F10:
				case KC_F11:case KC_F12:case KC_F13:case KC_F14:case KC_F15:
				
				case KC_KP_0:case KC_KP_1:case KC_KP_2:case KC_KP_3:case KC_KP_4:
				case KC_KP_5:case KC_KP_6:case KC_KP_7:case KC_KP_8:case KC_KP_9:
				
				case KC_KP_PLUS    : case KC_KP_MINUS   :
				case KC_KP_MULTIPLY: case KC_KP_DIVIDE  :
				case KC_KP_ENTER   :
				
				case KC_ARR_INSERT: case KC_ARR_DELETE:
				case KC_ARR_HOME  : case KC_ARR_END   :
				case KC_ARR_PGUP  : case KC_ARR_PGDOWN:
				case KC_ARR_UP    : case KC_ARR_DOWN  :
				case KC_ARR_LEFT  : case KC_ARR_RIGHT :
				
				return true;
				
				default: return false;
			}
		}
		
		// *********************************************************
		// GL
		std::string getGLError() {
			GLenum errCode = glGetError();
			std::string sErr = "OpenGL_Error(ERROR_CODE: " + Util::toString((int)errCode) + "): '";
			
			switch(errCode) {
				case GL_NO_ERROR:          sErr += "no error";          break;
				case GL_INVALID_ENUM:      sErr += "invalid enum";      break;
				case GL_INVALID_VALUE:     sErr += "invalid value";     break;
				case GL_INVALID_OPERATION: sErr += "invalid operation"; break;
				case GL_STACK_OVERFLOW:    sErr += "stack overflow";    break;
				case GL_STACK_UNDERFLOW:   sErr += "stack underflow";   break;
				case GL_OUT_OF_MEMORY:     sErr += "out of memory";     break;
				//case GL_TABLE_TO_LARGE:    sErr += "table to large";    break;
				//case GL_INVALID_FRAMEBUFFER_OPERATION: sErr += "invalid  framebuffer operation"; break;
				default: sErr += "unknown error";break;
			}
			sErr += "'";
			return sErr;
		}
		
		// *********************************************************
		// MATH
		bool isPointOnRay(const Vector3& ray1, const Vector3& ray2, const Vector3& point) {
			const float a = ray1.distance(ray2);
			const float b = ray1.distance(point);
			const float c = ray2.distance(point);
			if (Util::isNearlyZero(b+c-a)) return true;
			if (Util::isNearlyZero(a+c-b)) return true;
			if (Util::isNearlyZero(a+b-c)) return true;
			return false;
		}
		bool isPointInSphere(const Vector3& p, const Vector3& center, const double r) {
			return Vector3::squaredLength(p-center) <= r*r;
		}
		double distancePointToRay(const Vector3& p, const Vector3& rayPos, const Vector3& rayDir, bool rayisline) {
			/*const Vector3 a(rayPos-p);
			const double dot = Vector3::dotProduct(a,rayDir);
			return std::sqrt(Vector3::dotProduct(a,a)-dot*dot/Vector3::dotProduct(rayDir,rayDir));*/
			const double t = - Vector3::dotProduct((rayPos-p), rayDir) / Vector3::dotProduct(rayDir, rayDir);
			if (rayisline) {
				if (t >= 1.0) return (rayPos + rayDir - p).length();
				if (t <= 0.0) return (rayPos - p).length();
			}
			return (rayPos + t*rayDir - p).length();
		}
		Vector3 nearestPointOnRay (const Vector3& p, const Vector3& rayPos, const Vector3& rayDir, bool rayisline) {
			const double t = - Vector3::dotProduct((rayPos-p), rayDir) / Vector3::dotProduct(rayDir, rayDir);
			if (rayisline) {
				if (t >= 1.0) return rayPos + rayDir;
				if (t <= 0.0) return rayPos;
			}
			return rayPos + t*rayDir;
		}
		bool rayHitsSphere(const Vector3& rayPos, const Vector3& rayDir, const Vector3& m, const double r, Vector3 *pvOut, bool rayisline) {
			const Vector3 a = rayPos - m;
			const double udot = Vector3::dotProduct(rayDir, a);
			const double term = udot*udot - Vector3::dotProduct(rayDir,rayDir) * (Vector3::dotProduct(a,a) - r*r);
			if (term < 0.0) return false;
			if (rayisline == false && pvOut == nullptr) return true;
			const double t = (-udot - std::sqrt(term)) / Vector3::dotProduct(rayDir,rayDir);
			if (rayisline) {
				if (t < 0.0 || t > 1.0) return false;
			}
			if (pvOut != nullptr) {
				*pvOut = rayPos+t*rayDir;
			}
			return true;
		}
		bool rayHitsPlane(const Vector3& rayPos, const Vector3& rayDir, const Vector3& planePos, const Vector3& up, Vector3 *pvOut, bool rayisline) {
			const double dot1 = Vector3::dotProduct(rayDir, up);
			const double dot2 = Vector3::dotProduct((planePos-rayPos), up);
			if (std::abs(dot1) < 0.0001) {
				if (std::abs(dot2) < 0.0001) {
					if (pvOut != nullptr)
						*pvOut = rayPos;
					return true;
				}
				return false;
			}
			if (rayisline == false && pvOut == nullptr) return true;
			const double t = dot2 / dot1;
			if (rayisline) {
				if (t < 0.0 || t > 1.0) return false;
			}
			if (pvOut != nullptr) {
				*pvOut = rayPos+t*rayDir;
			}
			return true;
		}
		bool rayHitsPlane(const Vector3& rayPos, const Vector3& rayDir, const Plane& plane, Vector3 *pvOut, bool rayisline) {
			const double dot = Vector3::dotProduct(rayDir, plane.n);
			if (std::abs(dot) < 0.0001) {
				if (std::abs(Vector3::dotProduct(plane.n, rayPos) + plane.d) < 0.0001) {
					if (pvOut != nullptr)
						*pvOut = rayPos;
					return true;
				}
				return false;
			}
			if (rayisline == false && pvOut == nullptr) return true;
			const double t = -(Vector3::dotProduct(plane.n, rayPos) + plane.d) / dot;
			if (rayisline) {
				if (t < 0.0 || t > 1.0) return false;
			}
			if (pvOut != nullptr) {
				*pvOut = rayPos+t*rayDir;
			}
			return true;
		}
		bool calculateAimingVector(const Vector3& p, const Vector3& u, const Vector3& m, const double c, Vector3 *pvOut) {
			const Vector3 a = p - m;
			const double den   = Vector3::dotProduct(u,u)-c*c;
			const double pHalf = Vector3::dotProduct(a,u)/den;
			const double term  = pHalf*pHalf - Vector3::dotProduct(a,a)/den;
			if (term < 0.0) return false;
			if (pvOut != nullptr) {
				const double t = -pHalf - std::sqrt(term);
				*pvOut = p+t*u;
			}
			return true;
		}
		Vector3 calculateNormalOfTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3) {
			return Vector3::normalize(Vector3::crossProduct(v2-v1, v3-v1));
		}
		Vector3 calculateCenterOfCircle(const Vector3& v1, const Vector3& v2, const Vector3& v3) {
			const Vector3 vNormal = Vector3::normalize(Vector3::crossProduct(v2-v1, v3-v1));
			const Vector3 vDir1   = Vector3::normalize(Vector3::crossProduct(vNormal, v2-v1));
			const float a = (v2-v1).length();
			const float b = (v3-v2).length();
			const float c = (v1-v3).length();
			const float r = a*b*c/sqrt((a+b+c)*(-a+b+c)*(a-b+c)*(a+b-c));
			//if (Util::isNearlyZero(r*r-0.25f*a*a)) return 0.5f*(v1+v2);
			const float ft = sqrt(r*r-0.25f*a*a);
			return 0.5f*(v1+v2) + vDir1 * ft;
		}
		float calculateCurvatureOfSpline(const Vector3& v, const Vector3& a) {
			const float l = v.length();
			return Vector3::crossProduct(v,a).length()/(l*l*l);
		}
		Vector3 calculateCentripetalAccelerationOfSpline(const Vector3& v, const Vector3& a) {
			return Vector3::crossProduct(v,a).length()/v.length()*
			       Vector3::normalize(Vector3::makePerpendicularTo(a,v));
		}
		//matrix math-----------------------------------------------
		double solveMatrix3x4(double a, double b, double c, double d,
		                      double e, double f, double g, double h,
		                      double i, double j, double k, double l, double *px, double *py) {
			double inv_den = 1.0/(a*(g*j-f*k)+b*(e*k-g*i)+c*(f*i-e*j));
			if (px != nullptr)
				(*px) = (b*(h*k-g*l)+c*(f*l-h*j)+d*(g*j-f*k)) * inv_den;
			if (py != nullptr)
				(*py) = (a*(h*k-g*l)+c*(e*l-h*i)+d*(g*i-e*k)) * inv_den;
			return (a*(h*j-f*l)+b*(e*l-h*i)+d*(f*i-e*j)) * inv_den;;
		}
		
		// *********************************************************
		// FILES
		void saveImageBMP(std::string sPath, const byte* pixel_data, long  w, long  h) {
			byte *tmp = new byte[w*h*3];
			for (int i = 1; i < 3*w*h; i+=3) {
				tmp[i] = pixel_data[i];
				tmp[i-1] = pixel_data[i+1];
				tmp[i+1] = pixel_data[i-1]; //this swapping is new because BMP nees BGR instead of RGB
			}
			
			if (Util::stringEndsWith(sPath,".bmp") == false) sPath += ".bmp";
			BinFile bf;
			bf.openOut(sPath);
			
			bf << (short)19778;     //"BM"
			bf << (long)(54+3*w*h); //size of BMP file in bytes
			bf << (long)0;          //reserved
			bf << (long)54;         //offset to pixel_data in bytes
			
			bf << (long)40;       //size of this information block in bytes
			bf << (long)w;        //width
			bf << (long)h;        //height
			bf << (short)1;       //always1
			bf << (short)24;      //bits per pixel
			bf << (long)0;        //0 = RGB
			bf << (long)(3*w*h);  //size of pixel_data in bytes
			bf << (long)0;        //mostly 0
			bf << (long)0;        //mostly 0
			bf << (long)(256*256*256); //number of colors
			bf << (long)(256*256*256); //number of colors
			
			bf.writeSimpleTypeArray(tmp, 3*w*h);
			bf.closeOut();
		}
		void saveImageTGA(std::string sPath, const byte* pixel_data, short w, short h, bool hasAlpha) {
			const int bpp = hasAlpha ? 4 : 3;
			byte *tmp = new byte[w*h*bpp];
			for (int i = 1; i < bpp*w*h; i+=bpp) {
				tmp[i] = pixel_data[i];
				if (hasAlpha) {
					tmp[i+2] = pixel_data[i+2];
				}
				tmp[i-1] = pixel_data[i+1];
				tmp[i+1] = pixel_data[i-1]; //this swapping is new because TGA nees BGR instead of RGB
			}
			
			if (Util::stringEndsWith(sPath,".tga") == false) sPath += ".tga";
			BinFile bf;
			bf.openOut(sPath);
			bf << (char )0;         //length of image ID (0 = no ID)
			bf << (char )0;         //1 = color palette existing
			bf << (char )2;         //2 = RGB uncompressed
			bf << (short)0;         //always 0
			bf << (short)0;         //number of colors in palette
			bf << (char )0;         //number of bits for one palette entry
			bf << (short)0;         //always 0
			bf << (short)0;         //always 0
			bf << (short)w;         //width
			bf << (short)h;         //height
			bf << (char )(bpp*8);   //bits per pixel (accordingly to alpha enabled or not)
			bf << (char )0;         //attribute bits
			bf.writeSimpleTypeArray(tmp, bpp*w*h);
			bf.closeOut();
		}
		
		bool fileExists(const std::string& sPath) {
			std::ifstream fin;
			fin.open(sPath.c_str(), std::ios_base::in);
			if (!fin) return false;
			fin.close();
			return true;
		}
		byte4 fileSize(const std::string& sPath) {
			std::ifstream fin;
			fin.open(sPath.c_str(), std::ios_base::in);
			if (!fin) return -1;
			fin.seekg(0, std::ios_base::end);
			byte4 pos = fin.tellg();
			fin.close();
			return pos;
		}
		bool removeFile(const std::string& sPath) {return remove(sPath.c_str()) == 0;}
		bool renameFile(const std::string& sPathOld, const std::string& sPathNew) {return rename(sPathOld.c_str(), sPathNew.c_str()) == 0;}
		
		// *********************************************************
		int getLength (std::string    &p) {return p.length();}
		
	} // namespace Util
	
} // namespace Roe


