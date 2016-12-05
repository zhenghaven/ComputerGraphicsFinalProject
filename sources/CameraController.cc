#include "CameraController.h"
#include "Camera.h"

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

CameraController::CameraController(GLFWwindow* glwindow, Camera* cam)
{
	window = glwindow;
	camera = cam;
}

CameraController::~CameraController()
{
	// Nothing to do here - we don't need to free memory as all member variables
	// were declared on the stack.
}

float CameraController::GetMultiplier(bool add, bool substruct, float scale)
{
	float result = 0.0f;
	if (add)
	{
		result += 1.0f;
	}
	if (substruct)
	{
		result -= 1.0f;
	}
	result += scale;
	return result;
}

void CameraController::update(float deltaTime)
{
	if(window)
	{
		moveForward = (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS);
		moveBackward = (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS);
		moveRight = (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS);
		moveLeft = (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS);
		moveUp = (glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS);
		moveDown = (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS);
	}

	Eigen::Vector3f direction = camera->GetLookDirection();
	Eigen::Vector3f offset;

	Eigen::Vector3f xVector = camera->GetUpVector().cross(direction);
	xVector.normalize();

	Eigen::Vector3f zVecttor = camera->GetUpVector();

	float step = speed * deltaTime;

	offset += direction * step * GetMultiplier(moveForward, moveBackward, speed);
	offset += direction * step * GetMultiplier(moveLeft, moveRight, speed);
	offset += direction * step * GetMultiplier(moveUp, moveDown, speed);
	
	//offset = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	if(camera)
	{
		camera->Translate(offset);
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);




}
