/*
===============================================================================

	XOF
	===
	File	:	XOF_FirstPersonCamera.hpp
	Desc	:	Basic first-person camera.

===============================================================================
*/
#ifndef XOF_FIRST_PERSON_CAMERA_HPP
#define XOF_FIRST_PERSON_CAMERA_HPP


#include <glm/glm.hpp>


class FirstPersonCamera {
public:
						FirstPersonCamera();
						FirstPersonCamera( const glm::vec3 &pos, const glm::vec3 &forward, 
											float screenHeight, float screenWidth, 
											float aspectRatio, float zNear, float zFar );

	void				Setup( const glm::vec3 &pos, const glm::vec3 &forward, 
								float screenWidth, float screenHeight,
								float aspectRatio, float zNear, float zFar );

	void				Resize( float newWidth, float newHeight );

	void				Translate( float x, float y, float z );
	void				Pitch( float amount );
	void				Yaw( float amount );

	const glm::vec3&	GetPosition() const;
	const glm::vec3&	GetViewDirection() const;
	glm::mat4			GetViewMatrix() const;
	glm::mat4			GetProjectionMatrix() const;

private:
	glm::vec3			mPos;
	glm::vec3			mForward;
	glm::vec3			mUp;
	glm::vec3			mRight;

	float				mYaw;
	float				mPitch;

						// Projection params
	float				mFov;
	float				mNearPlane;
	float				mFarPlane;
	float				mAspectRatio;

	float				mScreenWidth;
	float				mScreenHeight;
};


#endif // XOF_FIRST_PERSON_CAMERA_HPP