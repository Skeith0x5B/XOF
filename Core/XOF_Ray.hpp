#ifndef XOF_RAY_HPP
#define XOF_RAY_HPP


#include <glm/vec3.hpp>


struct Ray {
	Ray() {
		origin = direction = glm::vec3( 0.f, 0.f, 0.f );
		start = end = 0.f;
	}

	Ray( const glm::vec3& origin, const glm::vec3& direction, float start, float end ) {
		this->origin = origin;
		this->direction = direction;
		this->start = start;
		this->end = end;
	}

	glm::vec3 origin;
	glm::vec3 direction;
	float	  start, end;
};


#endif // XOF_RAY_HPP