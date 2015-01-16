/*
===============================================================================

	XOF
	===
	File	:	XOF_StaticCamera.hpp
	Desc	:	Represents a static camera.

===============================================================================
*/
#ifndef XOF_STATIC_CAMERA_HPP
#define XOF_STATIC_CAMERA_HPP


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


class StaticCamera {
public:
				StaticCamera();
				StaticCamera( const glm::vec3 &pos, const glm::vec3 &forward, float fov, 
								float aspectRatio, float zNear, float zFar );

	void		Setup( const glm::vec3 &pos, const glm::vec3 &forward, float fov, 
						float aspectRatio, float zNear, float zFar );

	glm::mat4	GetViewProjection() const;

private:
	glm::mat4	mPerspective;

	glm::vec3	mPos;
	glm::vec3	mForward;
	glm::vec3	mUp;
};


#endif // XOF_STATIC_CAMERA_HPP