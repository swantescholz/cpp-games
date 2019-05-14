#include "RoeTimer.h"
#include <iostream>

namespace Roe {
	
	std::list<Timer*>   Timer::s_lpTimer;
	Timer::type_t       Timer::s_Elapsed  = 0.0;
	Timer::duration_t   Timer::s_Duration;
	Timer::time_point_t Timer::s_LastTimePoint;
	Timer::time_point_t Timer::s_FirstTimePoint;
	
	Timer::Timer(type_t pLimit)
	{
		m_Time    = 0.0;
		m_Limit   = pLimit;
		m_bPaused = false;
		s_lpTimer.push_back(this);
	}
	Timer::Timer(const Timer & rhs)
	{
		m_Time    = rhs.m_Time;
		m_Limit   = rhs.m_Limit;
		m_bPaused = rhs.m_bPaused;
		s_lpTimer.push_back(this);
	}
	Timer::~Timer()
	{
		//erasing this timer from static list
		for (std::list<Timer*>::iterator it = s_lpTimer.begin(); it != s_lpTimer.end(); it++) {
			if ( (*it) == this) { // its me!
				s_lpTimer.erase(it);
				it = s_lpTimer.begin();      //important!
				if (it == s_lpTimer.end()) {
					break;
				}
			}
		}
	}
	void Timer::operator = (const Timer & rhs) {
		m_Time    = rhs.m_Time;
		m_Limit   = rhs.m_Limit;
		m_bPaused = rhs.m_bPaused;
	}
	
	bool Timer::expired () {return m_Time >= m_Limit;}
	void Timer::start   () {m_bPaused = false;}
	void Timer::pause   () {m_bPaused = true;}
	void Timer::resume  () {m_bPaused = false;}
	void Timer::reset   () {m_Time = 0.0;}
	void Timer::restart () {m_Time = 0.0; m_bPaused = false;}
	void Timer::setTime  (type_t time )   {m_Time  = time;}
	void Timer::setLimit (type_t limit)   {m_Limit = limit;}
	void Timer::update   (type_t elapsed) {
		if (m_bPaused == false) {
			m_Time += elapsed;
		}
	}
	
	// *static*
	void Timer::init() {
		s_FirstTimePoint = std::chrono::monotonic_clock::now();
		s_LastTimePoint  = std::chrono::monotonic_clock::now();
	}
	void Timer::updateAll () {
		time_point_t newTimePoint = std::chrono::monotonic_clock::now();
		s_Duration = newTimePoint - s_LastTimePoint;
		updateAll(std::chrono::duration<type_t>(s_Duration).count());
		s_LastTimePoint = newTimePoint;
	}
	void Timer::updateAll (type_t elapsed) {
		for (std::list<Timer*>::iterator it = s_lpTimer.begin(); it != s_lpTimer.end(); it++) {
			(*it)->update(elapsed);
		}
		s_Elapsed = elapsed;
	}
	Timer::type_t Timer::getTimeSinceInit() {
		return std::chrono::duration<type_t>(std::chrono::monotonic_clock::now() - s_FirstTimePoint).count();
	}
	
} // namespace Roe

