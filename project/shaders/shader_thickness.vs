#version 410 core
layout (location = 0) in vec3 aPos;

out VS_OUT {
    float ViewDepth;
    float SphereRadius;
} vs_out;

uniform mat4 view;
uniform mat4 projection;
uniform int screenHeight;
uniform float fovy;
uniform float sphereRadius;

void main()
{
    
    vec3 viewPos = vec3(view * vec4(aPos, 1.0f));
    float viewDepth = -viewPos.z;
    int quadSize = int((sphereRadius / (tan(fovy / 2) * (viewDepth))) * screenHeight);
    
    vs_out.ViewDepth = viewDepth;
    vs_out.SphereRadius = sphereRadius;
    
    gl_PointSize = quadSize;
    gl_Position = projection * view * vec4(aPos, 1.0f);
}
