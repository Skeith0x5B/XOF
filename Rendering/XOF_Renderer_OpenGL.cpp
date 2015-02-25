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

#include "../GameplayFoundations/XOF_PlayerObject.hpp"

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

	glClearColor( 0.5f, 0.5f, 0.5f, 0.f );

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

	// Wireframe
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	return true;
}

void Renderer::Draw() {
	// ...
}

void Renderer::Draw( const GameObject& object ) {
	DrawMesh( const_cast<Mesh&>( *object.GetMesh() ) );

	/*glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	DrawMesh( object.GetCollisionVolume().GetVisualRepresentation() );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );*/
}


// EDITOR TESTING ----------------------------------------------------------------
#include "../Editor/XOF-ED_TransformGizmo.hpp"
void Renderer::Draw( const TransformGizmo& gizmo ) {
	glDisable( GL_DEPTH_TEST );
	DrawMesh( gizmo.GetMesh() );
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	DrawMesh( gizmo.mXAxisCollision.GetVisualRepresentation() );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glEnable( GL_DEPTH_TEST );
}
// EDITOR TESTING ----------------------------------------------------------------


void Renderer::ClearScreen() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Renderer::Resize( U32 newWidth, U32 newHeight ) {
	glViewport( 0, 0, newWidth, newHeight );
}

void Renderer::DrawMesh( const Mesh& mesh ) {
	glEnableVertexAttribArray( 0 ); // position
	glEnableVertexAttribArray( 1 ); // normal
	glEnableVertexAttribArray( 2 ); // texture-coordinates
	glEnableVertexAttribArray( 3 ); // tangent

	for( U32 i=0; i<mesh.GetSubMeshCount(); ++i ) {
		glBindBuffer( GL_ARRAY_BUFFER, mesh.GetSubMeshData()[i].VB );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), reinterpret_cast<GLvoid*>( 12 ) );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), reinterpret_cast<GLvoid*>( 24 ) );
		glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), reinterpret_cast<GLvoid*>( 32 ) );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh.GetSubMeshData()[i].IB );

		const U32 materialIndex = mesh.GetSubMeshData()[i].materialIndex;
		if( materialIndex < mesh.GetTextureData( XOF_TEXTURE_TYPE::DIFFUSE ).size() && 
			mesh.GetTextureData( XOF_TEXTURE_TYPE::DIFFUSE )[materialIndex].IsLoaded() ) {
			mesh.GetTextureData( XOF_TEXTURE_TYPE::DIFFUSE )[materialIndex].Bind( 0 );
		}

		if( materialIndex < mesh.GetTextureData( XOF_TEXTURE_TYPE::NORMAL ).size() )
			if( mesh.GetTextureData( XOF_TEXTURE_TYPE::NORMAL )[materialIndex].IsLoaded() ) {
			mesh.GetTextureData( XOF_TEXTURE_TYPE::NORMAL )[materialIndex].Bind( 1 );
			std::cerr << "----------------- ATTEMPTING TO DRAW NORMAL MAP..." << std::endl;

		}

		glDrawElements( GL_TRIANGLES, mesh.GetSubMeshData()[i].indexCount, GL_UNSIGNED_INT, 0 );
	}

	glDisableVertexAttribArray( 0 );
	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 2 );
	glDisableVertexAttribArray( 3 );
}