/*
===============================================================================

	XOF
	===
	File	:	XOF_ThirdPersonCamera.hpp
	Desc	:	Basic third-person camera.

===============================================================================
*/
#include "XOF_ThirdPersonCamera.hpp"
#include "../GameplayFoundations/XOF_GameObject.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


ThirdPersonCamera::ThirdPersonCamera() {
	// ...
}

ThirdPersonCamera::ThirdPersonCamera(  glm::vec3 &pos, const glm::vec3 &forward,
										float screenWidth, float screenHeight,
										float aspectRatio, float zNear, float zFar,
										const GameObject& objectToFollow) {
	Setup( pos, forward, screenWidth, screenHeight, aspectRatio, zNear, zFar, objectToFollow );
}

void ThirdPersonCamera::Setup(  glm::vec3 &pos, const glm::vec3 &forward,
								float screenWidth, float screenHeight,
								float aspectRatio, float zNear, float zFar,
								const GameObject& objectToFollow ) {
	mPos		= objectToFollow.GetTransform().mTranslation;
	mPos.y	   += 3.f;
	mPos.z	   += 5.f;
	mForward	= forward;
	mUp			= glm::vec3( 0.f, 1.f, 0.f );
	mRight		= glm::vec3( 1.f, 0.f, 0.f );

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	mFov		 = mScreenWidth / mScreenHeight;
	mNearPlane	 = zNear;
	mFarPlane	 = zFar;
	mAspectRatio = aspectRatio;

	mObjectToFollow = const_cast<GameObject*>( &objectToFollow );
}

void ThirdPersonCamera::Resize( float newWidth, float newHeight ) {
	mScreenWidth = newWidth;
	mScreenHeight = newHeight;
}

const glm::vec3& ThirdPersonCamera::GetViewDirection() const {
	return mForward;
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() const {
	return glm::lookAt( mPos, mPos + mForward, mUp );
}

glm::mat4 ThirdPersonCamera::GetProjectionMatrix() const {
	return glm::perspectiveFov<float>( mFov, mScreenWidth, mScreenHeight, mNearPlane, mFarPlane );
}

void ThirdPersonCamera::Translate( float x, float y, float z ) {
	mPos += glm::vec3( x, y, z );
}

void ThirdPersonCamera::Pitch( float amount ) {
	glm::vec3 right = glm::vec3( 1.f, 0.f, 0.f );

	glm::vec3 target = mObjectToFollow->GetTransform().mTranslation;
	mPos = glm::mat3( glm::rotate( amount, right ) ) * glm::vec3( mPos - target ) + target;

	mForward = glm::mat3( glm::rotate( amount, right ) ) * mForward;
	mUp = glm::mat3( glm::rotate( amount, right ) ) * mUp;
}

// NOTE: Put yaw clamp in here?
void ThirdPersonCamera::Yaw( float amount ) {
	glm::vec3 up = glm::vec3( 0.f, 1.f, 0.f );

	glm::vec3 target = mObjectToFollow->GetTransform().mTranslation;
	mPos = glm::mat3( glm::rotate( amount, up ) ) * glm::vec3( mPos - target ) + target;

	mForward = glm::mat3( glm::rotate( amount, up ) ) * mForward;
	mRight = glm::mat3( glm::rotate( amount, up ) ) * mRight;
}