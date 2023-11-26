#include "loadOBJ.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

bool loadOBJ(const char *fileName, std::vector<float> &vertices) {
  std::ifstream myfile(fileName);
  if (myfile.fail()) {
    std::cout << "Error open file: " << fileName << std::endl;
    return false;
  }
  std::vector<glm::vec3> verticesList;
  std::vector<glm::vec3> facesList;
  char c;
  while (myfile >> c) {
    if (c == '#' || c == 'o' || c == 's') {
      myfile.ignore(100, '\n');
    } else if (c == 'v') {
      glm::vec3 v;
      myfile >> v.x >> v.y >> v.z;
      verticesList.push_back(v);
    } else if (c == 'f') {
      int a,b,c;
      myfile >> a >> b >> c;
      a--; b--; c--;
      glm::vec3 normal = faceNormal(verticesList[a], verticesList[b], verticesList[c]);
      vertices.push_back(verticesList[a].x);
      vertices.push_back(verticesList[a].y);
      vertices.push_back(verticesList[a].z);
      //vertices.push_back(normal.x);
      //vertices.push_back(normal.z);
      //vertices.push_back(normal.y);
      vertices.push_back(verticesList[b].x);
      vertices.push_back(verticesList[b].y);
      vertices.push_back(verticesList[b].z);
      //vertices.push_back(normal.x);
      //vertices.push_back(normal.z);
      //vertices.push_back(normal.y);
      vertices.push_back(verticesList[c].x);
      vertices.push_back(verticesList[c].y);
      vertices.push_back(verticesList[c].z);
      //vertices.push_back(normal.x);
      //vertices.push_back(normal.z);
      //vertices.push_back(normal.y);
    }
  }
  return true;
}

glm::vec3 faceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
  glm::vec3 u = b - a;
  glm::vec3 v = c - a;
  glm::vec3 normal;
  normal.x = (u.x * v.z) - (u.z * v.y);
  normal.y = (u.z * v.x) - (u.x * v.z);
  normal.z = (u.x * v.y) - (u.y * v.x);

  return normal;
}
