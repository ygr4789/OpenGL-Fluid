#version 410 core

in VS_OUT {
    vec3 FragPos;
    vec3 ViewPos;
    float ViewDepth;
    float SphereRadius;
} fs_in;

uniform mat4 projection;

void main()
{
    vec3 N;
    vec2 pointCoord = gl_PointCoord / fs_in.ViewDepth;
    N.xy = pointCoord * 2.0f - 1.0f;
    float r2 = dot(N.xy, N.xy);
    if (r2 > 1.0f) discard;
    N.z = sqrt(1.0f - r2);
    
    vec4 pixelPos = vec4(fs_in.ViewPos + N * fs_in.SphereRadius, 1.0f);
    vec4 clipPos = projection * pixelPos;
    gl_FragDepth = clipPos.z / clipPos.w;
}