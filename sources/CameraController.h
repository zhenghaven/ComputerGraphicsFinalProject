#include "Model.h"

#include <vector>
#include <Eigen/Core>
#include <GL/glew.h>

#include <GLFW/glfw3.h>

class CameraController
{
public:
  CameraController(GLFWwindow* glwindow);

  ~CameraController();
private:
  void update(float deltaTime);

  GLFWwindow* window;
};
