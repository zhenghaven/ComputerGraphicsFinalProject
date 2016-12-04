#include "Model.h"
#include "Camera.h"

#include <iostream>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <GL/glew.h>

Camera::Camera()
{
	initCamera();
}

Camera::~Camera()
{
	// Nothing to do here - we don't need to free memory as all member variables
	// were declared on the stack.
}

void Camera::initCamera()
{
	// Set position, rotation and speed values to zero


	// How fast we move (higher values mean we move and strafe faster)
	movementSpeedFactor = 100.0;

	// To begin with, we aren't holding down any keys
	holdingForward     = false;
	holdingBackward    = false;
	holdingLeftStrafe  = false;
	holdingRightStrafe = false;
}

// Function to calculate which direction we need to move the camera and by what amount
void Camera::move(double deltaTime)
{
	// Vector to break up our movement into components along the X, Y and Z axis
	Eigen::Vector3f movement;

	if (holdingForward)
	{
		//movement.addX(sinYRot * pitchLimitFactor);
	//	movement.addY(-sinXRot);
		//movement.addZ(-cosYRot * pitchLimitFactor);
	}

	if (holdingBackward)
	{
		//movement.addX(-sinYRot * pitchLimitFactor);
		//movement.addY(sinXRot);
		//movement.addZ(cosYRot * pitchLimitFactor);
	}

	if (holdingLeftStrafe)
	{
		//movement.addX(-cosYRot);
		//movement.addZ(-sinYRot);
	}

	if (holdingRightStrafe)
	{
		//movement.addX(cosYRot);
		//movement.addZ(sinYRot);
	}

	// Normalise our movement vector
//	movement.normalise();

	// Calculate our value to keep the movement the same speed regardless of the framerate...
	double framerateIndependentFactor = movementSpeedFactor * deltaTime;

	// .. and then apply it to our movement vector.
	//movement *= framerateIndependentFactor;

	// Finally, apply the movement to our position
	//position += movement;
}
