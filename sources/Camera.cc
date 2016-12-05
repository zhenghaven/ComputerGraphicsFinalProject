#include "Model.h"
#include "Camera.h"
#include "sources/camera_utils.h"
#include "sources/transformations.h"

#include <iostream>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <GL/glew.h>

Camera::Camera(Eigen::Matrix4f projection) :
	m_projection(projection)
{
	initCamera();
}

Camera::~Camera()
{
	// Nothing to do here - we don't need to free memory as all member variables
	// were declared on the stack.
}

void Camera::initCamera()
{

}

void Camera::update(double deltaTime)
{

}

const Eigen::Matrix4f Camera::GetProjection() const
{
  return m_projection;
}
