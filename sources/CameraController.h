
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

  float ConvertDegreesToRadians(const float angle_in_degrees);

  Eigen::Vector3f offset;
  Eigen::Vector3f front;
  Eigen::Vector3f up;

  float speed = 1.0;

  double lastX = 400, lastY = 300;

  float pitch, yaw;

  GLFWwindow* window;

  Camera* camera;
};
