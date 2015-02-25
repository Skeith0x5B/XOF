/*
===============================================================================

	XOF
	===
	File	:	XOF_PlatformInfo.hpp
	Desc	:	Get more info on the platform (CPU, GPU, RAM)

===============================================================================
*/
#ifndef XOF_PLATFORM_INFO_HPP
#define XOF_PLATFORM_INFO_HPP


#include "XOF_Platform.hpp"


enum XOF_CPUID_QUERIES {
	VENDOR = 0,
	// SIMD sets etc...
};


struct CPUDesc {
				CPUDesc() { 
					std::memset( this, 0x00, sizeof( CPUDesc ) );
					vendor = "NotQueried";
				}

				// eax, ebx, ecx and edx registers
	U32			registers[4];
				// features flag
	U32			featuresFlag;
				// vendor name/string
	std::string vendor;
};


void CPUIDQuery( I32 query, const CPUDesc& cpuDesc );

// Generate info about CPU, GPU and RAM
void GetPlatformInfo();



#endif // XOF_PLATFORM_INFO_HPP