/*
===============================================================================

	XOF
	===
	File	:	XOF_GraphicsDevice_OpenGL.hpp
	Desc	:	OpenGL implementation of the GraphicsDevice interface.

				NOTE: Add ability to enable/disable certain vertex attributes?

===============================================================================
*/
#ifndef XOF_RENDERER_OPENGL_HPP
#define XOF_RENDERER_OPENGL_HPP


#include "../Platform/XOF_Platform.hpp"
#include "../GameplayFoundations/XOF_PlayerObject.hpp"
class TransformGizmo;


struct RendererState {
	// wireframe/solid
	// winding order
	// cull
	// etc..?
};


class Renderer {
public:
	bool	Initialise();

			// GAME
	void	Draw();
	void	Draw( const GameObject& object );

			// EDITOR
	void	Draw( const TransformGizmo& gizmo );

			// UTILITY
	void	ClearScreen();
	void	Resize( U32 newWidth, U32 newHeight );

			// SetState()?
			// Shutdown()?

private:
	void	DrawMesh( const Mesh& mesh );

};


#endif // XOF_RENDERER_OPENGL_HPP