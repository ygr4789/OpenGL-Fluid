#ifndef CONSTS_H
#define CONSTS_H

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <vector>

constexpr double PI = 3.14159265358979;
constexpr float KERNEL_DISTANCE = 1.0;
constexpr float SQR_KERNEL_DISTANCE = KERNEL_DISTANCE * KERNEL_DISTANCE;
constexpr float HEX_KERNEL_DISTANCE = SQR_KERNEL_DISTANCE * SQR_KERNEL_DISTANCE * SQR_KERNEL_DISTANCE;
constexpr float KERNEL_FACTOR = 315.0 / 64.0 / PI / HEX_KERNEL_DISTANCE;
constexpr float LAP_FACTOR = 45.0 / PI / HEX_KERNEL_DISTANCE;
constexpr float GRAD_FACTOR = -45.0 / PI / HEX_KERNEL_DISTANCE;

class Consts {
public:
  static float poly6Kernel(glm::vec3 r);
  static float poly6Lap(glm::vec3 r);
  static glm::vec3 poly6Grad(glm::vec3 r);
};

float Consts::poly6Kernel(glm::vec3 r) {
    float sqrd = glm::length2(r);
    return KERNEL_FACTOR * pow(SQR_KERNEL_DISTANCE - sqrd, 3);
}

float Consts::poly6Lap(glm::vec3 r) {
    float len = glm::length(r);
    return LAP_FACTOR * (KERNEL_DISTANCE - len);
}

glm::vec3 Consts::poly6Grad(glm::vec3 r) {
    float len = glm::length(r);
    if (len == 0) return glm::vec3(0);
    float val = GRAD_FACTOR * pow(KERNEL_DISTANCE - len, 2) / len;
    return val * r;
}

#endif