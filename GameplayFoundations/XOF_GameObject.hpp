/*
===============================================================================

	XOF
	===
	File	:	XOF_GameObject.hpp
	Desc	:	The base for all objects/entities in the game world.

===============================================================================
*/
#ifndef XOF_GAME_OBJECT_HPP
#define XOF_GAME_OBJECT_HPP


#include "../Rendering/XOF_Transform.hpp"
#include "../Collision&Physics/XOF_AxisAlignedBox.hpp"
class Mesh;


class GameObject {
public:
					GameObject();
					~GameObject();

	Transform&		GetTransform() const;

	void			SetMesh( const Mesh* ptr, bool isResponsibleFor ); // To be passed from Mesh manager
	Mesh*			GetMesh() const;

	AxisAlignedBox&	GetCollisionVolume() const;

	void			Translate( float alongX, float alongY, float alongZ );
	void			Rotate( float aboutX, float aboutY, float aboutZ );

protected:
	Transform		mTransform;
	Mesh*			mMesh;
	bool			mResponsibleForMesh;
	//Material*		mMaterial;
	AxisAlignedBox	mCollisionVolume;
};


#endif // XOF_GAME_OBJECT_HPP