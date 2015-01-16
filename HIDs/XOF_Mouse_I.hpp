/*
===============================================================================

	XOF
	===
	File	:	XOF_Mouse_I.hpp
	Desc	:	Basic mouse interface, implement using some API.

				TODO: Add some kind of API neutral window type, SFML 
				implementation needs a window reference to get mouse position
				relative to that window, currently gets position relative to
				desktop.

===============================================================================
*/
#ifndef XOF_MOUSE_I
#define XOF_MOUSE_I


#include "../Platform/XOF_Platform.hpp"
class Vector2f;


// Intended to be used for indexing into an OS/API specific keyCode array.
// Game/Application code should be able to use these enum values and be shielded from OS/API specifics.
enum XOF_MOUSE {
	// standard left, right and middle mouse buttons
	LEFT,
	RIGHT,
	MIDDLE,

	// 'X button(s)' - extra buttons outside the standard left, right and middle
	X1,
	X2,

	// used for array indexing
	BUTTON_COUNT,
};

class MouseI {
public:
	virtual			~MouseI() {}

					// Mouse position measurements will now be relevant to this specific window.
	virtual void	SetWindow( const void *relativeTo ) = 0;

	virtual void	Update() = 0;

	virtual bool	IsButtonUp( U32 key ) = 0;
	virtual bool	IsButtonDown( U32 key ) = 0;

	Vector2f		GetMousePosition();

protected:
	void *			mWindow;

private:

};

#endif // XOF_MOUSE_I