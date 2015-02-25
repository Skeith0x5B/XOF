/*
===============================================================================

	XOF
	===
	File	:	XOF_PlatformInfo.cpp
	Desc	:	Get more info on the platform

===============================================================================
*/
#include "XOF_PlatformInfo.hpp"
#include <iostream>
#include <SFML/OpenGL.hpp>


void CPUIDQuery( I32 query, const CPUDesc& cpuDesc ) {
#if XOF_PLATFORM == XOF_PLATFORM_WINDOW
	__cpuid( (I32*)cpuDesc.registers, query );
#elif XOF_PLATFORM == XOF_PLATFORM_LINUX
	#error Add Linux CPUID support!
#endif
}

void GetPlatformInfo() {
	CPUDesc cpuDesc;
	// CPU Vendor
	CPUIDQuery( 0, cpuDesc );

	cpuDesc.vendor =  reinterpret_cast<CHAR*>( &cpuDesc.registers[1] );
	*reinterpret_cast<U32*>(&cpuDesc.vendor[0] + 4) = cpuDesc.registers[3];
	*reinterpret_cast<U32*>(&cpuDesc.vendor[0] + 8) = cpuDesc.registers[2];

	std::cout << "CPU: " << cpuDesc.vendor << std::endl;

	CPUIDQuery( 1, cpuDesc );
	// Print CPU name and model eg. FX-6300

	// GPU
	std::cout << "GPU: " << glGetString( GL_RENDERER ) << std::endl;
}