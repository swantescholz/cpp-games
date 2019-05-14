#ifndef ROEUTIL_H_
#define ROEUTIL_H_



#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "RoeCommon.h"
#include "RoeVector2.h"
#include "RoeVector3.h"
#include "RoePlane.h"

namespace Roe {
	
	//useful string operands
	std::string operator+= (      std::string &s, bool   b);
	std::string operator+  (const std::string &s, bool   b);
	//std::string operator+= (      std::string &s, char   c);
	//std::string operator+  (const std::string &s, char   c);
	std::string operator+= (      std::string &s, int    i);
	std::string operator+  (const std::string &s, int    i);
	std::string operator+= (      std::string &s, float  f);
	std::string operator+  (const std::string &s, float  f);
	std::string operator+= (      std::string &s, double d);
	std::string operator+  (const std::string &s, double d);
	
	std::string operator+  (bool   b, const std::string &s);
	std::string operator+  (char   c, const std::string &s);
	std::string operator+  (int    i, const std::string &s);
	std::string operator+  (float  f, const std::string &s);
	std::string operator+  (double d, const std::string &s);
	
	
	namespace Util {
		
		void        delay         (double timeInSec);                    // a simple delay in seconds
		bool        isTimePassed  (int gap, double frameTime);           // returns true once a gap is passed
		double      timeDiff      ();                                    // returns the time since the last call
		int         getNextNumber (int pNum = -1);                       // returns a number higher than last
		bool        isNaN         (float  f);                            //returns wheather f is not a number
		bool        isNaN         (double d);                            //returns wheather d is not a number
		bool        isPowerOfTwo      (unsigned int i);                  //returns wheather i is a power of two
		int         nextPowerOfTwo    (unsigned int i);                  //returns next power of two
		int         previousPowerOfTwo(unsigned int i);                  //returns previous power of two
		float       floor         (float  f);                            //returns the floor
		double      floor         (double d);                            //returns the floor
		float       ceil          (float  f);                            //returns the ceiling
		double      ceil          (double d);                            //returns the ceiling
		bool        isNearlyZero  (float  f);                            //returns wheather f is nearly 0
		bool        isNearlyZero  (double d);                            //returns wheather d is nearly 0
		int         round         (double d);
		std::string replaceAnyWith(std::string str, const std::string& any, const std::string& with); //replaces all anys with with
		std::string deleteLetters          (std::string ps, std::string delims = " \t\n\v\f\r", bool atFront = true, bool inMiddle = true,  bool atEnd = true); //deletes letters where you want to
		std::string deleteOtherLettersThan (std::string ps, std::string delims = "1234567890.", bool atFront = true, bool inMiddle = true,  bool atEnd = true); //deletes letters which are not part of delims where you want to
		std::string reduceWhitespaces      (std::string ps);                                //deletes a space after a space
		std::string getFirstTokenOfStringAndErase   (std::string& ps, std::string delim = " ", bool delDelim = true); //first token is returned
		std::string getFirstNumberOfStringAndErase  (std::string& ps);                      //first XX.XX or XX
		std::string getFirstStringOfStringAndErase  (std::string& ps, char delim = '\"');   //first delimXXdelim (std: "XX" )
		bool        getFirstBoolOfStringAndErase    (std::string& ps);                      //first true of false
		std::string removeFoldersOfString(std::string  ps);
		std::string removeFileExtensionOfString(std::string  ps);
		int         getNumberOfNumbersInString      (std::string  ps);                      //returns the number of numbers  in ps
		int         getNumberOfStringsInString      (std::string  ps, char delim = '\"');   //returns the number of strings  in ps
		int         getNumberOfBoolsInString        (std::string  ps);                      //returns the number of booleans in ps
		bool        stringBeginsWith(const std::string& s1, const std::string& s2);         //does string s1 begin with s2?
		bool        stringEndsWith  (const std::string& s1, const std::string& s2);         //does string s1 end   with s2?
		bool        isStringWhitespace(const std::string& str);                             //is the whole string whitespace?
		std::string deleteComments  (std::string str, const std::string& com_line = "//",
		                             const std::string& com_block1 = "/*",
		                             const std::string& com_block2 = "*/", bool deleteWhiteLines = false); //deletes all line and block-comments from str
		bool        parseBool    (std::string  ps);  //returns the first number as bool
		int         parseInt     (std::string  ps);  //returns the first number as int
		int         parseUInt    (std::string  ps);  //returns the first number as unsigned int
		float       parseFloat   (std::string  ps);  //returns the first number as float
		double      parseDouble  (std::string  ps);  //returns the first number as double
		std::string addZeros     (std::string  ps, int newSize);  //returns a string with added '0' in front
		std::string toString     (bool   b);         //returns the value as std::string
		std::string toString     (char   c);         //returns the value as std::string
		std::string toString     (char*  c);         //returns the value as std::string
		std::string toString     (int    i);         //returns the value as std::string
		std::string toString     (float  f);         //returns the value as std::string
		std::string toString     (double d);         //returns the value as std::string
		std::string toStringHexNumber               (int    i);         //returns the value as std::string in hexa fromat
		void        toStringArray(int argc, char **argv, std::string *buffer); //converts command line arguments into a string array
		std::string toString                        (Roe::EKeyCode kc, bool pbShiftDown = false); //returns the value as std::string (more for german keyboards)
		bool        isPartOfRoeKeyCodes             (int kc); //returns wheather it is part of Roe::EKeyCode
		
		//gl
		std::string getGLError();
		
		//math
		bool    isPointOnRay(const Vector3& ray1, const Vector3& ray2, const Vector3& point); //is a point on a given ray?
		bool    isPointInSphere(const Vector3& p, const Vector3& center = Vector3::ZERO, const double r = 1.0);
		double  distancePointToRay(const Vector3& p, const Vector3& rayPos, const Vector3& rayDir, bool rayisline = false); //returns shortest possible distance
		bool    rayHitsSphere(const Vector3& rayPos, const Vector3& rayDir, const Vector3& m, const double r, Vector3 *pvOut = nullptr, bool rayisline = false);
		bool    rayHitsPlane (const Vector3& rayPos, const Vector3& rayDir, const Vector3& planePos, const Vector3& up, Vector3 *pvOut = nullptr, bool rayisline = false);
		bool    rayHitsPlane (const Vector3& rayPos, const Vector3& rayDir, const Plane& plane, Vector3 *pvOut = nullptr, bool rayisline = false);
		Vector3 nearestPointOnRay (const Vector3& p, const Vector3& rayPos, const Vector3& rayDir, bool rayisline = false); //returns the point on ray that is closest to p
		bool    calculateAimingVector(const Vector3& p, const Vector3& u, const Vector3& m, const double c, Vector3 *pvOut = nullptr); //returns correct aiming vector to shoot p
		Vector3 calculateNormalOfTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3); //calculates normal of an CCW triangle
		Vector3 calculateCenterOfCircle(const Vector3& v1, const Vector3& v2, const Vector3& v3); //calculates the center of the circle that crosses the 3 points
		float   calculateCurvatureOfSpline(const Vector3& v, const Vector3& a);
		Vector3 calculateCentripetalAccelerationOfSpline(const Vector3& v, const Vector3& a);
		//matrix math:
		/*solving a matrix, this function returns z, and maybe x and y
		ax + by + cz = d
		ex + fy + gz = h
		ix + jy + kz = l
		*/
		double solveMatrix3x4(double a, double b, double c, double d,
		                      double e, double f, double g, double h,
		                      double i, double j, double k, double l, double *px = nullptr, double *py = nullptr);
		
		//files
		void saveImageBMP(std::string sPath, const byte* pixel_data, long  w, long  h); //pixel_data in RGB!
		void saveImageTGA(std::string sPath, const byte* pixel_data, short w, short h, bool hasAlpha = false); //pixel_data in RGB!
		bool  fileExists  (const std::string& sPath); //does this file exist?
		byte4 fileSize    (const std::string& sPath); //returns the size of this file in bytes
		bool  removeFile  (const std::string& sPath); //removes the file if existent
		bool  renameFile  (const std::string& sPathOld, const std::string& sPathNew); //renames the file if existent
		
		template<typename T> void swap  (T &a, T &b)   {T c = a; a = b; b = c;} // swaps two values
		template<typename T> int  algebraicSign (const T &a) {if (a < 0.0) return -1; return 1;}
		template<typename T> T    absValue      (const T &a) {if (a < 0.0) return a* -1; return a;}
		template<typename T1, typename T2> void clamp (T1 &v, const T2& min, const T2& max) { //clamps a value v to min and max
			if (v < min) {v = min; return;}
			if (v > max)  v = max;
		}
		template<typename T> T min(const T& p1, const T& p2) {return (p2<p1)?p2:p1;}
		template<typename T> T min(const T& p1, const T& p2, const T& p3) {return min(min(p1,p2),p3);}
		template<typename T> T min(const T& p1, const T& p2, const T& p3, const T& p4) {return min(min(p1,p2),min(p3,p4));}
		template<typename T> T min(const T& p1, const T& p2, const T& p3, const T& p4, const T& p5) {return min(min(p1,p2,p3),min(p4,p5));}
		template<typename T> T max(const T& p1, const T& p2) {return (p2>p1)?p2:p1;}
		template<typename T> T max(const T& p1, const T& p2, const T& p3) {return max(max(p1,p2),p3);}
		template<typename T> T max(const T& p1, const T& p2, const T& p3, const T& p4) {return max(max(p1,p2),max(p3,p4));}
		template<typename T> T max(const T& p1, const T& p2, const T& p3, const T& p4, const T& p5) {return max(max(p1,p2,p3),max(p4,p5));}
		template<typename T> bool equalOR(const T& p1,const T& p2,const T& p3) {return p1==p2||p1==p3;}
		template<typename T> bool equalOR(const T& p1,const T& p2,const T& p3,const T& p4) {return p1==p2||p1==p3||p1==p4;}
		template<typename T> bool equalOR(const T& p1,const T& p2,const T& p3,const T& p4,const T& p5) {return p1==p2||p1==p3||p1==p4||p1==p5;}
		template<typename T> bool unequalAND(const T& p1,const T& p2,const T& p3) {return p1!=p2&&p1!=p3;}
		template<typename T> bool unequalAND(const T& p1,const T& p2,const T& p3,const T& p4) {return p1!=p2&&p1!=p3&&p1!=p4;}
		template<typename T> bool unequalAND(const T& p1,const T& p2,const T& p3,const T& p4,const T& p5) {return p1!=p2&&p1!=p3&&p1!=p4&&p1!=p5;}
		template<typename T> void println (const T& a, double delay = 0.0f) { // prints a line and delays if wanted
			std::cout << a << std::endl;
			if (delay > 0.0)
				Util::delay(delay);
		}
		template<typename T_To, typename T_From> void printArray2 (T_From *a, int w, int h) {
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					std::cout << (T_To)(a[i*w+j]) << "\t";
				}
				std::cout << std::endl;
			}
		}
		template<typename T_To, typename T_From> void printArray1 (T_From *a, int w) {
			for (int j = 0; j < w; j++) {
				std::cout << (T_To)(a[j]) << "\t";
			}
			std::cout << std::endl;
		}
		
		template<typename T> std::string toStringBinary (const T& p) { // returns the value p as binary string
			const size_t numBits = sizeof(p) * 8;
			std::string sTmp;
			for (unsigned int i = 0; i < numBits; i++) {
				sTmp = (( ((char*)(&p))[i/8] & (1 << (i%8)) ) ? ("1") : ("0")) + sTmp;
			}
			return sTmp;
		}
		
		template<typename T> int getNumBytes (T& p) {return static_cast<int>(sizeof(p));}
		template<typename T> int getLength (std::vector<T> &p) {return static_cast<int>(p.size());}
		template<typename T> int getLength (std::list  <T> &p) {return static_cast<int>(p.size());}
		                     int getLength (std::string    &p);
		
		
		// ******************************************************************
		// template-function for linear interpolation
		template <typename Type> Type interpolateLinear(const Type& Value1, const Type& Value2, const float x) {
			// simple linear interpolation
			return Value1 + x * (Value2 - Value1);
		}
		
		// ******************************************************************
		// template-function for hermite-interpolation
		template <typename Type> Type interpolateHermite(const Type& Value1, const Type& Tangent1,
		                                                 const Type& Value2, const Type& Tangent2,
		                                                 const float t) {
			// calculate coefficients A and B
			const Type A(2.0f * Value1 - 2.0f * Value2 + Tangent1 + Tangent2);
			const Type B(3.0f * Value2 - 3.0f * Value1 - 2.0f * Tangent1 - Tangent2);
			return A * (t*t*t) + B * (t*t) + Tangent1 * (t) + Value1; // perform interpolation
		}
		// template-function for calculating the first derivative of a hermite-interpolation-spline
		template <typename Type> Type interpolateHermiteDerivative1(const Type& Value1, const Type& Tangent1,
		                                                            const Type& Value2, const Type& Tangent2,
		                                                            const float t) {
			// calculate coefficients A and B
			const Type A(6.0f * Value1 - 6.0f * Value2 + 3.0f * Tangent1 + 3.0f * Tangent2);
			const Type B(6.0f * Value2 - 6.0f * Value1 - 4.0f * Tangent1 - 2.0f * Tangent2);
			return A * (t*t) + B * (t) + Tangent1; // perform interpolation
		}
		// template-function for calculating the second derivative of a hermite-interpolation-spline
		template <typename Type> Type interpolateHermiteDerivative2(const Type& Value1, const Type& Tangent1,
		                                                            const Type& Value2, const Type& Tangent2,
		                                                            const float t) {
			// calculate coefficients A and B
			const Type A(12.0f * Value1 - 12.0f * Value2 + 6.0f * Tangent1 + 6.0f * Tangent2);
			const Type B( 6.0f * Value2 -  6.0f * Value1 - 4.0f * Tangent1 - 2.0f * Tangent2);
			return A * t + B; // perform interpolation
		}
		
		// template-function for hermite-interpolation length (Type must have method length())
		template <typename Type> float hermiteSplineLength(const Type& Value1, const Type& Tangent1,
		                                                   const Type& Value2, const Type& Tangent2, const int steps = 1000) {
			float length = 0.0f;
			for (int i = 0; i < steps-1; i++) {
				length += (interpolateHermite(Value1,Tangent1,Value2,Tangent2,((float)i+1)/steps) -
				           interpolateHermite(Value1,Tangent1,Value2,Tangent2,((float)i  )/steps)).length();
			}
			return length;
		}
		
		// ******************************************************************
		// template-function for bilinear interpolation
		template <typename Type> Type interpolateBilinear(const Type& A, const Type& B, const Type& C,const Type& D,
		                                                  const float x, const float y) {
			// calculate halpful values P and Q
			const Type P(A + x * (B - A));
			const Type Q(C + x * (D - C));
		
			// interpolate between P and Q
			return P + y * (Q - P);
		}
		// ******************************************************************
		// template-function for hermite5-interpolation
		template <typename Type> Type interpolateHermite5(const Type& Value1, const Type& Der1_1, const Type& Der2_1,
		                                                  const Type& Value2, const Type& Der1_2, const Type& Der2_2,
		                                                  const float t) {
			// calculate coefficients A and B and C
			const Type A(-6.0f *Value1 + 6.0f  *Value2 - 3.0f*Der1_1 - 3.0f*Der1_2 - 0.5f*Der2_1 + 0.5f*Der2_2);
			const Type B(15.0f *Value1 - 15.0f *Value2 + 8.0f*Der1_1 + 7.0f*Der1_2 + 1.5f*Der2_1 - Der2_2);
			const Type C(-10.0f*Value1 + 10.0f *Value2 - 6.0f*Der1_1 - 4.0f*Der1_2 - 1.5f*Der2_1 + 0.5f*Der2_2);
			return A*(t*t*t*t*t) + B*(t*t*t*t) + C*(t*t*t) + 0.5f*Der2_1 * (t*t) + Der1_1 * t + Value1; // perform interpolation
		}
		// template-function for calculating the first derivative of a hermite-interpolation-spline
		template <typename Type> Type interpolateHermite5Derivative1(const Type& Value1, const Type& Der1_1, const Type& Der2_1,
		                                                             const Type& Value2, const Type& Der1_2, const Type& Der2_2,
		                                                             const float t) {
			// calculate coefficients A and B and C
			const Type A(-30.0f*Value1 + 30.0f *Value2 - 15.0f*Der1_1 - 15.0f*Der1_2 - 2.5f*Der2_1 + 2.5f*Der2_2);
			const Type B(60.0f *Value1 - 60.0f *Value2 + 32.0f*Der1_1 + 28.0f*Der1_2 + 6.0f*Der2_1 - 4.0f*Der2_2);
			const Type C(-30.0f*Value1 + 30.0f *Value2 - 18.0f*Der1_1 - 12.0f*Der1_2 - 4.5f*Der2_1 + 1.5f*Der2_2);
			return A*(t*t*t*t) + B*(t*t*t) + C*(t*t) + Der2_1 * (t) + Der1_1; // perform interpolation
		}
		// template-function for calculating the second derivative of a hermite-interpolation-spline
		template <typename Type> Type interpolateHermite5Derivative2(const Type& Value1, const Type& Der1_1, const Type& Der2_1,
		                                                             const Type& Value2, const Type& Der1_2, const Type& Der2_2,
		                                                             const float t) {
			// calculate coefficients A and B and C
			const Type A(-120.0f*Value1 + 120.0f *Value2 - 60.0f*Der1_1 - 60.0f*Der1_2 - 10.0f*Der2_1 + 10.0f*Der2_2);
			const Type B(180.0f *Value1 - 180.0f *Value2 + 96.0f*Der1_1 + 84.0f*Der1_2 + 18.0f*Der2_1 - 12.0f*Der2_2);
			const Type C(-60.0f *Value1 + 60.0f  *Value2 - 36.0f*Der1_1 - 24.0f*Der1_2 - 9.0f *Der2_1 + 3.0f *Der2_2);
			return A*(t*t*t) + B*(t*t) + C*(t) + Der2_1; // perform interpolation
		}
		// ******************************************************************
	
	} // namespace Util
} // namespace Roe


#endif /*ROEUTIL_H_*/
