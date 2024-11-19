#ifndef SPH_H
#define SPH_H

#include <glm/glm.hpp>
#include <vector>

#include "particle.h"

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
    float poly6Kernel(glm::vec3 r, float h);
    glm::vec3 poly6Grad(glm::vec3 r, float h);
    float poly6Lap(glm::vec3 r, float h);
};

float SPH::poly6Kernel(glm::vec3 r, float h) {
    float coef = 315 / (64 * AI_MATH_PI_F * (pow(h, 9)));
    float val = pow(h, 2) - pow(r.length(), 2);
    float res = coef * pow(val, 3);
    return res;
}

glm::vec3 SPH::poly6Grad(glm::vec3 r, float h) {
    float coef = -45 / (AI_MATH_PI_F * (pow(h, 6)));
    float len = r.length();
    float val = pow(h - len, 2) / len;
    glm::vec3 res = coef * val * r;
    return res;
}

float SPH::poly6Lap(glm::vec3 r, float h) {
    float coef = 45 / (AI_MATH_PI_F * (pow(h, 6)));
    float len = r.length();
    float val = h - len;
    float res = coef * val;
    return res;
}

#endif