#ifndef SPH_H
#define SPH_H

#include <glm/glm.hpp>

#include "particle.h"

#include <vector>
using namespace std;

class SPH {
public:
    vector<Particle> &particles;
    
    SPH(vector<Particle> &_particles): particles(_particles){}
    
    void update(float deltaTime) {
        // simulate SPH
        for (auto &p : particles) {
            p.pos.y += deltaTime * 0.1f;
        }
    }
private:
};

#endif