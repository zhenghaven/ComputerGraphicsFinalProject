
#include "Model.h"

#include <cstdlib>
#include <ctime>

#include <iostream>
#include <string>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <GL/glew.h>

#include "ShaderProgram.h"
#include "Material.h"
#include "ModelLoader.h"
#include "MaterialLoader.h"
#include "Camera.h"
#include "transformations.h"


static inline void SetVertexBufferObject(GLuint & vertex_buffer_object_id, const Eigen::MatrixXf & vertices)
{
	glGenBuffers(1, &vertex_buffer_object_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_id);

	const int vertices_size_in_bytes = vertices.rows() * vertices.cols() * sizeof(vertices(0, 0));
	glBufferData(GL_ARRAY_BUFFER, vertices_size_in_bytes, vertices.data(), GL_STATIC_DRAW);

	GLuint kIndex = 0;
	constexpr GLuint kNumElementsPerVertex = 5;
	constexpr GLuint kStride = kNumElementsPerVertex * sizeof(vertices(0, 0));
	int offset_ptr = 0;
	glVertexAttribPointer(kIndex, 3, GL_FLOAT, GL_FALSE, kStride, reinterpret_cast<GLvoid *>(offset_ptr));
	glEnableVertexAttribArray(kIndex);

	kIndex = 1;
	offset_ptr = 3 * sizeof(vertices(0, 0));
	glVertexAttribPointer(kIndex, 2, GL_FLOAT, GL_FALSE, kStride, reinterpret_cast<GLvoid *>(offset_ptr));
	glEnableVertexAttribArray(kIndex);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static inline void SetElementBufferObject(GLuint & element_buffer_object_id, const std::vector<GLuint> & indices)
{
	glGenBuffers(1, &element_buffer_object_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_id);

	const int indices_size_in_bytes = indices.size() * sizeof(indices[0]);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size_in_bytes, indices.data(), GL_STATIC_DRAW);
}

static inline void SetVertexArrayObject(GLuint & vertex_array_object_id, GLuint & vertex_buffer_object_id, GLuint & element_buffer_object_id, const Eigen::MatrixXf & vertices, const std::vector<GLuint> & indices)
{
	constexpr int kNumVertexArrays = 1;

	glGenVertexArrays(kNumVertexArrays, &vertex_array_object_id);

	glBindVertexArray(vertex_array_object_id);

	SetVertexBufferObject(vertex_buffer_object_id, vertices);
	SetElementBufferObject(element_buffer_object_id, indices);

	glBindVertexArray(0);
}

Model::Model() :
	m_parent(nullptr),
	m_relativePose(Eigen::Matrix4f::Identity()),
	vertex_buffer_object_id_(0),
	vertex_array_object_id_(0),
	element_buffer_object_id_(0),
	m_shader(nullptr),
	m_material(nullptr)
{

}

Model::Model(const Eigen::MatrixXf& vertices) :
	m_parent(nullptr),
	m_relativePose(Eigen::Matrix4f::Identity()),
	m_vertices(vertices),
	vertex_buffer_object_id_(0),
	vertex_array_object_id_(0),
	element_buffer_object_id_(0),
	m_shader(nullptr),
	m_material(nullptr)
{
	SetVertexArrayObject(vertex_array_object_id_, vertex_buffer_object_id_, element_buffer_object_id_, m_vertices, m_indices);
}

Model::Model(const Eigen::MatrixXf& vertices, const std::vector<GLuint>& indices) :
	m_parent(nullptr),
	m_relativePose(Eigen::Matrix4f::Identity()),
	m_vertices(vertices),
	m_indices(indices),
	vertex_buffer_object_id_(0),
	vertex_array_object_id_(0),
	element_buffer_object_id_(0),
	m_shader(nullptr),
	m_material(nullptr)
{
	SetVertexArrayObject(vertex_array_object_id_, vertex_buffer_object_id_, element_buffer_object_id_, m_vertices, m_indices);
}

Model::Model(const std::string &parentPath, const std::string &OBJfileName) :
	m_parent(nullptr),
	m_relativePose(Eigen::Matrix4f::Identity()),
	vertex_buffer_object_id_(0),
	vertex_array_object_id_(0),
	element_buffer_object_id_(0),
	m_shader(nullptr),
	m_material(nullptr)
{
	std::string MTLPath;
	Eigen::MatrixXf Vertices;
	std::vector<GLuint> Indices;
	wvu::GetElementsFromOBJ(parentPath + OBJfileName, MTLPath, Vertices, Indices, true, false);
	std::vector<wvu::MLTMaterial> materials;
	wvu::ParseMTL(parentPath, MTLPath, materials);
	m_vertices = Vertices;
	m_indices = Indices;
/*
	std::cout << m_vertices << std::endl << std::endl;
	for(int i = 0; i < m_indices.size(); ++i)
	{
		if(i % 3 == 0)
			std::cout << std::endl;
		std::cout << m_indices[i] << " ";
	}
*/
	SetVertexArrayObject(vertex_array_object_id_, vertex_buffer_object_id_, element_buffer_object_id_, m_vertices, m_indices);
	Material * mat = new Material(materials[0], parentPath);
	m_material = mat;
}

Model::~Model()
{
	if(vertex_array_object_id_ > 0)
		glDeleteVertexArrays(1, &vertex_array_object_id_);
	if(vertex_buffer_object_id_ > 0)
		glDeleteBuffers(1, &vertex_buffer_object_id_);
	if(element_buffer_object_id_ > 0)
		glDeleteBuffers(1, &element_buffer_object_id_);

	if(m_shader)
		delete m_shader;
	if(m_material)
		delete m_material;
}

void Model::SetShaderProgram(ShaderProgram * shader)
{
	if(m_shader)
		delete m_shader;
	m_shader = shader;
}

void Model::SetMaterial(Material * material)
{
	if(m_material)
		delete m_material;
	m_material = material;
}

void Model::SetPosition(const Eigen::Vector3f& position)
{
	if(position.hasNaN())
	{
		return;
	}
	m_relativePose.block(0, 3, 3, 1) = position;
}

void Model::Draw(const Camera * camera)
{
	if(!m_shader || !m_material || !camera)
		return;

	//const Eigen::Matrix4f model = ComputeModelMatrix();
	m_shader->Use();
	m_shader->SetUniformIfExistMatrix4fv("view", camera->GetAbsolutePose().data());
	m_shader->SetUniformIfExistMatrix4fv("projection", camera->GetProjection().data());
	m_shader->SetUniformIfExistMatrix4fv("model", GetAbsolutePose().data());

	m_material->BindMaterial(m_shader);
	if(m_vertices.size() > 0 && m_indices.size() == 0)
	{
		glBindVertexArray(vertex_array_object_id_);
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
		glBindVertexArray(0);
	}
	else if(m_vertices.size() > 0 && m_indices.size() > 0)
	{
		glBindVertexArray(vertex_array_object_id_);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	m_material->UnBindMaterial();
}

const Eigen::MatrixXf& Model::GetVertices() const
{
	return m_vertices;
}

const std::vector<GLuint>& Model::GetIndices() const
{
	return m_indices;
}

void Model::Translate(const Eigen::Vector3f & translation)
{
	m_relativePose = wvu::ComputeTranslationMatrix(translation) * m_relativePose;
}

const Eigen::Vector3f Model::GetUpVector() const
{
	Eigen::Vector3f upUnitVector = m_relativePose.block(0, 1, 3, 1);
	return upUnitVector;
}

const Eigen::Vector3f Model::GetLookDirection() const
{
	Eigen::Vector3f direction = Eigen::Vector3f(m_relativePose.block(0, 0, 3, 1)) - Eigen::Vector3f(m_relativePose.block(0, 2, 3, 1)) + Eigen::Vector3f(m_relativePose.block(0, 1, 3, 1));
	direction.normalize();

	return direction;
}

const Eigen::Matrix4f Model::GetLocalPose() const
{
	return m_relativePose;
}

const Eigen::Matrix4f Model::GetAbsolutePose() const
{
	Model * parent = GetParent();
	if(parent)
	{
		return (parent->GetAbsolutePose() * GetLocalPose());
	}
	else
	{
		return GetLocalPose();
	}
}

Model * Model::GetParent() const
{
	return m_parent;
}

Model * Model::GetChild(const std::string & childName) const
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

bool Model::AddChild(const std::string & childName, Model * child)
{
	auto found = m_children.find(childName);
	if(found != m_children.end())
	{
		return false;
	}
	
	m_children[childName] = child;
}

bool Model::AddChild(Model * child)
{
	srand(time(NULL));
	std::string randStr = std::to_string(rand());
	randStr = "child_" + randStr;
	
	return AddChild(randStr, child);
}

void Model::Rotate(float yaw, float pitch, float roll)
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

void Model::SetScale(float scale)
{
	m_relativePose = wvu::ComputeScalingMatrix(scale) * m_relativePose;
}
