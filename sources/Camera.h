#ifndef CAMERA_H
#define CAMERA_H

#include "Model.h"

#include <vector>
#include <Eigen/Core>
#include <GL/glew.h>

class Camera : public Model
{

  public:


    Camera(Eigen::Matrix4f projection);

    ~Camera();

    // Method to move the camera based on the current direction
    void update(double deltaTime);

    const Eigen::Matrix4f GetProjection() const;

  private:
    Camera();

    Eigen::Matrix4f m_projection;

    void initCamera();

};


#endif // CAMERA_H
