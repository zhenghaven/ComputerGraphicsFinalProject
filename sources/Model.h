
#pragma once
#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <vector>
#include <Eigen/Core>
#include <GL/glew.h>

//#include "ShaderProgram.h"

class ShaderProgram;
class Material;
class Camera;

class Model
{
public:
	Model();

	Model(const Eigen::MatrixXf& vertices);

	Model(const Eigen::MatrixXf& vertices, const std::vector<GLuint>& indices);

	Model(const std::string & parentPath, const std::string & OBJfileName);

	~Model();

	void SetShaderProgram(ShaderProgram * shader);

	void SetMaterial(Material * material);

	void Draw(const Camera * camera);

	void SetOrientation(const Eigen::Vector3f & orientation);

	void SetPosition(const Eigen::Vector3f & position);

	//const Eigen::Vector3f& GetOrientation() const;

	//const Eigen::Vector3f& GetPosition() const;

	const Eigen::MatrixXf& GetVertices() const;

	const std::vector<GLuint>& GetIndices() const;

private:

	//Eigen::Vector3f orientation;

	//Eigen::Vector3f position;

	Eigen::Matrix4f m_relativeLoc;

	Eigen::MatrixXf m_vertices;

	std::vector<GLuint> m_indices;

	ShaderProgram * m_shader;

	Material * m_material;


	GLuint vertex_buffer_object_id_;

	GLuint vertex_array_object_id_;

	GLuint element_buffer_object_id_;
};


#endif  // MODEL_HEADER
