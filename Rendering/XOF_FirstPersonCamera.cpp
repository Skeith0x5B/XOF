#include "XOF_FirstPersonCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


FirstPersonCamera::FirstPersonCamera() {
	// ...
}

FirstPersonCamera::FirstPersonCamera( const glm::vec3 &pos, const glm::vec3 &forward, float fov, 
										float aspectRatio, float zNear, float zFar ) {
	Setup( pos, forward, fov, aspectRatio, zNear, zFar );
}

void FirstPersonCamera::Setup( const glm::vec3 &pos, const glm::vec3 &forward, float fov, 
								float aspectRatio, float zNear, float zFar ) {
	mPos		= pos;
	mForward	= forward;
	mUp			= glm::vec3( 0.f, 1.f, 0.f );
	mRight		= glm::vec3( 1.f, 0.f, 0.f );

	mFov		 = fov;
	mNearPlane	 = zNear;
	mFarPlane	 = zFar;
	mAspectRatio = aspectRatio;
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

glm::mat4 FirstPersonCamera::GetViewProjection() const {
	return glm::perspective( mFov, mAspectRatio, mNearPlane, mFarPlane ) * 
		glm::lookAt( mPos, mPos + mForward, mUp );
}