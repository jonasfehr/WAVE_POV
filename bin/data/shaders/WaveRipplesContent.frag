// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
// http://patriciogonzalezvivo.com

uniform vec2 iResolution;
uniform float iGlobalTime;
uniform float rippleSize;
#define time iGlobalTime*0.03

#define PI 3.14159265359
#define TWO_PI 6.28318530718

float mapNum(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s-a1)*(b2-b1)/(a2-a1);
}

// easing
float cubicIn(float t) {
    return t * t * t;
}

float cubicOut(float t) {
    float f = t - 1.0;
    return f * f * f + 1.0;
}

//Functions from https://thebookofshaders.com/edit.php?log=160414041142
float smoothedge(float v) {
    return smoothstep(0.0, 1.0 / iResolution.x, v);
}

float circle(vec2 p, float radius) {
  return length(p) - radius;
}

float rect(vec2 p, vec2 size) {
  vec2 d = abs(p) - size;
  return min(max(d.x, d.y), 0.0) + length(max(d,0.0));
}

float roundRect(vec2 p, vec2 size, float radius) {
  vec2 d = abs(p) - size;
  return min(max(d.x, d.y), 0.0) + length(max(d,0.0))- radius;
}

float ring(vec2 p, float radius, float width) {
  return abs(length(p) - radius * 0.5) - width;
}

float hexagon(vec2 p, float radius) {
    vec2 q = abs(p);
    return max(abs(q.y), q.x * 0.866025 + q.y * 0.5) - radius;
}

float triangle(vec2 p, float size) {
    vec2 q = abs(p);
    return max(q.x * 0.866025 + p.y * 0.5, -p.y * 0.5) - size * 0.5;
}

float ellipse(vec2 p, vec2 r, float s) {
    return (length(p / r) - s);
}

float capsule(vec2 p, vec2 a, vec2 b, float r) {
    vec2 pa = p - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - r;
}

//http://thndl.com/square-shaped-shaders.html
float polygon(vec2 p, int vertices, float size) {
    float a = atan(p.x, p.y) + 0.2;
    float b = 6.28319 / float(vertices);
    return cos(floor(0.5 + a / b) * b - a) * length(p) - size;
}
float polygonRing(vec2 p, int vertices, float size) {
    float a = atan(p.x, p.y) + 0.2;
    float b = 6.28319 / float(vertices);
    return abs(cos((floor(0.5 + a / b) * b - a)) * length(p) - size-1.);
}

float stepLines(float v) {
    return (1.-step(0.1,  fract(v*10.)))*((1.-v));
}


mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

mat2 scale(vec2 _scale){
    return mat2(_scale.x,0.0,
                0.0,_scale.y);
}



void main(  ) {
  vec2 st = gl_FragCoord.xy/min(iResolution.x, iResolution.y);
   float size = cubicOut(rippleSize);// fract(iGlobalTime/10.); //
  st -= vec2(0.5, 0.5);
   st = scale(vec2((1.-size )*10.))*st;
   st = rotate2d(PI*.936)*st;
  //st = rotate2d( (fract(iGlobalTime/100.)-.5) * PI/6.)*st;


  float pulse = fract(time)*2.5-1.;//(sin(time)+1.)/2.;

  float d = polygonRing(st,5, rippleSize);
  gl_FragColor = vec4( vec3(stepLines(d)), 1.);
}
