#ifndef BINFILE_H_
#define BINFILE_H_


#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace Roe {
	
	//forward declaration of class BinFile
	class BinFile;
	
	//a simple abstract base class for any class that wants to be serializable
	class Serializable
	{
	public:
		Serializable() {}
		virtual ~Serializable() {}
		virtual void   serialize (BinFile& bf) = 0;
		virtual void unserialize (BinFile& bf) = 0;
	};
	
	//class BinFile
	//represents a binary file
	//it writes data and reads data(which was written with this class)
	//capable of writing strings, vectors and lists(which were written with this class)
	//static member functions are general usable
	//write string/vector/list will place their length before their data!
	class BinFile
	{
	private:
		std::ifstream   m_fin;      //input stream
		std::ofstream   m_fout;     //ouput stream
		std::string     m_sNameIn;  //input  file name
		std::string     m_sNameOut; //output file name
		
	public:
		BinFile();
		~BinFile();
		
		void openIn  (const std::string& pathIn );                       // reads  IN from a binary file
		void openOut (const std::string& pathOut, bool append = false);  // writes OUT to a binary file (maybe appended)
		void closeIn ();                            // closes ifstream
		void closeOut();                            // closes ofstream
		
		std::ifstream& getIfstream(); // returns the ifstream
		std::ofstream& getOfstream(); // returns the ofstream
		std::string    getNameIn  (); // returns the name of current ifstream file
		std::string    getNameOut (); // returns the name of current ofstream file
		
		bool isOpenIn (); //returns if ifstream is open
		bool isOpenOut(); //returns if ofstream is open
		bool eofIn    (); //returns if ifstream is at the end of the file
		bool eofOut   (); //returns if ofstream is at the end of the file
		bool goodIn   (); //returns if ifstream is good(correct) connected
		bool goodOut  (); //returns if ofstream is good(correct) connected
		unsigned long int sizeIn  (); //returns the byte size of input file
		unsigned long int sizeOut (); //returns the byte size of the complete output file at this moment
		void seekIn   (std::streamoff off, std::ios_base::seekdir dir); //seekg() in ifstream; seeks for a point from orgin dir with offset off(in bytes)
		void seekOut  (std::streamoff off, std::ios_base::seekdir dir); //seekp() in ofstream; seeks for a point from orgin dir with offset off(in bytes)
		void movePointerIn   (int offBytes, int startPos = 0); //simpler form of above, startPos=0->curPos, startPos=1->endPos, startPos=-1->beginning of file
		void movePointerOut  (int offBytes, int startPos = 0); //simpler form of above, startPos=0->curPos, startPos=1->endPos, startPos=-1->beginning of file
		long int  getPointerPosIn (); //returns position in bytes
		long int  getPointerPosOut(); //returns position in bytes
		
		
		// *output with stream operators*
		template<typename T> BinFile& operator<<  (const T& p)                   {writeSimpleType         (p); return (*this); }
		template<typename T> BinFile& operator<<  (std::vector<T>& p)            {writeSimpleVector       (p); return (*this); }
		template<typename T> BinFile& operator<<  (std::list  <T>& p)            {writeSimpleList         (p); return (*this); }
		                     BinFile& operator<<  (std::string& p)               {writeString             (p); return (*this); }
		                     BinFile& operator<<  (std::vector<std::string>& p)  {writeStringVector       (p); return (*this); }
		                     BinFile& operator<<  (std::list  <std::string>& p)  {writeStringList         (p); return (*this); }
		                     BinFile& operator<<  (Serializable& p)              {writeSerializable       (p); return (*this); }
		// *input with stream operators*
		template<typename T> BinFile& operator>>  (T& p)                         {readSimpleType         (p); return (*this); }
		template<typename T> BinFile& operator>>  (std::vector<T>& p)            {readSimpleVector       (p); return (*this); }
		template<typename T> BinFile& operator>>  (std::list<T>&   p)            {readSimpleList         (p); return (*this); }
		                     BinFile& operator>>  (std::string& p)               {readString             (p); return (*this); }
		                     BinFile& operator>>  (std::vector<std::string>& p)  {readStringVector       (p); return (*this); }
		                     BinFile& operator>>  (std::list  <std::string>& p)  {readStringList         (p); return (*this); }
		                     BinFile& operator>>  (Serializable& p)              {readSerializable       (p); return (*this); }
		
		// *output with references*
		template<typename T> void writeSimpleType       (const T& p)        { writeSimpleType(m_fout, p); }
		template<typename T> void writeSimpleTypePointer(T* p)              { writeSimpleTypePointer(m_fout, p); }    // p must be deleted later
		template<typename T> void writeSimpleTypeArray  (T p[], int num)    { writeSimpleTypeArray(m_fout, p, num); } // p should be a pointer to an array!
		template<typename T> void writeSimpleVector     (std::vector<T>& p) { writeSimpleVector(m_fout, p); }
		template<typename T> void writeSimpleList       (std::list  <T>& p) { writeSimpleList  (m_fout, p); }
		void writeString                                (std::string& p)    { writeString      (m_fout, p); }
		void writeStringVector                (std::vector<std::string>& p) { writeStringVector(m_fout, p); }
		void writeStringList                  (std::list  <std::string>& p) { writeStringList  (m_fout, p); }
		void writeSerializable                          (Serializable& p)   { p.serialize      ( (*this) );}
		// *input with references*
		template<typename T> void readSimpleType       (T& p)              { readSimpleType(m_fin, p); }
		template<typename T> void readSimpleTypePointer(T* p)              { readSimpleTypePointer(m_fin, p); }    // p must be deleted later
		template<typename T> void readSimpleTypeArray  (T p[], int num)    { readSimpleTypeArray(m_fin, p, num); } // p should be a pointer to an array!
		template<typename T> void readSimpleVector     (std::vector<T>& p) { readSimpleVector(m_fin, p); }
		template<typename T> void readSimpleList       (std::list  <T>& p) { readSimpleList  (m_fin, p); }
		void readString                                (std::string& p)    { readString      (m_fin, p); }
		void readStringVector                (std::vector<std::string>& p) { readStringVector(m_fin, p); }
		void readStringList                  (std::list  <std::string>& p) { readStringList  (m_fin, p); }
		void readSerializable                          (Serializable& p)   { p.unserialize   ( (*this) );}
		// *input with return values*
		template<typename T> T    readSimpleType      ()        {T               p; readSimpleType     (m_fin, p);      return p;}
		template<typename T> T*   readSimpleTypeArray (int num) {T* p = new T[num]; readSimpleTypeArray(m_fin, p, num); return p;} // returned pointer should be deleted by user
		std::string               readString          ()        {std::string     p; readString         (m_fin, p);      return p;}
		//template<typename T> std::vector<T> readSimpleVector () {std::vector<T>  p; readSimpleVector(m_fin, p); return p;} //too dangerous, not really useful
		//template<typename T> std::list<T>   readSimpleList   () {std::list<T>    p; readSimpleList  (m_fin, p); return p;} //too dangerous, not really useful
		
		
		// *static i/o*
		// *SIMPLE TYPE = OBJECT WITH PREDETERMINED SIZE (classes/structs possible, pointers and vectors/lists... NOT)*
		// *writing*
		// writes a simple type into fout
		template<typename T> static void writeSimpleType        (std::ofstream& fout, const T& p) { 
			fout.write( (char*) (&p), sizeof(T)); // sizeof(T) should be equal to sizeof(p)
		}
		// writes a pointer to a simple type into fout
		template<typename T> static void writeSimpleTypePointer (std::ofstream& fout, T* p) { 
			fout.write( (char*)  (p), sizeof(T)); // sizeof(T) should be equal to sizeof(p)
		}
		// writes an array of simple types into fout
		template<typename T> static void writeSimpleTypeArray	(std::ofstream& fout, T* p, int num) { // p should be a pointer to an array!
			fout.write( (char*)  (p), sizeof(T) * num); // one size * number of entries
		}
		// writes a std::string
		static void writeString                           (std::ofstream& fout, std::string& p) {
			const int length = p.length();
			writeSimpleType(fout, length);
			for (int i = 0; i < length; i++) {
				writeSimpleType(fout, p.at(i) );
			}
		}
		// writes a std::vector but with simple types!
		template<typename T> static void writeSimpleVector      (std::ofstream& fout, std::vector<T>& p) {
			const int length = static_cast<int>(p.size());
			writeSimpleType(fout, length);
			for (int i = 0; i < length; i++) {
				writeSimpleType(fout, p.at(i) );
			}
		}
		// writes a std::list but with simple types!
		template<typename T> static void writeSimpleList        (std::ofstream& fout, std::list<T>& p) {
			const int length = static_cast<int>(p.size());
			writeSimpleType(fout, length);
			for ( typename std::list<T>::iterator it1 = p.begin(); it1 != p.end(); it1++) {
				writeSimpleType(fout, (*it1) );
			}
		}
		// writes a std::vector but with std::strings
		static void writeStringVector      (std::ofstream& fout, std::vector<std::string>& p) {
			const int length = static_cast<int>(p.size());
			writeSimpleType(fout, length);
			for (int i = 0; i < length; i++) {
				writeString(fout, p.at(i) );
			}
		}
		// writes a std::list but with std::strings
		static void writeStringList        (std::ofstream& fout, std::list<std::string>& p) {
			const int length = static_cast<int>(p.size());
			writeSimpleType(fout, length);
			for (std::list<std::string>::iterator it1 = p.begin(); it1 != p.end(); it1++) {
				writeString(fout, (*it1) );
			}
		}
		
		
		// *reading*
		// reads a simple type from fin
		template<typename T> static void readSimpleType         (std::ifstream& fin, T& p) { 
			fin.read( (char*) (&p), sizeof(T));         // sizeof(T) should be equal to sizeof(p)
		}
		// reads a pointer to a simple type from fin
		template<typename T> static void readSimpleTypePointer  (std::ifstream& fin, T* p) {
			p = new T(); 
			fin.read( (char*)  (p), sizeof(T));         // sizeof(T) should be equal to sizeof(p)
		}
		// reads an array of simple types from fin
		template<typename T> static void readSimpleTypeArray    (std::ifstream& fin, T* p, int num) { // p should be a pointer to an array!
			fin.read( (char*)  (p), sizeof(T) * num);   // one size * number of entries
		}
		// reads a std::string
		static void readString                            (std::ifstream& fin, std::string& p) {
			int length;
			std::string sTmp;
			char chTmp;
			readSimpleType(fin, length);
			for (int i = 0; i < length; i++) {
				readSimpleType(fin, chTmp);
				sTmp += chTmp;
			}
			p = sTmp;
		}
		// reads a std::vector but with simple types!
		template<typename T> static void readSimpleVector (std::ifstream& fin, std::vector<T>& p) {
			int length;
			T   tmp;
			p.clear();
			readSimpleType(fin, length);
			for (int i = 0; i < length; i++) {
				readSimpleType(fin, tmp);
				p.push_back(tmp);
			}
		}
		// reads a std::list but with simple types!
		template<typename T> static void readSimpleList (std::ifstream& fin, std::list<T>& p) {
			int length;
			T   tmp;
			p.clear();
			readSimpleType(fin, length);
			for (int i = 0; i < length; i++) {
				readSimpleType(fin, tmp);
				p.push_back(tmp);
			}
		}
		// reads a std::vector but with std::strings
		static void readStringVector (std::ifstream& fin, std::vector<std::string>& p) {
			int         length;
			std::string tmp;
			p.clear();
			readSimpleType(fin, length);
			for (int i = 0; i < length; i++) {
				readString(fin, tmp);
				p.push_back(tmp);
			}
		}
		// reads a std::list but with std::strings
		static void readStringList (std::ifstream& fin, std::list<std::string>& p) {
			int         length;
			std::string tmp;
			p.clear();
			readSimpleType(fin, length);
			for (int i = 0; i < length; i++) {
				readString(fin, tmp);
				p.push_back(tmp);
			}
		}
		
	};
	
	
} // namespace Roe

#endif /*BINFILE_H_*/
