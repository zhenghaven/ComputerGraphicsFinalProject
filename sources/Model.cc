
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
	vertex_buffer_object_id_(0),
	vertex_array_object_id_(0),
	element_buffer_object_id_(0),
	m_shader(nullptr),
	m_material(nullptr)
{

}

Model::Model(const Eigen::MatrixXf& vertices) :
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
	
	for (auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		if(it->second)
		{
			delete it->second;
		}
	}
	m_children.clear();
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

void Model::Draw(const Camera * camera)
{
	if(m_shader && m_material && camera)
	{
		m_shader->Use();
		m_shader->SetUniformIfExistMatrix4fv("view", camera->GetLocalPose().data());
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

	
	for (auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		if(it->second)
		{
			it->second->Draw(camera);
		}
	}
}

const Eigen::MatrixXf& Model::GetVertices() const
{
	return m_vertices;
}

const std::vector<GLuint>& Model::GetIndices() const
{
	return m_indices;
}
