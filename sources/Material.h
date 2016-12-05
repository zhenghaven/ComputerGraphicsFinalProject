
#pragma once
#ifndef MATERIAL_HEADER
#define MATERIAL_HEADER


#include <string>

#include <Eigen/Core>


#include "MaterialLoader.h"

class Texture;
class ShaderProgram;

class Material
{
public:
	Material(const wvu::MLTMaterial & materialInfo, const std::string & parentPath = "");

	Material();

	Material(const Eigen::Vector4f & m_baseColor);
	
	bool IsValid() const;
	
	void BindMaterial(ShaderProgram * shader) const;
	
	void UnBindMaterial() const;



private:

	
	std::string m_name;
	Eigen::Vector3f m_Ka;
	Eigen::Vector3f m_Kd;
	Eigen::Vector3f m_Ks;
	float m_Tr;
	float m_illum;
	float m_Ns;
	Texture * m_map_Kd;

	Eigen::Vector4f m_baseColor;
	
};


#endif
