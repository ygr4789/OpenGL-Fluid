#ifndef SPH_H
#define SPH_H

#include <glm/glm.hpp>
#include <vector>

#include "particle.h"
#include "consts.h"
#include "hash.h"

using namespace std;

class SPH {
public:
    vector<Particle> &particles;
    
    SPH(vector<Particle> &_particles): particles(_particles){}

    void init() {
        computeDensity();
        float max_density = 0.0;
        for (auto &p : particles) {
            max_density = max(max_density, p.density);
        }
        const float WATER_PARTICLE_MASS = WATER_DENSITY / max_density;
        printf("%f\n", WATER_PARTICLE_MASS);
        for (auto &p : particles) {
            p.mass = WATER_PARTICLE_MASS;
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
    void computeDensity();
    void computeProperties();
    void computeAcceleration();
};

void SPH::computeDensity() {
    for (auto &p : particles) {
        p.density = 0;
        for (const auto &p_ : particles) {
            glm::vec3 r = p.pos - p_.pos;
            p.density += p.mass * Consts::poly6Kernel(r);
        }
    }
}

void SPH::computeProperties() {
    for (auto &p : particles) {
        p.density = 0;
        for (const auto &p_ : particles) {
            glm::vec3 r = p.pos - p_.pos;
            p.density += p_.mass * Consts::poly6Kernel(r);
        }
        p.pressure = WATER_GAS_CONSTANT * (p.density - WATER_DENSITY);
    }
}

void SPH::computeAcceleration() {
    Hash hash;
    hash.hashing(particles);
    for (auto& p : particles) {
        p.acc = glm::vec3(0, GRAVITY, 0);
        glm::vec3 acc_pressure = glm::vec3(0, 0, 0);
        glm::vec3 acc_viscosity = glm::vec3(0, 0, 0);
        
        for (auto& p_ : particles) {
            glm::vec3 r = p.pos - p_.pos;
            acc_pressure += Consts::poly6Grad(r) * (p_.mass/p_.density) * ((p.pressure+p_.pressure)/2);
            acc_viscosity += Consts::poly6Lap(r) * (p_.mass/p_.density) * (p.vel-p_.vel);
        }
        
        p.acc -= acc_pressure / p.density;
        p.acc -= acc_viscosity * WATER_VISCOSITY;
    }
}

#endif