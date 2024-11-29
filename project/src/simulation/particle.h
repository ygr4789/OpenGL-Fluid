#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <vector>

/* Properties of Water */
#define WATER_DENSITY 998
#define WATER_GAS_CONSTANT 2000
#define WATER_VISCOSITY 0.5
#define WATER_COLOR "#0000ff"
#define WATER_PARTICLE_RADIUS 0.1
#define GRAVITY glm::vec3(0, -9.8, 0)

using namespace std;

struct Particle {
    Particle(float x, float y, float z, float m, bool b);
    void update(float dt);
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 acc;
    glm::vec3 viscosity;
    float pressure;
    float density;
    float mass;
    bool boundary;
};

Particle::Particle(float x = 0, float y = 0, float z = 0, float m = 0, bool b = false) {
    Particle::pos = glm::vec3(x, y, z);
    Particle::vel = glm::vec3(0, 0, 0);
    Particle::acc = glm::vec3(0, 0, 0);
    Particle::viscosity = glm::vec3(0,0,0);
    Particle::pressure = 0;
    Particle::density = 0;
    Particle::mass = m;
    Particle::boundary = b;
}

void Particle::update(float dt) {
    vel += (acc * dt);
    pos += (vel * dt);
}

#endif