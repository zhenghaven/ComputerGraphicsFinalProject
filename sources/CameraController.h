
#include <vector>
#include <Eigen/Core>
#include <GL/glew.h>

#include <GLFW/glfw3.h>

class Camera;

class CameraController
{
public:
  CameraController(GLFWwindow* glwindow, Camera* cam);

  ~CameraController();

  void update(float deltaTime);
private:
  CameraController();

  Eigen::Vector3f offset;
  Eigen::Vector3f front;
  Eigen::Vector3f up;

  float speed = 0.05;

  GLFWwindow* window;

  Camera* camera;
};
