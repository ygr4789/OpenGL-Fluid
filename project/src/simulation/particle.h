#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <vector>

/* Properties of Water */
constexpr float WATER_DENSITY = 300;
constexpr float WATER_GAS_CONSTANT = 10;
constexpr float WATER_VISCOSITY = 0.5;
constexpr float GRAVITY = -9.8;

/* Boundary Condition */
constexpr float BOUNDARY = 5;
constexpr float EPSILON = 0.1;

using namespace std;

struct Particle {
    Particle(float x, float y, float z, float m, bool b);
    void update(float dt);
    void print();
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
    if (pos.x < -(BOUNDARY - EPSILON))
        pos.x = -(BOUNDARY - EPSILON);
    if (pos.x > BOUNDARY - EPSILON)
        pos.x = BOUNDARY - EPSILON;
    if (pos.y < -(BOUNDARY - EPSILON))
        pos.y = -(BOUNDARY - EPSILON);
    if (pos.y > BOUNDARY - EPSILON)
        pos.y = BOUNDARY - EPSILON;
    if (pos.z < -(BOUNDARY - EPSILON))
        pos.z = -(BOUNDARY - EPSILON);
    if (pos.z > BOUNDARY - EPSILON)
        pos.z = BOUNDARY - EPSILON;
}

void Particle::print() {
    printf("position : %f, %f, %f\nacceleration : %f, %f, %f\n", pos.x, pos.y, pos.z, acc.x, acc.y, acc.z);
    printf("density : %f, mass : %f\n", density, mass);
}

#endif