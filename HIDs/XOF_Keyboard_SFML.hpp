/*
===============================================================================

	XOF
	===
	File	:	XOF_Keyboard_SFLM.hpp
	Desc	:	SFML implementation of basic keyboard interface.

===============================================================================
*/
#ifndef XOF_KEYBOARD_SFML
#define XOF_KEYBOARD_SFML


#include "XOF_Keyboard_I.hpp"
#include <SFML/Window/Keyboard.hpp>


// implementation specific button/key/control codes
static const U8 KeyboardKeyCodes[XOF_KEYBOARD::KEY_COUNT] =  {
	// esc
	sf::Keyboard::Escape, 

	// function keys
	sf::Keyboard::F1, sf::Keyboard::F2, sf::Keyboard::F3, sf::Keyboard::F4, sf::Keyboard::F5, sf::Keyboard::F6,
	sf::Keyboard::F7, sf::Keyboard::F8, sf::Keyboard::F9, sf::Keyboard::F10, sf::Keyboard::F11, sf::Keyboard::F12, 

	// numerical keys
	sf::Keyboard::Num0, sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3, sf::Keyboard::Num4,
	sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7, sf::Keyboard::Num8, sf::Keyboard::Num9,

	// alphabetic keys
	sf::Keyboard::A, 0x42, 0x43, sf::Keyboard::D, sf::Keyboard::E, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 
	0x4E, 0x4F, 0x50, sf::Keyboard::Q, 0x52, sf::Keyboard::S, 0x54, 0x55, 0x56, sf::Keyboard::W, 0x58, 0x59, 0x5A,

	// numpad
	sf::Keyboard::Numpad0, sf::Keyboard::Numpad1, sf::Keyboard::Numpad2, sf::Keyboard::Numpad3, sf::Keyboard::Numpad4,
	sf::Keyboard::Numpad5, sf::Keyboard::Numpad6, sf::Keyboard::Numpad7, sf::Keyboard::Numpad8, sf::Keyboard::Numpad9,
	0, 0, 0, 0, 0, 0, 0,

	// arrows
	sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, 

	// 'special' cluster 1
	0, 0, 0,

	// 'special' cluster 2
	sf::Keyboard::Insert, sf::Keyboard::Home, 0, sf::Keyboard::Delete, 0, 0,

	// left & right variant keys
	sf::Keyboard::LShift, sf::Keyboard::RShift, sf::Keyboard::LControl, sf::Keyboard::RControl, 0, 0, sf::Keyboard::LSystem, sf::Keyboard::RSystem,

	// cluster 3
	0, 0,
	0, 0,
	0, 0,

	// cluster 4
	0, 0,
	0, 0,

	// OEM
	0, 0, 0, 0, 0, 0, 0, 0,
};

class Keyboard : public KeyboardI {
public:
	void	Update();

	bool	IsKeyUp( U32 key );
	bool	IsKeyDown( U32 key );
private:

};

#endif // XOF_KEYBOARD_SFML