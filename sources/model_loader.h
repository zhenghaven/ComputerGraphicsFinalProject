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

#ifndef MODEL_LOADER_H_
#define MODEL_LOADER_H_

#include <string>
#include <vector>
#include <Eigen/Core>
#include <Eigen/StdVector>

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Vector2f)

namespace wvu {
// A face defines the vertices, the normals, and the texels to use for a
// triangle. Some objs only define the vertices, so check the size of
// the vectors first.
struct Face {
  std::vector<int> vertex_indices;
  std::vector<int> normal_indices;
  std::vector<int> texel_indices;
};

// Loads a 3D model in OBJ format. Retruns true when successful and false
// otherwise.
// Paremters:
//   filepath  The filepath of the model.
//   vertices  The vertices of the model.
//   texels  The texels for the model and vertices.
//   normals  The normal vectors.
//   faces  The faces of the model (i.e., triangles).
bool LoadObjModel(const std::string& filepath,
                  std::vector<Eigen::Vector3f>* vertices,
                  std::vector<Eigen::Vector2f>* texels,
                  std::vector<Eigen::Vector3f>* normals,
                  std::vector<Face>* faces);
}  // namespace wvu

#endif //  MODEL_LOADER_H_

