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

#include <ctime>

#include <iostream>
#include <string>
#include <unistd.h>


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#ifdef GFLAGS_NAMESPACE_GOOGLE
#define GLUTILS_GFLAGS_NAMESPACE google
#else
#define GLUTILS_GFLAGS_NAMESPACE gflags
#endif


#include "sources/camera_utils.h"
#include "sources/Camera.h"
#include "sources/CameraController.h"
#include "sources/transformations.h"
#include "sources/Model.h"
#include "sources/ModelInstance.h"
#include "sources/ShaderProgram.h"
#include "sources/ModelLoader.h"
#include "sources/Material.h"
#include "sources/MaterialLoader.h"
#include "sources/Scene.h"

DEFINE_string(workdir, "./", "The path to working directory.");

// Window dimensions.
constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;

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

bool ConstructWorld(Model ** worldPtr)
{
	std::string tmpStr1, tmpStr2;
	ShaderProgram::ReadShaderStrFromFile("shaders/vertex_shader.glsl", tmpStr1);
	ShaderProgram::ReadShaderStrFromFile("shaders/fragment_shader.glsl", tmpStr2);
	ShaderProgram * shader = new ShaderProgram(tmpStr1, tmpStr2);

	if(!shader->IsValid())
	{
		std::cout << "Shader error: " << shader->GetErrorMessage() << std::endl;
		return false;
	}


	*worldPtr = new Model();

	ModelInstance * sky = new ModelInstance();

	Model * skyBox = new  Model("models/Sky/", "skybox.obj");
	skyBox->SetShaderProgram(shader);
	sky->GetRealModel()->AddChild("skyBox", skyBox);
	skyBox->SetScale(1000.0);
	skyBox->Translate(Eigen::Vector3f(0.0f,0.0f,0.0f));

	ModelInstance * FlatWall = new ModelInstance();

	Model * FlatWallWall = new Model("models/brick_wall_flat/", "wall.obj");
	FlatWallWall->SetShaderProgram(shader);
	Model * FlatWallCement = new Model("models/brick_wall_flat/", "cement.obj");
	FlatWallCement->SetShaderProgram(shader);
	FlatWall->GetRealModel()->AddChild("FlatWallWall", FlatWallWall);
	FlatWall->GetRealModel()->AddChild("FlatWallCement", FlatWallCement);

	ModelInstance * FlatWall2 = FlatWall->CreateNewInstance();
	FlatWall->SetScale(0.01);
	FlatWall->Rotate(90.0f, 0.0f, 0.0f);
	FlatWall->Translate(Eigen::Vector3f(0.0f, 3.0f, 0.0f));
	FlatWall2->SetScale(0.01);
	FlatWall2->Translate(Eigen::Vector3f(0.0f, -3.0f, 0.0f));

	(*worldPtr)->AddChild(sky);
	(*worldPtr)->AddChild(FlatWall);
	(*worldPtr)->AddChild(FlatWall2);
	return true;
}

int main(int argc, char** argv)
{
	GLUTILS_GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
	google::InitGoogleLogging(argv[0]);
	srand(time(NULL));

	if(chdir(FLAGS_workdir.c_str()) != 0)
	{
		std::cerr << "Failed to change the working directory to: " << FLAGS_workdir << std::endl;
	}

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
	const float near_plane = 0.01f;
	const float far_plane = 10000.0f;
	const Eigen::Matrix4f& projection = wvu::ComputePerspectiveProjectionMatrix(field_of_view, aspect_ratio, near_plane, far_plane);
	const Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

	//constexpr static GLfloat rotSpeed = 35.0;
	//const Eigen::Vector3f rotAxis(0.0f, 1.0f, 0.0f);
	Camera* camera = new Camera(projection);
	CameraController* cameraController = new CameraController(window, camera);


	Model * world = nullptr;

	if(!ConstructWorld(&world))
	{
		std::cerr << "Failed to construct the world!" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	Scene scene(world);

	//world->SetScale(0.01);
	world->SetPosition(Eigen::Vector3f(0.0f, 0.0f, 0.0f));
	//world->Rotate(0.0f, 0.0f, 0.0f);
	//camera->Rotate(90.0f, 0.0f, 0.0f);
	camera->Translate(Eigen::Vector3f(0.0f, 0.0f, -20.0f));


	double lastFrame = 0;
	
	while (!glfwWindowShouldClose(window))
	{
		
		ClearTheFrameBuffer();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		double currentFrame = glfwGetTime();
		float deltaTime = static_cast<float>(currentFrame - lastFrame);
		lastFrame = currentFrame;
		
		scene.Update(deltaTime);

		cameraController->update(deltaTime);
		// Render the scene!
		world->Draw(camera);

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
