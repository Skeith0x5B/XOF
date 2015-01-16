/*
===============================================================================

	XOF
	===
	File	:	XOF_Keyboard_SFLM.cpp
	Desc	:	SFML implementation of basic keyboard interface.

===============================================================================
*/
#include "XOF_Keyboard_SFML.hpp"

void Keyboard::Update() {
	// ...
}

bool Keyboard::IsKeyUp( U32 key ) {
	return !sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>( KeyboardKeyCodes[key] ) );
}
 
bool Keyboard::IsKeyDown( U32 key ) {
	return sf::Keyboard::isKeyPressed( static_cast<sf::Keyboard::Key>( KeyboardKeyCodes[key] ) );
}