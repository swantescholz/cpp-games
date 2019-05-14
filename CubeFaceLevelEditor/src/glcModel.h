#ifndef GLCMODEL_H_
#define GLCMODEL_H_

#include "glcSDL_GL.h"


struct S_ModelIntern {			//nur fuer die modelklasse
	int iDispList;			//display-list index
	tbVector3 vFarthest;	//am weitesten entfernter vektor
	tbVector3 vBoxMin;		//bounding-box standart-min
	tbVector3 vBoxMax;		//bounding-box standart-max
};


class glcModel			//eine klasse fuer models
{
	
public:
	glcModel();
	glcModel(const string &s);			//ruft nur load auf
	~glcModel();
	
	void Render();							//rendert die passende display-list aus der map
	void Transform(const tbMatrix &m);		//transformiert die weltmatrix (nicht scalen!!)
	void Load(const string &s);				//loaded nur fuer eine instanz ein model aus der model-map
	
	//infomationen ueber den transformierten stand
	tbVector3 GetCenter	() const {return m_vCenter;}
	tbVector3 GetXAxis	() const {return m_vXAxis;}
	tbVector3 GetYAxis	() const {return m_vYAxis;}
	tbVector3 GetZAxis	() const {return m_vZAxis;}
	void	  SetXAxis	(const tbVector3 &v)	{tbVector3 vTmp = m_vCenter; Transform(tbMatrixTranslation(-vTmp)); Transform(tbMatrixRotationAxis(tbVector3Cross( m_vXAxis , v) , tbVector3Angle(m_vXAxis, v) ) ); Transform(tbMatrixTranslation(vTmp));}
	void	  SetYAxis	(const tbVector3 &v)	{tbVector3 vTmp = m_vCenter; Transform(tbMatrixTranslation(-vTmp)); Transform(tbMatrixRotationAxis(tbVector3Cross( m_vYAxis , v) , tbVector3Angle(m_vYAxis, v) ) ); Transform(tbMatrixTranslation(vTmp));}
	void	  SetZAxis	(const tbVector3 &v)	{tbVector3 vTmp = m_vCenter; Transform(tbMatrixTranslation(-vTmp)); Transform(tbMatrixRotationAxis(tbVector3Cross( m_vZAxis , v) , tbVector3Angle(m_vZAxis, v) ) ); Transform(tbMatrixTranslation(vTmp));}
	tbVector3 GetBoxMin	() const {return tbVector3TransformCoords(c_mpModel[m_sPath].vBoxMin, m_mScale);}
	tbVector3 GetBoxMax	() const {return tbVector3TransformCoords(c_mpModel[m_sPath].vBoxMax, m_mScale);}
	float	  GetRadius	() const {return m_fRadius;}
	tbMatrix  GetMatrix	() const {return m_mMatrix;}
	void	  SetMatrix (const tbMatrix &m) {m_mMatrix = m;}
	
	//scaling
	void SetScale(const tbVector3 &v)	{m_vScale  = v; Update();}		//standart-scalierung todo update
	void MultScale(const tbVector3 &v) 	{m_vScale *= v; Update();}
	tbVector3 GetScale() {return m_vScale;}
	
	// fuer die statischen variablen
	static void Init(string sStart, string sEnd)	{c_sPathStart = sStart; c_sPathEnd = sEnd;}
	static void SetPathStart(string s) 				{c_sPathStart = s;}
	static void SetPathEnd  (string s) 				{c_sPathEnd   = s;}
	static void Quit()								{for(c_mpi=c_mpModel.begin(); c_mpi!=c_mpModel.end(); ++c_mpi){glDeleteLists(c_mpi->second.iDispList, 1);}c_mpModel.clear();}
	
	//funktion(en) zum laden eines models aus einem file; [alle modelle sollten in die negative z-achse blicken; mit center auf zero; laenge auf y-achse=2!,r=1]
	//sPath ist dabei der noch unvollstaendige pfad und mTransform ist die matrix, durch die alle vektoren transformiert werden sollen (z.B. fuer standart-skalierungen)
	static void LoadOFF(string sPath, const tbMatrix &mTransform = tbMatrixIdentity());	//kann models aus object file format (off) einlesen 
	
	
	
	
private:
	
	void Update();		//updatet informationen ueber das modell
	
	
	static map<string, S_ModelIntern> 				c_mpModel;	//map mit allen display lists
	static map<string, S_ModelIntern>::iterator 	c_mpi;		//map iterator
	static pair<string, S_ModelIntern> 				c_pair;		//paar-objekt fuer die map
	static string c_sPathStart, c_sPathEnd;		//standart path
	
	tbMatrix m_mMatrix;							//transformationsmatrix
	string m_sPath;								//vollstaendiger eigener path
	tbVector3 m_vScale;							//standart-scaling
	tbMatrix m_mScale;							//dazugehoerige matrix
	
	//update daten
	tbVector3 m_vCenter;						//center
	float m_fRadius;							//radius der umgebungskugel
	tbVector3 m_vXAxis, m_vYAxis, m_vZAxis;		//eigene achsen
	tbVector3 m_vBoxMin, m_vBoxMax;				//gescalte bounding box (ohne andere transformationen);
};

inline bool tbModelHitsModel(const glcModel &mod1, const glcModel &mod2, const int iSamples) {
	if (!tbSphereHitsSphere(mod1.GetCenter(), mod1.GetRadius(), mod2.GetCenter(), mod2.GetRadius()))
		return false;
	
	return tbBoxHitsBox(mod1.GetBoxMin(), mod1.GetBoxMax(), mod1.GetMatrix(),
						mod2.GetBoxMin(), mod2.GetBoxMax(), mod2.GetMatrix(), iSamples);
}
inline bool tbModelHitsModel(const glcModel &mod1, const glcModel &mod2) {
	if (!tbSphereHitsSphere(mod1.GetCenter(), mod1.GetRadius(), mod2.GetCenter(), mod2.GetRadius()))
		return false;
	
	//cout << mod1.GetBoxMin() << endl << mod1.GetBoxMax() << endl << mod1.GetMatrix() << endl;
	//cout << mod2.GetBoxMin() << endl << mod2.GetBoxMax() << endl << mod2.GetMatrix() << endl;
	
	if (tbBoxHitsBox2(mod1.GetBoxMin(), mod1.GetBoxMax(), mod1.GetMatrix(),
						 mod2.GetBoxMin(), mod2.GetBoxMax(), mod2.GetMatrix()))
		return true;
	return false;
	
}

#endif /*GLCMODEL_H_*/
