#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <vector>

/* Properties of Water */
constexpr float WATER_DENSITY = 998;
constexpr float WATER_GAS_CONSTANT = 1000;
constexpr float WATER_VISCOSITY = 1;
constexpr float GRAVITY = -9.8;

/* Boundary Condition */
constexpr float BOUNDARY = 5;
constexpr float EPSILON = 0.1;

using namespace std;

struct Particle {
    Particle(float x, float y, float z, float m);
    void update(float dt);
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 acc;
    glm::vec3 viscosity;
    float pressure;
    float density;
    float mass;
};

Particle::Particle(float x = 0, float y = 0, float z = 0, float m = 1) {
    Particle::pos = glm::vec3(x, y, z);
    Particle::vel = glm::vec3(0);
    Particle::acc = glm::vec3(0);
    Particle::viscosity = glm::vec3(0);
    Particle::pressure = 0;
    Particle::density = 0;
    Particle::mass = m;
}

void Particle::update(float dt) {
    vel += (acc * dt);
    pos += (vel * dt);
    if (pos.x < -(BOUNDARY - EPSILON)) {
        pos.x = -(BOUNDARY - EPSILON);
        vel.x = 0.1;
    }
    if (pos.x > BOUNDARY - EPSILON) {
        pos.x = BOUNDARY - EPSILON;
        vel.x = - 0.1;
    }
    if (pos.y < -(BOUNDARY - EPSILON)) {
        pos.y = -(BOUNDARY - EPSILON);
        vel.y = 0.1;
    }
    if (pos.y > BOUNDARY - EPSILON) {
        pos.y = BOUNDARY - EPSILON;
        vel.y = - 0.1;
    }
    if (pos.z < -(BOUNDARY - EPSILON)) {
        pos.z = -(BOUNDARY - EPSILON);
        vel.z = 0.1;
    }
    if (pos.z > BOUNDARY - EPSILON) {
        pos.z = BOUNDARY - EPSILON;
        vel.z = - 0.1;
    }
}

#endif