#include "Scene.h"


#include <Eigen/Core>
#include <Eigen/Geometry>

#include "Camera.h"
#include "ModelBase.h"
#include "Model.h"
#include "ModelInstance.h"
#include "ShaderProgram.h"

Scene::Scene(ModelBase * world) : 
	m_world(world)
{
	std::string tmpStr1, tmpStr2;
	ShaderProgram::ReadShaderStrFromFile("shaders/vertex_shader.glsl", tmpStr1);
	ShaderProgram::ReadShaderStrFromFile("shaders/fragment_shader.glsl", tmpStr2);
	ShaderProgram * shader = new ShaderProgram(tmpStr1, tmpStr2);
	
	//Setup the first wall
	ModelInstance * BrickWall90_1 = new ModelInstance();
	Model * BrickWall90Cement = new Model("models/brick_wall_90degree/", "cement.obj");
	BrickWall90Cement->SetShaderProgram(shader);
	Model * BrickWall90Wall = new Model("models/brick_wall_90degree/", "wall.obj");
	BrickWall90Wall->SetShaderProgram(shader);
	Model * BrickWall90Column = new Model("models/brick_wall_90degree/", "column.obj");
	BrickWall90Column->SetShaderProgram(shader);
	Model * BrickWallFlatCement = new Model("models/brick_wall_flat/", "cement.obj");
	BrickWallFlatCement->SetShaderProgram(shader);
	Model * BrickWallFlatWall = new Model("models/brick_wall_flat/", "wall.obj");
	BrickWallFlatWall->SetShaderProgram(shader);
	
	BrickWallFlatCement->Translate(100.0f * Eigen::Vector3f(-4.1f,-0.4f,8.0f));
	BrickWallFlatWall->Translate(100.0f * Eigen::Vector3f(-4.1f,-0.4f,8.0f));
	
	BrickWall90_1->GetRealModel()->AddChild("BrickWall90Cement", BrickWall90Cement); 
	BrickWall90_1->GetRealModel()->AddChild("BrickWall90Wall", BrickWall90Wall); 
	BrickWall90_1->GetRealModel()->AddChild("BrickWall90Column", BrickWall90Column); 
	BrickWall90_1->GetRealModel()->AddChild("BrickWallFlatCement", BrickWallFlatCement); 
	BrickWall90_1->GetRealModel()->AddChild("BrickWallFlatWall", BrickWallFlatWall); 
	
	BrickWall90_1->SetScale(0.01);
	
	//Setup the second wall
	ModelInstance * BrickWall90_2 = BrickWall90_1->CreateNewInstance();
	BrickWall90_2->SetScale(0.01);
	BrickWall90_2->Rotate(90.0f,0.0f,0.0f);
	BrickWall90_2->Translate(Eigen::Vector3f(0.0f, 0.0f, 15.0f));
	
	//Setup the third wall
	ModelInstance * BrickWall90_3 = BrickWall90_1->CreateNewInstance();
	BrickWall90_3->SetScale(0.01);
	BrickWall90_3->Rotate(180.0f,0.0f,0.0f);
	BrickWall90_3->Translate(Eigen::Vector3f(15.0f, 0.0f, 15.0f));
	
	//Setup the fourth wall
	ModelInstance * BrickWall90_4 = BrickWall90_1->CreateNewInstance();
	BrickWall90_4->SetScale(0.01);
	BrickWall90_4->Rotate(270.0f,0.0f,0.0f);
	BrickWall90_4->Translate(Eigen::Vector3f(15.0f, 0.0f, 0.0f));
	
	world->AddChild(BrickWall90_1);
	world->AddChild(BrickWall90_2);
	world->AddChild(BrickWall90_3);
	world->AddChild(BrickWall90_4);

	
	ModelInstance * Grass_1 = new ModelInstance();
	
	Model * GrassTile1 = new Model("models/Grass/", "grass.obj");
	GrassTile1->SetShaderProgram(shader);
	Grass_1->GetRealModel()->AddChild("GrassTile1", GrassTile1); 
	Grass_1->SetScale(3.0);
	Grass_1->Translate(Eigen::Vector3f(-1.5f, -1.5f, -1.5f));
	
	ModelInstance * Grass_2 = Grass_1->CreateNewInstance();
	Grass_2->SetScale(3.0);
	Grass_2->Translate(Eigen::Vector3f(-1.5f, -1.5f, 4.5f));
	
	ModelInstance * Grass_3 = Grass_1->CreateNewInstance();
	Grass_3->SetScale(3.0);
	Grass_3->Translate(Eigen::Vector3f(-1.5f, -1.5f, 9.5f));
	
	ModelInstance * Grass_4 = Grass_1->CreateNewInstance();
	Grass_4->SetScale(3.0);
	Grass_4->Translate(Eigen::Vector3f(-1.5f, -1.5f, 14.5f));
	
	ModelInstance * Grass_5 = Grass_1->CreateNewInstance();
	Grass_5->SetScale(3.0);
	Grass_5->Translate(Eigen::Vector3f(-1.5f, -1.5f, 19.5f));
	
	ModelInstance * Grass_row_1 = new ModelInstance();
	Grass_row_1->GetRealModel()->AddChild("Grass_1", Grass_1); 
	Grass_row_1->GetRealModel()->AddChild("Grass_2", Grass_2); 
	Grass_row_1->GetRealModel()->AddChild("Grass_3", Grass_3); 
	Grass_row_1->GetRealModel()->AddChild("Grass_4", Grass_4); 
	Grass_row_1->GetRealModel()->AddChild("Grass_5", Grass_5); 
	
	ModelInstance * Grass_row_2 = Grass_row_1->CreateNewInstance();
	Grass_row_2->Translate(Eigen::Vector3f(5.0f, 0.0f, 0.0f));
	
	ModelInstance * Grass_row_3 = Grass_row_1->CreateNewInstance();
	Grass_row_3->Translate(Eigen::Vector3f(10.0f, 0.0f, 0.0f));
	
	ModelInstance * Grass_row_4 = Grass_row_1->CreateNewInstance();
	Grass_row_4->Translate(Eigen::Vector3f(15.0f, 0.0f, 0.0f));
	
	ModelInstance * Grass_row_5 = Grass_row_1->CreateNewInstance();
	Grass_row_5->Translate(Eigen::Vector3f(20.0f, 0.0f, 0.0f));
	
	world->AddChild(Grass_row_1);
	world->AddChild(Grass_row_2);
	world->AddChild(Grass_row_3);
	world->AddChild(Grass_row_4);
	world->AddChild(Grass_row_5);
	
	
	m_skyBox = new  Model("models/Sky/", "skybox.obj");
	m_skyBox->SetShaderProgram(shader);
	m_skyBox->SetScale(1000);
	
	world->AddChild(m_skyBox);
}

Scene::Scene() : 
	m_world(nullptr)
{

}

Scene::~Scene()
{
	
}

void Scene::Update(float deltaTime)
{
	//Update position here.
	//The world and its children will be draw in the main loop.
	m_skyBox->Rotate(deltaTime * 0.5f, 0.0f, 0.0f);
}
