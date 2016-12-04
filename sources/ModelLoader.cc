
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


