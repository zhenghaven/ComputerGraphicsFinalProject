#include "Scene.h"


#include <Eigen/Core>
#include <Eigen/Geometry>

#include "Camera.h"
#include "ModelBase.h"
#include "Model.h"
#include "ModelInstance.h"

Scene::Scene(ModelBase * world) : 
	m_world(world)
{
	
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
	
}
