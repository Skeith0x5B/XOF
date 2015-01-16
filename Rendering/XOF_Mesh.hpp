/*
===============================================================================

	XOF
	===
	File	:	XOF_Mesh.hpp
	Desc	:	Represents a mesh, be it from a file or manually created.

===============================================================================
*/
#ifndef XOF_MESH_HPP
#define XOF_MESH_HPP


#include "../Platform/XOF_Platform.hpp"
#include "XOF_Vertex.hpp"
#include "XOF_Texture.hpp"
#include <GL/glew.h>
#include <assimp/scene.h>
#include <string>
#include <vector>


class Mesh {
public:
			Mesh();
			Mesh( const std::string &fileName );
	virtual ~Mesh();

	bool	Load( const std::string &fileName );
	bool	IsLoaded() const;

	void	Draw();

private:
	bool	mIsLoaded;

			enum XOF_MESH_BUFFER {
				POSITION_VB = 0,
				NORMAL_VB,
				TEXCOORD_VB,
				INDEX_VB,
				COUNT,
			};

			// A whole mesh is built up as a collection of submeshes
			struct SubMesh {
				GLuint  VB;
				GLuint  IB;
				U32		indexCount;
				U32		materialIndex;

				SubMesh();
				~SubMesh();

				void Init( const std::vector<Vertex> &vertexData, 
					const std::vector<U32> &indexData );
			};

	std::vector<SubMesh> mSubMeshes;
	std::vector<Texture> mTextures;

	void	InitMaterials( const aiScene *scene );
};


#endif // XOF_MESH_HPP