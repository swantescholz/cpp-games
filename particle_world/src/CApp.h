#ifndef CAPP_H_
#define CAPP_H_


#include "Roe.h"
#include "CSurface.h"
#include <iostream>

using namespace std;
using namespace Roe;

class CApp : public Application
{
public:
	CApp();
	~CApp();
	
	virtual void run();
	void loadInputFile(string sFilename);
	void loadWorldFile(string sFilename);
	ForceField* loadForceField(vector<string> vsText);
	Matrix loadMatrix(vector<string> vsText);
	void loadDirection(vector<string> vsText, ForceField *ff);
	void loadStrength (vector<string> vsText, ForceField *ff);
private:
	void drawArrowVector(Vector3 pos, Vector3 dir, Color col, Vector3 scale = Vector3(1,1,1));
	void drawBox(Vector3 pos, float size);
	void drawSphere(Vector3 pos, float r);
	void drawSphere(const Matrix& m);
	void drawForceField(const ForceField& s);
	void drawModel(string name, const Matrix& m);
	
	int   m_iNumParticles;
	float m_fCamSpeed, m_fCamRotSpeed;
	bool  m_bShow2DText;
	float m_fCoordSystemArrowSize;
	float m_fFPS;
	Vector3 m_vGravity;
	float m_fMinFPS;
	
	Particle **m_pParticles;
	ForceFieldGroup m_ffg;
};



#endif /*CAPP_H_*/
