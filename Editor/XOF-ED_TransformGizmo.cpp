/*
===============================================================================

	XOF
	===
	File	:	XOF-ED_TransformGizmo.cpp
	Desc	:	Basic representation of a transform gizmo (translate, scale, rotate).

===============================================================================
*/
#include "XOF-ED_TransformGizmo.hpp"
#include "../Rendering/XOF_Mesh.hpp"


TransformGizmo::TransformGizmo() {
	mMesh = nullptr;
}

TransformGizmo::~TransformGizmo() {
	if( mMesh ) {
		delete mMesh;
	}
}

void TransformGizmo::Setup( const std::string& meshFileName ) {
	mMesh = new Mesh( meshFileName );

	// Create collision voumes?
	mXAxisCollision.Create( 3, 1, 1 );
}

Mesh& TransformGizmo::GetMesh() const {
	return const_cast<Mesh&>( *mMesh );
}

Transform& TransformGizmo::GetTransform() const {
	return const_cast<Transform&>( mTransformation );
}