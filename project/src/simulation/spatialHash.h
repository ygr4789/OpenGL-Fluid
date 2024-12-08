#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include <glm/glm.hpp>
#include <vector>
#include <cstdio>
#include <chrono>

#include "particle.h"
#include "consts.h"

const float PADDING = 2;

// Globals
int max_xi = static_cast<int>(std::floor(((BOUNDARY + PADDING) * 2) / KERNEL_DISTANCE));
int max_yi = static_cast<int>(std::floor(((BOUNDARY + PADDING) * 2) / KERNEL_DISTANCE));
int max_zi = static_cast<int>(std::floor(((BOUNDARY + PADDING) * 2) / KERNEL_DISTANCE));

class Hash {
private:
  int particleNum;
  std::vector<Particle>& particles;
  std::vector<int> indexOfNum;
  std::vector<int> countTable;
  std::vector<int> particleTable;
  std::vector<vector<int>> NNTable;
  void calcNN(int from);
public:
  int tableSize = max_xi * max_yi * max_zi;
  float totRuntime = 0;
  Hash(std::vector<Particle>& particles_): particles(particles_) {}
  
  void init();
  void update();
  const std::vector<int> &getNN(int from);
};

// Hash function to calculate index
int index(int xi, int yi, int zi) {
  return xi * max_xi * max_yi + yi * max_yi + zi;
}

// Bind hash table
void Hash::init() {
  particleNum = particles.size();
  indexOfNum.resize(particleNum);
  countTable.resize(tableSize + 1, 0);
  particleTable.resize(particleNum);
  NNTable.resize(particleNum);
}

// Update hash table
void Hash::update() {
  totRuntime = 0;
  for (int i = 0; i < particleNum; i++) {
    int xi = static_cast<int>(std::floor((particles[i].pos.x + (BOUNDARY + PADDING)) / KERNEL_DISTANCE));
    int yi = static_cast<int>(std::floor((particles[i].pos.y + (BOUNDARY + PADDING)) / KERNEL_DISTANCE));
    int zi = static_cast<int>(std::floor((particles[i].pos.z + (BOUNDARY + PADDING)) / KERNEL_DISTANCE));
    indexOfNum[i] = index(xi, yi, zi);
  }

  std::fill(countTable.begin(), countTable.end(), 0);
  
  for (int i = 0; i < particleNum; i++) {
    countTable[indexOfNum[i]]++;
  }
  for (int i = 0; i < tableSize; i++) {
    countTable[i + 1] += countTable[i];
  }
  for (int i = 0; i < particleNum; i++) {
    particleTable[--countTable[indexOfNum[i]]] = i;
  }
  
  for (int i = 0; i < particleNum; i++) {
    calcNN(i);
  }
}

// Find neighbors using hash table
void Hash::calcNN(int from) {
  NNTable[from].clear();
  NNTable[from].resize(0);
  const glm::vec3& loc = particles[from].pos;
  int xi = static_cast<int>(std::floor((loc.x + (BOUNDARY + PADDING)) / KERNEL_DISTANCE));
  int yi = static_cast<int>(std::floor((loc.y + (BOUNDARY + PADDING)) / KERNEL_DISTANCE));
  int zi = static_cast<int>(std::floor((loc.z + (BOUNDARY + PADDING)) / KERNEL_DISTANCE));

  for (int xi_ = xi - 1; xi_ <= xi + 1; xi_++) {
    if (xi_ < 0 || xi_ >= max_xi) continue;
    for (int yi_ = yi - 1; yi_ <= yi + 1; yi_++) {
      if (yi_ < 0 || yi_ >= max_yi) continue;
      for (int zi_ = zi - 1; zi_ <= zi + 1; zi_++) {
        if (zi_ < 0 || zi_ >= max_zi) continue;
        int i = index(xi_, yi_, zi_);
        int begin = countTable[i];
        int end = countTable[i + 1];
        for (int k = begin; k < end; k++) {
          int to = particleTable[k];
          if (glm::distance2(particles[from].pos, particles[to].pos) >= SQR_KERNEL_DISTANCE) continue;
          NNTable[from].push_back(to);
        }
      }
    }
  }
}

const std::vector<int> &Hash::getNN(int from) {
  return NNTable[from];
}

#endif