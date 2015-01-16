/*
===============================================================================

	XOF
	===
	File	:	XOF_Vertex.hpp
	Desc	:	Represents a vertex.

===============================================================================
*/
#ifndef XOF_VERTEX_HPP
#define XOF_VERTEX_HPP


#include <glm/glm.hpp>


struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;

	Vertex(const glm::vec3& pos, glm::vec2& texCoords, 
		const glm::vec3 &normal = glm::vec3( 0.f, 0.f, 0.f ) ) {
		this->pos = pos; 
		this->normal = normal;
		this->texCoords = texCoords;
	}
};


#endif // XOF_VERTEX_HPP