#include <glm/glm.hpp>

using namespace std;

struct Water {
	float density = 1; // g/mL
	float pressure = 3.1690; // kPa
	float viscosity = 0.890; // mPa s
};

float gravity = -9.8; // m/s^2

float poly6Kernel(glm::vec3 r, float h) {
}