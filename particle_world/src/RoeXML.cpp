
#include "RoeXML.h"

#include "RoeUtil.h"

namespace Roe {
	
	XML_BranchTag::XML_BranchTag()
		 	: name        ("")
		 	, index       (-1)
		 	, aBranchTags (NULL)
		 	, aLeafTags   (NULL)
		 	, numBranches (0)
		 	, numLeaves   (0)
	{
	}
	
	XML_BranchTag::XML_BranchTag(const XML_BranchTag& rhs) {
		(*this) = rhs;
	}
	XML_BranchTag& XML_BranchTag::operator= (const XML_BranchTag& rhs) {
		name      = rhs.name;
		index     = rhs.index;
		numBranches = rhs.numBranches;
		numLeaves   = rhs.numLeaves;
		if (numBranches > 0) {
			aBranchTags  = new XML_BranchTag[numBranches];
			for (int i = 0; i < numBranches; i++) {aBranchTags[i] = rhs.aBranchTags[i];}
		} else {
			aBranchTags = NULL;
		}
		if (numLeaves > 0) {
			aLeafTags    = new XML_LeafTag  [numLeaves];
			for (int i = 0; i < numLeaves;   i++) {aLeafTags  [i] = rhs.aLeafTags  [i];}
		} else {
			aLeafTags = NULL;
		}
		return (*this);
	}
	 XML_BranchTag::~XML_BranchTag()
	 {
	 	clear();
	 }
	
	
	XML_BranchTag XML_BranchTag::getBranch  (int num) const {
		if (num < numBranches)
			return aBranchTags[num];
		XML_BranchTag tag;
		tag.index = -1;
		return tag;
	}
	XML_LeafTag   XML_BranchTag::getLeaf    (int num) const {
		if (num < numLeaves)
			return aLeafTags[num];
		XML_LeafTag tag;
		tag.index = -1;
		return tag;
	}
	void XML_BranchTag::clear() {
		ROE_SAFE_DELETE_ARRAY(aLeafTags);
		ROE_SAFE_DELETE_ARRAY(aBranchTags);
		numLeaves   = 0;
		numBranches = 0;
	}
	
	// ***********************************************
	
	XMLParser::XMLParser()
	{
		m_aiTagTypes = NULL;
		m_Root.index = -1;
	}
	XMLParser::XMLParser(const std::string& psFilename)
	{
		m_aiTagTypes = NULL;
		m_Root.index = -1;
		openFile(psFilename);
	}
	
	XMLParser::~XMLParser()
	{
		closeFile();
		clear();
	}
	
	void XMLParser::openFile(const std::string& psFilename) {
		m_Textfile.setFilename(psFilename);
		m_Textfile.readFile(false);
	}
	void XMLParser::closeFile() {
		if (m_Textfile.getNumLines() > 0) {
			m_Textfile.clear();
		}
		if (m_aiTagTypes != NULL) { // root has elements
			ROE_SAFE_DELETE_ARRAY(m_aiTagTypes);
		}
	}
	void XMLParser::clear() {
		if (m_Root.index != -1) {
			m_Root.clear();
		}
	}
	
	void XMLParser::parseFile() {
		trimFile();
		std::string sLine;
		for (int i = 0; i < m_iNumLines; i++) {
			sLine = m_Textfile.getLine(i);
			if (sLine[1] == '/') {       // '/' means closing tag
				m_aiTagTypes[i] = TT_CLOSING;
			}
			else if (sLine.find("<",1) != std::string::npos) { // closing tag in same line -> leaf node
				m_aiTagTypes[i] = TT_LEAF;
			}
			else { // otherwise it must be an opening tag
				m_aiTagTypes[i] = TT_OPENING;
			}
		}
		
		m_Root.index = 0;
		m_Root.name  = getBranchName(0);
		
		parseTag(m_Root, 0);
	}
	
	void XMLParser::parseTag(XML_BranchTag& pTag, int startLineIndex) {
		const int iNumChildLeaves   = getNumberOfChildLeafTags  (startLineIndex);
		const int iNumChildBranches = getNumberOfChildBranchTags(startLineIndex);
		
		pTag.numLeaves   = iNumChildLeaves;
		pTag.numBranches = iNumChildBranches;
		if (iNumChildLeaves > 0) {
			pTag.aLeafTags   = new XML_LeafTag   [iNumChildLeaves];
		} else {
			pTag.aLeafTags = NULL;
		}
		if (iNumChildBranches > 0) {
			pTag.aBranchTags = new XML_BranchTag [iNumChildBranches];
		} else {
			pTag.aBranchTags = NULL;
		}
		
		int iLine = 0;
		for (int i = 0; i < iNumChildLeaves; i++) {
			iLine = getChildLeaf(startLineIndex, i);
			pTag.aLeafTags[i].index = i;
			pTag.aLeafTags[i].name     = getLeafName(iLine);
			pTag.aLeafTags[i].element  = getLeafElement(iLine);
		}
		for (int i = 0; i < iNumChildBranches; i++) {
			iLine = getChildBranch(startLineIndex, i);
			
			pTag.aBranchTags[i].index = i;
			pTag.aBranchTags[i].name  = getBranchName(iLine);
			parseTag(pTag.aBranchTags[i], iLine); // recursion
		}
		
	}
	
	XML_BranchTag XMLParser::getRoot() {
		return m_Root;
	}
	
	void XMLParser::trimFile() {
		std::string sLine;
		
		//first deleting whitespaces at front and back
		for (int i = 0; i < m_Textfile.getNumLines(); i++) {
			m_Textfile.setLine(i, Util::deleteLetters(m_Textfile.getLine(i), " \t\n\v\f\r", true, false, true) );
		}
		
		//deleting comments and prolog
		for (int i = 0; i < m_Textfile.getNumLines(); i++) {
			sLine = m_Textfile.getLine(i);
			if (sLine.length() >= 3) { // three is minimum: "<b>"
				if (sLine[0] == '<') {
					if (sLine[1] == '!' || sLine[1] == '?') { // part of prolog or a comment -> delete line 
						m_Textfile.erase(i);
					}
				}
				else { // no beginning tag -> delete line
					m_Textfile.erase(i);
				}
			}
			else { // too short -> delete line
				m_Textfile.erase(i);
			}
		}
		
		//register number of lines
		m_iNumLines = m_Textfile.getNumLines();
		m_aiTagTypes = new ETagType[m_iNumLines];
	}
	
	int XMLParser::getNumberOfChildBranchTags(int openingTagLine) {
		const int closingTagLine = getLineNumberOfClosingTag(openingTagLine);
		if (closingTagLine == -1) return 0;
		int iNumChilds = 0;
		int iLine = openingTagLine + 1; // excluding beginning and end
		while (iLine < closingTagLine) {
			if (m_aiTagTypes[iLine] == TT_OPENING) {
				iNumChilds++;
				iLine = getLineNumberOfClosingTag(iLine);
			}
			iLine++; 
		}
		return iNumChilds;
	}
	int XMLParser::getNumberOfChildLeafTags(int openingTagLine) {
		const int closingTagLine = getLineNumberOfClosingTag(openingTagLine);
		if (closingTagLine == -1) return 0;
		int iNumChilds = 0;
		int iLine = openingTagLine + 1; // excluding beginning and end
		while (iLine < closingTagLine) {
			if (m_aiTagTypes[iLine] == TT_OPENING) {
				iLine = getLineNumberOfClosingTag(iLine);
			}
			else if (m_aiTagTypes[iLine] == TT_LEAF) {
				iNumChilds++;
			}
			iLine++; 
		}
		return iNumChilds;
	}
	int XMLParser::getLineNumberOfClosingTag(int openingTagLine) {
		std::string psTagName = m_Textfile.getLine(openingTagLine);
		psTagName.erase(0,1);
		psTagName = "</" + psTagName;
		
		for (int i = openingTagLine; i < m_iNumLines; i++) {
			if (m_Textfile.getLine(i) == psTagName) {
				return i;
			}
		}
		return -1; 
	}
	int XMLParser::getChildLeaf(int startingLineNumber, int num) {
		const int iNumLeaves = getNumberOfChildLeafTags( startingLineNumber);
		if (num >= iNumLeaves) return -1;
		const int iEndLine  = getLineNumberOfClosingTag( startingLineNumber);
		int iNumFound = 0;
		for (int i = startingLineNumber; i < iEndLine; i++) {
			if (m_aiTagTypes[i] == TT_LEAF) {
				if (iNumFound == num) {
					return i;
				}
				else {
					iNumFound++;
				}
			}
		}
		return -1;
	}
	int XMLParser::getChildBranch(int startingLineNumber, int num) {
		const int iNumBranches = getNumberOfChildBranchTags( startingLineNumber);
		if (num >= iNumBranches) return -1;
		const int iEndLine     = getLineNumberOfClosingTag ( startingLineNumber);
		int iNumFound = 0;
		for (int i = startingLineNumber+1; i < iEndLine; i++) {
			if (m_aiTagTypes[i] == TT_OPENING) {
				if (iNumFound == num) {
					return i;
				}
				else {
					iNumFound++;
					i = getLineNumberOfClosingTag ( i );
				}
			}
		}
		return -1;
	}
	std::string XMLParser::getLeafName (int iLine) {
		std::string sName;
		if (m_aiTagTypes[iLine] != TT_LEAF) return sName;
		sName = m_Textfile.getLine(iLine);
		sName.erase(0,1);
		sName.erase(sName.find(">"));
		return sName;
	}
	std::string XMLParser::getLeafElement (int iLine) {
		std::string sName;
		if (m_aiTagTypes[iLine] != TT_LEAF) return sName;
		sName = m_Textfile.getLine(iLine);
		sName.erase(0,sName.find(">"));
		sName.erase(sName.find("<"));
		return sName;
	}
	std::string XMLParser::getBranchName (int iLine) {
		std::string sName;
		if (m_aiTagTypes[iLine] != TT_OPENING) return sName;
		sName = m_Textfile.getLine(iLine);
		sName.erase(0,1);
		sName.erase(sName.length()-1 );
		return sName;
	}
	
	// *****************************
	
	XMLWriter::XMLWriter()
	{
	}
	XMLWriter::XMLWriter(const std::string& psFilename)
	{
		openFile(psFilename);
	}
	XMLWriter::~XMLWriter()
	{
	}
	
	
	void XMLWriter::openFile(const std::string& psFilename) {
		m_Textfile.setFilename(psFilename);
		m_iNumMoreOpenedTags = 0;
	}
	void XMLWriter::closeFile() {
		m_Textfile.saveFile(false);
		m_Textfile.clear();
	}
	
	void XMLWriter::openTag(const std::string& psTagName) {
		std::string sLine;
		for (int i = 0; i < m_iNumMoreOpenedTags; i++) {
			sLine += "    "; // indentation (4 spaces per level)
		}
		sLine += "<" + psTagName + ">";
		m_iNumMoreOpenedTags++;
		m_OpenTagNames.push(psTagName);
		m_Textfile.pushBack(sLine);
	}
	void XMLWriter::closeTag() {
		m_iNumMoreOpenedTags--;
		std::string sLine;
		for (int i = 0; i < m_iNumMoreOpenedTags; i++) {
			sLine += "    "; // indentation (4 spaces per level)
		}
		sLine += "</" + m_OpenTagNames.top() + ">";
		m_OpenTagNames.pop();
		m_Textfile.pushBack(sLine);
	}
	void XMLWriter::writeElementTag(const std::string& psTagName, const std::string& psElementName) {
		std::string sLine;
		for (int i = 0; i < m_iNumMoreOpenedTags; i++) {
			sLine += "    "; // indentation (4 spaces per level)
		}
		sLine += "<" + psTagName + ">";
		sLine += psElementName;
		sLine += "</" + psTagName + ">";
		m_Textfile.pushBack(sLine);
	}
	void XMLWriter::writeComment(const std::string& psComment) {
		std::string sLine;
		for (int i = 0; i < m_iNumMoreOpenedTags; i++) {
			sLine += "    "; // indentation (4 spaces per level)
		}
		sLine += "<!--" + psComment + "-->";
		m_Textfile.pushBack(sLine);
	}
	
} // namespace Roe

