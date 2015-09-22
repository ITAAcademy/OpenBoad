//pixelize
uniform sampler2D from;
 uniform float animationKey;
 vec2 resolution=vec2(640,480);

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
  float revProgress = (1.0 - animationKey);
  float distFromEdges = min(animationKey, revProgress);
  float squareSize = (50.0 * distFromEdges) + 1.0;

  vec2 p = (floor((gl_FragCoord.xy + squareSize * 0.5) / squareSize) * squareSize) / resolution.xy;
  vec4 fromColor = texture2D(from, p);
  vec4 toColor = vec4(fromColor.rgb,0);
  vec4 testColor = vec4(0.12,0.5,0.3,1);
  gl_FragColor = mix(fromColor, toColor, animationKey);
}
