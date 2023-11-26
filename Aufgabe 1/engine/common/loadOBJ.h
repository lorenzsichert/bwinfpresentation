#include <glm/glm.hpp>
#include <vector>

bool loadOBJ(const char* fileName, std::vector<float>& vertices);

glm::vec3 faceNormal(glm::vec3 a, glm::vec3 b, glm:: vec3 c);
