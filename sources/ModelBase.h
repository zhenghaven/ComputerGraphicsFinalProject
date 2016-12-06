
#pragma once
#ifndef MODELBASE_HEADER
#define MODELBASE_HEADER

#include <vector>
#include <map>

#include <Eigen/Core>


class Camera;

class ModelBase
{
public:
	ModelBase();

	~ModelBase();
	
	

	virtual void Draw(const Camera * camera);

	virtual void SetPosition(const Eigen::Vector3f & position);

	virtual void Translate(const Eigen::Vector3f & translation);

	virtual void Rotate(float yaw, float pitch, float roll);

	virtual void SetScale(float scale);
	
	

	virtual const Eigen::Vector3f GetUpVector() const;

	virtual const Eigen::Vector3f GetLookDirection() const;
	
	
	
	virtual void SetLocalPose(const Eigen::Matrix4f & pose);

	virtual const Eigen::Matrix4f GetLocalPose() const;

	virtual const Eigen::Matrix4f GetAbsolutePose() const;
	
	
	
	virtual ModelBase * GetParent() const;
	
	virtual ModelBase * GetChild(const std::string & childName) const;
	
	virtual bool AddChild(const std::string & childName, ModelBase * child);
	
	virtual bool AddChild(ModelBase * child);
	
	virtual void DetachNode();



protected:

	Eigen::Matrix4f m_relativePose;
	
	std::map<std::string, ModelBase * > m_children;
	
	ModelBase * m_parent;

};


#endif  // MODELBASE_HEADER
