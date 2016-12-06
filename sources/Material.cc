
#include "Material.h"
#include "Texture.h"
#include "ShaderProgram.h"


Material::Material(const wvu::MLTMaterial & materialInfo, const std::string & parentPath) :
	m_name(materialInfo.name),
	m_Ka(materialInfo.Ka),
	m_Kd(materialInfo.Kd),
	m_Ks(materialInfo.Ks),
	m_Tr(materialInfo.Tr),
	m_illum(materialInfo.illum),
	m_Ns(materialInfo.Ns),
	m_baseColor(Eigen::Vector4f(1.0f, 0.0f, 0.0f, 1.0f))
{
	std::string filePath = materialInfo.map_Kd;
	if(!wvu::IsFileExist(filePath))
	{
		filePath = parentPath + filePath;
	}
	
	
	if(!wvu::IsFileExist(filePath))
	{
		m_map_Kd == nullptr;
	}
	else
	{
		m_map_Kd = new Texture(filePath);
	}
	
}

Material::~Material()
{
	if(m_map_Kd)
		delete m_map_Kd;
}
	
bool Material::IsValid() const
{
	return m_map_Kd != nullptr;
}

void Material::BindMaterial(ShaderProgram * shader) const
{
	if(IsValid())
	{
		shader->SetUniformIfExist1f("u_useTexture", 1.0f);
		m_map_Kd->BindTexture();
	} else
	{
		shader->SetUniformIfExist1f("u_useTexture", 0.0f);
		shader->SetUniformIfExist4fv("u_baseColor", m_baseColor.data());
	}
}

void Material::UnBindMaterial() const
{
	if(IsValid())
	{
		m_map_Kd->UnBindTexture();
	}
}

Material::Material() :
	m_name(""),
	m_Ka(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
	m_Kd(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
	m_Ks(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
	m_Tr(0.0f),
	m_illum(0.0f),
	m_Ns(0.0f),
	m_map_Kd(nullptr),
	m_baseColor(Eigen::Vector4f(1.0f, 0.0f, 0.0f, 1.0f))
{

}

Material::Material(const Eigen::Vector4f & m_baseColor) :
		m_name(""),
		m_Ka(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
		m_Kd(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
		m_Ks(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
		m_Tr(0.0f),
		m_illum(0.0f),
		m_Ns(0.0f),
		m_map_Kd(nullptr),
		m_baseColor(m_baseColor)
{

}
