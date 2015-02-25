/*
===============================================================================

	XOF
	===
	File	:	XOF-ED_TransformGizmo.hpp
	Desc	:	Basic representation of a transform gizmo (translate, scale, rotate).

				NOTE: Gizmo size seems to be inverse of the cameras distance to the gizmo.

===============================================================================
*/
#ifndef XOF_ED_TRANSFORM_GIZMO_HPP
#define XOF_ED_TRANSFORM_GIZMO_HPP


#include "../Platform/XOF_Platform.hpp"
#include "../Collision&Physics/XOF_AxisAlignedBox.hpp"
#include "../Rendering/XOF_Transform.hpp"
class Mesh;


class TransformGizmo {
public:
				TransformGizmo();
				~TransformGizmo();

	void		Setup( const std::string& meshFileName );
	
	Mesh&		GetMesh() const;
	Transform&  GetTransform() const;

//private:
	Mesh*		mMesh;
	AxisAlignedBox mXAxisCollision;

	Transform	mTransformation;
};


#endif // XOF_ED_TRANSFORM_GIZMO_HPP