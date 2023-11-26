#include <SFML/Graphics.hpp>
#include <vector>

//Basic Functions
float getDistance(sf::Vector2f a, sf::Vector2f b);
bool checkAngle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c);
float totalDist(std::vector<sf::Vector2f>& map, std::vector<int>& path);

//Load Input File
void loadMap(std::vector<sf::Vector2f>& map, std::vector<std::vector<int>>& graph, const char* fileName);
//Output Path to File
void outputPathText(std::vector<int>& path, char* fileName);

//Get Angle
float getAngle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c);
//Angle Rating
float getRating(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, int w);
float totalRating(std::vector<sf::Vector2f> &map, std::vector<int> &path, int w);
//Trace Angle Path
bool traceAnglePath(std::vector<sf::Vector2f>& map, std::vector<int>& path, std::vector<int>& bestPath, std::vector<bool>& visited, float& bestRating, int t, int& l, int w);
