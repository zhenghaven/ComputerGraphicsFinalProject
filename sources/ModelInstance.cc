
#include "ModelInstance.h"

#include <cstdlib>
#include <ctime>

#include <iostream>
#include <string>

#include <Eigen/Core>
#include <Eigen/Geometry>


#include "Model.h"
#include "Camera.h"
#include "transformations.h"


ModelInstance::ModelInstance() :
	m_actualModel(new Model())
{
}

ModelInstance::ModelInstance(const Eigen::MatrixXf& vertices) :
	m_actualModel(new Model(vertices))
{

}

ModelInstance::ModelInstance(const Eigen::MatrixXf& vertices, const std::vector<GLuint>& indices) :
	m_actualModel(new Model(vertices, indices))
{

}

ModelInstance::ModelInstance(const std::string &parentPath, const std::string &OBJfileName) :
	m_actualModel(new Model(parentPath, OBJfileName))
{

}

ModelInstance::ModelInstance(const ModelInstance & other) :
	m_actualModel(other.GetRealModel())
{

}

ModelInstance::~ModelInstance()
{
	for (auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		if(it->second)
		{
			delete it->second;
		}
	}
	m_children.clear();
}

std::shared_ptr<Model> ModelInstance::GetRealModel() const
{
	return m_actualModel;
}

ModelInstance * ModelInstance::CreateNewInstance() const
{
	return new ModelInstance(*this);
}

void ModelInstance::Draw(const Camera * camera)
{
	if(camera)
	{
		m_actualModel->SetLocalPose(GetAbsolutePose());
		m_actualModel->Draw(camera);
	}

	for (auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		if(it->second)
		{
			it->second->Draw(camera);
		}
	}
}
