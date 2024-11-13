#version 410 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoord;
    vec3 FragPos;
    vec3 Normal;
    vec3 Tangent;
} fs_in;

struct Material {
    sampler2D diffuseSampler;
    sampler2D specularSampler;
    sampler2D normalSampler;
    float shininess;
}; 

struct Light {
    vec3 dir;
    vec3 color; // this is I_d (I_s = I_d, I_a = 0.3 * I_d)
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform float useNormalMap;
uniform float useSpecularMap;

void main()
{
	vec3 color = texture(material.diffuseSampler, fs_in.TexCoord).rgb;
    
    vec3 I_d = light.color;
    vec3 I_a = 0.3 * I_d;
    vec3 I_s = I_d;
    
    vec3 k_d = color;
    vec3 k_a = k_d;
    float k_s = texture(material.specularSampler, fs_in.TexCoord).r;
    
    vec3 N = normalize(fs_in.Normal);
    vec3 L = -normalize(light.dir);
    
    vec3 ambient = I_a * k_a;

    // if model does not have a normal map, this should be always 0.
    // if useNormalMap is 0, we use a geometric normal as a surface normal.
    // if useNormalMap is 1, we use a geometric normal altered by normal map as a surface normal.
	if(useNormalMap > 0.5)
	{
        vec3 T = normalize(fs_in.Tangent);
        vec3 B = normalize(cross(N, T));
        mat3 TBN = mat3(T, B, N);
        vec3 TBNCoord = texture(material.normalSampler, fs_in.TexCoord).rgb * 2.0 - 1.0;
        N = normalize(TBN * TBNCoord);
	}
	
    float diff = max(0.0f, dot(L, N));
    vec3 diffuse = I_d * (diff * k_d);
    
    // if model does not have a specular map, this should be always 0.
    // if useSpecularMap is 0, ignore specular lighting.
    // if useSpecularMap is 1, calculate specular lighting.
    vec3 specular = vec3(0.0);
	if(useSpecularMap > 0.5)
	{
        //use only red channel of specularSampler as a reflectance coefficient(k_s).
        vec3 V = normalize(viewPos - fs_in.FragPos);
        vec3 R = reflect(-L, N);
        float spec =  pow(max(dot(V, R), 0.0), material.shininess);
        specular = I_s * (spec * k_s);
	}
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}