/*
===============================================================================

	XOF
	===
	File	:	XOF_Timer.hpp
	Desc	:	Choose timer implementation based on platform.

===============================================================================
*/
#ifndef XOF_TIMER_HPP
#define XOF_TIMER_HPP


#include "XOF_Platform.hpp"


#if XOF_PLATFORM == XOF_PLATFORM_WINDOWS
	#include "XOF_Timer_Windows.hpp"
#else
	#error Add the Linux timer! 
#endif 


#endif // XOF_TIMER_HPP