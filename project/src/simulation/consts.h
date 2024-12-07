#ifndef CONSTS_H
#define CONSTS_H

#include <glm/glm.hpp>
#include <vector>

const double PI = 3.14159265358979;
const float KERNEL_DISTANCE = 1.0;
const float SQR_KERNEL_DISTANCE = KERNEL_DISTANCE * KERNEL_DISTANCE;
const float KERNEL_FACTOR = 315.0 / 64.0 / PI / pow(KERNEL_DISTANCE, 6);
const float LAP_FACTOR = 45.0 / PI / pow(KERNEL_DISTANCE, 6);
const float GRAD_FACTOR = -45.0 / PI / pow(KERNEL_DISTANCE, 6);

class Consts {
public:
  static float poly6Kernel(glm::vec3 r);
  static float poly6Lap(glm::vec3 r);
  static glm::vec3 poly6Grad(glm::vec3 r);
};

float Consts::poly6Kernel(glm::vec3 r) {
    float len = glm::length(r);
    if (KERNEL_DISTANCE <= len) return 0.0;
    float sqrd = pow(len, 2);
    return KERNEL_FACTOR * pow(SQR_KERNEL_DISTANCE - sqrd, 3);
}

float Consts::poly6Lap(glm::vec3 r) {
    float len = glm::length(r);
    if (KERNEL_DISTANCE <= len) return 0.0;
    return LAP_FACTOR * (KERNEL_DISTANCE - len);
}

glm::vec3 Consts::poly6Grad(glm::vec3 r) {
    float len = glm::length(r);
    if (len == 0) return glm::vec3(0);
    if (KERNEL_DISTANCE <= len) return glm::vec3(0);
    float val = GRAD_FACTOR * pow(KERNEL_DISTANCE - len, 2) / len;
    return val * r;
}

#endif