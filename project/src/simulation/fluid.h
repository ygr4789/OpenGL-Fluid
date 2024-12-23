#ifndef SPH_FLUID_H
#define SPH_FLUID_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "particle.h"
#include "sph.h"

#include <vector>
#include <random>

using namespace std;


class Fluid {
public:
    /*  Fluid Data  */
    vector<Particle> particles;
    SPH simulator;
    unsigned int VAO;

    /*  Functions  */
    // constructor
    Fluid(): simulator(particles)
    {
        // test init
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<float> dist(0, 1);
        
        const float resolution = 0.5;
        const float initialSize = 7;
        for (float x = -initialSize/2; x <= initialSize/2; x+= resolution) {
            for (float y = -initialSize/2; y<= initialSize/2; y+= resolution) {
                for (float z = -initialSize/2; z<= initialSize/2; z+= resolution) {
                    float noiseX = (dist(mt) * resolution) / 10;
                    float noiseY = (dist(mt) * resolution) / 10;
                    float noiseZ = (dist(mt) * resolution) / 10;
                    
                    Particle p(x + noiseX, y + noiseY, z + noiseZ, 1);
                    particles.push_back(p);
                }
            }
        }
        simulator.init();
        setupFluid();
    }
    
    void update(float deltaTime)
    {
        simulator.update(deltaTime);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), &particles[0], GL_DYNAMIC_DRAW);  
    }

private:
    /*  Render data  */
    unsigned int VBO;

    /*  Functions    */
    // initializes all the buffer objects/arrays
    void setupFluid()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), &particles[0], GL_DYNAMIC_DRAW);  
        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);

        glBindVertexArray(0);
    }
};

#endif