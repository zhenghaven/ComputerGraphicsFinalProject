
#include "ModelBase.h"

#include <cstdlib>
#include <ctime>

#include <iostream>
#include <string>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "transformations.h"


ModelBase::ModelBase() :
	m_parent(nullptr),
	m_relativePose(Eigen::Matrix4f::Identity())
{
}

ModelBase::~ModelBase()
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

void ModelBase::Draw(const Camera * camera)
{
	for (auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		if(it->second)
		{
			it->second->Draw(camera);
		}
	}
}

void ModelBase::SetPosition(const Eigen::Vector3f& position)
{
	if(position.hasNaN())
	{
		return;
	}
	m_relativePose.block(0, 3, 3, 1) = position;
}

void ModelBase::Translate(const Eigen::Vector3f & translation)
{
	m_relativePose = wvu::ComputeTranslationMatrix(translation) * m_relativePose;
}

void ModelBase::Rotate(float yaw, float pitch, float roll)
{
	Eigen::Matrix3f transform3f(Eigen::Matrix3f::Identity());
	if(yaw != 0)
	{
		Eigen::AngleAxisf rotationYaw(wvu::ConvertDegreesToRadians(yaw), Eigen::Vector3f::UnitY());
		transform3f = (rotationYaw.matrix() * transform3f);
	}
	if(pitch != 0)
	{
		Eigen::AngleAxisf rotationPitch(wvu::ConvertDegreesToRadians(pitch), Eigen::Vector3f::UnitX());
		transform3f = (rotationPitch.matrix() * transform3f);
	}
	if(roll != 0)
	{
		Eigen::AngleAxisf rotationRoll(wvu::ConvertDegreesToRadians(roll), Eigen::Vector3f::UnitZ());
		transform3f = (rotationRoll.matrix() * transform3f);
	}

	Eigen::Matrix4f transform(Eigen::Matrix4f::Identity());
	transform.block(0, 0, 3, 3) = transform3f;
	m_relativePose = transform * m_relativePose;
}

void ModelBase::SetScale(float scale)
{
	m_relativePose = wvu::ComputeScalingMatrix(scale) * m_relativePose;
}



const Eigen::Vector3f ModelBase::GetUpVector() const
{
	Eigen::Vector3f upUnitVector = m_relativePose.block(0, 1, 3, 1);
	return upUnitVector;
}

const Eigen::Vector3f ModelBase::GetLookDirection() const
{
	Eigen::Vector3f direction = Eigen::Vector3f(m_relativePose.block(0, 0, 3, 1)) - Eigen::Vector3f(m_relativePose.block(0, 2, 3, 1)) + Eigen::Vector3f(m_relativePose.block(0, 1, 3, 1));
	direction.normalize();

	return direction;
}

void ModelBase::SetLocalPose(const Eigen::Matrix4f & pose)
{
	if(pose.hasNaN())
	{
		return;
	}
	m_relativePose = pose;
}

const Eigen::Matrix4f ModelBase::GetLocalPose() const
{
	return m_relativePose;
}

const Eigen::Matrix4f ModelBase::GetAbsolutePose() const
{
	ModelBase * parent = GetParent();
	if(parent)
	{
		return (parent->GetAbsolutePose() * GetLocalPose());
	}
	else
	{
		return GetLocalPose();
	}
}




ModelBase * ModelBase::GetParent() const
{
	return m_parent;
}

ModelBase * ModelBase::GetChild(const std::string & childName) const
{
	auto found = m_children.find(childName);
	if(found != m_children.end())
	{
		return found->second;
	}
	else
	{
		return nullptr;
	}
}

bool ModelBase::AddChild(const std::string & childName, ModelBase * child)
{
	auto found = m_children.find(childName);
	if(found != m_children.end())
	{
		return false;
	}
	
	m_children[childName] = child;
	child->m_parent = this;
}

bool ModelBase::AddChild(ModelBase * child)
{
	srand(time(NULL));
	std::string randStr = std::to_string(rand());
	randStr = "child_" + randStr;
	
	return AddChild(randStr, child);
}

void ModelBase::DetachNode()
{
	if (m_parent != nullptr)
	{
		for (auto it = m_parent->m_children.begin(); it != m_parent->m_children.end(); ++it)
		{
			ModelBase* model = it->second;
			if (model == this)
			{
				m_parent->m_children.erase(it);
				break;
			}
		}
		m_parent = nullptr;
	}
}
