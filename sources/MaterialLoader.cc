#include "MaterialLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


bool wvu::ParseMTL(const std::string & ParentPath, const std::string & fileName, std::vector<struct MLTMaterial> & outMaterials)
{
	bool result = false;
	bool ErrorFlag = false;
	int chIndex = 0;
	std::ifstream inFileStream(fileName, std::ifstream::in);
	if(!inFileStream.is_open())
	{
		inFileStream.open(ParentPath + fileName, std::ifstream::in);
	}
	
	outMaterials.clear();
	
	if (inFileStream.is_open())
	{
		std::string LineStr;
		std::stringstream ssLine;
		std::string FlagStr;
		
		while (inFileStream.good())
		{
			std::getline(inFileStream, LineStr);
			ssLine.str(LineStr);
			//pick the first word.
			ssLine >> FlagStr;
			if (FlagStr.compare("newmtl") == 0)
			{
				outMaterials.push_back(MLTMaterial());
				ssLine >> (outMaterials[outMaterials.size() - 1].name);
			}
			else if (FlagStr.compare("Ka") == 0 && outMaterials.size() > 0)
			{
				ssLine >> (outMaterials[outMaterials.size() - 1].Ka.x());
				ssLine >> (outMaterials[outMaterials.size() - 1].Ka.y());
				ssLine >> (outMaterials[outMaterials.size() - 1].Ka.z());
			}
			else if (FlagStr.compare("Kd") == 0 && outMaterials.size() > 0)
			{
				ssLine >> outMaterials[outMaterials.size() - 1].Kd.x();
				ssLine >> outMaterials[outMaterials.size() - 1].Kd.y();
				ssLine >> outMaterials[outMaterials.size() - 1].Kd.z();
			}
			else if (FlagStr.compare("Ks") == 0 && outMaterials.size() > 0)
			{
				ssLine >> outMaterials[outMaterials.size() - 1].Ks.x();
				ssLine >> outMaterials[outMaterials.size() - 1].Ks.y();
				ssLine >> outMaterials[outMaterials.size() - 1].Ks.z();
			}
			else if (FlagStr.compare("Tr") == 0 && outMaterials.size() > 0)
			{
				ssLine >> outMaterials[outMaterials.size() - 1].Tr;
			}
			else if (FlagStr.compare("illum") == 0 && outMaterials.size() > 0)
			{
				ssLine >> outMaterials[outMaterials.size() - 1].illum;
			}
			else if (FlagStr.compare("Ns") == 0 && outMaterials.size() > 0)
			{
				ssLine >> outMaterials[outMaterials.size() - 1].Ns;
			}
			else if (FlagStr.compare("map_Kd") == 0 && outMaterials.size() > 0)
			{
				ssLine >> outMaterials[outMaterials.size() - 1].map_Kd;
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
