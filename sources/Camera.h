#ifndef CAMERA_H
#define CAMERA_H

#include "Model.h"

#include <vector>
#include <Eigen/Core>
#include <GL/glew.h>

class Camera : public Model
{
  protected:
    Eigen::Vector3f position;

    Eigen::Vector3f orientation;

    Eigen::Vector3f speed;

    double movementSpeedFactor; // Controls how fast the camera moves

    void initCamera();


  public:

    // Holding any keys down?
    bool holdingForward;
    bool holdingBackward;
    bool holdingLeftStrafe;
    bool holdingRightStrafe;

    Camera();

    ~Camera();

    // Method to move the camera based on the current direction
    void move(double deltaTime);

    void SetShaderProgram(ShaderProgram * shader);

    void Draw(const Eigen::Matrix4f& projection, const Eigen::Matrix4f& view);

    void SetOrientation(const Eigen::Vector3f & orientation);

    void SetPosition(const Eigen::Vector3f & position);

    //const Eigen::Vector3f& GetOrientation() const;

    //const Eigen::Vector3f& GetPosition() const;

    const Eigen::MatrixXf& GetVertices() const;

    const std::vector<GLuint>& GetIndices() const;

  private:

};


#endif // CAMERA_H
