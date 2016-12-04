// Copyright (C) 2016 West Virginia University.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
//     * Neither the name of West Virginia University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Please contact the author of this library if you have any questions.
// TODO: Add your names and emails using the following format:
// Author: Victor Fragoso (victor.fragoso@mail.wvu.edu)

// TODO: Include the headers you need for your project.

#include <iostream>
#include <string>


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "sources/camera_utils.h"
#include "sources/transformations.cc"

// Window dimensions.
constexpr int kWindowWidth = 640;
constexpr int kWindowHeight = 480;

static void ErrorCallback(int error, const char* description)
{
	std::cerr << "ERROR: " << description << std::endl;
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
 }
 
 void SetWindowHints() 
 {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void ConfigureViewPort(GLFWwindow* window)
{
	int width;
	int height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

void ClearTheFrameBuffer() 
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main(int argc, char** argv) 
{
	if (!glfwInit()) 
	{
		return -1;
	}

	glfwSetErrorCallback(ErrorCallback);

	SetWindowHints();

	const std::string window_name = "CS 470 Final Project";
	GLFWwindow* window = glfwCreateWindow(kWindowWidth, kWindowHeight, window_name.c_str(), nullptr, nullptr);
  
	if (!window) 
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, KeyCallback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) 
	{
		std::cerr << "Glew did not initialize properly!" << std::endl;
		glfwTerminate();
		return -1;
	}

	ConfigureViewPort(window);

	const float field_of_view = wvu::ConvertDegreesToRadians(45.0f);
	const float aspect_ratio = static_cast<float>(kWindowWidth / kWindowHeight);
	const float near_plane = 0.1f;
	const float far_plane = 10.0f;
	const Eigen::Matrix4f& projection = wvu::ComputePerspectiveProjectionMatrix(field_of_view, aspect_ratio, near_plane, far_plane);
	//const Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

	//constexpr static GLfloat rotSpeed = 35.0;
	//const Eigen::Vector3f rotAxis(0.0f, 1.0f, 0.0f);
  
	while (!glfwWindowShouldClose(window)) 
	{

		// Render the scene!
		

		// Swap front and back buffers.
		glfwSwapBuffers(window);

		// Poll for and process events.
		glfwPollEvents();
	}

	// Destroy window.
	glfwDestroyWindow(window);
	// Tear down GLFW library.
	glfwTerminate();

	return 0;
}
