
#pragma once
#ifndef SCENE_HEADER
#define SCENE_HEADER

#include <Eigen/Core>

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

	Model * m_alderaan;

	Model * m_textDisplay;

	Model * m_mileStone;
	ModelBase * m_mileStoneProxy;

	int m_mileStoneDirection;
	float m_mileMinX;
	float m_mileMaxX;
	float m_mileStonePosX;
};


#endif
