#include <fstream>
#include <vector>
#include "helper.h"
using namespace std;

void loadFile(std::vector<Triplet>& list, const char* fileName) {
  ifstream myfile(fileName);
  if (myfile.fail()) {
    perror("Error open");
    exit(EXIT_FAILURE);
  }
  int n;
  myfile >> n;
  Triplet t;
  t.z = 1;
  while (myfile >> t.x >> t.y) {
    if (t.y < t.x) {
      swap(t.x,t.y);
    }
    list.push_back(t);
  }
}

void outputOrder(std::vector<Triplet> &list, std::vector<int> &route, char* fileName) {
  ofstream out(fileName);
  for (int i = 0; i < route.size()-1; i++) {
    out << list[route[i]].x << ", " << list[route[i]].y << " -> ";
  }
  out << list[route[route.size()-1]].x << ", " << list[route[route.size()-1]].y << endl;
  out.close();
}

bool compare(Triplet a, Triplet b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

vector<int> getStartingPoints(vector<Triplet>& list) {
  vector<int> starts;
  vector<int> possibleStarts;
  int m = list[0].x;
  if (m > list.size() || list.size() <= 2) {
    possibleStarts.push_back(0);
    return possibleStarts;
  }
  //Get Start Points
  for (int i = 0; i < list.size(); i++) {
    if (list[i].x == m) {
      starts.push_back(list[i].y);
      int j = i;
      while (list[i+1].y == list[j].y) {
        i++;
      }
    }
  }
  //Find Matching Multiples
  int minStart = starts[starts.size()-1];
  for (int i = 0; i < list.size()-m+1; i++) {
    bool mult = true;
    int k = i;
    for (k = i; k < i+m; k++) {
      if (!compare(list[i], list[k])) {
        mult = false;
      }
    }
    if (mult) {
      for (int j = 0; j < starts.size(); j++) {
        if (starts[j] == list[i].x || starts[j] == list[i].y) {
            possibleStarts.push_back(i);
        }
      }
      int j = i;
      while (compare(list[i+1], list[j])) {
        i++;
      }
    }
  }
  return possibleStarts;
}
