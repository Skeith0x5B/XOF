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
	mIsLoaded = false;
	normals = false;
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
		aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_FlipWindingOrder | aiProcess_CalcTangentSpace );

	if( scene ) {
		mSubMeshes.resize( scene->mNumMeshes );

		for( U32 i=0; i<scene->mNumMeshes; ++i ) {
			std::vector<Vertex> tempVertexData;
			std::vector<U32> tempIndexData;

			aiMesh *sceneMesh = scene->mMeshes[i];
			// temp vertex data
			for( U32 j = 0; j < sceneMesh->mNumVertices; ++j ) {
				tempVertexData.push_back( Vertex( 
					// Pos
					glm::vec3( sceneMesh->mVertices[j].x, sceneMesh->mVertices[j].y, sceneMesh->mVertices[j].z ),
					// Texture
					glm::vec2( sceneMesh->mTextureCoords[0][j].x, sceneMesh->mTextureCoords[0][j].y ), // SET TO ZERO FOR EDITOR
					// Normal
					glm::vec3( sceneMesh->mNormals[j].x, sceneMesh->mNormals[j].y, sceneMesh->mNormals[j].z ),
					// Tangent
					glm::vec3( sceneMesh->mTangents[j].x, sceneMesh->mTangents[j].y, sceneMesh->mTangents[j].z ) ) ); // SET TO ZERO FOR EDITOR
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

std::vector<Mesh::SubMesh>& Mesh::GetSubMeshData() const {
	return const_cast<std::vector<Mesh::SubMesh>&>(mSubMeshes);
}

U32	Mesh::GetSubMeshCount() const {
	return mSubMeshes.size();
}

std::vector<Texture>& Mesh::GetTextureData( XOF_TEXTURE_TYPE type ) const {
	return const_cast<std::vector<Texture>&>( mTextures[type] );
}

U32 Mesh::GetTextureCount( XOF_TEXTURE_TYPE type ) const {
	return mTextures[type].size();
}

void Mesh::InitMaterials( const aiScene *scene ) {
	mTextures[XOF_TEXTURE_TYPE::DIFFUSE].resize( scene->mNumMaterials );
	mTextures[XOF_TEXTURE_TYPE::NORMAL].resize( scene->mNumMaterials );

	for( U32 i=0; i<scene->mNumMaterials; ++i ) {
		const aiMaterial *material = scene->mMaterials[i];

		// Diffuse maps
		if( material->GetTextureCount( aiTextureType_DIFFUSE ) > 0 ) {
			aiString path;

			if( material->GetTexture( aiTextureType_DIFFUSE, 0, &path, 
				nullptr, nullptr, nullptr, nullptr, nullptr ) == AI_SUCCESS ) {
				if( !mTextures[XOF_TEXTURE_TYPE::DIFFUSE][i].Load( "./Resources/" + std::string( path.data ) ) ) {
					mTextures[XOF_TEXTURE_TYPE::DIFFUSE][i].Load( "./Resources/UVGrid.jpg" );
				}
			}
		}

		// Normal maps
		if( material->GetTextureCount( aiTextureType_HEIGHT ) > 0 ) {
			aiString path;

			std::cerr << "----------------- ATTEMPTING TO LOAD NORMAL MAP..." << path.data << std::endl;
			if( material->GetTexture( aiTextureType_HEIGHT, 0, &path, 
				nullptr, nullptr, nullptr, nullptr, nullptr ) == AI_SUCCESS ) {
				if( !mTextures[XOF_TEXTURE_TYPE::NORMAL][i].Load( "./Resources/" + std::string( path.data ) ) ) {
					std::cerr << "ERROR: Couldn't load normal map: " << path.data << std::endl;
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