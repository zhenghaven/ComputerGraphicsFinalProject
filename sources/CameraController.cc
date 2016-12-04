#include "CameraController.h"
#include "Camera.h"

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

CameraController::CameraController(GLFWwindow* glwindow)
{
	window = glwindow;
}

CameraController::~CameraController()
{
	// Nothing to do here - we don't need to free memory as all member variables
	// were declared on the stack.
}


void CameraController::update(float deltaTime)
{

  //int xpos, ypos;
  //glfwGetMousePos(&xpos, &ypos);
  //glfwSetMousePos(1024/2, 768/2);
  //horizontalAngle += mouseSpeed * deltaTime * float(1024/2 - xpos );
  //verticalAngle   += mouseSpeed * deltaTime * float( 768/2 - ypos );
  //Move forward
  if (glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS){
    //position += direction * deltaTime * speed;
    std::cout << "up" << "\n";
  }
  // Move backward
  if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
    //position -= direction * deltaTime * speed;
    std::cout << "down" << "\n";
  }
  // Strafe right
  if (glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
    //position += right * deltaTime * speed;
    std::cout << "right" << "\n";
  }
  // Strafe left
  if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS){
    //position -= right * deltaTime * speed;
    std::cout << "left" << "\n";
  }
}
