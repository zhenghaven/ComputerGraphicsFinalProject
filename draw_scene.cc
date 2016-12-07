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



//Initialize models

	//Sky box
	Model * skyBox = new  Model("models/Sky/", "skybox.obj");
	skyBox->SetShaderProgram(shader);
	skyBox->SetScale(1000);

	//Walls
	ModelInstance * BackWall1 = new ModelInstance();
	Model * BrickWall1 = new Model("models/brick_wall_flat/", "wall.obj");
	BrickWall1->SetShaderProgram(shader);
	BackWall1->AddChild(BrickWall1);
	Model * CementWall1 = new Model("models/brick_wall_flat/", "cement.obj");
	CementWall1->SetShaderProgram(shader);
	BackWall1->AddChild(CementWall1);
	BackWall1->SetScale(0.01);
	BackWall1->Rotate(90.0f,0.0f,0.0f);
	BackWall1->Translate(Eigen::Vector3f(0.0f,0.0f,1.0f));
	(*worldPtr)->AddChild(BackWall1);

	ModelInstance * BackWall2 = new ModelInstance();
	Model * BrickWall2 = new Model("models/brick_wall_flat/", "wall.obj");
	BrickWall2->SetShaderProgram(shader);
	BackWall2->AddChild(BrickWall2);
	Model * CementWall2 = new Model("models/brick_wall_flat/", "cement.obj");
	CementWall2->SetShaderProgram(shader);
	BackWall2->AddChild(CementWall2);
	BackWall2->SetScale(0.01);
	BackWall2->Rotate(90.0f,0.0f,0.0f);
	BackWall2->Translate(Eigen::Vector3f(6.0f,0.0f,1.0f));
	(*worldPtr)->AddChild(BackWall2);

	ModelInstance * BackWall3 = new ModelInstance();
	Model * BrickWall3 = new Model("models/brick_wall_flat/", "wall.obj");
	BrickWall3->SetShaderProgram(shader);
	BackWall3->AddChild(BrickWall3);
	Model * CementWall3 = new Model("models/brick_wall_flat/", "cement.obj");
	CementWall3->SetShaderProgram(shader);
	BackWall3->AddChild(CementWall3);
	BackWall3->SetScale(0.01);
	BackWall3->Rotate(0.0f,0.0f,0.0f);
	BackWall3->Translate(Eigen::Vector3f(10.0f,0.0f,5.0f));
	(*worldPtr)->AddChild(BackWall3);

	ModelInstance * BackWall4 = new ModelInstance();
	Model * BrickWall4 = new Model("models/brick_wall_flat/", "wall.obj");
	BrickWall4->SetShaderProgram(shader);
	BackWall4->AddChild(BrickWall4);
	Model * CementWall4 = new Model("models/brick_wall_flat/", "cement.obj");
	CementWall4->SetShaderProgram(shader);
	BackWall4->AddChild(CementWall4);
	BackWall4->SetScale(0.01);
	BackWall4->Rotate(0.0f,0.0f,0.0f);
	BackWall4->Translate(Eigen::Vector3f(10.0f,0.0f,13.0f));
	(*worldPtr)->AddChild(BackWall4);

	ModelInstance * BackWall5 = new ModelInstance();
	Model * BrickWall5 = new Model("models/brick_wall_flat/", "wall.obj");
	BrickWall5->SetShaderProgram(shader);
	BackWall5->AddChild(BrickWall5);
	Model * CementWall5 = new Model("models/brick_wall_flat/", "cement.obj");
	CementWall5->SetShaderProgram(shader);
	BackWall5->AddChild(CementWall5);
	BackWall5->SetScale(0.01);
	BackWall5->Rotate(90.0f,0.0f,0.0f);
	BackWall5->Translate(Eigen::Vector3f(0.0f,0.0f,17.0f));
	(*worldPtr)->AddChild(BackWall5);

	ModelInstance * BackWall6 = new ModelInstance();
	Model * BrickWall6 = new Model("models/brick_wall_flat/", "wall.obj");
	BrickWall6->SetShaderProgram(shader);
	BackWall6->AddChild(BrickWall6);
	Model * CementWall6 = new Model("models/brick_wall_flat/", "cement.obj");
	CementWall6->SetShaderProgram(shader);
	BackWall6->AddChild(CementWall6);
	BackWall6->SetScale(0.01);
	BackWall6->Rotate(90.0f,0.0f,0.0f);
	BackWall6->Translate(Eigen::Vector3f(6.0f,0.0f,17.0f));
	(*worldPtr)->AddChild(BackWall6);

	ModelInstance * BackWall7 = new ModelInstance();
	Model * BrickWall7 = new Model("models/brick_wall_flat/", "wall.obj");
	BrickWall7->SetShaderProgram(shader);
	BackWall7->AddChild(BrickWall7);
	Model * CementWall7 = new Model("models/brick_wall_flat/", "cement.obj");
	CementWall7->SetShaderProgram(shader);
	BackWall7->AddChild(CementWall7);
	BackWall7->SetScale(0.01);
	BackWall7->Rotate(0.0f,0.0f,0.0f);
	BackWall7->Translate(Eigen::Vector3f(-4.0f,0.0f,5.0f));
	(*worldPtr)->AddChild(BackWall7);

	ModelInstance * BackWall8 = new ModelInstance();
	Model * BrickWall8 = new Model("models/brick_wall_flat/", "wall.obj");
	BrickWall8->SetShaderProgram(shader);
	BackWall8->AddChild(BrickWall8);
	Model * CementWall8 = new Model("models/brick_wall_flat/", "cement.obj");
	CementWall8->SetShaderProgram(shader);
	BackWall8->AddChild(CementWall8);
	BackWall8->SetScale(0.01);
	BackWall8->Rotate(0.0f,0.0f,0.0f);
	BackWall8->Translate(Eigen::Vector3f(-4.0f,0.0f,13.0f));
	(*worldPtr)->AddChild(BackWall8);

	// Grass Tiles

	ModelInstance * Grass1 = new ModelInstance();
	Model * GrassTile1 = new Model("models/Grass/", "grass.obj");
	GrassTile1->SetShaderProgram(shader);
	Grass1->AddChild(GrassTile1);
	Grass1->SetScale(2.0);
	Grass1->Translate(Eigen::Vector3f(-2.0f,-1.1f,3.0f));
	(*worldPtr)->AddChild(Grass1);

	ModelInstance * Grass2 = new ModelInstance();
	Model * GrassTile2 = new Model("models/Grass/", "grass.obj");
	GrassTile2->SetShaderProgram(shader);
	Grass2->AddChild(GrassTile2);
	Grass2->SetScale(2.0);
	Grass2->Translate(Eigen::Vector3f(2.0f,-1.1f,3.0f));
	(*worldPtr)->AddChild(Grass2);

	ModelInstance * Grass3 = new ModelInstance();
	Model * GrassTile3 = new Model("models/Grass/", "grass.obj");
	GrassTile3->SetShaderProgram(shader);
	Grass3->AddChild(GrassTile3);
	Grass3->SetScale(2.0);
	Grass3->Translate(Eigen::Vector3f(6.0f,-1.1f,3.0f));
	(*worldPtr)->AddChild(Grass3);

	ModelInstance * Grass4 = new ModelInstance();
	Model * GrassTile4 = new Model("models/Grass/", "grass.obj");
	GrassTile4->SetShaderProgram(shader);
	Grass4->AddChild(GrassTile4);
	Grass4->SetScale(2.0);
	Grass4->Translate(Eigen::Vector3f(10.0f,-1.1f,3.0f));
	(*worldPtr)->AddChild(Grass4);

	ModelInstance * Grass5 = new ModelInstance();
	Model * GrassTile5 = new Model("models/Grass/", "grass.obj");
	GrassTile5->SetShaderProgram(shader);
	Grass5->AddChild(GrassTile5);
	Grass5->SetScale(2.0);
	Grass5->Translate(Eigen::Vector3f(-2.0f,-1.1f,7.0f));
	(*worldPtr)->AddChild(Grass5);

	ModelInstance * Grass6 = new ModelInstance();
	Model * GrassTile6 = new Model("models/Grass/", "grass.obj");
	GrassTile6->SetShaderProgram(shader);
	Grass6->AddChild(GrassTile6);
	Grass6->SetScale(2.0);
	Grass6->Translate(Eigen::Vector3f(2.0f,-1.1f,7.0f));
	(*worldPtr)->AddChild(Grass6);

	ModelInstance * Grass7 = new ModelInstance();
	Model * GrassTile7 = new Model("models/Grass/", "grass.obj");
	GrassTile7->SetShaderProgram(shader);
	Grass7->AddChild(GrassTile7);
	Grass7->SetScale(2.0);
	Grass7->Translate(Eigen::Vector3f(6.0f,-1.1f,7.0f));
	(*worldPtr)->AddChild(Grass7);

	ModelInstance * Grass8 = new ModelInstance();
	Model * GrassTile8 = new Model("models/Grass/", "grass.obj");
	GrassTile8->SetShaderProgram(shader);
	Grass8->AddChild(GrassTile8);
	Grass8->SetScale(2.0);
	Grass8->Translate(Eigen::Vector3f(10.0f,-1.1f,7.0f));
	(*worldPtr)->AddChild(Grass8);

	ModelInstance * Grass9 = new ModelInstance();
	Model * GrassTile9 = new Model("models/Grass/", "grass.obj");
	GrassTile9->SetShaderProgram(shader);
	Grass9->AddChild(GrassTile9);
	Grass9->SetScale(2.0);
	Grass9->Translate(Eigen::Vector3f(-2.0f,-1.1f,11.0f));
	(*worldPtr)->AddChild(Grass9);

	ModelInstance * Grass10 = new ModelInstance();
	Model * GrassTile10 = new Model("models/Grass/", "grass.obj");
	GrassTile10->SetShaderProgram(shader);
	Grass10->AddChild(GrassTile10);
	Grass10->SetScale(2.0);
	Grass10->Translate(Eigen::Vector3f(2.0f,-1.1f,11.0f));
	(*worldPtr)->AddChild(Grass10);

	ModelInstance * Grass11 = new ModelInstance();
	Model * GrassTile11 = new Model("models/Grass/", "grass.obj");
	GrassTile11->SetShaderProgram(shader);
	Grass11->AddChild(GrassTile11);
	Grass11->SetScale(2.0);
	Grass11->Translate(Eigen::Vector3f(6.0f,-1.1f,11.0f));
	(*worldPtr)->AddChild(Grass11);

	ModelInstance * Grass12 = new ModelInstance();
	Model * GrassTile12 = new Model("models/Grass/", "grass.obj");
	GrassTile12->SetShaderProgram(shader);
	Grass12->AddChild(GrassTile12);
	Grass12->SetScale(2.0);
	Grass12->Translate(Eigen::Vector3f(10.0f,-1.1f,11.0f));
	(*worldPtr)->AddChild(Grass12);

	ModelInstance * Grass13 = new ModelInstance();
	Model * GrassTile13 = new Model("models/Grass/", "grass.obj");
	GrassTile13->SetShaderProgram(shader);
	Grass13->AddChild(GrassTile13);
	Grass13->SetScale(2.0);
	Grass13->Translate(Eigen::Vector3f(-2.0f,-1.1f,15.0f));
	(*worldPtr)->AddChild(Grass13);

	ModelInstance * Grass14 = new ModelInstance();
	Model * GrassTile14 = new Model("models/Grass/", "grass.obj");
	GrassTile14->SetShaderProgram(shader);
	Grass14->AddChild(GrassTile14);
	Grass14->SetScale(2.0);
	Grass14->Translate(Eigen::Vector3f(2.0f,-1.1f,15.0f));
	(*worldPtr)->AddChild(Grass14);

	ModelInstance * Grass15 = new ModelInstance();
	Model * GrassTile15 = new Model("models/Grass/", "grass.obj");
	GrassTile15->SetShaderProgram(shader);
	Grass15->AddChild(GrassTile15);
	Grass15->SetScale(2.0);
	Grass15->Translate(Eigen::Vector3f(6.0f,-1.1f,15.0f));
	(*worldPtr)->AddChild(Grass15);

	ModelInstance * Grass16 = new ModelInstance();
	Model * GrassTile16 = new Model("models/Grass/", "grass.obj");
	GrassTile16->SetShaderProgram(shader);
	Grass16->AddChild(GrassTile16);
	Grass16->SetScale(2.0);
	Grass16->Translate(Eigen::Vector3f(10.0f,-1.1f,15.0f));
	(*worldPtr)->AddChild(Grass16);

	//Planets

	Model * alderaan = new  Model("models/Alderaan/", "Alderaan.obj");
	alderaan->SetShaderProgram(shader);
	alderaan->SetScale(0.01);
	alderaan->Translate(Eigen::Vector3f(0.0f,0.0f,0.0f));
	(*worldPtr)->AddChild(alderaan);



	(*worldPtr)->AddChild(skyBox);

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
	camera->Translate(Eigen::Vector3f(0.0f, -10.0f, -20.0f));


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
