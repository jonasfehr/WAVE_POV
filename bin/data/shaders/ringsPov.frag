//Noise animation - Electric
//by nimitz (stormoid.com) (twitter: @stormoid)
// https://www.shadertoy.com/view/ldlXRS
//The domain is displaced by two fbm calls one for each axis.
//Turbulent fbm (aka ridged) is used for better effect.

uniform vec2 iResolution;
uniform float iGlobalTime;


#define time iGlobalTime*0.05
#define PI 3.14159265359


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
float polygonRing(vec2 p, int vertices, float size, float width) {
    float a = atan(p.x, p.y) + 0.2;
    float b = 6.28319 / float(vertices);
    return abs(cos((floor(0.5 + a / b) * b - a)) * length(p) - size)  - width;
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


void main( )
{


  vec2 st = gl_FragCoord.xy/min(iResolution.x, iResolution.y);
  float pulse = (fract(time)*8.)-2.5;//(sin(time)+1.)/2.;
  st -= vec2(0.5, 0.5);
  //  st = scale(vec2(pulse))*st;
  //  st = rotate2d(PI*.936)*st;
  //st = rotate2d( (fract(iGlobalTime/100.)-.5) * PI/6.)*st;



  float d = ring(st, pulse, 0.05);

  vec3 col = vec3(stepLines(d));

	gl_FragColor = vec4(col,1.);
}
