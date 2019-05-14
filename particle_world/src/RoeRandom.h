#ifndef ROERANDOM_H_
#define ROERANDOM_H_

#include <ctime>
#include <cstdlib>
//#include <random>
#include "RoeSingleton.h"

#define roe_random Roe::Random::getSingleton()
#define roe_rand_unit (Roe::Random::getSingleton().uniformReal(0.0, 1.0))

namespace Roe {
	
	//class Random, a random number generator
	class Random : public Roe::Singleton<Random>
	{
		friend class Roe::Singleton<Random>;
	public:
		~Random() {}
		/*
		int    nextRand   ()                                       {return rand();}
		int    nextIntFast(int    min=0,    int    max=RAND_MAX)   {return (rand() % (max-min + 1)) + min;}
		int    nextInt    (int    min=0,    int    max=RAND_MAX)   {return (max - min) * (static_cast<long double>(rand()) / RAND_MAX+1.0) + min;}
		float  nextFloat  (float  min=0.0f, float  max=1.0f)       {return (max - min) * (static_cast<float>(rand()) / RAND_MAX) + min;}
		double nextDouble (double min=0.0,  double max=1.0 )       {return (max - min) * (static_cast<double>(rand()) / RAND_MAX) + min;}
		bool   nextBool   ()                                       {return rand() % 2 == 1;}//*/
		int    uniformInt (int    min=0,    int    max=RAND_MAX)   {return (max - min) * (static_cast<long double>(rand()) / RAND_MAX+1.0) + min;}
		float  uniformReal(float  min=0.0f, float  max=1.0f)       {return (max - min) * (static_cast<float>(rand()) / RAND_MAX) + min;}
		double uniformReal(double min=0.0,  double max=1.0 )       {return (max - min) * (static_cast<double>(rand()) / RAND_MAX) + min;}//*/
		double normal(double min,  double max)       {return (max - min) * (static_cast<double>(rand()) / RAND_MAX) + min;}
		
		/*template<typename T> T uniformInt(T min, T max) { //uniform distribution for integers
			return std::uniform_int_distribution<T>(min, max)(engine);
		}
		double uniformReal() { //uniform distribution real 0.0-1.0
			return std::uniform_real_distribution<double>(0.0, 1.0)(engine);
		}
		template<typename T> T uniformReal(T min, T max) { //uniform distribution for reals
			return std::uniform_real_distribution<T>(min, max)(engine);
		}
		template<typename T> T normal(T mean, T sigma) { //normal distribution of gauss
			return std::normal_distribution<T>(mean, sigma)(engine);
		}
		template<typename T> T exponential(T lambda) { //exponention function distribution ( lambda*e^(-lambda*x) )
			return std::exponential_distribution<T>(lambda)(engine);
		}
		
		//mt19937 getEngine() {return engine}//*/
	private:
		Random() {srand(time(NULL));} //initializing random numbers with time
		//Random() : engine(time(NULL))  {srand(time(NULL));} //initializing random numbers with time
		//std::mt19937 engine;
	};
	
	
	/*
	//random number function for different number distributaions
	template<typename T> T randUniformInt(T min, T max) { //uniform distribution for integers
		static mt19937 seeder(time(NULL));
		mt19937 engine(seeder);
		uniform_int<T> dist(min, max);
		variate_generator<mt19937, uniform_int<T>> gen(engine, dist);
		return gen();
	}
	template<typename T> T randUniformReal(T min, T max) { //uniform distribution for reals
		static mt19937 seeder(time(NULL));
		mt19937 engine(seeder);
		uniform_real<T> dist(min, max);
		variate_generator<mt19937, uniform_real<T>> gen(engine, dist);
		return gen();
	}
	template<typename T> T randNormal(T mean, T sigma) { //normal distribution of gauss
		static mt19937 seeder(time(NULL));
		mt19937 engine(seeder);
		normal_distribution<T> dist(mean, sigma);
		variate_generator<mt19937, normal_distribution<T>> gen(engine, dist);
		return gen();
	}
	template<typename T> T randExponential(T lambda) { //exponention function distribution ( lambda*e^(-lambda*x) )
		static mt19937 seeder(time(NULL));
		mt19937 engine(seeder);
		exponential_distribution<T> dist(lambda);
		variate_generator<mt19937, exponential_distribution<T>> gen(engine, dist);
		return gen();
	}//*/
	
	
	
} // namespace Roe

#endif /*ROERANDOM_H_*/
