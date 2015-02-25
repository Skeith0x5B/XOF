/*
===============================================================================

	XOF
	===
	File	:	XOF_GameObject.cpp
	Desc	:	The base for all objects/entities in the game world.

===============================================================================
*/
#include "XOF_GameObject.hpp"
#include "../Rendering/XOF_Mesh.hpp"


GameObject::GameObject() {
	mMesh = nullptr;
	mResponsibleForMesh = false;
}

GameObject::~GameObject() {
	if( mResponsibleForMesh ) {
		delete mMesh;
	}
	mMesh = nullptr;
}

Transform& GameObject::GetTransform() const {
	return const_cast<Transform&>( mTransform );
}

void GameObject::SetMesh( const Mesh* ptr, bool isResponsibleFor  ) {
	mMesh = const_cast<Mesh*>( ptr );
	// TODO: Code to generate scale of mesh along X, Y and Z
	mCollisionVolume.Create( 5, 5, 5 );
}

Mesh* GameObject::GetMesh() const {
	return const_cast<Mesh*>( mMesh );
}

AxisAlignedBox&	GameObject::GetCollisionVolume() const {
	return const_cast<AxisAlignedBox&>( mCollisionVolume );
}

void GameObject::Translate( float alongX, float alongY, float alongZ ) {
	mTransform.mTranslation += glm::vec3( alongX, alongY, alongZ );
}

void GameObject::Rotate( float aboutX, float aboutY, float aboutZ ) {
	mTransform.mRotation = glm::vec3( aboutX, aboutY, aboutZ );
}