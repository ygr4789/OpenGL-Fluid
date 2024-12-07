#version 410 core

in VS_OUT {
    float ViewDepth;
    float SphereRadius;
} fs_in;

uniform float fillRate;

void main()
{
    vec3 N;
    vec2 pointCoord = gl_PointCoord / fs_in.ViewDepth;
    N.xy = pointCoord * 2.0f - 1.0f;
    float r2 = dot(N.xy, N.xy);
    if (r2 > 1.0f) discard;
    
    float thickness = sqrt(1.0f - r2);
    gl_FragColor = vec4(thickness * fillRate);
}