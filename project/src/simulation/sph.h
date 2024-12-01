#ifndef SPH_H
#define SPH_H

#include <glm/glm.hpp>
#include <vector>

#include "particle.h"

constexpr float power(float x, int n) {
    float res = 1;
    for (int i = 0; i < n; i++) {
        res *= x;
    }
    return res;
}

/* Kernel Function Coefficient */
constexpr float KERNEL_DISTANCE = 1;
constexpr float SQR_KERNEL_DISTANCE = power(KERNEL_DISTANCE, 2);
constexpr float KERNEL_FACTOR = 315 / 64 / AI_MATH_PI / power(KERNEL_DISTANCE, 9);
constexpr float GRAD_FACTOR = -45 / AI_MATH_PI / power(KERNEL_DISTANCE, 6);
constexpr float LAP_FACTOR = 45 / AI_MATH_PI / power(KERNEL_DISTANCE, 6);

using namespace std;

class SPH {
public:
    vector<Particle> &particles;
    
    SPH(vector<Particle> &_particles): particles(_particles){}

    void init() {
        computeDensity();
        float max_density = 0.0;
        for (auto p : particles) {
            max_density = max(max_density, p.density);
        }
        float mass_particle = WATER_DENSITY / max_density;
        for (auto p : particles) {
            p.mass = mass_particle;
        }
        computeProperties();
    }
    
    void update(float dt) {
        computeProperties();
        computeAcceleration();
        for (auto &p : particles) {
            p.update(dt);
        }
    }
private:
    float poly6Kernel(glm::vec3 r);
    glm::vec3 poly6Grad(glm::vec3 r);
    float poly6Lap(glm::vec3 r);
    void computeDensity();
    void computeProperties();
    void computeAcceleration();
};

float SPH::poly6Kernel(glm::vec3 r) {
    float len = glm::length(r);
    if (KERNEL_DISTANCE <= len) return 0.0;
    float sqrd = pow(len, 2);
    return KERNEL_FACTOR * pow(SQR_KERNEL_DISTANCE - sqrd, 3);
}

glm::vec3 SPH::poly6Grad(glm::vec3 r) {
    float len = glm::length(r);
    if (KERNEL_DISTANCE <= len) return glm::vec3(0, 0, 0);
    float val = pow(KERNEL_DISTANCE - len, 2) / len;
    return val * r;
}

float SPH::poly6Lap(glm::vec3 r) {
    float len = r.length();
    if (KERNEL_DISTANCE <= len) return 0.0;
    return LAP_FACTOR * (KERNEL_DISTANCE - len);
}

void SPH::computeDensity() {
    for (auto& p : particles) {
        p.density = 0;
        for (auto& p_ : particles) {
            glm::vec3 r = p.pos - p_.pos;
            p.density += p.mass * poly6Kernel(r);
        }
    }
}

void SPH::computeProperties() {
    for (auto& p : particles) {
        p.density = 0;
        for (auto& p_ : particles) {
            glm::vec3 r = p.pos - p_.pos;
            //printf("%f, %f, %f\n", r.x, r.y, r.z);
            p.density += p.mass * poly6Kernel(r);
            //p.print();
        }
        p.pressure = WATER_GAS_CONSTANT * (p.density - WATER_DENSITY);
    }
}

void SPH::computeAcceleration() {
    for (auto& p : particles) {
        p.acc = glm::vec3(0, GRAVITY, 0);
        glm::vec3 acc_pressure = glm::vec3(0, 0, 0);
        glm::vec3 acc_viscosity = glm::vec3(0, 0, 0);
        glm::vec3 acc_p_ = glm::vec3(0, 0, 0);
        glm::vec3 acc_v_ = glm::vec3(0, 0, 0);
        //p.print();
        for (auto& p_ : particles) {
            glm::vec3 r = p.pos - p_.pos;
            acc_p_ = poly6Grad(r);
            if (p.boundary) acc_p_ *= (p_.mass / p.density) * MAX(0.0, p.pressure);
            else acc_p_ *= ((p_.mass / p_.density) * (p.pressure + p_.pressure)) / 2;
            acc_pressure += acc_p_;

            acc_v_ = p.vel - p_.vel;
            if (p.boundary) acc_v_ *= (p_.mass / p.density) * poly6Lap(r);
            else acc_v_ *= (p_.mass / p_.density) * poly6Lap(r);
            acc_viscosity += acc_v_;
        }
        p.acc = -(p.acc + acc_pressure) / p.density;
        p.acc = -WATER_VISCOSITY * (p.acc + acc_viscosity);
        //p.print();
    }
}

#endif