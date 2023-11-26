#ifndef SOLUTION_H
#define SOLUTION_H

#include "engine/object.h"
#include "engine/shapes/circle.h"
#include "engine/system.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

class Solution {
  System* system;
  std::vector<sf::Vector2f> map;
  std::vector<glm::vec2> scaledMap;
  std::vector<int> bestPath;
  std::vector<std::vector<int>> graph;
  std::vector<Circle*> circles;
  std::vector<bool> visited;
  Object line;
  Object path;
  float bestDist;
  int l = 0;

  float radius = 12;
public:
  void init(System* System, const char* fileName);
  void initRandom(System* System, int count, unsigned int Seed);

  bool tracePath(std::vector<sf::Vector2f>& map, std::vector<std::vector<int>>& graph, std::vector<int>& path, std::vector<int>& bestPath, std::vector<bool>& visited, float& bestDist, int t, int& l);
  int getIndex(int n, std::vector<int>& a, std::vector<int>& b, std::vector<int>& c, std::vector<int>& d);
  bool stitch(std::vector<sf::Vector2f>& map, std::vector<int>& path, std::vector<int>& a, std::vector<int>& b, std::vector<int>& c, std::vector<int>& d);
  bool check3Opt(std::vector<sf::Vector2f>& map, std::vector<int>& path, int I, int J, int K);
  void kOpt(std::vector<sf::Vector2f>& map, std::vector<int>& path);

  void solve();
  void solveDFS();
  void solve3Opt();
  void draw();
};
  
#endif
