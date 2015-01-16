/*
===============================================================================

	XOF
	===
	File	:	XOF_StaticCamera.cpp
	Desc	:	Represents a static camera.

===============================================================================
*/
#include "XOF_StaticCamera.hpp"

StaticCamera::StaticCamera() {
	// ...
}

StaticCamera::StaticCamera( const glm::vec3 &pos, const glm::vec3 &forward, float fov, 
								float aspectRatio, float zNear, float zFar ) {
	Setup( pos, forward, fov, aspectRatio, zNear, zFar );
}

void StaticCamera::Setup( const glm::vec3 &pos, const glm::vec3 &forward, float fov, 
							float aspectRatio, float zNear, float zFar ) {
	mPerspective = glm::perspective( fov, aspectRatio, zNear, zFar );

	mPos = pos;
	mForward = forward;
	mUp = glm::vec3( 0.f, 1.f, 0.f );
}

glm::mat4 StaticCamera::GetViewProjection() const {
	return mPerspective * glm::lookAt( mPos, mPos + mForward, mUp );
}