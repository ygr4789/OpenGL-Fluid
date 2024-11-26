#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <vector>

/* Properties of Water */
#define WATER_DENSITY = 998;
#define WATER_GAS_CONSTANT = 2000;
#define WATER_VISCOSITY = 0.5;
#define WATER_COLOR = "#0000ff";
#define WATER_PARTICLE_RADIUS = 0.1;

using namespace std;

class Particle {
public:
    Particle(float x, float y, float z, float m);
    void update(float dt);
private:
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 acc;
    float viscosity;
    float pressure;
    float density;
    float mass;
};

Particle::Particle(float x = 0, float y = 0, float z = 0, float m = 0) {
    Particle::pos = glm::vec3(x, y, z);
    Particle::vel = glm::vec3(0, 0, 0);
    Particle::acc = glm::vec3(0, 0, 0);
    Particle::viscosity = 0;
    Particle::pressure = 0;
    Particle::density = 0;
    Particle::mass = m;
}

void Particle::update(float dt) {
    vel += (acc * dt);
    pos += (vel * dt);
}

#endif