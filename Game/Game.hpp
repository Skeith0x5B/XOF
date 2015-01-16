/*
===============================================================================

	XOF
	===
	File	:	Game.hpp
	Desc	:	Basic application/game class.

===============================================================================
*/
#ifndef GAME_HPP
#define GAME_HPP


#include "../Platform/XOF_Platform.hpp"
// TODO: Replace these implementation-specific includes with the abstract ones
#include "../HIDs/XOF_Keyboard_SFML.hpp"
#include "../HIDs/XOF_Mouse_SFML.hpp"
#include "../Rendering/XOF_Renderer_OpenGL.hpp"


namespace sf {
	class RenderWindow;
}


class Game {
public:
	static const U32	mFrameCap = 60;

						Game();
						~Game();

	bool				Initialise( U16 width, U16 height, CHAR* caption );	
	void				Run();

private:
	bool				mIsInitialised;

	sf::Window *		mWindow;
	Renderer			mRenderer;
	Keyboard			mKeyboard;
	Mouse				mMouse;

	void				SetupScene();

	void				HandleEvents();
	void				HandleInput();
	void				Render();
};


#endif // GAME_HPP