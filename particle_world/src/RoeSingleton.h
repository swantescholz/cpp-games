#ifndef ROESINGLETON_H_
#define ROESINGLETON_H_

#include <cassert>

namespace Roe {
	
	// a nice and easy singleton class
	// use the static methods to create/delete on instance or new/delete
	// after deleting the singleton can be created again!
	// if you really want that a instance can just be called via this Singleton class..
	// .. you have to declare all constructors private(or protected) and declare the..
	// .. class Singleton as friend in the class you want to be a Singleton
	template <typename T>
	class Singleton
	{
	protected:
		static T *s_pSingleton; // the static pointer to the single instance
		
	private:
		Singleton(const Singleton<T> &);             //that way these can not be called
		Singleton& operator=(const Singleton<T> &);  //that way these can not be called
		
	public:
		Singleton()
		{
			assert(!s_pSingleton); // m_pSingleton should not already been initialized
			s_pSingleton = static_cast<T*>(this);
		}
		
		virtual ~Singleton()
		{
			s_pSingleton = 0;      // set it back to NULL after deleting
		}
		
		static T* createSingleton() // use this or use the operator new
		{
			if (!s_pSingleton)
				return new T;
			return 0;
		}
		
		static void destroySingleton() // use this or use the operator delete on your pointer to the instance
		{
			if (s_pSingleton)        // only if existing
				delete s_pSingleton; // destructor will set m_pSingleton automatically to NULL
		}
		
		static T& getSingleton()
		{
			assert(s_pSingleton);
			return (*s_pSingleton);
		}
		static T* getSingletonPtr()
		{
			return s_pSingleton;
		}
	};
	template <class T> T* Singleton<T>::s_pSingleton = 0; // initialize singleton pointer with NULL
	
} // namespace Roe

#endif /*ROESINGLETON_H_*/
