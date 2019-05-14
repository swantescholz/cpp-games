#ifndef ROEKEYFRAMEINTERPOLATOR_H_
#define ROEKEYFRAMEINTERPOLATOR_H_


#include <list>
#include "RoeException.h"
#include "RoeUtil.h"
#include "RoeVector3.h"
#include "RoeColor.h"

namespace Roe {
	
	//NOTE: usefull typedefs at the end of this file
	
	typedef float kfiTime_t; //change this maybe to double
	
	template<typename Type> struct KeyframeL { // keyframe for linear interpolation
		Type        Value;
		kfiTime_t   fTime;
	};
	template<typename Type> struct KeyframeT { // keyframe for hermite or catmull interpolation (with tangent)
		Type        Value;
		Type        Tangent; //1st derivative
		kfiTime_t   fTime;
	};
	template<typename Type> struct KeyframeTN { // keyframe for hermite interpolation(with tangent and 2nd derivative)
		Type        Value;
		Type        Tangent; //1st derivative
		Type        Deriv2; //2nd derivative
		kfiTime_t   fTime;
	};
	
	//an abstract base class for all key frame interpolations
	template<typename Type> class KeyframeInterpolator {
		public:
			virtual void makeConsistentTimeSpacing (bool excludeEnds = true, kfiTime_t fBeginTime = 0.0f, kfiTime_t fEndTime = 1.0f) = 0;
			virtual Type interpolate            (const kfiTime_t fTime) = 0;
			virtual Type interpolateDerivative1 (const kfiTime_t fTime) = 0;
			virtual Type interpolateDerivative2 (const kfiTime_t fTime) = 0;
			virtual kfiTime_t getKeyframeTime   (int index) = 0;
			virtual Type      getKeyframeValue  (int index) = 0;
			virtual int       getNumKeyframes   () const = 0;
			kfiTime_t getMinTime      () const {return m_MinTime;}
			kfiTime_t getMaxTime      () const {return m_MaxTime;}
			kfiTime_t getTimeSpan     () const {return m_MaxTime - m_MinTime;}
		protected:
			kfiTime_t m_MinTime;
			kfiTime_t m_MaxTime;
	};
		
	//a template class for interpolation key-frames linear (vectors, colours and matrices possible)
	template<typename Type> class KeyframeInterpolatorLinear : public KeyframeInterpolator<Type> 
	{
	private:
		std::list<KeyframeL<Type> > m_Keyframes;               //list of key-frames
		typename std::list<KeyframeL<Type> >::iterator m_it1;  //typename was here VERY important!
	public:
		KeyframeInterpolatorLinear()  {}
		KeyframeInterpolatorLinear(const KeyframeInterpolatorLinear<Type> & rhs) {
			for (m_it1 = rhs.m_Keyframes.begin(); m_it1 != rhs.m_Keyframes.end(); ++m_it1) {
				m_Keyframes.push_back((*m_it1));
			}
		}
		~KeyframeInterpolatorLinear() {m_Keyframes.clear();}
		
		void operator = (KeyframeInterpolatorLinear<Type> & rhs) {
			m_Keyframes.clear();
			for (m_it1 = rhs.m_Keyframes.begin(); m_it1 != rhs.m_Keyframes.end(); ++m_it1) {
				m_Keyframes.push_back((*m_it1));
			}
		}
		
		virtual int getNumKeyframes() const {return static_cast<int>(m_Keyframes.size());}
		KeyframeL<Type>* getKeyframe(int index) {
			m_it1 = m_Keyframes.begin();
			for (int i = 0; i < static_cast<int>(m_Keyframes.size()); ++i) {
				if (i == index) return &(*m_it1);
				m_it1++;
			}
			return NULL;
		}
		
		void addKeyframe(const Type& Value) { // without implicit time, call makeConsistentSpacing() after
			if (m_Keyframes.empty() == false)
				addKeyframe(m_Keyframes.back().fTime + ROE_REAL_TOLERANCE, Value); // move a bit in time
			else
				addKeyframe(0.0, Value); // first, so put to start
		}
		void addKeyframe(const kfiTime_t fTime, const Type& Value) {
			KeyframeL<Type> keyFrame;
			keyFrame.Value = Value;
			keyFrame.fTime = fTime;
			 
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime > keyFrame.fTime) {
					break;
				}
			}
			if (m_it1 != m_Keyframes.end()) {
				m_Keyframes.insert(m_it1,keyFrame); //insert keyFrame at the right time
			} else {
				m_Keyframes.push_back(keyFrame); //insert keyFrame at the back
			}
			
			//updating min and max times
			KeyframeInterpolator<Type>::m_MinTime = m_Keyframes.front().fTime;
			KeyframeInterpolator<Type>::m_MaxTime = m_Keyframes.back ().fTime;
		}
		
		//adjusts the time spacing consistend
		virtual void makeConsistentTimeSpacing(bool excludeEnds = true, kfiTime_t fBeginTime = 0.0f, kfiTime_t fEndTime = 1.0f) {
			if (fBeginTime > fEndTime) Util::swap(fBeginTime, fEndTime);
			
			//updating min and max times
			KeyframeInterpolator<Type>::m_MinTime = fBeginTime;
			KeyframeInterpolator<Type>::m_MaxTime = fEndTime;
			
			kfiTime_t difference    = fEndTime - fBeginTime;
			kfiTime_t part;
			if (excludeEnds == true) {
				part            = difference / (getNumKeyframes() - 3); // the part must here be ... / frames-1 - 2
				fBeginTime     -= part;
			} else {
				part            = difference / (getNumKeyframes() - 1); // the part must here be ... / frames-1
			}
			kfiTime_t time = fBeginTime;
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				m_it1->fTime = time;
				time += part;
			}
		}
		
		KeyframeL<Type>* findKeyframeBefore(const kfiTime_t fTime) {
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime >= fTime) {
					if (m_it1 == m_Keyframes.begin()) {
						if (m_it1->fTime == fTime)
							return &(*m_it1);
						else return NULL;
					}
					--m_it1;
					return &(*m_it1);
				}
			}
			return NULL; //no such keyFrame found
		}
		KeyframeL<Type>* findKeyframeAfter(const kfiTime_t fTime) {
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime >= fTime) {
					if (m_it1 == m_Keyframes.begin()) {
						if (m_it1->fTime == fTime) {
							++m_it1;
							return &(*m_it1);
						}
						else return NULL;
					}
					return &(*m_it1);
				}
			}
			return NULL; //no such keyFrame found
		}
		
		virtual Type interpolate(const kfiTime_t fTime) {
			
			KeyframeL<Type>* pBefore = findKeyframeBefore(fTime);
			KeyframeL<Type>* pAfter  = findKeyframeAfter (fTime);
			
			/*if (pBefore == NULL || pAfter == NULL) {
				throw Exception("not ok fTime(" +Util::toString(fTime)+")",
				                "interpolate", "RoeKeyframeInterpolator.h", ROE_LINE);
			}//*/
			
			//doing the interpolatoin
			return Util::interpolateLinear<Type>(pBefore->Value, pAfter->Value, (fTime - pBefore->fTime) / (pAfter->fTime - pBefore->fTime));
		}
		virtual Type interpolateDerivative1(const kfiTime_t fTime) {
			if (fTime+ROE_REAL_TOLERANCE > KeyframeInterpolator<Type>::m_MaxTime)
				return interpolate(fTime) - interpolate(fTime-ROE_REAL_TOLERANCE);
			if (fTime-ROE_REAL_TOLERANCE < KeyframeInterpolator<Type>::m_MinTime)
				return interpolate(fTime+ROE_REAL_TOLERANCE) - interpolate(fTime);
			return interpolate(fTime+ROE_REAL_TOLERANCE) - interpolate(fTime-ROE_REAL_TOLERANCE);
		}
		virtual Type interpolateDerivative2(const kfiTime_t fTime) {
			Type tmp = interpolate(KeyframeInterpolator<Type>::m_MinTime); return (tmp-tmp);}
		virtual kfiTime_t getKeyframeTime (int index) {return getKeyframe(index)->fTime;}
		virtual Type      getKeyframeValue(int index) {return getKeyframe(index)->Value;}
	};
	
	
	//a template class for interpolation key-frames hermite / catmull (vectors, colours and matrices possible)
	template<typename Type> class KeyframeInterpolatorHermite : public KeyframeInterpolator<Type> 
	{
	private:
		std::list<KeyframeT<Type> > m_Keyframes;               //list of key-frames
		typename std::list<KeyframeT<Type> >::iterator m_it1;  //typename was here VERY important!
	public:
		KeyframeInterpolatorHermite()  {}
		KeyframeInterpolatorHermite(const KeyframeInterpolatorHermite<Type> & rhs) {
			for (m_it1 = rhs.m_Keyframes.begin(); m_it1 != rhs.m_Keyframes.end(); ++m_it1) {
				m_Keyframes.push_back((*m_it1));
			}
		}
		~KeyframeInterpolatorHermite() {m_Keyframes.clear();}
		
		void operator = (KeyframeInterpolatorHermite<Type> & rhs) {
			m_Keyframes.clear();
			for (m_it1 = rhs.m_Keyframes.begin(); m_it1 != rhs.m_Keyframes.end(); ++m_it1) {
				m_Keyframes.push_back((*m_it1));
			}
		}
		
		virtual int getNumKeyframes() const {return static_cast<int>(m_Keyframes.size());}
		KeyframeT<Type>* getKeyframe(int index) {
			m_it1 = m_Keyframes.begin();
			for (int i = 0; i < static_cast<int>(m_Keyframes.size()); ++i) {
				if (i == index) return &(*m_it1);
				m_it1++;
			}
			return NULL;
		}
		
		void addKeyframe(const Type& Value) { // without implicit time, call makeConsistentSpacing() after
			if (m_Keyframes.empty() == false)
				addKeyframe(m_Keyframes.back().fTime + ROE_REAL_TOLERANCE, Value); // move a bit in time
			else
				addKeyframe(0.0, Value); // first, so put to start
		}
		void addKeyframe(const Type& Value, const Type& Tangent) { // without implicit time, call makeConsistentSpacing() after
			if (m_Keyframes.empty() == false)
				addKeyframe(m_Keyframes.back().fTime + ROE_REAL_TOLERANCE, Value, Tangent);  // move a bit in time
			else
				addKeyframe(0.0, Value, Tangent); // first, so put to start
		}
		void addKeyframe(const kfiTime_t fTime, const Type& Value) {
			KeyframeT<Type> keyFrame;
			keyFrame.Value   = Value;
			keyFrame.Tangent = Value-Value;
			keyFrame.fTime   = fTime;
			 
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime > keyFrame.fTime) {
					break;
				}
			}
			if (m_it1 != m_Keyframes.end()) {
				m_Keyframes.insert(m_it1,keyFrame); //insert keyFrame at the right time
			} else {
				m_Keyframes.push_back(keyFrame);    //insert keyFrame at the back
			}
			
			//updating min and max times
			KeyframeInterpolator<Type>::m_MinTime = m_Keyframes.front().fTime;
			KeyframeInterpolator<Type>::m_MaxTime = m_Keyframes.back ().fTime;
		}
		void addKeyframe(const kfiTime_t fTime, const Type& Value, const Type& Tangent) {
			KeyframeT<Type> keyFrame;
			keyFrame.Value   = Value;
			keyFrame.Tangent = Tangent;
			keyFrame.fTime   = fTime;
			 
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime > keyFrame.fTime) {
					break;
				}
			}
			if (m_it1 != m_Keyframes.end()) {
				m_Keyframes.insert(m_it1,keyFrame); //insert keyFrame at the right time
			} else {
				m_Keyframes.push_back(keyFrame); //insert keyFrame at the back
			}
			
			//updating min and max times
			KeyframeInterpolator<Type>::m_MinTime = m_Keyframes.front().fTime;
			KeyframeInterpolator<Type>::m_MaxTime = m_Keyframes.back ().fTime;
		}
		
		void calculateTangentCatmull(int index, kfiTime_t factor = 0.5) {
			if (index < 0 || index > getNumKeyframes() -1) return;
			if (index == 0) { // start
				m_it1 = m_Keyframes.begin();
				m_it1++;
				KeyframeT<Type> next = (*m_it1);
				m_it1--;
				m_it1->Tangent = (next.Tangent - m_it1->Tangent) * factor;
			} else if (index == getNumKeyframes() - 1) { // end
				m_it1 = m_Keyframes.end();
				m_it1--;
				m_it1--;
				KeyframeT<Type> last = (*m_it1);
				m_it1++;
				m_it1->Tangent = (m_it1->Value - last.Value) * factor;
			} else { // middle
				int i;
				for (m_it1 = m_Keyframes.begin(), i = 0; i < index; ++m_it1, ++i) {}
				m_it1--;
				KeyframeT<Type> last = (*m_it1);
				m_it1++;
				m_it1++;
				KeyframeT<Type> next = (*m_it1);
				m_it1--;
				m_it1->Tangent = (next.Value - last.Value) * factor;
			}
		}
		void calculateAllTangentsCatmull(kfiTime_t factor = 0.5) { // computes all tangents using the catmull-rom algorithm
			KeyframeT<Type> last;
			KeyframeT<Type> next;
			m_it1 = m_Keyframes.begin();
			m_it1++;
			next = (*m_it1);
			m_it1--;
			m_it1->Tangent = (next.Value - m_it1->Value) * 2.0f * factor;
			last = (*m_it1);
			for (; m_it1 != m_Keyframes.end(); ++m_it1) {
				m_it1++;
				if (m_it1 == m_Keyframes.end()) {
					m_it1--;
					m_it1->Tangent = (m_it1->Value - last.Value) * 2.0f * factor;
					break;
				}
				next = (*m_it1);
				m_it1--;
				m_it1->Tangent = (next.Value - last.Value) * factor;
				last = (*m_it1);
			}
		}
		
		//adjusts the time spacing consistend
		virtual void makeConsistentTimeSpacing(bool excludeEnds = true, kfiTime_t fBeginTime = 0.0f, kfiTime_t fEndTime = 1.0f) {
			if (fBeginTime > fEndTime) Util::swap(fBeginTime, fEndTime);
			
			//updating min and max times
			KeyframeInterpolator<Type>::m_MinTime = fBeginTime;
			KeyframeInterpolator<Type>::m_MaxTime = fEndTime;
			
			kfiTime_t difference   = fEndTime - fBeginTime;
			kfiTime_t part;
			if (excludeEnds == true) {
				part             = difference / (getNumKeyframes() - 3); // the part must here be ... / frames-1 - 2
				fBeginTime      -= part;
			} else {
				part             = difference / (getNumKeyframes() - 1); // the part must here be ... / frames-1
			}
			kfiTime_t time = fBeginTime;
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				m_it1->fTime = time;
				time += part;
			}
		}
		
		
		KeyframeT<Type>* findKeyframeBefore(const kfiTime_t fTime) {
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime >= fTime) {
					if (m_it1 == m_Keyframes.begin()) {
						if (m_it1->fTime == fTime)
							return &(*m_it1);
						else return NULL;
					}
					--m_it1;
					return &(*m_it1);
				}
			}
			return NULL; //no such keyFrame found
		}
		KeyframeT<Type>* findKeyframeAfter(const kfiTime_t fTime) {
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime >= fTime) {
					if (m_it1 == m_Keyframes.begin()) {
						if (m_it1->fTime == fTime) {
							++m_it1;
							return &(*m_it1);
						}
						else return NULL;
					}
					return &(*m_it1);
				}
			}
			return NULL; //no such keyFrame found
		}
		
		virtual Type interpolate(const kfiTime_t fTime) {
			KeyframeT<Type>* pBefore = findKeyframeBefore(fTime);
			KeyframeT<Type>* pAfter  = findKeyframeAfter (fTime);
			
			/*if (pBefore == NULL || pAfter == NULL) {
				throw Exception("not ok fTime(" +Util::toString(fTime)+")",
				                "interpolate", "RoeKeyframeInterpolator.h", ROE_LINE);
			}//*/
			
			//calculating interpolation factor (0 -> Keyframe before, 1 -> Keyframe after)
			const float t = (fTime - pBefore->fTime) / (pAfter->fTime - pBefore->fTime);
			
			//doing the interpolatoin
			return Util::interpolateHermite<Type>(pBefore->Value, pBefore->Tangent, pAfter->Value, pAfter->Tangent, t);
		}
		virtual Type interpolateDerivative1(const kfiTime_t fTime) {
			KeyframeT<Type>* pBefore = findKeyframeBefore(fTime);
			KeyframeT<Type>* pAfter  = findKeyframeAfter (fTime);
			//calculating interpolation factor (0 -> Keyframe before, 1 -> Keyframe after)
			const float t = (fTime - pBefore->fTime) / (pAfter->fTime - pBefore->fTime);
			return Util::interpolateHermiteDerivative1<Type>(pBefore->Value, pBefore->Tangent, pAfter->Value, pAfter->Tangent, t);
		}
		virtual Type interpolateDerivative2(const kfiTime_t fTime) {
			KeyframeT<Type>* pBefore = findKeyframeBefore(fTime);
			KeyframeT<Type>* pAfter  = findKeyframeAfter (fTime);
			//calculating interpolation factor (0 -> Keyframe before, 1 -> Keyframe after)
			const float t = (fTime - pBefore->fTime) / (pAfter->fTime - pBefore->fTime);
			return Util::interpolateHermiteDerivative2<Type>(pBefore->Value, pBefore->Tangent, pAfter->Value, pAfter->Tangent, t);
		}
		virtual kfiTime_t getKeyframeTime   (int index) {return getKeyframe(index)->fTime;}
		virtual Type      getKeyframeValue  (int index) {return getKeyframe(index)->Value;}
		virtual Type      getKeyframeTangent(int index) {return getKeyframe(index)->Tangent;}
	};
	
	//a template class for interpolation key-frames hermite / catmull (vectors, colours and matrices possible)
	template<typename Type> class KeyframeInterpolatorHermite5 : public KeyframeInterpolator<Type> 
	{
	private:
		std::list<KeyframeTN<Type> > m_Keyframes;               //list of key-frames
		typename std::list<KeyframeTN<Type> >::iterator m_it1;  //typename was here VERY important!
	public:
		KeyframeInterpolatorHermite5()  {}
		KeyframeInterpolatorHermite5(const KeyframeInterpolatorHermite5<Type> & rhs) {
			for (m_it1 = rhs.m_Keyframes.begin(); m_it1 != rhs.m_Keyframes.end(); ++m_it1) {
				m_Keyframes.push_back((*m_it1));
			}
		}
		~KeyframeInterpolatorHermite5() {m_Keyframes.clear();}
		
		void operator = (KeyframeInterpolatorHermite5<Type> & rhs) {
			m_Keyframes.clear();
			for (m_it1 = rhs.m_Keyframes.begin(); m_it1 != rhs.m_Keyframes.end(); ++m_it1) {
				m_Keyframes.push_back((*m_it1));
			}
		}
		
		virtual int getNumKeyframes() const {return static_cast<int>(m_Keyframes.size());}
		KeyframeTN<Type>* getKeyframe(int index) {
			m_it1 = m_Keyframes.begin();
			for (int i = 0; i < static_cast<int>(m_Keyframes.size()); ++i) {
				if (i == index) return &(*m_it1);
				m_it1++;
			}
			return NULL;
		}
		
		void addKeyframe(const Type& Value) { // without implicit time, call makeConsistentTimeSpacing() after
			if (m_Keyframes.empty() == false)
				addKeyframe(m_Keyframes.back().fTime + ROE_REAL_TOLERANCE, Value); // move a bit in time
			else
				addKeyframe(0.0, Value); // first, so put to start
		}
		void addKeyframe(const Type& Value, const Type& Tangent) { // without implicit time, call makeConsistentTimeSpacing() after
			if (m_Keyframes.empty() == false)
				addKeyframe(m_Keyframes.back().fTime + ROE_REAL_TOLERANCE, Value, Tangent);  // move a bit in time
			else
				addKeyframe(0.0, Value, Tangent); // first, so put to start
		}
		void addKeyframe(const Type& Value, const Type& Tangent, const Type& Deriv2) { // without implicit time, call makeConsistentTimeSpacing() after
			if (m_Keyframes.empty() == false)
				addKeyframe(m_Keyframes.back().fTime + ROE_REAL_TOLERANCE, Value, Tangent, Deriv2);  // move a bit in time
			else
				addKeyframe(0.0, Value, Tangent, Deriv2); // first, so put to start
		}
		void addKeyframe(const kfiTime_t fTime, const Type& Value) {
			KeyframeTN<Type> keyFrame;
			keyFrame.Value   = Value;
			keyFrame.Tangent = Value-Value; //just to shut up warnings
			keyFrame.Deriv2  = Value-Value; //just to shut up warnings
			keyFrame.fTime   = fTime;
			 
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime > keyFrame.fTime) {
					break;
				}
			}
			if (m_it1 != m_Keyframes.end()) {
				m_Keyframes.insert(m_it1,keyFrame); //insert keyFrame at the right time
			} else {
				m_Keyframes.push_back(keyFrame);    //insert keyFrame at the back
			}
			
			//updating min and max times
			KeyframeInterpolator<Type>::m_MinTime = m_Keyframes.front().fTime;
			KeyframeInterpolator<Type>::m_MaxTime = m_Keyframes.back ().fTime;
		}
		void addKeyframe(const kfiTime_t fTime, const Type& Value, const Type& Tangent) {
			KeyframeTN<Type> keyFrame;
			keyFrame.Value   = Value;
			keyFrame.Tangent = Tangent;
			keyFrame.Deriv2  = Tangent-Tangent; //just to shut up warnings
			keyFrame.fTime   = fTime;
			 
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime > keyFrame.fTime) {
					break;
				}
			}
			if (m_it1 != m_Keyframes.end()) {
				m_Keyframes.insert(m_it1,keyFrame); //insert keyFrame at the right time
			} else {
				m_Keyframes.push_back(keyFrame); //insert keyFrame at the back
			}
			
			//updating min and max times
			KeyframeInterpolator<Type>::m_MinTime = m_Keyframes.front().fTime;
			KeyframeInterpolator<Type>::m_MaxTime = m_Keyframes.back ().fTime;
		}
		void addKeyframe(const kfiTime_t fTime, const Type& Value, const Type& Tangent, const Type& Deriv2) {
			KeyframeTN<Type> keyFrame;
			keyFrame.Value   = Value;
			keyFrame.Tangent = Tangent;
			keyFrame.Deriv2  = Deriv2;
			keyFrame.fTime   = fTime;
			 
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime > keyFrame.fTime) {
					break;
				}
			}
			if (m_it1 != m_Keyframes.end()) {
				m_Keyframes.insert(m_it1,keyFrame); //insert keyFrame at the right time
			} else {
				m_Keyframes.push_back(keyFrame); //insert keyFrame at the back
			}
			
			//updating min and max times
			KeyframeInterpolator<Type>::m_MinTime = m_Keyframes.front().fTime;
			KeyframeInterpolator<Type>::m_MaxTime = m_Keyframes.back ().fTime;
		}
		
		void calculateTangentCatmull(int index, kfiTime_t factor = 0.5) {
			if (index < 0 || index > getNumKeyframes() -1) return;
			if (index == 0) { // start
				m_it1 = m_Keyframes.begin();
				m_it1++;
				KeyframeT<Type> next = (*m_it1);
				m_it1--;
				m_it1->Tangent = (next.Tangent - m_it1->Tangent) * factor;
			} else if (index == getNumKeyframes() - 1) { // end
				m_it1 = m_Keyframes.end();
				m_it1--;
				m_it1--;
				KeyframeT<Type> last = (*m_it1);
				m_it1++;
				m_it1->Tangent = (m_it1->Value - last.Value) * factor;
			} else { // middle
				int i;
				for (m_it1 = m_Keyframes.begin(), i = 0; i < index; ++m_it1, ++i) {}
				m_it1--;
				KeyframeT<Type> last = (*m_it1);
				m_it1++;
				m_it1++;
				KeyframeT<Type> next = (*m_it1);
				m_it1--;
				m_it1->Tangent = (next.Value - last.Value) * factor;
			}
		}
		void calculateAllTangentsCatmull(kfiTime_t factor = 0.5) { // computes all tangents using the catmull-rom algorithm
			KeyframeTN<Type> last;
			KeyframeTN<Type> next;
			m_it1 = m_Keyframes.begin();
			m_it1++;
			next = (*m_it1);
			m_it1--;
			m_it1->Tangent = (next.Value - m_it1->Value) * 2.0f * factor;
			last = (*m_it1);
			for (; m_it1 != m_Keyframes.end(); ++m_it1) {
				m_it1++;
				if (m_it1 == m_Keyframes.end()) {
					m_it1--;
					m_it1->Tangent = (m_it1->Value - last.Value) * 2.0f * factor;
					break;
				}
				next = (*m_it1);
				m_it1--;
				m_it1->Tangent = (next.Value - last.Value) * factor;
				last = (*m_it1);
			}
		}
		void calculateAllDeriv2(kfiTime_t factor = 0.5) { // computes all 2nd derivs AFTER tangents were calculated!
			KeyframeTN<Type> last;
			KeyframeTN<Type> next;
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1 == m_Keyframes.begin()) {//first
					m_it1++;
					next = (*m_it1);
					m_it1--;
					m_it1->Deriv2 = factor*2.0f*
					  Util::interpolateHermiteDerivative2(m_it1->Value,m_it1->Tangent,next.Value,next.Tangent,0.0f);
					last = (*m_it1);
					continue;
				}
				m_it1++;
				if (m_it1 == m_Keyframes.end()) {//last
					m_it1--;
					m_it1->Deriv2 = factor*2.0f*
					  Util::interpolateHermiteDerivative2(last.Value,last.Tangent,m_it1->Value,m_it1->Tangent,1.0f);
					break;
				}
				next = (*m_it1);
				m_it1--;
				m_it1->Deriv2 = factor*
					  (Util::interpolateHermiteDerivative2(last.Value,last.Tangent,m_it1->Value,m_it1->Tangent,1.0f) +
					   Util::interpolateHermiteDerivative2(m_it1->Value,m_it1->Tangent,next.Value,next.Tangent,0.0f));
				last = (*m_it1);
			}
		}
		//adjusts the time spacing consistend
		virtual void makeConsistentTimeSpacing(bool excludeEnds = true, kfiTime_t fBeginTime = 0.0f, kfiTime_t fEndTime = 1.0f) {
			if (fBeginTime > fEndTime) Util::swap(fBeginTime, fEndTime);
			
			//updating min and max times
			KeyframeInterpolator<Type>::m_MinTime = fBeginTime;
			KeyframeInterpolator<Type>::m_MaxTime = fEndTime;
			
			kfiTime_t difference   = fEndTime - fBeginTime;
			kfiTime_t part;
			if (excludeEnds == true) {
				part             = difference / (getNumKeyframes() - 3); // the part must here be ... / frames-1 - 2
				fBeginTime      -= part;
			} else {
				part             = difference / (getNumKeyframes() - 1); // the part must here be ... / frames-1
			}
			kfiTime_t time = fBeginTime;
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				m_it1->fTime = time;
				time += part;
			}
		}
		
		
		KeyframeTN<Type>* findKeyframeBefore(const kfiTime_t fTime) {
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime >= fTime) {
					if (m_it1 == m_Keyframes.begin()) {
						if (m_it1->fTime == fTime)
							return &(*m_it1);
						else return NULL;
					}
					--m_it1;
					return &(*m_it1);
				}
			}
			return NULL; //no such keyFrame found
		}
		KeyframeTN<Type>* findKeyframeAfter(const kfiTime_t fTime) {
			for (m_it1 = m_Keyframes.begin(); m_it1 != m_Keyframes.end(); ++m_it1) {
				if (m_it1->fTime >= fTime) {
					if (m_it1 == m_Keyframes.begin()) {
						if (m_it1->fTime == fTime) {
							++m_it1;
							return &(*m_it1);
						}
						else return NULL;
					}
					return &(*m_it1);
				}
			}
			return NULL; //no such keyFrame found
		}
		
		virtual Type interpolate(const kfiTime_t fTime) {
			KeyframeTN<Type>* pBefore = findKeyframeBefore(fTime);
			KeyframeTN<Type>* pAfter  = findKeyframeAfter (fTime);
			
			
			/*if (pBefore == NULL || pAfter == NULL) {
				throw Exception("not ok fTime(" +Util::toString(fTime)+")",
				                "interpolate", "RoeKeyframeInterpolator.h", ROE_LINE);
			}//*/
			
			//calculating interpolation factor (0 -> Keyframe before, 1 -> Keyframe after)
			const float t = (fTime - pBefore->fTime) / (pAfter->fTime - pBefore->fTime);
			
			//doing the interpolatoin
			return Util::interpolateHermite5<Type>(pBefore->Value, pBefore->Tangent, pBefore->Deriv2,
			  pAfter->Value, pAfter->Tangent, pAfter->Deriv2, t);
		}
		virtual Type interpolateDerivative1(const kfiTime_t fTime) {
			KeyframeTN<Type>* pBefore = findKeyframeBefore(fTime);
			KeyframeTN<Type>* pAfter  = findKeyframeAfter (fTime);
			//calculating interpolation factor (0 -> Keyframe before, 1 -> Keyframe after)
			const float t = (fTime - pBefore->fTime) / (pAfter->fTime - pBefore->fTime);
			return Util::interpolateHermite5Derivative1<Type>(pBefore->Value, pBefore->Tangent, pBefore->Deriv2,
			  pAfter->Value, pAfter->Tangent, pAfter->Deriv2, t);
		}
		virtual Type interpolateDerivative2(const kfiTime_t fTime) {
			KeyframeTN<Type>* pBefore = findKeyframeBefore(fTime);
			KeyframeTN<Type>* pAfter  = findKeyframeAfter (fTime);
			//calculating interpolation factor (0 -> Keyframe before, 1 -> Keyframe after)
			const float t = (fTime - pBefore->fTime) / (pAfter->fTime - pBefore->fTime);
			return Util::interpolateHermite5Derivative2<Type>(pBefore->Value, pBefore->Tangent, pBefore->Deriv2,
			  pAfter->Value, pAfter->Tangent, pAfter->Deriv2, t);
		}
		virtual kfiTime_t getKeyframeTime   (int index) {return getKeyframe(index)->fTime;}
		virtual Type      getKeyframeValue  (int index) {return getKeyframe(index)->Value;}
		virtual Type      getKeyframeTangent(int index) {return getKeyframe(index)->Tangent;}
	};
	
	
	//simplified typdefs for splines(Vector3)
	typedef KeyframeInterpolator        <Vector3> BasicSpline;
	typedef KeyframeInterpolatorLinear  <Vector3> LinearSpline;
	typedef KeyframeInterpolatorHermite <Vector3> HermiteSpline;
	typedef KeyframeInterpolatorHermite5<Vector3> Hermite5Spline;
	//simplified typdefs for color(Color) interpolations
	typedef KeyframeInterpolator        <Color> BasicColorInterpolator;
	typedef KeyframeInterpolatorLinear  <Color> LinearColorInterpolator;
	typedef KeyframeInterpolatorHermite <Color> HermiteColorInterpolator;
	typedef KeyframeInterpolatorHermite5<Color> Hermite5ColorInterpolator;
	
} //namespace Roe

#endif /*ROEFRAMEINTERPOLATOR_H_*/

