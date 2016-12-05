// Copyright (C) 2016 West Virginia University.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
//     * Neither the name of West Virginia University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Please contact the author of this library if you have any questions.
// Author: Victor Fragoso (victor.fragoso@mail.wvu.edu)

#define _USE_MATH_DEFINES  // For using M_PI.
#include <math.h>

#include "transformations.h"
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace wvu 
{
	// Compute translation transformation matrix.
	// Params:
	//   offset  The translation offset vector.
	Eigen::Matrix4f ComputeTranslationMatrix(const Eigen::Vector3f& offset) 
	{
		Eigen::Matrix4f transformation = Eigen::Matrix4f::Identity();
		if(offset.norm() > 0.0f)
		{
			transformation.col(3) = offset.homogeneous();
		}
		return transformation;
	}

	// Compute rotation transformation matrix.
	// Params:
	//   rotation_axis  The rotation axis.
	//   angle_in_radians  Angle in radians.
	Eigen::Matrix4f ComputeRotationMatrix(const Eigen::Vector3f& rotation_axis, const float angle_in_radians) 
	{
		Eigen::Matrix3f rotationMatrix = Eigen::Matrix3f::Identity();
		if(!rotation_axis.hasNaN() && rotation_axis.norm() != 0)
		{
			Eigen::AngleAxisf rotation(angle_in_radians, rotation_axis);
			rotationMatrix = rotation.matrix();
		}
		Eigen::Matrix4f transformation = Eigen::Matrix4f::Identity();
		transformation.block(0, 0, 3, 3) = rotationMatrix;
		return transformation;
	}

	// Compute scaling transformation matrix.
	// Params:
	//   scale  Scale factor.
	Eigen::Matrix4f ComputeScalingMatrix(const float scale) 
	{
		Eigen::Matrix3f scaleMatrix = (scale * Eigen::Matrix3f::Identity());
		Eigen::Matrix4f transformation = Eigen::Matrix4f::Identity();
		transformation.block(0, 0, 3, 3) = scaleMatrix;
		return transformation;
	}

	// Converts angles in degrees to radians.
	// Parameters:
	//   angle_in_degrees  The angle in degrees.
	float ConvertDegreesToRadians(const float angle_in_degrees)
	{
		return (angle_in_degrees * (M_PI / 180.0f));
	}

}  // namespace wvu
