#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

#include <vector>
using namespace std;

struct Particle {
    glm::vec3 pos;
    glm::vec3 vel;
    float pressure;
    float density;
};

#endif