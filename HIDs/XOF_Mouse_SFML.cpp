/*
===============================================================================

	XOF
	===
	File	:	XOF_Mouse_SFLM.cpp
	Desc	:	SFML implementation of basic mouse interface.

===============================================================================
*/
#include "XOF_Mouse_SFML.hpp"


#if defined window
	#undef window
	#define window sf::Window*
#endif


Mouse::Mouse() {
	MouseI::mWindow = nullptr;
}

void Mouse::SetWindow( const void *relativeTo ) {
	mWindow = const_cast<void*>( relativeTo );
}

void Mouse::Update() {
	// ...
}

bool Mouse::IsButtonUp( U32 button ) {
	return !sf::Mouse::isButtonPressed( static_cast<sf::Mouse::Button>( MouseButtonCodes[button] ) );
}

bool Mouse::IsButtonDown( U32 button ) {
	return sf::Mouse::isButtonPressed( static_cast<sf::Mouse::Button>( MouseButtonCodes[button] ) );
}

glm::vec2 Mouse::GetMousePosition() {
	if( mWindow ) {
		return  glm::vec2( static_cast<float>( sf::Mouse::getPosition( *((sf::Window*)mWindow)).x ), 
			static_cast<float>( sf::Mouse::getPosition( *((sf::Window*)mWindow)).y ) );
	} else {
		return glm::vec2( -1.f, -1.f );
	}
}