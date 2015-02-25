/*
===============================================================================

	XOF
	===
	File	:	XOF_Platform.hpp
	Desc	:	Platform id & atomic data types.

===============================================================================
*/
#ifndef XOF_PLATFORM_HPP
#define XOF_PLATFORM_HPP


//namespace xof {


// Detect OS/Platform
#if defined( _WIN64 )
	#define XOF_PLATFORM  XOF_PLATFORM_WINDOWS
	#define WIN64_LEAN_AND_MEAN
	#include <Windows.h>
	#include <time.h>

	typedef HWND handle;
#elif defined ( _WIN32 )
	#define XOF_PLATFORM  XOF_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <time.h>

	typedef HWND handle;
#elif defined( __linux__ )
	#define XOF_PLATFORM  XOF_PLATFORM_XOF
	#include <time.h>
	
	typedef void* handle; // 7/10/12
#else
	#error Unknown/unsupported platform (OS) - compilation stopped. 
#endif // OS


#include <memory>
#include <string>


// Unsigned data types
typedef unsigned char	U8;
typedef unsigned short	U16;
#ifdef UINT
#undef UINT
#endif
typedef unsigned int	U32, UINT;

// Signed data types
typedef char	I8 , CHAR;
typedef short	I16;
#ifdef INT
#undef INT
#endif
typedef int		I32, INT;


// 64 bit can be a little compiler-specific
#if defined( _MSC_VER )
	typedef unsigned __int64 U64;
	typedef __int64 I64;
#else
	typedef unsigned long long U64;
	typedef long long I64;
#endif // #if defined( _MSC_VER )

//} // namespace xof


#endif // XOF_PLATFORM_HPP