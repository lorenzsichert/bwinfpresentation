#include <vector>
struct Triplet {
  int x = 0;
  int y = 0;
  int z = 0;
};

//Load File
void loadFile(std::vector<Triplet>& list, const char* fileName);
//Output File
void outputOrder(std::vector<Triplet>& list, std::vector<int>& route, char* fileName);

//Basic Functions
bool compare(Triplet a, Triplet b);
std::vector<int> getStartingPoints(std::vector<Triplet>& list);
