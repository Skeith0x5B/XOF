/*
===============================================================================

	XOF
	===
	File	:	XOF_FirstPersonCamera.hpp
	Desc	:	Represents a first-person camera.

===============================================================================
*/
#ifndef XOF_FIRST_PERSON_CAMERA_HPP
#define XOF_FIRST_PERSON_CAMERA_HPP


#include <glm/glm.hpp>


class FirstPersonCamera {
public:
						FirstPersonCamera();
						FirstPersonCamera( const glm::vec3 &pos, const glm::vec3 &forward, float fov,
											float aspectRatio, float zNear, float zFar );

	void				Setup( const glm::vec3 &pos, const glm::vec3 &forward, float fov, 
								float aspectRatio, float zNear, float zFar );

	void				Translate( float x, float y, float z );
	void				Pitch( float amount );
	void				Yaw( float amount );

	const glm::vec3&	GetPosition() const;
	glm::mat4			GetViewProjection() const;

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
};


#endif // XOF_FIRST_PERSON_CAMERA_HPP