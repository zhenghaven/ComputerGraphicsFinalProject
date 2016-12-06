
#pragma once
#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <vector>
#include <map>

#include <Eigen/Core>
#include <GL/glew.h>

#include "ModelBase.h"

class ShaderProgram;
class Material;
class Camera;

class Model : public ModelBase
{
public:
	Model();

	Model(const Eigen::MatrixXf& vertices);

	Model(const Eigen::MatrixXf& vertices, const std::vector<GLuint>& indices);

	Model(const std::string & parentPath, const std::string & OBJfileName);

	~Model();

	virtual void SetShaderProgram(ShaderProgram * shader);

	virtual void SetMaterial(Material * material);

	virtual void Draw(const Camera * camera) override;

	virtual const Eigen::MatrixXf& GetVertices() const;

	virtual const std::vector<GLuint>& GetIndices() const;

private:

	Eigen::MatrixXf m_vertices;

	std::vector<GLuint> m_indices;

	ShaderProgram * m_shader;

	Material * m_material;


	GLuint vertex_buffer_object_id_;

	GLuint vertex_array_object_id_;

	GLuint element_buffer_object_id_;
};


#endif  // MODEL_HEADER
