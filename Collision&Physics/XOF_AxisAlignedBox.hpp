/*
===============================================================================

	XOF
	==========
	File		:	RtAxisAlignedBox.h
	Desc		:	Basic AABB (axis-aligned bounding box).
				
===============================================================================
*/
#ifndef XOF_AXIS_ALIGNED_BOX_H
#define XOF_AXIS_ALIGNED_BOX_H


#include "../Platform/XOF_Platform.hpp"
#include "../Rendering/XOF_Mesh.hpp"
#include "../Rendering/XOF_GeoPrimitiveGenerator.hpp"
#include "../Core/XOF_Ray.hpp"


class CollisionVolume {
public:
					CollisionVolume() { ; }
	virtual			~CollisionVolume() { if( mVisualRepresentation ) delete mVisualRepresentation; }

	virtual void	Create( const Mesh &visualRepresentation ) { /*DO NOTHING*/ ; }

	virtual bool	IsHit( const Ray& r ) const { /*DO NOTHING*/ return false; }

	Mesh&			GetVisualRepresentation() const { return const_cast<Mesh&>( *mVisualRepresentation ); }

protected:
	Mesh*			mVisualRepresentation;
};


class AxisAlignedBox : public CollisionVolume {
public:
					AxisAlignedBox() { 
						mMin = mMax = glm::vec3( 0.f, 0.f, 0.f );
					}

					~AxisAlignedBox() { ; }

	virtual void	Create( const Mesh &mesh ) {
		
	};

	void			Create( I32 sizeAlongX, I32 sizeAlongY, I32 sizeAlongZ ) {
		mVisualRepresentation = GeoPrimitiveGenerator::GenerateCube( sizeAlongX, sizeAlongY, sizeAlongZ );
		
		// Assuming box is always created at (0, 0, 0)
		mMin.x = -sizeAlongX / 2.f;
		mMin.y = -sizeAlongY / 2.f;
		mMin.z = -sizeAlongZ / 2.f;

		mMax.x = sizeAlongX / 2.f;
		mMax.y = sizeAlongY / 2.f;
		mMax.z = sizeAlongZ / 2.f;
	}

	// goo.gl/tGl0wy
	virtual bool	IsHit( const Ray& r ) const { 
		float tmin = ( mMin.x - r.origin.x ) / r.direction.x;
		float tmax = ( mMax.x - r.origin.x ) / r.direction.x;
		if( tmin > tmax ) std::swap( tmin, tmax );

		float tymin = ( mMin.y - r.origin.y ) / r.direction.y;
		float tymax = ( mMax.y - r.origin.y ) / r.direction.y;
		if( tymin > tymax ) std::swap( tymin, tymax );

		if( ( tmin > tymax ) || ( tymin > tmax ) ) return false;

		if( tymin > tmin ) tmin = tymin;
		if( tymax < tmax ) tmax = tymax;

		// bring into 3d
		float tzmin = ( mMin.z - r.origin.z ) / r.direction.z;
		float tzmax = ( mMax.z - r.origin.z ) / r.direction.z;
		if( tzmin > tzmax ) std::swap( tzmin, tzmax );

		if( ( tmin > tzmax ) || ( tzmin > tmax ) ) return false;

		if( tzmin > tmin ) tmin = tzmin;
		if( tzmax < tmax ) tmax = tzmax;

		if ( ( tmin > r.end ) || ( tmax < r.start ) ) return false;
		//if( r.start < tmin ) r.start = tmin;
		//if( r.end > tmax ) r.end = tmax;

		return true;
	}

private:
	glm::vec3		mMin;
	glm::vec3		mMax;
};


#endif // XOF_AXIS_ALIGNED_BOX_H