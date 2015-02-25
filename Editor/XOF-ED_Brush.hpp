/*
===============================================================================

	XOF
	===
	File	:	XOF-ED_Brush.hpp
	Desc	:	Basis for all brush types.

===============================================================================
*/
#ifndef XOF_ED_BRUSH_HPP
#define XOF_ED_BRUSH_HPP


class Mesh;


class Brush {
public:
	Brush() { ; }
	virtual ~Brush() { ; }

	virtual Mesh*	AddGeometry() = 0;
	virtual void	ResetConfig() = 0;

private:
	// ...
};


// class CubeBrush, CylinderBrush etc... (specifiying theoretical properties like height, width, raidus and so on - not an actual mesh)


#endif // XOF_ED_BRUSH_HPP