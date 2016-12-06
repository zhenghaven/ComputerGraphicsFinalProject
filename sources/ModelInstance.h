
#pragma once
#ifndef MODELINSTANCE_HEADER
#define MODELINSTANCE_HEADER

#include <vector>
#include <map>
#include <memory>

#include <Eigen/Core>
#define GLEW_STATIC
#include <GL/glew.h>


#include "ModelBase.h"

class Model;
class Camera;

class ModelInstance : public ModelBase
{
public:
	ModelInstance();

	ModelInstance(const Eigen::MatrixXf& vertices);

	ModelInstance(const Eigen::MatrixXf& vertices, const std::vector<GLuint>& indices);

	ModelInstance(const std::string & parentPath, const std::string & OBJfileName);

	ModelInstance(const ModelInstance & other);

	~ModelInstance();
	
	virtual std::shared_ptr<Model> GetRealModel() const;
	
	virtual ModelInstance * CreateNewInstance() const;
	
	

	virtual void Draw(const Camera * camera) override;

private:

	ModelInstance(const ModelInstance * other);
	
	std::shared_ptr<Model> m_actualModel;
};


#endif  // MODELINSTANCE_HEADER
