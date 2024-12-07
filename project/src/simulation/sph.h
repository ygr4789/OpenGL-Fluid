#ifndef SPH_H
#define SPH_H

#include <glm/glm.hpp>
#include <vector>

#include "particle.h"
#include "consts.h"

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
    
    void testUpdate(float dt) {
        for (auto& p : particles) {
            p.acc = glm::vec3(0, GRAVITY, 0);
        }
        for (auto& p : particles) {
            p.update(dt);
        }
    }
private:
    void computeDensity();
    void computeProperties();
    void computeAcceleration();
};

void SPH::computeDensity() {
    for (auto& p : particles) {
        p.density = 0;
        for (auto& p_ : particles) {
            glm::vec3 r = p_.pos - p.pos;
            p.density += p.mass * Consts::poly6Kernel(r);
        }
    }
}

void SPH::computeProperties() {
    for (auto& p : particles) {
        p.density = 0;
        for (auto& p_ : particles) {
            glm::vec3 r = p_.pos - p.pos;
            p.density += p_.mass * Consts::poly6Kernel(r);
        }
        p.pressure = WATER_GAS_CONSTANT * (p.density - WATER_DENSITY);
    }
}

void SPH::computeAcceleration() {
    for (auto& p : particles) {
        p.acc = glm::vec3(0, GRAVITY, 0);
        glm::vec3 acc_pressure = glm::vec3(0, 0, 0);
        glm::vec3 acc_viscosity = glm::vec3(0, 0, 0);
        
        for (auto& p_ : particles) {
            glm::vec3 r = p_.pos - p.pos;
            acc_pressure += Consts::poly6Grad(r) * (p_.mass/p_.density) * ((p.pressure+p_.pressure)/2);
            acc_viscosity += Consts::poly6Lap(r) * (p_.mass/p_.density) * (p_.vel-p.vel);
        }
        
        p.acc -= acc_pressure / p.density;
        p.acc += acc_viscosity * WATER_VISCOSITY;
    }
}

#endif