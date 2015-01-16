/*
===============================================================================

	XOF
	===
	File	:	XOF_Transform.hpp
	Desc	:	Represents a transformation, be it scale, rotation etc...
				NOTE: pos == translate?
				NOTE: Quarternions would be preferable for roatation to
						avoid gimbal lock...

				NOTE: Order of transformations?

===============================================================================
*/
#ifndef XOF_TRANSFORM_HPP
#define XOF_TRANSFORM_HPP


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


struct Transform {
	glm::vec3 mPos;
	glm::vec3 mRot;
	glm::vec3 mScale;

	Transform( const glm::vec3 &pos, const glm::vec3 &rot, 
		const glm::vec3 &scale = glm::vec3( 1.f, 1.f, 1.f ) ) {
		mPos = pos;
		mRot = rot;
		mScale = scale;
	}

	// OpenGL tends to refer to world matrix as model matrix?
	inline glm::mat4 GetModelMatrix() const {
		glm::mat4 posMatrix = glm::translate( mPos );

		// NOTE: Quarternions would be preferable
		glm::mat4 rotX = glm::rotate( mRot.x, glm::vec3( 1.f, 0.f, 0.f ) );
		glm::mat4 rotY = glm::rotate( mRot.y, glm::vec3( 0.f, 1.f, 0.f ) );
		glm::mat4 rotZ = glm::rotate( mRot.z, glm::vec3( 0.f, 0.f, 1.f ) );	
		// Will be applied in inverse order (X, Y then Z)
		glm::mat4 rotMatrix = rotZ * rotY * rotX;

		glm::mat4 scaleMatrix = glm::scale( mScale );

		// Again, will be applied in inverse order (scale, rotation, translation)
		return posMatrix * rotMatrix * scaleMatrix;
	}
};


#endif // XOF_TRANSFORM_HPP