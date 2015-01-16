/*
===============================================================================

	XOF
	===
	File	:	XOF_Mesh.cpp
	Desc	:	Represents a mesh, be it from a file or manually created.

===============================================================================
*/
#include "XOF_Mesh.hpp"
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <iostream>


Mesh::Mesh() {
	// ...
}

Mesh::Mesh( const std::string &fileName ) {
	mIsLoaded = Load( fileName );
}

Mesh::~Mesh() {
	// ...
}

bool Mesh::Load( const std::string &fileName ) {
	mIsLoaded = true;

	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile( fileName.c_str(), aiProcess_Triangulate |
		aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_FlipWindingOrder );

	if( scene ) {
		mSubMeshes.resize( scene->mNumMeshes );

		for( U32 i=0; i<scene->mNumMeshes; ++i ) {
			std::vector<Vertex> tempVertexData;
			std::vector<U32> tempIndexData;

			aiMesh *sceneMesh = scene->mMeshes[i];
			// temp vertex data
			for( U32 j = 0; j < sceneMesh->mNumVertices; ++j ) {
				tempVertexData.push_back( Vertex( glm::vec3( sceneMesh->mVertices[j].x,
					sceneMesh->mVertices[j].y, sceneMesh->mVertices[j].z ),
					glm::vec2( sceneMesh->mTextureCoords[0][j].x, sceneMesh->mTextureCoords[0][j].y ),
					glm::vec3( sceneMesh->mNormals[j].x, sceneMesh->mNormals[j].y, sceneMesh->mNormals[j].z ) ) );
			}

			// temp index data
			for( U32 k=0; k<sceneMesh->mNumFaces; ++k ) {
				const aiFace &face = sceneMesh->mFaces[k];
				assert( face.mNumIndices == 3 );

				tempIndexData.push_back( face.mIndices[0] );
				tempIndexData.push_back( face.mIndices[1] );
				tempIndexData.push_back( face.mIndices[2] );
			}

			mSubMeshes[i].Init( tempVertexData, tempIndexData );
			mSubMeshes[i].materialIndex = sceneMesh->mMaterialIndex;
		}
		InitMaterials( scene );
	} else {
		std::cerr << "Error: Mesh failed to load " + fileName << std::endl;
		mIsLoaded = false;
	}

	return mIsLoaded;
}

bool Mesh::IsLoaded() const {
	return mIsLoaded;
}

void Mesh::Draw() {
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	glEnableVertexAttribArray( 2 );

	for( U32 i=0; i<mSubMeshes.size(); ++i ) {
		glBindBuffer( GL_ARRAY_BUFFER, mSubMeshes[i].VB );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), reinterpret_cast<GLvoid*>( 12 ) );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), reinterpret_cast<GLvoid*>( 24 ) );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mSubMeshes[i].IB );

		const U32 materialIndex = mSubMeshes[i].materialIndex;
		if( materialIndex < mTextures.size() && mTextures[materialIndex].IsLoaded() ) {
			mTextures[materialIndex].Bind( 0 );
		}

		glDrawElements( GL_TRIANGLES, mSubMeshes[i].indexCount, GL_UNSIGNED_INT, 0 );
	}

	glDisableVertexAttribArray( 0 );
	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 2 );
}

void Mesh::InitMaterials( const aiScene *scene ) {
	mTextures.resize( scene->mNumMaterials );

	for( U32 i=0; i<scene->mNumMaterials; ++i ) {
		const aiMaterial *material = scene->mMaterials[i];

		if( material->GetTextureCount( aiTextureType_DIFFUSE ) > 0 ) {
			aiString path;

			if( material->GetTexture( aiTextureType_DIFFUSE, 0, &path, 
				nullptr, nullptr, nullptr, nullptr, nullptr ) == AI_SUCCESS ) {
				if( !mTextures[i].Load( "./Resources/" + std::string( path.data ) ) ) {
					mTextures[i].Load( "./Resources/UVGrid.jpg" );
				}
			}
		}
	}
}

Mesh::SubMesh::SubMesh() {
	VB = -1;
	IB = -1;

	indexCount = -1;
	materialIndex = -1;
}

Mesh::SubMesh::~SubMesh() {
	if( VB != -1 ) {
		glDeleteBuffers( 1, &VB );
	}

	if( IB != -1 ) {
		glDeleteBuffers( 1, &IB );
	}
}

void Mesh::SubMesh::Init( const std::vector<Vertex> &vertexData, 
						const std::vector<U32> &indexData ) {
	indexCount = indexData.size();

	glGenBuffers( 1, &VB );
	glBindBuffer( GL_ARRAY_BUFFER, VB );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &IB );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IB );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( U32 ) * indexCount, &indexData[0], GL_STATIC_DRAW );
}