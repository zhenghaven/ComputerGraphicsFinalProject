
#pragma once
#ifndef SCENE_HEADER
#define SCENE_HEADER

class Camera;
class ModelBase;
class Model;

class Scene
{
public:
	Scene(ModelBase * world);
	
	~Scene();
	
	void Update(float deltaTime);

private:
	Scene();

	ModelBase * m_world;
	
	Model * m_skyBox;
	
};


#endif
