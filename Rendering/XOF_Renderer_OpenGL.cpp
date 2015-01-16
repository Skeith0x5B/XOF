/*
===============================================================================

	XOF
	===
	File	:	XOF_GraphicsDevice_OpenGL.cpp
	Desc	:	OpenGL implementation of the GraphicsDevice interface.

				glEnable( GL_FRAMEBUFFER_SRGB ); undefined error/issues
				were resolved by getting glew (use this in future, never
				rely on Microsoft's implementation - ever.)

===============================================================================
*/
#include "XOF_Renderer_OpenGL.hpp"
#include "XOF_Vertex.hpp"
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <iostream>


GLuint VBO;


bool Renderer::Initialise() {
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		std::cerr << "Error: Glew failed to initialise!" << std::endl;
		return false;
	}

	glClearColor( 0.f, 0.f, 0.f, 0.f );

	glFrontFace( GL_CW ); // Either swap this or the vertex ordering (this is more sensible option)
	glCullFace( GL_BACK );
	glEnable( GL_CULL_FACE );
	
	//glDepthMask( GL_TRUE );
	//glDepthFunc( GL_LEQUAL );
	//glDepthRange( 0.f, 1.f );
	//glClearDepth( 1.f );
	glEnable( GL_DEPTH_TEST );

	// Essentially free gamma correction
	glEnable( GL_FRAMEBUFFER_SRGB );

	return true;
}

void Renderer::Draw() {
	// ...
}

void Renderer::ClearScreen() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}