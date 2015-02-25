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
	glm::vec3 mTranslation;
	glm::vec3 mRotation;
	glm::vec3 mScale;

	Transform() {
		mTranslation = glm::vec3( 0.f, 0.f, 0.f );
		mRotation = glm::vec3( 0.f, 0.f, 0.f );
		mScale = glm::vec3( 1.f, 1.f, 1.f );
	}

	Transform( const glm::vec3 &translation, const glm::vec3 &rotation, 
		const glm::vec3 &scale = glm::vec3( 1.f, 1.f, 1.f ) ) {
		mTranslation = translation;
		mRotation = rotation;
		mScale = scale;
	}

	// OpenGL tends to refer to world matrix as model matrix?
	inline glm::mat4 GetModelMatrix() const {
		glm::mat4 translationMatrix = glm::translate( mTranslation );

		// NOTE: Quarternions would be preferable
		glm::mat4 rotX = glm::rotate( mRotation.x, glm::vec3( 1.f, 0.f, 0.f ) );
		glm::mat4 rotY = glm::rotate( mRotation.y, glm::vec3( 0.f, 1.f, 0.f ) );
		glm::mat4 rotZ = glm::rotate( mRotation.z, glm::vec3( 0.f, 0.f, 1.f ) );	
		// Will be applied in inverse order (X, Y then Z)
		glm::mat4 rotationMatrix = rotZ * rotY * rotX;

		glm::mat4 scaleMatrix = glm::scale( mScale );

		// Again, will be applied in inverse order (scale, rotation, translation)
		return translationMatrix * rotationMatrix * scaleMatrix;
	}
};


#endif // XOF_TRANSFORM_HPP