/*
===============================================================================

	XOF
	===
	File	:	XOF_FirstPersonCamera.cpp
	Desc	:	Basic first-person camera.

===============================================================================
*/
#include "XOF_FirstPersonCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


FirstPersonCamera::FirstPersonCamera() {
	// ...
}

FirstPersonCamera::FirstPersonCamera( const glm::vec3 &pos, const glm::vec3 &forward,
										float screenWidth, float screenHeight,
										float aspectRatio, float zNear, float zFar ) {
	Setup( pos, forward, screenWidth, screenHeight, aspectRatio, zNear, zFar );
}

void FirstPersonCamera::Setup( const glm::vec3 &pos, const glm::vec3 &forward,
								float screenWidth, float screenHeight,
								float aspectRatio, float zNear, float zFar ) {
	mPos		= pos;
	mForward	= forward;
	mUp			= glm::vec3( 0.f, 1.f, 0.f );
	mRight		= glm::vec3( 1.f, 0.f, 0.f );

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	mFov		 = mScreenWidth / mScreenHeight;
	mNearPlane	 = zNear;
	mFarPlane	 = zFar;
	mAspectRatio = aspectRatio;
}

void FirstPersonCamera::Resize( float newWidth, float newHeight ) {
	mScreenWidth = newWidth;
	mScreenHeight = newHeight;
}

void FirstPersonCamera::Translate( float x, float y, float z ) {
	glm::vec3 zv( x, y, z );

	zv = glm::mat3( glm::rotate( mYaw, glm::vec3( 0.f,1.f,0.f ) ) ) * zv;
	zv = glm::mat3( glm::rotate( mPitch, glm::vec3( 1.f,0.f,0.f ) ) ) * zv;

	mPos += zv;
}

void FirstPersonCamera::Pitch( float amount ) {
	mPitch += amount;
	mForward = glm::mat3( glm::rotate( amount, mRight ) ) * mForward;
	mUp = glm::mat3( glm::rotate( amount, mRight ) ) * mUp;
}

void FirstPersonCamera::Yaw( float amount ) {
	mYaw += amount;
	mForward = glm::mat3( glm::rotate( amount, glm::vec3( 0.f ,1.f, 0.f ) ) ) * mForward;
	mRight = glm::mat3( glm::rotate( amount, glm::vec3( 0.f, 1.f, 0.f ) ) ) * mRight;
}

const glm::vec3& FirstPersonCamera::GetPosition() const {
	return mPos;
}

const glm::vec3& FirstPersonCamera::GetViewDirection() const {
	return mForward;
}

glm::mat4 FirstPersonCamera::GetViewMatrix() const {
	return glm::lookAt( mPos, mPos + mForward, mUp );
}

glm::mat4 FirstPersonCamera::GetProjectionMatrix() const {
	return glm::perspectiveFov<float>( mFov, mScreenWidth, mScreenHeight, mNearPlane, mFarPlane );
}