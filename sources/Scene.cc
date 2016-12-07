#include "Scene.h"


#include <Eigen/Core>
#include <Eigen/Geometry>

#include "Camera.h"
#include "ModelBase.h"
#include "Model.h"
#include "ModelInstance.h"
#include "ShaderProgram.h"

Scene::Scene(ModelBase * world) : 
	m_world(world),
	m_skyBox(nullptr),
	m_alderaan(nullptr),
	m_mileStone(nullptr),
	m_mileStoneProxy(nullptr),
	m_mileStoneDirection(1),
	m_mileMinX(1.0f),
	m_mileMaxX(10.0f)
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
	
	
	m_alderaan = new  Model("models/Alderaan/", "Alderaan.obj");
	m_alderaan->SetShaderProgram(shader);
	m_alderaan->SetScale(0.05);
	m_alderaan->Translate(Eigen::Vector3f(2.0f, 4.0f, 0.0f));
	world->AddChild(m_alderaan);
	
	m_mileStone = new Model("models/milestone/", "milestone_01_Low_20k_tris_.obj");
	m_mileStone->SetShaderProgram(shader);
	
	m_mileStoneProxy = new ModelBase();
	m_mileStoneProxy->SetScale(2.0f);
	m_mileStoneProxy->AddChild("m_mileStone", m_mileStone);
	m_mileStonePosX = 7.0f;
	m_mileStoneProxy->Translate(Eigen::Vector3f(m_mileStonePosX, 0.0f, 0.0f));
	world->AddChild(m_mileStoneProxy);
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
	m_mileStone->Rotate(deltaTime * 25.0f, 0.0f, deltaTime * 23.0f);
	m_alderaan->Rotate(deltaTime * 25.0f, 0.0f, 0.0f);
	
	float mileStoneSpeed = 1.0f;
	if(m_mileStonePosX > m_mileMaxX)
	{
		m_mileStoneDirection = -1;
	}
	else if(m_mileStonePosX < m_mileMinX)
	{
		m_mileStoneDirection = 1;
	}
	m_mileStoneProxy->Translate(Eigen::Vector3f(deltaTime * m_mileStoneDirection * mileStoneSpeed, 0.0f, 0.0f));
	m_mileStonePosX += (deltaTime * m_mileStoneDirection * mileStoneSpeed);
}



