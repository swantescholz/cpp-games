#ifndef ROEXML_H_
#define ROEXML_H_

#include "RoeCommon.h"
#include "RoeTextfile.h"

#include <stack>

namespace Roe {
	
	// a System for parsing and writing well formed xml-files; attributes are forbidden, prolog not parsed
	
	class XMLParser;
	class XMLWriter;
	
	class XML_LeafTag // has no childs, but one element
	{
	public:
		XML_LeafTag()
			: name   ("")
			, element("")
			, index  (-1) {}
		
		std::string name;
		std::string element;
		int         index; // 0 -> first tag at this level... ; -1 -> incorrect leaf
	};
	
	// ********************************************
	
	class XML_BranchTag // has childs
	{
		friend class XMLParser;
		
	public:
		XML_BranchTag();
		XML_BranchTag(const XML_BranchTag& rhs);
		XML_BranchTag& operator= (const XML_BranchTag& rhs);
		~XML_BranchTag();                                 //this will close the file and clear the xml branches
		
		std::string getName       () const {return name;}
		int         getIndex      () const {return index;}
		int         getNumBranches() const {return numBranches;}
		int         getNumLeaves  () const {return numLeaves;}
		XML_BranchTag getBranch  (int num) const;
		XML_LeafTag   getLeaf    (int num) const;
		
		void clear(); // clears this branch tag with its leaves and sub-branches
		
	private:
		std::string name;
		int         index; // 0 -> first tag at this level... ; -1 -> incorrect branch
		XML_BranchTag *aBranchTags;
		XML_LeafTag   *aLeafTags;
		int            numBranches;
		int            numLeaves;
	};
	
	// **********************************
	
	class XMLParser
	{
		enum ETagType {
			TT_OPENING = 0,
			TT_CLOSING,
			TT_LEAF
		};
	public:
		XMLParser();
		XMLParser(const std::string& psFilename);
		~XMLParser();
		
		void openFile(const std::string& psFilename);           // reads in the text
		void closeFile();                                       // clears the text lines
		void clear();                                           // clears the tag tree
		void parseFile();                                       // does all the parsing
		void parseTag(XML_BranchTag& pTag, int startLineIndex);
		XML_BranchTag getRoot();                                // returns the root branch tag
		
	private:
		
		void trimFile(); // deletes prolog, comments unused whitspaces and empty lines
		int getNumberOfChildBranchTags  (int openingTagLine);
		int getNumberOfChildLeafTags    (int openingTagLine);
		int getLineNumberOfClosingTag   (int openingTagLine);
		int getChildLeaf                (int startingLineNumber, int num);
		int getChildBranch              (int startingLineNumber, int num);
		std::string getLeafName         (int iLine);
		std::string getLeafElement      (int iLine);
		std::string getBranchName       (int iLine);
		
		Textfile       m_Textfile;
		XML_BranchTag  m_Root;
		int            m_iNumLines;
		ETagType      *m_aiTagTypes;
	};
	
	
	// ***************************
	
	class XMLWriter
	{
	public:
		XMLWriter();
		XMLWriter(const std::string& psFilename);
		~XMLWriter();
		
		void openFile(const std::string& psFilename);  //this opens the file
		void closeFile();                              //this will do the actual saving to file
		
		void openTag        (const std::string& psTagName);                                   // opens a tag
		void closeTag       ();                                                               // closes a tag with the fitting tag name
		void writeElementTag(const std::string& psTagName, const std::string& psElementName); // a tag with a element value
		void writeComment   (const std::string& psComment);                                   // writes a simple indented comment
		
	private:
		Textfile         m_Textfile;             //the actual textfile
		int              m_iNumMoreOpenedTags;   //opened tags - closed tags
		std::stack<std::string> m_OpenTagNames;  //a stack with the fitting tag names
	};
	
} // namespace Roe

#endif /*ROEXML_H_*/
