
#include "ModelLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


bool wvu::ParseOBJFile(const std::string & filePath, std::string & outMtlLib, std::vector<Eigen::Vector3f> & outPoints, std::vector<Eigen::Vector3f> & outNormals, std::vector<Eigen::Vector2f> & outUVs, std::vector<struct Face> & outFaces)
{
	bool result = false;
	bool ErrorFlag = false;
	int chIndex = 0;
	std::ifstream inFileStream(filePath, std::ifstream::in);
	
	outMtlLib.clear();
	outPoints.clear();
	outNormals.clear();
	outUVs.clear();
	outFaces.clear();
	
	if (inFileStream.is_open())
	{
		std::string LineStr;
		std::stringstream ssLine;
		std::string FlagStr;
		
		Eigen::Vector2f tmpVec2D;
		Eigen::Vector3f tmpVec3D;
		std::string faceStr;
		std::string tmpStr;
		struct Face tmpFace;
		
		while (inFileStream.good())
		{
			std::getline(inFileStream, LineStr);
			ssLine.str(LineStr);
			//pick the first word.
			ssLine >> FlagStr;
			if (FlagStr.compare("mtllib") == 0)
			{
				ssLine >> outMtlLib;
			}
			else if (FlagStr.compare("v") == 0)
			{
				ssLine >> tmpVec3D.x();
				ssLine >> tmpVec3D.y();
				ssLine >> tmpVec3D.z();
				outPoints.push_back(tmpVec3D);
			}
			else if (FlagStr.compare("vn") == 0)
			{
				ssLine >> tmpVec3D.x();
				ssLine >> tmpVec3D.y();
				ssLine >> tmpVec3D.z();
				outNormals.push_back(tmpVec3D);
			}
			else if (FlagStr.compare("vt") == 0)
			{
				ssLine >> tmpVec2D.x();
				ssLine >> tmpVec2D.y();
				outUVs.push_back(tmpVec2D);
			}
			else if (FlagStr.compare("f") == 0)
			{
				faceStr.clear();
				tmpFace.clear();

				for(int i = 0; i < 3 && ssLine.good(); ++i)
				{
					ssLine >> faceStr;
					std::stringstream faceStream(faceStr);
					for(int j = 0; j < 3 && faceStream.good(); ++j)
					{
						std::getline(faceStream, tmpStr, '/');
						tmpFace.verts[i].indices[j] = std::stoi(tmpStr);
					}
				}
				
				outFaces.push_back(tmpFace);
			}
			LineStr.clear();
			FlagStr.clear();
			ssLine.clear();
			ssLine.str("");
		}

		result = true;
		inFileStream.close();
	}
	else
	{
		result = false;
	}

	return result;
}

struct Vertices
{
	Eigen::Vector3f pos;
	Eigen::Vector3f nor;
	Eigen::Vector2f uv;

	Vertices() :
			pos(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
			nor(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
			uv(Eigen::Vector2f(0.0f, 0.0f))
	{

	}

	bool operator== (const Vertices & other)
	{
		return (pos == other.pos) && (nor == other.nor) && (uv == other.uv);
	}
};

void wvu::ToSingleIndices(const std::vector<Eigen::Vector3f> & inPoints, const std::vector<Eigen::Vector3f> & inNormals, const std::vector<Eigen::Vector2f> & inUVs, const std::vector<struct Face> & inFaces, Eigen::MatrixXf & outVertices, std::vector<GLuint> & outIndices, bool inIgnoreNormal, bool inIgnoreUV)
{
	outIndices.clear();
	std::vector<struct Vertices> OriVertices(inPoints.size());
	std::vector<bool> IsFilled(inPoints.size(), false);

	std::vector<struct Vertices> AddVertices;
	struct Vertices tmpVertex;
	for(int i = 0; i < inFaces.size(); ++i)
	{
		for(int j = 0; j < 3 && inFaces[i].verts.size() >= 3; ++j)
		{
			tmpVertex.pos = inPoints[(inFaces[i].verts[j].indices[0]) - 1];
			if(!inIgnoreUV)
			{
				tmpVertex.uv = inUVs[(inFaces[i].verts[j].indices[1]) - 1];
			}
			if(!inIgnoreNormal)
			{
				tmpVertex.nor = inNormals[(inFaces[i].verts[j].indices[2]) - 1];
			}

			if(IsFilled[(inFaces[i].verts[j].indices[0]) - 1])
			{
				if(OriVertices[(inFaces[i].verts[j].indices[0]) - 1] == tmpVertex)
				{
					outIndices.push_back((inFaces[i].verts[j].indices[0]) - 1);
				} else
				{
					int foundedPos = -1;
					for(int k = 0; k < AddVertices.size(); ++k)
					{
						if(AddVertices[k] == tmpVertex)
							foundedPos = k;
					}
					if(foundedPos >= 0)
					{
						outIndices.push_back(OriVertices.size() + foundedPos);
					} else
					{
						AddVertices.push_back(tmpVertex);
						outIndices.push_back(OriVertices.size() + AddVertices.size() - 1);
					}
				}
			} else
			{
				OriVertices[(inFaces[i].verts[j].indices[0]) - 1] = tmpVertex;
				outIndices.push_back((inFaces[i].verts[j].indices[0]) - 1);
				IsFilled[(inFaces[i].verts[j].indices[0]) - 1] = true;
			}
		}
	}

	unsigned int sizePerVertex = 3 + (inIgnoreUV ? 0 : 2) + (inIgnoreNormal ? 0 : 3);
	unsigned int VerticesTotalSize = (OriVertices.size() + AddVertices.size());
	outVertices.resize(sizePerVertex, VerticesTotalSize);

	for( int i = 0 ; i < OriVertices.size(); ++i)
	{
		outVertices.block(0, i, 3, 1) = OriVertices[i].pos;
		if(!inIgnoreUV)
		{
			outVertices.block(3, i, 2, 1) = OriVertices[i].uv;
		}
		if(!inIgnoreNormal && !inIgnoreUV)
		{
			outVertices.block(5, i, 3, 1) = OriVertices[i].nor;
		} else if(!inIgnoreNormal && inIgnoreUV)
		{
			outVertices.block(3, i, 3, 1) = OriVertices[i].nor;
		}
	}
	for(int i = 0; i < AddVertices.size(); ++i)
	{
		outVertices.block(0, OriVertices.size() + i, 3, 1) = AddVertices[i].pos;
		if(!inIgnoreUV)
		{
			outVertices.block(3, OriVertices.size() + i, 2, 1) = AddVertices[i].uv;
		}
		if(!inIgnoreNormal && !inIgnoreUV)
		{
			outVertices.block(5, OriVertices.size() + i, 3, 1) = AddVertices[i].nor;
		} else if(!inIgnoreNormal && inIgnoreUV)
		{
			outVertices.block(3, OriVertices.size() + i, 3, 1) = AddVertices[i].nor;
		}
	}
	
	std::cout << outVertices << std::endl << std::endl;
	for(int i = 0; i < outIndices.size(); ++i)
	{
		if(i % 3 == 0)
			std::cout << std::endl;
		std::cout << outIndices[i] << " ";
	}
}

bool wvu::GetElementsFromOBJ(const std::string & filePath, std::string & outMtlLib, Eigen::MatrixXf & outVertices, std::vector<GLuint> & outIndices, bool inIgnoreNormal, bool inIgnoreUV)
{
	std::vector<Eigen::Vector3f> Points;
	std::vector<Eigen::Vector3f> Normals;
	std::vector<Eigen::Vector2f> UVs;
	std::vector<wvu::Face> Faces;
	wvu::ParseOBJFile(filePath, outMtlLib, Points, Normals, UVs, Faces);
	wvu::ToSingleIndices(Points, Normals, UVs, Faces, outVertices, outIndices, inIgnoreNormal, inIgnoreUV);
}
