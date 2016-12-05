#include "Model.h"
#include "Camera.h"

#include <vector>
#include <Eigen/Core>
#include <GL/glew.h>

#include <GLFW/glfw3.h>

class CameraController
{
public:
  CameraController(GLFWwindow* glwindow, Camera* cam);

  ~CameraController();

  void update(float deltaTime);
private:
  float speed = 20.0;

  bool moveForward = false;
  bool moveBackward = false;
  bool moveRight = false;
  bool moveLeft = false;
  bool moveUp = false;
  bool moveDown = false;

  GLFWwindow* window;

  Camera* camera;

  float GetMultiplier(bool add, bool substruct, float scale = 0.0f);
};
