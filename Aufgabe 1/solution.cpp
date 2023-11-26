#include "solution.h"
#include "helper.h"
#include <GL/glew.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
using namespace std;
using namespace sf;

void Solution::initRandom(System* System, int count, unsigned int Seed) {
  rand_r(&Seed);
  system = System;
  for (int i = 0; i < count; i++) {
    map.push_back(Vector2f(rand_r(&Seed)%1000, rand_r(&Seed)%1000));
  }

  for (int i = 0; i < map.size(); i++) {
    std::vector<int> v;
    for (int j = 0; j < map.size(); j++) {
      v.push_back(j);
    }
    sort(v.begin(),v.end(), [this, &i] (int& lhs, int& rhs) { return getDistance(map[i],map[lhs]) < getDistance(map[i],map[rhs]); });
    graph.push_back(v);
  }

  glm::vec2 begin(10000,10000);
  glm::vec2 end(-10000,-10000);
  for (int i = 0; i < map.size(); i++) {
    begin.x = std::min(begin.x, map[i].x);
    begin.y = std::min(begin.y, map[i].y);
    end.x = std::max(end.x, map[i].x);
    end.y = std::max(end.y, map[i].y);
  }
  glm::vec2 center = (end+begin)/2.f;
  glm::vec2 rect = end-begin;
  float scale = system->unit/std::max(rect.x, rect.y) * 1.7;
  
  for (int i = 0; i < map.size(); i++) {
    Circle* circle = new Circle;
    circle->init(system, radius, 20);
    scaledMap.push_back(glm::vec2(scale*(map[i].x-center.x), scale*(map[i].y - center.y)));
    circle->setPosition(glm::vec3(scaledMap[i],0));
    circle->setColor(glm::vec4(0,0,0,1));
    circles.push_back(circle);
  }
  std::cout << rect.x << " " << rect.y << std::endl;


  std::vector<float> vertices;
  for (int i = 0; i < map.size(); i++) {
    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(0);
  }
  line.init(system, vertices, GL_LINE_STRIP);
  line.setColor(glm::vec4(0,0,0,1));
}

void Solution::init(System* System, const char* fileName) {
  system = System;
  loadMap(map, graph, fileName);
  glm::vec2 begin(10000,10000);
  glm::vec2 end(-10000,-10000);
  for (int i = 0; i < map.size(); i++) {
    begin.x = std::min(begin.x, map[i].x);
    begin.y = std::min(begin.y, map[i].y);
    end.x = std::max(end.x, map[i].x);
    end.y = std::max(end.y, map[i].y);
  }
  glm::vec2 center = (end+begin)/2.f;
  glm::vec2 rect = end-begin;
  float scale = system->unit/std::max(rect.x, rect.y) * 1.9;
  
  for (int i = 0; i < map.size(); i++) {
    Circle* circle = new Circle;
    circle->init(system, radius, 20);
    scaledMap.push_back(glm::vec2(scale*(map[i].x-center.x), scale*(map[i].y - center.y)));
    circle->setPosition(glm::vec3(scaledMap[i],0));
    circle->setColor(glm::vec4(0,0,0,1));
    circles.push_back(circle);
  }
  std::cout << rect.x << " " << rect.y << std::endl;


  std::vector<float> vertices;
  for (int i = 0; i < map.size(); i++) {
    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(0);
  }
  line.init(system, vertices, GL_LINE_STRIP);
  line.setColor(glm::vec4(0,0,0,1));

}



void Solution::solve() {
  solveDFS();

  solve3Opt();
}

void Solution::solve3Opt() {
  if (bestPath.size() == map.size()) {
    cout << "3-Opt: ";
    kOpt(map, bestPath);
    cout << totalDist(map, bestPath) << " km" << endl;

    std::vector<float> vertices;
    for (int i = 0; i < bestPath.size(); i++) {
      vertices.push_back(scaledMap[bestPath[i]].x);
      vertices.push_back(scaledMap[bestPath[i]].y);
      vertices.push_back(0);
    }
    line.reloadVertices(vertices);
  }
}


void Solution::solveDFS() {

  l = 0;
  bestDist = numeric_limits<float>::max();
  vector<int> path;

  for (int i = 0; i < map.size(); i++) {
    visited.push_back(false);
  }

  int start = 0;
  path.push_back(start);
  visited[start] = true;
  bool found = false;

  cout << "DFS: ";
  tracePath(map, graph, path, bestPath, visited, bestDist, 1, l);
  if (bestPath.size() == map.size()) {
    cout << totalDist(map, bestPath) << " km" << endl;
  }

  std::vector<float> vertices;
  for (int i = 0; i < bestPath.size(); i++) {
    vertices.push_back(scaledMap[bestPath[i]].x);
    vertices.push_back(scaledMap[bestPath[i]].y);
    vertices.push_back(0);
  }
  line.reloadVertices(vertices);
}

void Solution::draw() {
  for (int i = 0; i < circles.size(); i++) {
    circles[i]->draw();
  }
  line.draw();
}

bool Solution::tracePath(vector<Vector2f>& map, vector<vector<int>>& graph, vector<int>& path, vector<int>& bestPath, vector<bool>& visited, float& bestDist, int t, int& l) {
  ++l;
  if (path[0] == map.size()) { //All Nodes Checked
    return true;
  }
  if (t > 2 && checkAngle(map[path[t-3]], map[path[t-2]], map[path[t-1]])) { // Angle Check
    return false;
  }
  if (path.size() == map.size()) { // Save best Path
    float dist = totalDist(map, path);
    if (dist < bestDist) {
      bestDist = dist;
      bestPath = path;
    }
  }
  if (path.size() == map.size() || l >= map.size() * map.size()) { // Restart
    l = 0;
    int start = path[0] + 1;
    path.clear();
    path.push_back(start);
    for (int i = 0; i < visited.size(); i++) { visited[i] = false; }
    visited[start] = true;
    tracePath(map, graph, path, bestPath, visited, bestDist, 1, l);
    return true;
  }
  int index = path[path.size()-1];
  for (int i = 0; i < graph[index].size(); i++) {
    if (visited[graph[index][i]] == false) {
      path.push_back(graph[index][i]);
      visited[graph[index][i]] = true;
      if (tracePath(map, graph, path, bestPath, visited, bestDist, t+1, l)) {
        return true;
      }
      visited[graph[index][i]] = false;
      path.pop_back();
    }
  }
  return false;
}

int Solution::getIndex(int n, vector<int>& a, vector<int>& b, vector<int>& c, vector<int>& d) {
  if (n < a.size()) {
    return a[n];
  }
  if (n < a.size()+b.size()) {
    return b[n-a.size()];
  }
  if (n < a.size()+b.size()+c.size()) {
    return c[n-(a.size()+b.size())];
  }
  if (n < a.size()+b.size()+c.size()+d.size()) {
    return d[n-(a.size()+b.size()+c.size())];
  }
  return 0;
}

bool Solution::stitch(vector<Vector2f>& map, vector<int>& path, vector<int>& a, vector<int>& b, vector<int>& c, vector<int>& d) {
  // A-B
  if (a.size() >= 2 && checkAngle(map[getIndex(a.size()-2,a,b,c,d)], map[getIndex(a.size()-1,a,b,c,d)], map[getIndex(a.size(),a,b,c,d)])) {
    return false;
  }
  if (a.size() >= 1 && checkAngle(map[getIndex(a.size()-1,a,b,c,d)], map[getIndex(a.size(),a,b,c,d)], map[getIndex(a.size()+1,a,b,c,d)])) {
    return false;
  }
  // B-C
  int bEnd = a.size()+b.size();
  if ((b.size() >= 2 || (b.size() >= 1 && a.size() >= 1)) && checkAngle(map[getIndex(bEnd-2,a,b,c,d)], map[getIndex(bEnd-1,a,b,c,d)], map[getIndex(bEnd,a,b,c,d)])) {
    return false;
  }
  if (b.size() >= 1 && checkAngle(map[getIndex(bEnd-1,a,b,c,d)], map[getIndex(bEnd,a,b,c,d)], map[getIndex(bEnd+1,a,b,c,d)])) {
    return false;
  }
  // C-D
  int cEnd = a.size()+b.size()+c.size();
  if (c.size() >= 2 && d.size() >= 1 && checkAngle(map[getIndex(cEnd-2,a,b,c,d)], map[getIndex(cEnd-1,a,b,c,d)], map[getIndex(cEnd,a,b,c,d)])) {
    return false;
  }
  if (c.size() >= 1 && d.size() >= 2 && checkAngle(map[getIndex(cEnd-1,a,b,c,d)], map[getIndex(cEnd,a,b,c,d)], map[getIndex(cEnd+1,a,b,c,d)])) {
    return false;
  }
  path.clear();
  path.insert(path.end(),a.begin(),a.end());
  path.insert(path.end(),b.begin(),b.end());
  path.insert(path.end(),c.begin(),c.end());
  path.insert(path.end(),d.begin(),d.end());
  return true;
}

bool Solution::check3Opt(vector<Vector2f>& map, vector<int>& path, int I, int J, int K) {
  int a = path[I];
  int b = path[I+1];
  int c = path[J];
  int d = path[J+1];
  int e = path[K];
  int f = path[K+1];
  float d0 = getDistance(map[a], map[b]) + getDistance(map[c], map[d]) + getDistance(map[e], map[f]);
  float d1 = getDistance(map[a], map[b]) + getDistance(map[c], map[e]) + getDistance(map[d], map[f]);
  float d2 = getDistance(map[a], map[c]) + getDistance(map[b], map[d]) + getDistance(map[e], map[f]);
  float d3 = getDistance(map[a], map[c]) + getDistance(map[b], map[e]) + getDistance(map[d], map[f]);
  float d4 = getDistance(map[a], map[d]) + getDistance(map[e], map[b]) + getDistance(map[c], map[f]);
  float d5 = getDistance(map[a], map[d]) + getDistance(map[e], map[c]) + getDistance(map[b], map[f]);
  float d6 = getDistance(map[a], map[e]) + getDistance(map[d], map[b]) + getDistance(map[c], map[f]);
  float d7 = getDistance(map[a], map[e]) + getDistance(map[d], map[c]) + getDistance(map[b], map[f]);
  vector<float> D = {d1,d2,d3,d4,d5,d6,d7};
  bool found = false;
  for (int i = 0; i < D.size(); i++) {
    if (d0 > D[i]) {
      found = true;
    }
  }
  if (found == false) {
    return false;
  }
  //Parts
  vector<int> OA;
  for (int i = 0; i <= I; i++) {
    OA.push_back(path[i]);
  }
  vector<int> BC;
  for (int i = I+1; i <= J; i++) {
    BC.push_back(path[i]);
  }
  vector<int> CB(BC.size());
  reverse_copy(BC.begin(), BC.end(), CB.begin());
  vector<int> DE;
  for (int i = J+1; i <= K; i++) {
    DE.push_back(path[i]);
  }
  vector<int> ED(DE.size());
  reverse_copy(DE.begin(), DE.end(), ED.begin());
  vector<int> FO;
  for (int i = K+1; i < path.size(); i++) {
    FO.push_back(path[i]);
  }
  if (d1 < d0) {
    return stitch(map, path, OA, BC, ED, FO);
  }
  if (d2 < d0) {
    return stitch(map, path, OA, CB, DE, FO);
  }
  if (d3 < d0) {
    return stitch(map, path, OA, CB, ED, FO);
  }
  if (d4 < d0) {
    return stitch(map, path, OA, DE, BC, FO);
  }
  if (d5 < d0) {
    return stitch(map, path, OA, DE, CB, FO);
  }
  if (d6 < d0) {
    return stitch(map, path, OA, ED, BC, FO);
  }
  if (d7 < d0) {
    return stitch(map, path, OA, ED, CB, FO);
  }
  return false;
}

void Solution::kOpt(vector<Vector2f>& map, vector<int>& path) {
  bool found = true;
  while (found) {
    found = false;
    for (int i = 0; i < map.size(); i++) {
      for (int j = i+1; j < map.size(); j++) {
        for (int k = j+1; k < map.size()-1; k++) {
          if (check3Opt(map,path,i,j,k)) {
            found = true;
          }
        }
      }
    }
  }
}

