/*
===============================================================================

	XOF
	===
	File	:	XOF_Timer_Window.cpp
	Desc	:	Windows timer.

===============================================================================
*/
#include "XOF_Timer_Windows.hpp"
#include <iostream>

Timer::Timer() {
	Reset();
}

Timer::~Timer() {

}

void Timer::Reset() {
	if( !QueryPerformanceCounter( &mStartTime ) ) {
		std::cerr << "!QueryPerformanceCounter( &mStartTime ) FAILED" << std::endl;
	}
	if( !QueryPerformanceFrequency( &mFrequency ) ) {
		std::cerr << "!QueryPerformanceFrequency( &mFrequency ) FAILED" << std::endl;
	}
}

double Timer::GetMilliseconds() const {
	LARGE_INTEGER current;
	if( !QueryPerformanceCounter( &current ) ) {
		std::cerr << "!QueryPerformanceCounter( &current ) FAILED" << std::endl;
	}
	
	// This line breaks the build if incremental linking is enabled
	return static_cast<double>( ( current.QuadPart - mStartTime.QuadPart ) ) / ( mFrequency.QuadPart / 1000.0 );
}