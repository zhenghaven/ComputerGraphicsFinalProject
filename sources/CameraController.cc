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
	offset << 0, 0, 0;
	front << 0, 0, -1;
	up << 0, 1, 0;
}

CameraController::~CameraController()
{
	// Nothing to do here - we don't need to free memory as all member variables
	// were declared on the stack.
}


void CameraController::update(float deltaTime)
{
	speed = 5.0 * deltaTime;
	if(window)
	{
		if(glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS){
			offset -= speed * front;
		}
		if(glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS){
			offset += speed * front;
		}
		if(glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
			offset -= (front.cross(up)).normalized() * speed;
		}
		if(glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
			offset += (front.cross(up)).normalized() * speed;
		}
		if(glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS){
			offset -= speed * up;
		}
		if(glfwGetKey(window, GLFW_KEY_E ) == GLFW_PRESS){
			offset += speed * up;
		}
	}
	else{
		printf("Error no window\n");
		return;
	}

	if(camera)
	{
		camera->Translate(offset);
		offset << 0,0,0;
		front << 0,0,-1;
		up << 0,1,0;
	}
	else{
		printf("Error: no camera\n");
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);




}
