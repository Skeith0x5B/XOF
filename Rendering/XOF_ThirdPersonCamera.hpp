/*
===============================================================================

	XOF
	===
	File	:	XOF_ThirdPersonCamera.hpp
	Desc	:	Basic third-person camera.

===============================================================================
*/
#ifndef XOF_THIRD_PERSON_CAMERA_HPP
#define XOF_THIRD_PERSON_CAMERA_HPP


#include <glm/glm.hpp>
class GameObject;


class ThirdPersonCamera {
public:
						ThirdPersonCamera();
						ThirdPersonCamera(  glm::vec3 &pos, const glm::vec3 &forward, 
											float screenHeight, float screenWidth, 
											float aspectRatio, float zNear, float zFar,
											const GameObject& objectToFollow );

	void				Setup(  glm::vec3 &pos, const glm::vec3 &forward, 
								float screenWidth, float screenHeight,
								float aspectRatio, float zNear, float zFar,
								const GameObject& objectToFollow );

	void				Resize( float newWidth, float newHeight );

	const glm::vec3&	GetViewDirection() const;
	glm::mat4			GetViewMatrix() const;
	glm::mat4			GetProjectionMatrix() const;

	void				Translate( float x, float y, float z );
	void				Pitch( float amount );
	void				Yaw( float amount );

private:
	glm::vec3			mPos;
	glm::vec3			mForward;
	glm::vec3			mUp;
	glm::vec3			mRight;

	GameObject*			mObjectToFollow;

						// Projection params
	float				mFov;
	float				mNearPlane;
	float				mFarPlane;
	float				mAspectRatio;

	float				mScreenWidth;
	float				mScreenHeight;
};


#endif // XOF_THIRD_PERSON_CAMERA_HPP