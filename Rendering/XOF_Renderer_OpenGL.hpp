/*
===============================================================================

	XOF
	===
	File	:	XOF_GraphicsDevice_OpenGL.hpp
	Desc	:	OpenGL implementation of the GraphicsDevice interface.

===============================================================================
*/
#ifndef XOF_RENDERER_OPENGL_HPP
#define XOF_RENDERER_OPENGL_HPP


class Renderer {
public:
	bool	Initialise();

	void	Draw();

	void	ClearScreen();

private:

};


#endif // XOF_RENDERER_OPENGL_HPP