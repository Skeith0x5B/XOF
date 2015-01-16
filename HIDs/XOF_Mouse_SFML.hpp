/*
===============================================================================

	XOF
	===
	File	:	XOF_Mouse_SFLM.hpp
	Desc	:	SFML implementation of basic mouse interface.

===============================================================================
*/
#ifndef XOF_MOUSE_SFML
#define XOF_MOUSE_SFML


#include "XOF_Mouse_I.hpp"
#include <SFML/Window/Mouse.hpp>
#include <glm/vec2.hpp>


// implementation specific button/key/control codes
static const U16 MouseButtonCodes[XOF_MOUSE::BUTTON_COUNT] =  {
	// standard left, right and middle mouse buttons
	sf::Mouse::Left,
	sf::Mouse::Right,
	sf::Mouse::Middle,

	// 'X button(s)' - extra buttons outside the standard left, right and middle
	sf::Mouse::XButton1,
	sf::Mouse::XButton1,
};


class Mouse : public MouseI {
public:
				Mouse();

				// Mouse position measurements will now be relevant to this specific window.
	void		SetWindow( const void *relativeTo );

	void		Update();

	bool		IsButtonUp( U32 button );
	bool		IsButtonDown( U32 button );

	glm::vec2	GetMousePosition();

private:

};

#endif // XOF_MOUSE_SFML