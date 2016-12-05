#include "Model.h"
#include "Camera.h"
#include "sources/camera_utils.h"
#include "sources/transformations.h"

#include <iostream>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <GL/glew.h>

Camera::Camera()
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

void Camera::Translate(const Eigen::Vector3f & translation)
{
  position *= wvu::ComputeTranslationMatrix(translation);
}

void Camera::Rotate(float yaw, float pitch)
{
  Eigen::Vector3f upUnitVector = position.block(0, 1, 3, 1);
  position *= wvu::ComputeRotationMatrix(upUnitVector, wvu::ConvertDegreesToRadians(yaw));

  Eigen::Vector3f horizontalUnitVector = Eigen::Vector3f(position.block(0, 0, 3, 1)) + Eigen::Vector3f(position.block(0, 2, 3, 1));
  horizontalUnitVector.normalize();
  position *= wvu::ComputeRotationMatrix(horizontalUnitVector, wvu::ConvertDegreesToRadians(pitch));
}

const Eigen::Matrix4f Camera::GetPose() const
{
  return position;
}

const Eigen::Vector3f Camera::GetUpVector() const
{
  Eigen::Vector3f upUnitVector = position.block(0, 1, 3, 1);
  return upUnitVector;
}

const Eigen::Vector3f Camera::GetLookDirection() const
{
  Eigen::Vector3f direction = Eigen::Vector3f(position.block(0, 0, 3, 1)) - Eigen::Vector3f(position.block(0, 2, 3, 1)) + Eigen::Vector3f(position.block(0, 1, 3, 1));
  direction.normalize();

  return direction;
}
