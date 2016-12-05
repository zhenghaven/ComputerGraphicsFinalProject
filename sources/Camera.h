#ifndef CAMERA_H
#define CAMERA_H

#include "Model.h"

#include <vector>
#include <Eigen/Core>
#include <GL/glew.h>

class Camera : public Model
{

  public:


    Camera();

    ~Camera();

    // Method to move the camera based on the current direction
    void update(double deltaTime);

    void SetShaderProgram(ShaderProgram * shader);

    void Draw(const Eigen::Matrix4f& projection, const Eigen::Matrix4f& view);

    void SetOrientation(const Eigen::Vector3f & orientation);

    const Eigen::Vector3f GetLookDirection() const;

    const Eigen::Vector3f GetUpVector() const;

    const Eigen::Matrix4f GetPose() const;

    void SetPosition(const Eigen::Vector3f & position);

    void Translate(const Eigen::Vector3f & translation);

    void Rotate(float yaw, float pitch);

    const Eigen::MatrixXf& GetVertices() const;

    const std::vector<GLuint>& GetIndices() const;

  private:

    Eigen::Matrix4f position;

    Eigen::Vector3f speed;

    double movementSpeedFactor; // Controls how fast the camera moves

    void initCamera();

};


#endif // CAMERA_H
