
#pragma once
#ifndef MATERIALLOADER_HEADER
#define MATERIALLOADER_HEADER

#include <Eigen/Core>
#include <string>
#include <vector>

namespace wvu
{
	struct MLTMaterial
	{
		std::string name;
		Eigen::Vector3f Ka;
		Eigen::Vector3f Kd;
		Eigen::Vector3f Ks;
		float Tr;
		float illum;
		float Ns;
		std::string map_Kd;
		
		MLTMaterial() :
			name(""),
			Ka(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
			Kd(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
			Ks(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
			Tr(0.0f),
			illum(0.0f),
			Ns(0.0f),
			map_Kd("")
		{
		}
	};
	
	bool ParseMTL(const std::string & ParentPath, const std::string & fileName, std::vector<struct MLTMaterial> & outMaterials);
}

#endif
