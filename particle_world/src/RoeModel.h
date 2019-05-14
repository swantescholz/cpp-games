#ifndef ROEMODEL_H_
#define ROEMODEL_H_

#include "RoeVector3.h"
#include "RoeMatrix.h"
#include "RoeCommon.h"
#include <GL/gl.h>
#include <vector>
#include <map>

namespace Roe {

	struct S_ModelIntern {      //only for the Model class
		int iDispList;          //display list index
		Vector3 vFarthest;      //farthest away vertex
		Vector3 vBoxMin;        //bounding-box standard-min
		Vector3 vBoxMax;        //bounding-box standard-max
	};
	struct S_ModelInternOneSurface {    //represents one surface
		int iNumVertices;               //number of vertices
		std::vector<Vector3> vVertices; //vertices
		Vector3 vNormal;                //normal vector
	};
	//lighting + texture2d works now:
	//best automatic tex gen:
	/*
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	float currentCoeff1[4] = {1.0,0.0,1.0,0.0};
	float currentCoeff2[4] = {0.0,1.0,1.0,0.0};
	int currentGenMode = GL_OBJECT_LINEAR;
	int currentPlane = GL_OBJECT_PLANE;
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);
	glTexGenfv(GL_S, currentPlane, currentCoeff1);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, currentGenMode);
	glTexGenfv(GL_T, currentPlane, currentCoeff2);
	*/
	class Model //a class for (OFF) models
	{
		
	public:
		Model();
		Model(const std::string& s, bool bCompleteThePath = true); //calls load
		Model(const Model& rhs);
		virtual ~Model();
		
		Model& operator = (const Model& rhs);
		
		void load(const std::string& s, bool bCompleteThePath = true);   //load the right model
		void render();                     //renders the fitting display list from the map
		void transform(const Matrix& m);   //transforms the object (do not scale here!)
		void   setTransformation(const Matrix& m);   //sets the transformation matrix (no scaling)
		Matrix getTransformation(const Matrix& m);   //get the transformation matrix
		
		std::string getName () const {return m_sName;}
		std::string getPath () const {return m_sPath;}
		//informations about the transformation status
		Vector3   getCenter() const {return m_vCenter;}
		Vector3   getXAxis () const {return m_vXAxis;}
		Vector3   getYAxis () const {return m_vYAxis;}
		Vector3   getZAxis () const {return m_vZAxis;}
		void      setXAxis (const Vector3& v) {Vector3 vTmp = m_vCenter; transform(Matrix::translation(-vTmp)); transform(Matrix::rotationAxis(Vector3::crossProduct( m_vXAxis , v) , Vector3::angleBetween(m_vXAxis, v) ) ); transform(Matrix::translation(vTmp));}
		void      setYAxis (const Vector3& v) {Vector3 vTmp = m_vCenter; transform(Matrix::translation(-vTmp)); transform(Matrix::rotationAxis(Vector3::crossProduct( m_vYAxis , v) , Vector3::angleBetween(m_vYAxis, v) ) ); transform(Matrix::translation(vTmp));}
		void      setZAxis (const Vector3& v) {Vector3 vTmp = m_vCenter; transform(Matrix::translation(-vTmp)); transform(Matrix::rotationAxis(Vector3::crossProduct( m_vZAxis , v) , Vector3::angleBetween(m_vZAxis, v) ) ); transform(Matrix::translation(vTmp));}
		Vector3   getBoxMin() const {return Matrix::transformCoords(s_mpModel[m_sPath].vBoxMin, m_mScale);}
		Vector3   getBoxMax() const {return Matrix::transformCoords(s_mpModel[m_sPath].vBoxMax, m_mScale);}
		float     getRadius() const {return m_fRadius;}
		Matrix    getMatrix() const {return m_mMatrix;}
		void      setMatrix(const Matrix& m) {m_mMatrix = m;}
		
		//scaling
		//if you use scaling and lighting, you should use glEnable(GL_RESCALE_NORMAL) for correct lighting
		void setScale (const Vector3& v) {m_vScale  = v; update();} //standard-scaling
		void multScale(const Vector3& v) {m_vScale *= v; update();}
		Vector3 getScale() {return m_vScale;}
		
		// for the static variables
		static void init(std::string sStart, std::string sEnd)  {s_sPathStart = sStart; s_sPathEnd = sEnd;}
		static void setPathStart(std::string s)                 {s_sPathStart = s;}
		static void setPathEnd  (std::string s)                 {s_sPathEnd   = s;}
		static std::string getPathStart()                       {return s_sPathStart;}
		static std::string getPathEnd  ()                       {return s_sPathEnd;}
		static void quit();
		
		//function(s) functions to load models from files
		//[all models should look in the positive z-axis; with center on zero; length on z-axis=2!]
		//[up vector should be positive y-axis, the surfaces should be CW or CCW respectively]
		static void loadOFF(std::string sPath, bool bCompleteThePath = true, ECWCCW cwccw = CW_CCW,
		                    const float pfSmoothingAngle = 0.0f, const Matrix& mTransform = Matrix::IDENTITY);
		
		static void deleteOFF(std::string sPath, bool bCompleteThePath = true); //deletes one model
		
		//other useful things:
		Model  createTerrain(std::string imagePath, Vector3 vScale = Vector3(1,1,1)); //creates a terrain from an image
		
	protected:
		
		void update(); //updates informations about this model
		
		Matrix m_mMatrix;      //transformation matrix
		std::string m_sPath;   //complete path
		std::string m_sName;   //just the name
		Vector3 m_vScale;      //standard scaling
		Matrix m_mScale;       //scale matrix
		
		//update data
		Vector3 m_vCenter;   //center
		float m_fRadius;     //radius der umgebungskugel
		Vector3 m_vXAxis, m_vYAxis, m_vZAxis;  //local axes
		Vector3 m_vBoxMin, m_vBoxMax;          //scaled bounding box (without other transformations)
		
	private:
		static std::map <std::string, S_ModelIntern>           s_mpModel;   //map with all display lists
		static std::map <std::string, S_ModelIntern>::iterator s_mpi;       //map iterator
		static std::pair<std::string, S_ModelIntern>           s_pair;      //pair object for the map
		static std::string s_sPathStart, s_sPathEnd;                        //standard path
	};
	
} // namespace Roe

#endif /*ROEMODEL_H_*/
