#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>
#include "helper.h"
using namespace std;

bool addSlice(Triplet& cube, Triplet slice) {
  if (slice.x == cube.x) { // 1 0 0
    if (slice.y == cube.y) { // 1 2 0
      cube.z += 1;
      return true;
    }
    if (slice.y == cube.z) { // 1 0 2
      cube.y += 1;
      return true;
    }
  }
  if (slice.x == cube.y) { // 0 1 0
    if (slice.y == cube.x) { // 2 1 0
      cube.z += 1;
      return true;
    }
    if (slice.y == cube.z) { // 0 1 2
      cube.x += 1;
      return true;
    }
  }
  if (slice.x == cube.z) { // 0 0 1
    if (slice.y == cube.x) { // 2 0 1
      cube.y += 1;
      return true;
    }
    if (slice.y == cube.y) { // 0 2 1
      cube.x += 1;
      return true;
    }
  }
  return false;
  
}

int getFirstVal(vector<Triplet>& list,int l, int r, int val, vector<bool>& used) {
  if (l <= r) {
    int mid = (l+r)/2;
    if (list[mid].x == val) {
      while (list[mid-1].x == list[mid].x) {
        mid = mid-1;
      }
      while (used[mid] == true) {
        if (list[mid].x != list[mid+1].x) {
          return -1;
        }
        mid++;
      }
      return mid;
    } else if (list[mid].x > val) {
      return getFirstVal(list, l, mid-1, val, used);
    } else {
      return getFirstVal(list, mid+1, r, val, used);
    }
  }
  return -1;
}

int getFirst(vector<Triplet>& list, Triplet& cube, vector<bool>& used) {
  int indX = getFirstVal(list, 0, list.size(), cube.x, used);
  int indY = getFirstVal(list, 0, list.size(), cube.y, used);
  int indZ = getFirstVal(list, 0, list.size(), cube.z, used);
  int indexs[3] = {indX,indY,indZ};
  int index = INT32_MAX;
  for (int i = 0; i < 3; i++) {
    if (indexs[i] != -1 && indexs[i] < index) {
      index = indexs[i];
    }
  }
  for (index = index; index < list.size(); index++) { // Find Match
    if (used[index] == false) {
      if (addSlice(cube, list[index])) {
        used[index] = true;
        return index;
      }
    }
  }
  return -1;
}

bool addMissingSlice(vector<Triplet> list, Triplet slice, int start, char* outputFileName) {
  if (slice.x > slice.y) {
    swap(slice.x,slice.y);
  }
  list.push_back(slice);
  sort(list.begin(), list.end(), [](const Triplet lhs, const Triplet rhs){
        if (lhs.x == rhs.x) {
          return lhs.y < rhs.y;
        } 
        return lhs.x < rhs.x;
      });
  vector<bool> used;
  used.resize(list.size());

  vector<int> route;
  Triplet cube;
  
  cube = list[start];
  used[start] = true;
  route.push_back(start);

  bool found = true;
  while (route.size() < list.size()) {
    route.push_back(getFirst(list, cube, used));
    if (route[route.size()-1] == -1) {
      found = false;
      break;
    }
  }
  if (found) {
    cout << slice.x << ", " << slice.y << endl;
    cout << "Finaler Quader: " << cube.x << ", " << cube.y << ", " << cube.z << endl;
    outputOrder(list, route, outputFileName);
    return true;
  }
  return false;
}


int main(int argc, char** argv) {
  vector<Triplet> list;
  const char* fileName = "res/kaese1.txt";
  loadFile(list, fileName);
  auto a = chrono::steady_clock::now();
  sort(list.begin(), list.end(), [](const Triplet lhs, const Triplet rhs){
        if (lhs.x == rhs.x) {
          return lhs.y < rhs.y;
        } 
        return lhs.x < rhs.x;
      });

  vector<bool> used;
  used.resize(list.size());

  vector<int> possibleStarts = getStartingPoints(list);

  vector<int> route;

  Triplet cube;

  Triplet maxCube;
  int possibleStart = 0;

  bool found;

  for (int i = 0; i < possibleStarts.size(); i++) {
    cube = list[possibleStarts[i]];
    used[possibleStarts[i]] = true;
    route.push_back(possibleStarts[i]);

    found = true;
    while (route.size() < list.size()) {
      route.push_back(getFirst(list, cube, used));
      cout << list[route[route.size()-1]].x << ", " << list[route[route.size()-1]].y << endl;
      if (route[route.size()-1] == -1) {
        found = false;
        if (cube.x >= maxCube.x && cube.y >= maxCube.y && cube.z >= maxCube.z) {
          maxCube = cube;
          possibleStart = possibleStarts[i];
        }
        break;
      }
    }
    if (found) {
      break;
    }
    for (int i = 0; i < used.size(); i++) { used[i] = false; };
    route.clear();
    found = false;
  }
  if (found) {
    auto b = chrono::steady_clock::now();
    cout << "Finaler Quader: " << cube.x << ", " << cube.y << ", " << cube.z << endl;
    cout << "Gefunden in " << chrono::duration_cast<chrono::milliseconds>(b-a).count() << " Millisekunden." << endl;
    outputOrder(list, route, argv[2]);
  } else {
    Triplet slice;
    slice.x = maxCube.x;
    slice.y = maxCube.y;
    cout << "Lösung mit fehlender Scheibe: ";
    if (!addMissingSlice(list, slice, possibleStart, argv[2])) {
      slice.x = maxCube.x;
      slice.y = maxCube.z;
      if (!addMissingSlice(list, slice, possibleStart, argv[2])) {
        slice.x = maxCube.y;
        slice.y = maxCube.z;
        if (!addMissingSlice(list, slice, possibleStart, argv[2])) {
          cout << endl;
          cout << "Möglicherweise fehlende Scheibe nicht gefunden." << endl;
          return 0;
        }
      }
    }
    auto b = chrono::steady_clock::now();
    cout << "Gefunden in " << chrono::duration_cast<chrono::milliseconds>(b-a).count() << " Millisekunden." << endl;
  }
}
