#version 330 core
// declare input and output
  
in VS_OUT {
    vec2 TexCoords;
} fs_in;

uniform sampler2D screenTexture;

void main()
{
    float depth = texture(screenTexture, fs_in.TexCoords).r;
    if (depth == 1.0) discard;
    gl_FragColor = vec4(depth);
}