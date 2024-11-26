#ifndef SPH_H
#define SPH_H

#include <glm/glm.hpp>
#include <vector>

#include "particle.h"

/* Kernel Function Coefficient */
#define KERNEL_DISTANCE 1
#define SQR_KERNEL_DISTANCE pow(KERNEL_DISTANCE, 2)
#define KERNEL_FACTOR 315 / 64 / AI_MATH_PI / pow(KERNEL_DISTANCE, 9)
#define GRAD_FACTOR -45 / AI_MATH_PI / pow(KERNEL_DISTANCE, 6)
#define LAP_FACTOR 45 / AI_MATH_PI / pow(KERNEL_DISTANCE, 6)

using namespace std;

class SPH {
public:
    vector<Particle> &particles;
    
    SPH(vector<Particle> &_particles): particles(_particles){}
    
    void update(float dt) {
        // simulate SPH
        for (auto &p : particles) {
            //p.pos.y += dt * 0.1f;
        }
    }
private:
    float poly6Kernel(glm::vec3 r);
    glm::vec3 poly6Grad(glm::vec3 r);
    float poly6Lap(glm::vec3 r);
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

#endif