/*
===============================================================================

	XOF
	===
	File	:	XOF_Timer_Window.hpp
	Desc	:	Windows timer.

===============================================================================
*/
#ifndef XOF_TIMER_WINDOWS_HPP
#define XOF_TIMER_WINDOWS_HPP


#include "XOF_Platform.hpp"


class Timer {
public:
					Timer();
					~Timer();

	void			Reset();
	double			GetMilliseconds() const;

private:
	LARGE_INTEGER	mStartTime;
	LARGE_INTEGER	mFrequency;
};


#endif // XOF_TIMER_WINDOWS_HPP