
#pragma once
#ifndef SCENE_HEADER
#define SCENE_HEADER

class Camera;
class ModelBase;

class Scene
{
public:
	Scene(ModelBase * world);
	
	~Scene();
	
	void Update(float deltaTime);

private:
	Scene();

	ModelBase * m_world;
	
};


#endif
