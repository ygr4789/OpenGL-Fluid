#version 330 core
// declare input and output
  
in VS_OUT {
    vec2 TexCoords;
} fs_in;

uniform sampler2D screenTexture;

uniform mat4 view;
uniform mat4 projection;
uniform float texelSizeU;
uniform float texelSizeV;

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
    
    vec3 n = cross(ddx, ddy);
    n = normalize(n);
    
    // gl_FragColor = vec4(viewPos, 1.0);
    gl_FragColor = vec4((n+1)/2, 1.0);
}
