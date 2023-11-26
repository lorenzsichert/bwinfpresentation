#include "object.h"
#include "system.h"
#include <glm/glm.hpp>
#include <vector>
class InstanceObject : public Object {
private:
  unsigned int mbo;
  unsigned int cbo;
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> scales;
  std::vector<glm::vec3> rotations;
  std::vector<glm::vec4> colors;
  std::vector<glm::mat4> mvps;
  int count;
public:
  // Positions: 3 float,
  // Sizes: 3 float,
  // Colors: 4 float,
  // Rotations: 3 float
  void init(System* System, std::vector<float>& vertices, int Count, unsigned int DrawPattern);
  void setInstancePositions(std::vector<glm::vec3>& Positions);
  void setInstanceScales(std::vector<glm::vec3>& Scales);
  void setInstanceRotations(std::vector<glm::vec3>& Rotations);
  void setInstanceColors(std::vector<glm::vec4>& Colors);
  void calcInstanceMVPs();
  void updateBuffers();
  void draw();
  ~InstanceObject();
};
