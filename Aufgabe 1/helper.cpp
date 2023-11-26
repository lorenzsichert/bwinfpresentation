#include "helper.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
using namespace sf;

//Basic Functions
float getDistance(sf::Vector2f a, sf::Vector2f b) {
  return std::abs(sqrt(pow(b.x - a.x,2)+pow(b.y - a.y,2)));
}

bool checkAngle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c) {
  a.x = b.x-a.x;
  a.y = b.y-a.y;
  c.x = b.x-c.x;
  c.y = b.y-c.y;
  return a.x*c.x + a.y*c.y > 0;
}

float totalDist(std::vector<sf::Vector2f> &map, std::vector<int> &path) {
  float dist = 0;
  for (int i = 0; i < path.size()-1; i++) {
    dist += getDistance(map[path[i]], map[path[i+1]]);
  }
  return dist;
}

void loadMap(std::vector<sf::Vector2f> &map, std::vector<std::vector<int>> &graph, const char *fileName) {
  std::ifstream myfile(fileName);
  if (myfile.fail()) {
    perror("Error open");
    exit(EXIT_FAILURE);
  }
  //Load File to Map
  Vector2f vec;
  while (myfile >> vec.x >> vec.y) {
    map.push_back(vec);
  }
  //Map to Graph
  for (int i = 0; i < map.size(); i++) {
    std::vector<int> v;
    for (int j = 0; j < map.size(); j++) {
      v.push_back(j);
    }
    sort(v.begin(),v.end(), [&map,&i] (int& lhs, int& rhs) { return getDistance(map[i],map[lhs]) < getDistance(map[i],map[rhs]); });
    graph.push_back(v);
  }
}

void outputPathText(std::vector<int> &path, char *fileName) {
  std::ofstream out(fileName);
  for (int i = 0; i < path.size(); i++) {
    out << path[i] << " ";
  }
  out.close();
}


float getAngle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c) {
  float ang1 = (atan2(b.y - a.y, b.x - a.x)*180)/3.14;
  float ang2 = (atan2(c.y - b.y, c.x - b.x)*180)/3.14;
  return std::abs(ang1-ang2);
}

float getRating(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, int w) {
  return getDistance(b, c) + (w * getAngle(a,b,c));
}

float totalRating(std::vector<sf::Vector2f> &map, std::vector<int> &path, int w) {
  float dist = 0;
  float angle = 0;
  for (int i = 0; i < path.size()-1; i++) {
    dist += getDistance(map[path[i]],map[path[i+1]]);
  }
  for (int i = 1; i < path.size()-1; i++) {
    angle += getAngle(map[path[i-1]], map[path[i]], map[path[i+1]]);
  }
  return dist + (w*angle);
}

bool traceAnglePath(std::vector<sf::Vector2f> &map, std::vector<int> &path, std::vector<int> &bestPath, std::vector<bool> &visited, float &bestRating, int t, int &l, int w) {
  ++l;
  if (path[0] == map.size()) { //All Nodes Checked
    return true;
  }
  if (t > 2 && checkAngle(map[path[t-3]], map[path[t-2]], map[path[t-1]])) { // Angle Check
    return false;
  }
  if (path.size() == map.size()) { // Save best Path
    float dist = totalRating(map, path, w);
    if (dist < bestRating) {
      bestRating = dist;
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
    traceAnglePath(map, path, bestPath, visited, bestRating, 1, l, w);
    return true;
  }
  int index = path[path.size()-1];
  std::vector<int> sortedMap;
  for (int i = 0; i < map.size(); i++) {
    sortedMap.push_back(i);
  }
  if (t == 1) {
    sort(sortedMap.begin(), sortedMap.end(), [&index, &map](int& lhs, int& rhs){ return getDistance(map[lhs],map[index]) < getDistance(map[lhs],map[index]);});
  } else {
    int preIndex = path[path.size()-2];
    sort(sortedMap.begin(), sortedMap.end(), [&w, &preIndex, &index, &map](int& lhs, int& rhs){ return getRating(map[preIndex],map[lhs],map[index],w) < getRating(map[preIndex],map[rhs],map[index],w);});
  }

  for (int i = 0; i < sortedMap.size(); i++) {
    if (visited[sortedMap[i]] == false) {
      path.push_back(sortedMap[i]);
      visited[sortedMap[i]] = true;
      if (traceAnglePath(map, path, bestPath, visited, bestRating, t+1, l, w)) {
        return true;
      }
      visited[sortedMap[i]] = false;
      path.pop_back();
    }
  }
  return false;
}

