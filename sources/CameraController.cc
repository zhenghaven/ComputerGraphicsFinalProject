#include "CameraController.h"
#include "Camera.h"

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "transformations.h"

CameraController::CameraController(GLFWwindow* glwindow, Camera* cam)  : 
	offset(Eigen::Vector3f(0.0f, 0.0f, 0.0f)),
	front(Eigen::Vector3f(0.0f, 0.0f, -1.0f)),
	up(Eigen::Vector3f(0.0f, 1.0f, 0.0f)),
	speed(1.0f),
	window(glwindow),
	camera(cam),
	m_wasMousePressed(false),
	m_lastMouseX(0.0),
	m_lastMouseY(0.0)
{

}

CameraController::~CameraController()
{
	// Nothing to do here - we don't need to free memory as all member variables
	// were declared on the stack.
}


void CameraController::update(float deltaTime)
{
	speed = 5.0 * deltaTime;
	
	offset = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	front = Eigen::Vector3f(0.0f, 0.0f, -1.0f);
	up = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
	
	if(window)
	{
		if(glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS)
		{
			offset -= speed * front;
		}
		
		if(glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS)
		{
			offset += speed * front;
		}
		
		if(glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS)
		{
			offset -= (front.cross(up)).normalized() * speed;
		}
		
		if(glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS)
		{
			offset += (front.cross(up)).normalized() * speed;
		}
		
		if(glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS)
		{
			offset -= speed * up;
		}
		
		if(glfwGetKey(window, GLFW_KEY_E ) == GLFW_PRESS)
		{
			offset += speed * up;
		}
		
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS)
		{
			if(m_wasMousePressed)
			{
				double currentMouseX, currentMouseY = 0.0;
				glfwGetCursorPos(window, &currentMouseX, &currentMouseY);
				float MouseOffsetX = wvu::ConvertDegreesToRadians(currentMouseX - m_lastMouseX);
				float MouseOffsetY = wvu::ConvertDegreesToRadians(currentMouseY - m_lastMouseY);
				m_lastMouseX = currentMouseX;
				m_lastMouseY = currentMouseY;
				if(camera)
				{
					camera->Rotate(2 * MouseOffsetX, 2 * MouseOffsetY, 0.0f);
				}
			}
			else
			{
				glfwGetCursorPos(window, &m_lastMouseX, &m_lastMouseY);
				m_wasMousePressed = true;
				
			}
		}
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_RELEASE)
		{
			m_wasMousePressed = false;
		}
	}
	else{
		printf("Error no window\n");
		return;
	}

	if(camera)
	{
		camera->Translate(offset);
	}
	else{
		printf("Error: no camera\n");
	}


}
