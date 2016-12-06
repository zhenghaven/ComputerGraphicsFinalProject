#define _USE_MATH_DEFINES

#include <cmath>
#include <math.h>
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
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	xOffset *= 0.005;
	yOffset *= 0.005;

	lastY = ypos;
	lastX = xpos;

	yaw   += xOffset;
	pitch += yOffset;

	if(pitch > 89.0f)
  	pitch =  89.0f;
	if(pitch < -89.0f)
  	pitch = -89.0f;

	Eigen::Vector3f newfront;
	newfront(0) = cos(ConvertDegreesToRadians(pitch)) * cos(ConvertDegreesToRadians(yaw));
	newfront(1) = sin(ConvertDegreesToRadians(pitch));
	newfront(2) = cos(ConvertDegreesToRadians(pitch)) * sin(ConvertDegreesToRadians(yaw));
	front = newfront.normalized();

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
		std::cout << yaw << ", " << pitch << "\n";
		camera->Translate(offset);
		//camera->Rotate(yaw, pitch, 0);
		offset << 0,0,0;
	}
	else{
		printf("Error: no camera\n");
	}
}

float CameraController::ConvertDegreesToRadians(const float angle_in_degrees) {
  return (angle_in_degrees * (M_PI / 180.0f));
}
