/*
===============================================================================

	XOF
	===
	File	:	XOF_Texture.cpp
	Desc	:	Represents a texture map; diffuse, normal etc...

===============================================================================
*/
#include "XOF_Texture.hpp"
#include "../ThirdParty/stb_image.h"
#include <assert.h>
#include <iostream>


Texture::Texture() {
	// ...
}

Texture::Texture( const std::string &fileName ) {
	Load( fileName );
}

Texture::~Texture() {
	glDeleteTextures( 1, &mTexture );
}

bool Texture::Load( const std::string &fileName ) {
	I32 width, height, componentCount;
	U8 *imageData = stbi_load( fileName.c_str(), &width, &height, &componentCount, 4 );

	if( imageData == nullptr ) {
		std::cerr << "Texture failed to load: " + fileName << std::endl;
		mIsLoaded = false;
		return mIsLoaded;
	}

	glGenTextures( 1, &mTexture );
	glBindTexture( GL_TEXTURE_2D, mTexture );

	// Set wrapping
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	// Set filtering
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// Send texture to GPU
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData );

	stbi_image_free( imageData );

	mIsLoaded = true;
	return mIsLoaded;
}

bool Texture::IsLoaded() const {
	return mIsLoaded;
}

void Texture::Bind( U32 unit ) {
	assert( ( unit >= 0 ) && ( unit <= 31 ) );

	glActiveTexture( GL_TEXTURE0 + unit );
	glBindTexture( GL_TEXTURE_2D, mTexture );
}