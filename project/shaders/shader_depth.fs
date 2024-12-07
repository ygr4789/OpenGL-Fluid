#version 410 core

in VS_OUT {
    vec3 FragPos;
    vec3 ViewPos;
    float ViewDepth;
    float SphereRadius;
} fs_in;

struct Light {
    vec3 dir;
    vec3 color; // this is I_d (I_s = I_d, I_a = 0.3 * I_d)
};

uniform mat4 projection;
uniform Light light;

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
    
    vec3 color = vec3(0.0f, 0.0f, 1.0f);
    float diffuse = max(0.0f, dot(N, light.dir));
    gl_FragColor = vec4(diffuse * color, 1.0);
}