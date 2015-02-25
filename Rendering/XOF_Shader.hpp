/*
===============================================================================

	XOF
	===
	File	:	XOF_Shader.hpp
	Desc	:	Represents a shader; vertex, pixel (fragment) etc...

===============================================================================
*/
#ifndef XOF_SHADER_HPP
#define XOF_SHADER_HPP


#include "../Platform/XOF_Platform.hpp"
#include "XOF_Transform.hpp"
#include "XOF_StaticCamera.hpp" // NOTE: Remove this?
#include "XOF_FirstPersonCamera.hpp"
#include <GL/glew.h>
#include <string>
#include <map>


class Shader {
public:
			Shader();
			Shader( const std::string &fileName );
	virtual	~Shader();

	bool	Load( const std::string & fileName );
	bool	IsLoaded() const;

			// TODO: Add shader? (vertex, frag, tessellation, geometry etc?)
	void	AddUniform( const std::string &uniformName );
	void	SetUniform( const std::string &uniformName, float value );
	void	SetUniform( const std::string &uniformName, glm::vec3 &value );
	void	SetUniform( const std::string &uniformName, glm::mat4 &value );

	void	Bind();
	
private:
	bool	mIsLoaded;

			enum XOF_SHADER_TYPE {
				VERTEX = 0,
				FRAGMENT,
				// TESS_CONTROL, TESS_EVAL, GEOMETRY...
				COUNT,
			};
	
	GLuint	mProgram;
	GLuint	mShaders[XOF_SHADER_TYPE::COUNT];
	std::map<std::string, U32> mUniforms;
};


#endif // XOF_SHADER_HPP