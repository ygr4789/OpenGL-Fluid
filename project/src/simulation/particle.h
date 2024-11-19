#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <vector>

using namespace std;

class Particle {
public:
    Particle(float x, float y, float z);
    void update(float dt);
private:
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 accel;
    float viscosity;
    float pressure;
    float density;
    float mass;
};

Particle::Particle(float x, float y, float z) {
    pos = glm::vec3(x, y, z);
}

#endif