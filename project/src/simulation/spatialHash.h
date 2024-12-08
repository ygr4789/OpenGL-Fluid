#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include <glm/glm.hpp>
#include <vector>
#include <cstdio>

#include "particle.h"
#include "consts.h"

const float PADDING = 2;

// Globals
int max_xi = static_cast<int>(std::floor(((BOUNDARY + PADDING) * 2) / KERNEL_DISTANCE));
int max_yi = static_cast<int>(std::floor(((BOUNDARY + PADDING) * 2) / KERNEL_DISTANCE));
int max_zi = static_cast<int>(std::floor(((BOUNDARY + PADDING) * 2) / KERNEL_DISTANCE));

int particleNum;
std::vector<int> indexOfNum;
std::vector<int> countTable;
std::vector<int> particleTable;
int tableSize = max_xi * max_yi * max_zi;

// Hash function to calculate index
int index(int xi, int yi, int zi) {
  return xi * max_xi * max_yi + yi * max_yi + zi;
}

// Bind hash table
void bindHashTable(const std::vector<Particle>& particles) {
  tableSize = max_xi * max_yi * max_zi;
  particleNum = particles.size();
  indexOfNum.resize(particleNum);
  countTable.resize(tableSize + 1, 0);
  particleTable.resize(particleNum);
}

// Update hash table
void updateHashTable(const std::vector<Particle>& particles) {
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
}

// Find neighbors using hash table
std::vector<int> hashNearNeighbors(const glm::vec3& loc) {
  std::vector<int> ret;
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
          int t = particleTable[k];
          assert(particleNum > k && particleNum > t);
          ret.push_back(t);
        }
      }
    }
  }
  return ret;
}

#endif