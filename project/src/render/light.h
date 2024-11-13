#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


class DirectionalLight {
public:
	float azimuth;
	float elevation;
	glm::vec3 lightDir; // direction of light. If elevation is 90, it would be (0,-1,0)
	glm::vec3 lightColor; // this is I_d (I_s = I_d, I_a = 0.3 * I_d)

	DirectionalLight(float azimuth, float elevation, glm::vec3 lightColor) {
		this->azimuth = azimuth;
		this->elevation = elevation;
		updateLightDir();
		this->lightColor = lightColor;
	}

	DirectionalLight(glm::vec3 lightDir, glm::vec3 lightColor) {
		this->lightDir = lightDir;
		this->lightColor = lightColor;
	}

	glm::mat4 getViewMatrix(glm::vec3 center) {
		// directional light has no light position. Assume fake light position depending on camera position.
		float lightDistance = 15.0f;
		glm::vec3 lightPos = center - this->lightDir * lightDistance;
		return glm::lookAt(lightPos, center, glm::vec3(0, 1, 0));
	}

	glm::mat4 getProjectionMatrix(float near = 0.1f, float far = 50.0f) {
		// For simplicity, just use static projection matrix. (Actually we have to be more accurate with considering camera's frustum)
		return glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, near, far);
	}
	
	glm::mat4 getLightSpaceMatrix(const glm::mat4 &proj, const glm::mat4 &view) {
		const auto inv = glm::inverse(proj * view);
    std::vector<glm::vec4> corners;
    for (unsigned int x = 0; x < 2; ++x) {
        for (unsigned int y = 0; y < 2; ++y) {
            for (unsigned int z = 0; z < 2; ++z) {
                const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                corners.push_back(pt / pt.w);
            }
        }
    }

    glm::vec3 center = glm::vec3(0, 0, 0);
    for (const auto& v : corners) {
        center += glm::vec3(v);
    }
    center /= corners.size();
    const auto lightView = glm::lookAt(center, center + lightDir, glm::vec3(0.0f, 1.0f, 0.0f));

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();
    for (const auto& v : corners)
    {
        const auto trf = lightView * v;
        minX = MIN(minX, trf.x);
        maxX = MAX(maxX, trf.x);
        minY = MIN(minY, trf.y);
        maxY = MAX(maxY, trf.y);
        minZ = MIN(minZ, trf.z);
        maxZ = MAX(maxZ, trf.z);
    }

    // Tune this parameter according to the scene
    constexpr float zMult = 10.0f;
    if (minZ < 0) minZ *= zMult;
    else minZ /= zMult;
    if (maxZ < 0) maxZ /= zMult;
    else maxZ *= zMult;

    const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
		
    return lightProjection * lightView;
	}

	void updateLightDir() {
		// TODO:
		glm::mat4 rotation(1.0f);
		rotation = glm::rotate(rotation, glm::radians(azimuth), glm::vec3(0.0f, 1.0f, 0.0f));
		rotation = glm::rotate(rotation, glm::radians(elevation), glm::vec3(0.0f, 0.0f, 1.0f));
		lightDir = -glm::mat3(rotation) * glm::vec3(1.0f, 0.0f, 0.0f);
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x(azimuth) and y(elevation) direction.
	void processKeyboard(float xoffset, float yoffset)
	{
		// TODO:
		// set elevation between 15 to 80 (degree)!
		azimuth += xoffset; 
		elevation += yoffset;
		elevation = MAX(15.0f, MIN(80.0f, elevation));
		updateLightDir();
	}
};

#endif