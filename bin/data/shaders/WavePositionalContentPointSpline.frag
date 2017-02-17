uniform vec2 iResolution;
uniform float iGlobalTime;

uniform sampler2DRect tex0;
uniform vec2 texResolution;

uniform int u_mode;

#define TAU 6.2831853

float random (in vec2 _st) {
    return fract( sin( dot( _st.xy, vec2(12.9898,78.233) ) ) * 43758.5453123);
}


float circle(vec2 p, float radius) {
  p.x = p.x*43.333333;
  return length(p) - radius;
}

float ring(vec2 p, float radius, float width) {
  p.x = p.x*86.666666/1.5;

  return 1.-step(0.000001,abs(length(p) - radius + width) - width*2.);
}

float smoothRing(vec2 p, float radius, float width) {
  p.x = p.x*86.666666/1.5;

  return 1.-smoothstep(0.0001,0.1,abs(length(p) - radius + width) - width*2.);
}

//  Function from Iñigo Quiles
//  www.iquilezles.org/www/articles/functions/functions.htm
float pcurve( float x, float a, float b ){
    float k = pow(a+b,a+b) / (pow(a,a)*pow(b,b));
    return k * pow( x, a ) * pow( 1.0-x, b );
}

void main (){

    vec2 st = gl_TexCoord[0].st;

    float ring = smoothRing(st-vec2(0.5, 0.5),0.38,0.001); //,0.1,0.01);//
    // float circle = 1.-circle(st-vec2(0.5, 0.5),0.00001);
    float rnd=0.;
    if(ring > 0.){
      float resRnd = 500.;
      st+=gl_FragCoord.x;
      st *= resRnd;
      st = floor(st)/resRnd;
      st.x += iGlobalTime*0.0001;
      rnd = random( vec2(st) );
    }
    gl_FragColor += vec4(vec3(ring), 1.);//*rnd);//max(ring+circle, 0.));
}
