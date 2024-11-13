#version 410 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
} fs_in;

void main()
{
    vec3 color = vec3(1.0, 1.0, 1.0);
    FragColor = vec4(color, 1.0);
}