#version 330 core
#define EPS 1e-5

in VS_OUT {
  vec2 TexCoords;
} fs_in;

uniform float sigmaS;
uniform float sigmaL;

uniform sampler2D screenTexture;

void main()
{
  float depth = texture(screenTexture, fs_in.TexCoords).r;
  if (depth == 1.0) discard;
  
  float sigS = max(sigmaS, EPS);
  float sigL = max(sigmaL, EPS);

  float facS = -1./(2.*sigS*sigS);
  float facL = -1./(2.*sigL*sigL);

  float sumW = 0.;
  float sumC = 0.;
  float halfSize = sigS * 2;
  ivec2 textureSize2 = textureSize(screenTexture, 0);

  for (float i = -halfSize; i <= halfSize; i ++){
    for (float j = -halfSize; j <= halfSize; j ++){
      vec2 pos = vec2(i, j);
      float offsetDepth = texture(screenTexture, fs_in.TexCoords + pos / textureSize2).r;
      if (offsetDepth == 1.0) offsetDepth = depth;

      float distS = length(pos);
      float distL = offsetDepth - depth;

      float wS = exp(facS*float(distS*distS));
      float wL = exp(facL*float(distL*distL));
      float w = wS*wL;

      sumW += w;
      sumC += offsetDepth * w;
    }
  }

  gl_FragDepth = sumC/sumW;
}