/*
===============================================================================

	XOF
	===
	File	:	XOF_GeoPrimitiveGenerator.cpp
	Desc	:	Class used to generate a handful of common geometric primitives.

===============================================================================
*/
#include "XOF_GeoPrimitiveGenerator.hpp"


GeoPrimitiveGenerator::GeoPrimitiveGenerator() {
	// ...
}

GeoPrimitiveGenerator::~GeoPrimitiveGenerator() {
	// ...
}

// What about texture coordinates?
Mesh* GeoPrimitiveGenerator::GenerateCube( I32 unitsAlongX, I32 unitsAlongY, I32 unitsAlongZ ) {
	//std::unique_ptr<Mesh> ptr( new Mesh() );
	Mesh *ptr = new Mesh();
	GenerateCube( unitsAlongX, unitsAlongY, unitsAlongZ, *ptr );
	return ptr;
}

void GeoPrimitiveGenerator::GenerateCube( I32 unitsAlongX, I32 unitsAlongY, I32 unitsAlongZ, Mesh& mesh ) {
	// Add the 8 vertices for a box
	std::vector<Vertex> tempVertexData;
	std::vector<U32> tempIndexData;

	tempVertexData.push_back( Vertex( glm::vec3( -unitsAlongX / 2.f, unitsAlongY / 2.f, -unitsAlongZ / 2.f ), glm::vec2( 0.f, 1.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3(  unitsAlongX / 2.f, unitsAlongY / 2.f, -unitsAlongZ / 2.f ), glm::vec2( 1.f, 1.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3(  unitsAlongX / 2.f, unitsAlongY / 2.f,  unitsAlongZ / 2.f ), glm::vec2( 1.f, 0.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3( -unitsAlongX / 2.f, unitsAlongY / 2.f,  unitsAlongZ / 2.f ), glm::vec2( 0.f, 0.f ) ) );

	tempVertexData.push_back( Vertex( glm::vec3( -unitsAlongX / 2.f, -unitsAlongY / 2.f, -unitsAlongZ / 2.f ), glm::vec2( 0.f, 1.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3(  unitsAlongX / 2.f, -unitsAlongY / 2.f, -unitsAlongZ / 2.f ), glm::vec2( 1.f, 1.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3(  unitsAlongX / 2.f, -unitsAlongY / 2.f,  unitsAlongZ / 2.f ), glm::vec2( 1.f, 0.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3( -unitsAlongX / 2.f, -unitsAlongY / 2.f,  unitsAlongZ / 2.f ), glm::vec2( 0.f, 0.f ) ) );

	// top
	tempIndexData.push_back( 0 ); tempIndexData.push_back( 1 ); tempIndexData.push_back( 2 );
	tempIndexData.push_back( 0 ); tempIndexData.push_back( 2 ); tempIndexData.push_back( 3 );

	// front
	tempIndexData.push_back( 3 ); tempIndexData.push_back( 2 ); tempIndexData.push_back( 6 );
	tempIndexData.push_back( 3 ); tempIndexData.push_back( 6 ); tempIndexData.push_back( 7 );

	// right
	tempIndexData.push_back( 2 ); tempIndexData.push_back( 1 ); tempIndexData.push_back( 5 );
	tempIndexData.push_back( 2 ); tempIndexData.push_back( 5 ); tempIndexData.push_back( 6 );

	// left
	tempIndexData.push_back( 0 ); tempIndexData.push_back( 3 ); tempIndexData.push_back( 7 );
	tempIndexData.push_back( 0 ); tempIndexData.push_back( 7 ); tempIndexData.push_back( 4 );

	// back
	tempIndexData.push_back( 1 ); tempIndexData.push_back( 0 ); tempIndexData.push_back( 4 );
	tempIndexData.push_back( 1 ); tempIndexData.push_back( 4 ); tempIndexData.push_back( 5 );

	// bot
	tempIndexData.push_back( 5 ); tempIndexData.push_back( 4 ); tempIndexData.push_back( 7 );
	tempIndexData.push_back( 5 ); tempIndexData.push_back( 7 ); tempIndexData.push_back( 6 );

	mesh.mSubMeshes.resize( 1 );
	mesh.mSubMeshes[0].Init( tempVertexData, tempIndexData );	

	// Just let brushes made by the geoprimgen use a default material
	mesh.mTextures[XOF_TEXTURE_TYPE::DIFFUSE].resize( 1 );
	mesh.mTextures[XOF_TEXTURE_TYPE::DIFFUSE][0].Load( "./Resources/EditorBrushTexture.png" );
	mesh.mSubMeshes[0].materialIndex = 0;

	mesh.mIsLoaded= true;
}