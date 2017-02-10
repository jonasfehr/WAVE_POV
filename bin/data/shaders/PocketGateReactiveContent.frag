
// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
// http://patriciogonzalezvivo.com

uniform vec2 iResolution;
uniform float iGlobalTime;
uniform float effectPos;
uniform float width;
#define time iGlobalTime*0.03
#define TAU 6.2831853

float random (in vec2 _st) {
    return fract( sin( dot( _st.xy, vec2(12.9898,78.233) ) ) * 43758.5453123);
}


float cubicPulse( float c, float w, float x ){
    x = abs(x - c);
    if( x>w ) return 0.0;
    x /= w;
    return 1.0 - x*x*(3.0-2.0*x);
}

float mapNum(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s-a1)*(b2-b1)/(a2-a1);
}


void main(  ) {

// random
vec2 st = gl_FragCoord.xy/iResolution.xy;
float walker = cubicPulse(effectPos,width, st.x);
float rnd = 0.;
if(walker > 0.){
  float resRnd = 250.;
  st *= resRnd;
  st = floor(st)/resRnd;
  st.x += time;
  rnd = random( vec2(st) );
}

    vec3 finalColor = vec3(clamp(rnd*(walker)*2., 0., 1.));

    gl_FragColor = vec4( finalColor, 1.);
}
