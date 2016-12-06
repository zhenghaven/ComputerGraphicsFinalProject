
#pragma once
#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <vector>
#include <map>

#include <Eigen/Core>
#include <GL/glew.h>

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

	void SetPosition(const Eigen::Vector3f & position);

	//const Eigen::Vector3f& GetOrientation() const;

	//const Eigen::Vector3f& GetPosition() const;

	const Eigen::MatrixXf& GetVertices() const;

	const std::vector<GLuint>& GetIndices() const;

	void Translate(const Eigen::Vector3f & translation);

	const Eigen::Vector3f GetUpVector() const;

	const Eigen::Matrix4f GetLocalPose() const;

	const Eigen::Matrix4f GetAbsolutePose() const;
	
	Model * GetParent() const;
	
	Model * GetChild(const std::string & childName) const;
	
	bool AddChild(const std::string & childName, Model * child);
	
	bool AddChild(Model * child);

	const Eigen::Vector3f GetLookDirection() const;

	void Rotate(float yaw, float pitch, float roll);

	void SetScale(float scale);

protected:

	Eigen::Matrix4f m_relativePose;


private:

	std::map<std::string, Model * > m_children;
	
	Model * m_parent;

	Eigen::MatrixXf m_vertices;

	std::vector<GLuint> m_indices;

	ShaderProgram * m_shader;

	Material * m_material;


	GLuint vertex_buffer_object_id_;

	GLuint vertex_array_object_id_;

	GLuint element_buffer_object_id_;
};


#endif  // MODEL_HEADER
