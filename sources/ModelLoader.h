
#pragma once
#ifndef MODELLOADER_HEADER
#define MODELLOADER_HEADER

#include <vector>
#include <Eigen/Core>

#define GLEW_STATIC
#include <GL/glew.h>

namespace wvu
{

	struct VertexIndices
	{
		std::vector<unsigned int> indices;

		VertexIndices()
		{
			indices.resize(3, 0);
		}

		VertexIndices(unsigned int inv, unsigned int invn, unsigned int invt)
		{
			indices.resize(3);
			indices[0] = inv;
			indices[1] = invn;
			indices[2] = invt;
		}

		void clear()
		{
			indices[0] = 0;
			indices[1] = 0;
			indices[2] = 0;
		}
	};

	struct Face
	{
		std::vector<struct VertexIndices> verts;

		Face()
		{
			verts.resize(3);
		}

		Face(VertexIndices inV1, VertexIndices inV2, VertexIndices inV3)
		{
			verts.resize(3);
			verts[0] = inV1;
			verts[1] = inV2;
			verts[2] = inV3;
		}

		void clear()
		{
			verts[0].clear();
			verts[1].clear();
			verts[2].clear();
		}
	};

	bool ParseOBJFile(const std::string & filePath, std::string & outMtlLib, std::vector<Eigen::Vector3f> & outPoints, std::vector<Eigen::Vector3f> & outNormals, std::vector<Eigen::Vector2f> & outUVs, std::vector<struct Face> & outFaces);

	void ToSingleIndices(const std::vector<Eigen::Vector3f> & inPoints, const std::vector<Eigen::Vector3f> & inNormals, const std::vector<Eigen::Vector2f> & inUVs, const std::vector<struct Face> & inFaces, Eigen::MatrixXf & outVertices, std::vector<GLuint> & outIndices, bool inIgnoreNormal, bool inIgnoreUV);

	bool GetElementsFromOBJ(const std::string & filePath, std::string & outMtlLib, Eigen::MatrixXf & outVertices, std::vector<GLuint> & outIndices, bool inIgnoreNormal, bool inIgnoreUV);
}

#endif
