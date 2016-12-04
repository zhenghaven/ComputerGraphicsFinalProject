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

#include "model_loader.h"

#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>

#include <Eigen/Core>
#include <glog/logging.h>

namespace wvu {
namespace {
enum EntryType {
  VERTEX = 0,
  TEXEL = 1,
  NORMAL = 2,
  COMMENT = 3,
  NOT_RECOGNIZED = 4,
  FACE = 5
};

EntryType DetermineEntryType(const std::string& line) {
  if (line.size() < 1) return NOT_RECOGNIZED;
  if (line[0] == '#') return COMMENT;
  if (line[0] == 'v') {
    if (line.find("vt") != std::string::npos) return TEXEL;
    if (line.find("vn") != std::string::npos) return NORMAL;
    return VERTEX;
  }
  if (line[0] == 'f') return FACE;
  return NOT_RECOGNIZED;
}

void ParseVertexLine(const std::string& line,
                     std::vector<Eigen::Vector3f>* vertices) {
  VLOG(1) << "Vertex line: " << line;
  float x, y ,z;
  char temp;
  sscanf(line.c_str(), "%c %f %f %f", &temp, &x, &y, &z);
  vertices->emplace_back(x, y, z);
}

constexpr int kStringSize = 32;
void ParseFaceElement(char str[kStringSize], Face* face) {
  char *token = strtok(str, "/");
  std::string temp_str(str);
  // TODO(vfragoso): How to deal with // or /.
  // TODO(vfragoso): Right now it supports 3 elements or 1 element.
  int entry_counter = 0;
  // Format 0: vertex; 1: texel; 2: normal.
  int entries[3];
  while (token) {
    VLOG(1) << "Token: " << token;
    if (token) {
      // Indices in OBJ model start at 1. But C++ is 0-based indexing.
      entries[entry_counter] = std::stoi(token) - 1;
      VLOG(1) << "0-based index: " << entries[entry_counter];
    }
    token = strtok(NULL, "/");
    ++entry_counter;
  }
  face->vertex_indices.push_back(entries[0]);
  // Only vertices were passed?
  if (entry_counter == 1) return;
  if (std::string(str).find("//") != std::string::npos) {
    // Means that vertex and normal were passed.
    face->normal_indices.push_back(entries[1]);
    return;
  }
  if (entry_counter == 2) {
    // Means that vertex and texels were passed.
    face->texel_indices.push_back(entries[1]);
    return;
  }
  // Means that vertex/texel/normal were passed.
  face->texel_indices.push_back(entries[1]);
  face->normal_indices.push_back(entries[2]);
}

void ParseFaceLine(const std::string& line, std::vector<Face>* faces) {
  VLOG(1) << "Face line: " << line;
  char temp;
  char str1[kStringSize];
  char str2[kStringSize];
  char str3[kStringSize];
  sscanf(line.c_str(), "%c %s %s %s", &temp, str1, str2, str3);
  faces->emplace_back();
  ParseFaceElement(str1, &faces->back());
  ParseFaceElement(str2, &faces->back());
  ParseFaceElement(str3, &faces->back());
  CHECK_EQ(faces->back().vertex_indices.size(), 3);
}

void ParseTexelLine(const std::string& line,
                    std::vector<Eigen::Vector2f>* texels) {
  VLOG(1) << "Texel line: " << line;
  float x, y;
  char temp[kStringSize];
  sscanf(line.c_str(), "%s %f %f", &temp[0], &x, &y);
  texels->emplace_back(x, y);
}

void ParseNormalLine(const std::string& line,
                     std::vector<Eigen::Vector3f>* normals) {
  VLOG(1) << "Normal line: " << line;
  float x, y ,z;
  char temp[kStringSize];
  sscanf(line.c_str(), "%s %f %f %f", &temp[0], &x, &y, &z);
  normals->emplace_back(x, y, z);
}

void IgnoreLine(const std::string& line) {
  VLOG(1) << "Line ignored: " << line;
}

}  // namespace

// Loads a 3D model in OBJ format.
// Paremters:
//   filepath  The filepath of the model.
//   vertices  The vertices of the model.
//   texels  The texels for the model and vertices.
//   normals  The normal vectors.
//   faces  Defines the vertices, texels, and normals.
bool LoadObjModel(const std::string& filepath,
                  std::vector<Eigen::Vector3f>* vertices,
                  std::vector<Eigen::Vector2f>* texels,
                  std::vector<Eigen::Vector3f>* normals,
                  std::vector<Face>* faces) {
  std::ifstream in(filepath);
  if (!in.is_open()) return false;
  // Load all the lines and parse.
  while (in.good()) {
    // Read line from file.
    std::string line;
    std::getline(in, line);
    //    if (line.size() < 2) continue;
    const EntryType line_type = DetermineEntryType(line);
    switch (line_type) {
      case VERTEX:
        ParseVertexLine(line, vertices);
        break;
      case TEXEL:
        ParseTexelLine(line, texels);
        break;
      case NORMAL:
        ParseNormalLine(line, normals);
        break;
      case FACE:
        ParseFaceLine(line, faces);
        break;
      case NOT_RECOGNIZED:
      case COMMENT:
        IgnoreLine(line);
        break;
    }
  }
  in.close();
  return true;
}

}  // namespace wvu


