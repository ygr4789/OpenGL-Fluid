#version 410 core
// declare input and output
  
in VS_OUT {
    vec2 TexCoords;
} fs_in;

struct Light {
    vec3 dir;
    vec3 color; // this is I_d (I_s = I_d, I_a = 0.3 * I_d)
};

uniform sampler2D screenTexture;

uniform mat4 view;
uniform mat4 projection;
uniform float texelSizeU;
uniform float texelSizeV;

uniform Light light;

vec3 uvToView(vec2 uv);
vec3 blinnPhong(vec3 N, vec3 viewPos);

void main()
{
    vec2 texCoords = fs_in.TexCoords;
    float depth = texture(screenTexture, texCoords).r;
    if (depth == 1.0) discard;
    
    vec3 viewPos = uvToView(texCoords);
    
    vec3 ddx = uvToView(texCoords + vec2(texelSizeU, 0)) - viewPos;
    vec3 ddx2 = viewPos - uvToView(texCoords + vec2(-texelSizeU, 0));
    if (abs(ddx.z) > abs(ddx2.z)) ddx = ddx2;
    
    vec3 ddy = uvToView(texCoords + vec2(0, texelSizeV)) - viewPos;
    vec3 ddy2 = viewPos - uvToView(texCoords + vec2(0, -texelSizeU));
    if (abs(ddy2.z) < abs(ddy2.z)) ddy = ddy2;
    
    vec3 normal = cross(ddx, ddy);
    normal = normalize(normal);
    
    gl_FragColor = vec4(blinnPhong(normal, normalize(-viewPos)), 1.0);
}

vec3 uvToView(vec2 uv) 
{
    float depth = texture(screenTexture, uv).r;
    if (depth == 1.0) discard;
    vec2 clipXY = uv * 2.0 - 1.0;
    vec3 clipPos = vec3(clipXY, depth);
    vec4 viewSpace = vec4(inverse(projection) * vec4(clipPos, 1.0));
    vec3 viewPos = viewSpace.xyz / viewSpace.w;
    return viewPos;
}

vec3 blinnPhong(vec3 N, vec3 V)
{
    // N/V/L is normal/view/light direction expressed in view space coordinate
    
	vec3 color = vec3(1.0, 0.0, 0.0);
    
    vec3 I_d = light.color;
    vec3 I_a = 0.3 * I_d;
    vec3 I_s = I_d;
    
    vec3 k_d = color;
    vec3 k_a = k_d;
    float k_s = 0.5;
    
    vec3 L = vec3(view * vec4(-light.dir, 0.0));
    L = normalize(L);
    
    vec3 ambient = I_a * k_a;

    float diff = max(0.0f, dot(L, N));
    vec3 diffuse = I_d * (diff * k_d);

    
    float shininess = 64.0;
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), shininess);
    vec3 specular = I_s * (spec * k_s);
    
    vec3 result = ambient + diffuse + specular;
    return result;
}