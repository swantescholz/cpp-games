#ifndef GLCKEYFRAMEINTERPOLATOR_H_
#define GLCKEYFRAMEINTERPOLATOR_H_

#include "glcSDL_GL.h"
#include "glcMaterial.h"

template<typename Type> struct S_KeyFrame {
	Type Value;
	//Type Tangent;
	float fTime;
};

//a template class for interpolation key-frames (tbVector2,tbVector3,tbColor,tbMatrix possible)
template<typename Type> class glcKeyFrameInterpolator
{
public:
	glcKeyFrameInterpolator()  {}
	glcKeyFrameInterpolator(const glcKeyFrameInterpolator<Type> & rhs) {
		for (m_it1 = rhs.m_KeyFrames.begin(); m_it1 != rhs.m_KeyFrames.end(); m_it1++) {
			m_KeyFrames.push_back((*m_it1));
		}
	}
	~glcKeyFrameInterpolator() {m_KeyFrames.clear();}
	
	void operator = (glcKeyFrameInterpolator<Type> & rhs) {
		m_KeyFrames.clear();
		for (m_it1 = rhs.m_KeyFrames.begin(); m_it1 != rhs.m_KeyFrames.end(); m_it1++) {
			m_KeyFrames.push_back((*m_it1));
		}
	}
	
	int GetNumFrames() {return static_cast<int>(m_KeyFrames.size());}
	
	void AddKeyFrame(const Type& Value, const float fTime) {
		S_KeyFrame<Type> keyFrame;
		keyFrame.Value = Value;
		keyFrame.fTime = fTime;
		 
		for (m_it1 = m_KeyFrames.begin(); m_it1 != m_KeyFrames.end(); m_it1++) {
			if (m_it1->fTime > keyFrame.fTime) {
				break;
			}
		}
		if (m_it1 != m_KeyFrames.end()) {
			m_KeyFrames.insert(m_it1,keyFrame); //insert keyFrame at the right time
		} else {
			m_KeyFrames.push_back(keyFrame); //insert keyFrame at the back
		}
	}
	
	S_KeyFrame<Type>* FindKeyFrameBefore(const float fTime) {
		for (m_it1 = m_KeyFrames.begin(); m_it1 != m_KeyFrames.end(); m_it1++) {
			if (m_it1->fTime >= fTime && m_it1 != m_KeyFrames.begin()) {
				m_it1--;
				return &(*m_it1);
			}
		}
		return NULL; //no such keyFrame found
	}
	S_KeyFrame<Type>* FindKeyFrameAfter(const float fTime) {
		for (m_it1 = m_KeyFrames.begin(); m_it1 != m_KeyFrames.end(); m_it1++) {
			if (m_it1->fTime >= fTime && m_it1 != m_KeyFrames.begin()) {
				return &(*m_it1);
			}
		}
		return NULL; //no such keyFrame found
	}
	
	Type Interpolate(const float fTime) {
		S_KeyFrame<Type>* pBefore = FindKeyFrameBefore(fTime);
		S_KeyFrame<Type>* pAfter  = FindKeyFrameAfter (fTime);
		
		/*
		if (pBefore == NULL || pAfter == NULL) {
			throw glcException("not ok fTime in interpolate of glcKeyFrameInterpolator");
		}
		*/
		//calculating interpolation factor (0 -> KeyFrame before, 1 -> KeyFrame after)
		//const float fInterpolation = (fTime - pBefore->fTime) / (pAfter->fTime - pBefore->fTime);
		
		//doing the interpolatoin
		return tbInterpolateLinear(pBefore->Value, pAfter->Value, (fTime - pBefore->fTime) / (pAfter->fTime - pBefore->fTime));
	}
	
private:
	list<S_KeyFrame<Type> > m_KeyFrames; 		//list of key-frames
	typename list<S_KeyFrame<Type> >::iterator m_it1; 	//typename was here VERY important!
};

/*//THAT WAS BEFORE IN CPP remember!
//that one was VERY complicated and important; the following lines are needed for right template-linking
//template glcMaterial glcKeyFrameInterpolator<glcMaterial>::Interpolate(const float fTime); //actually not needed
//template tbColor     glcKeyFrameInterpolator<tbColor    >::Interpolate(const float fTime); //actually not needed
template glcKeyFrameInterpolator<glcMaterial>; //this is simpler
template glcKeyFrameInterpolator<tbColor>;     //this is simpler
template glcKeyFrameInterpolator<tbMatrix>;    //this is simpler
template glcKeyFrameInterpolator<tbVector2>;   //this is simpler
template glcKeyFrameInterpolator<tbVector3>;   //this is simpler
template glcKeyFrameInterpolator<float>;   //this is simpler
*/

#endif /*GLCKEYFRAMEINTERPOLATOR_H_*/

