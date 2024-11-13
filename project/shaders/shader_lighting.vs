#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec2 TexCoord;
    vec3 FragPos;
    vec3 Normal;
		vec3 Tangent;
} vs_out;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_out.TexCoord = aTexCoord;
	vs_out.FragPos = vec3(world * vec4(aPos, 1.0f));
	vs_out.Normal = mat3(world) * aNormal;
	vs_out.Tangent = mat3(world) * aTangent;

	gl_Position = projection * view * world * vec4(aPos, 1.0f);
}
