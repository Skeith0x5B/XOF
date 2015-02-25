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
private:
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

public:
			// Give the geoprimgenerator easy access to the vertex and index data
			// for easy construction of primitives for use in the scene, editor brushes etc...
	friend	class				GeoPrimitiveGenerator;

								Mesh();
								Mesh( const std::string &fileName );
	virtual						~Mesh();

	bool						Load( const std::string &fileName );
	bool						IsLoaded() const;

	std::vector<Mesh::SubMesh>&	GetSubMeshData() const;
	U32							GetSubMeshCount() const;
	std::vector<Texture>&		GetTextureData( XOF_TEXTURE_TYPE type ) const;
	U32							GetTextureCount( XOF_TEXTURE_TYPE type ) const;

	const Vertex*				GetVertexData() const;
	U32							GetVertexCount() const;
	bool normals;
private:
	bool						mIsLoaded;

								enum XOF_MESH_BUFFER {
									POSITION_VB = 0,
									NORMAL_VB,
									TEXCOORD_VB,
									INDEX_VB,
									COUNT,
								};

	std::vector<Mesh::SubMesh>	mSubMeshes;
	std::vector<Texture>		mTextures[XOF_TEXTURE_TYPE::COUNT];

	void						InitMaterials( const aiScene *scene );
};


#endif // XOF_MESH_HPP