/*
===============================================================================

	XOF
	===
	File	:	XOF_Texture.hpp
	Desc	:	Represents a texture map; diffuse, normal etc...

===============================================================================
*/
#ifndef XOF_TEXTURE_HPP
#define XOF_TEXTURE_HPP


#include "../Platform/XOF_Platform.hpp"
#include <GL/glew.h>
#include <string>


class Texture {
public:
			Texture();
			Texture( const std::string &fileName );
	virtual ~Texture();

	bool	Load( const std::string &fileName );
	bool	IsLoaded() const;

	void	Bind( U32 unit );

private:
	bool	mIsLoaded;
	GLuint	mTexture;
};


#endif // XOF_TEXTURE_HPP