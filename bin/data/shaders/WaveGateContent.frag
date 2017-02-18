#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect texForSlit;
uniform sampler2DRect texCounters;

//uniforms
//tempodivision 20]
uniform vec2 u_resolution;//100,1000;100,1000]
uniform vec2 u_texResolution;
uniform float u_time;
uniform int u_easing;
uniform int u_mode;

#define PI 3.14159265359
#define TWO_PI 6.28318530718
//  Function from Iñigo Quiles
//  www.iquilezles.org/www/articles/functions/functions.htm
float impulse( float k, float x){
    float h = k*x;
    return h*exp(1.0-h);
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

float brightness(vec3 rgb){
    return 0.299*rgb.r + 0.587*rgb.g + 0.114*rgb.b;
}

// --- Easing Functions ---
// from https://github.com/stackgl/glsl-easings
float cubicIn(float t) {
    return t * t * t;
}

float cubicOut(float t) {
    float f = t - 1.0;
    return f * f * f + 1.0;
}

float cubicInOut(float t) {
    return t < 0.5
    ? 4.0 * t * t * t
    : 0.5 * pow(2.0 * t - 2.0, 3.0) + 1.0;
}
//Circular
float circularIn(float t) {
    return 1.0 - sqrt(1.0 - t * t);
}

float easing( int mode, float value){
  if(mode == 0){
      return value;
  }else if(mode == 1){
      return cubicIn( value );
  }else if(mode == 2){
        return cubicOut( value );
    }else if(mode == 3){
        return cubicInOut( value );
    }else if(mode == 4){
        return circularIn( value );
    }
}
float ring(vec2 p, float radius, float width) {
  return abs(length(p) - radius * 0.5) - width;
}

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

mat2 scale(vec2 _scale){
    return mat2(_scale.x,0.0,
                0.0,_scale.y);
}

float lines(vec2 p){
  p = clamp(p, vec2(0.), vec2(1.));
  float lines = step(0.99, sin(p.x*TWO_PI*150.));

  return lines;
}

void main()
{

    vec2 st = gl_FragCoord.xy / u_resolution.xy;

    vec2 countersRes = vec2(40,1);
    float counter = texture2DRect(texCounters, gl_FragCoord.xx).r;

    counter = easing(u_easing, counter);

    vec3 finalCol = vec3(0);

    if(u_mode == 1){ // TEXTURE SLIT
        vec2 coord = vec2( counter, st.y);
        vec3 tex = texture2DRect(texForSlit, coord  * vec2(u_texResolution.x, u_texResolution.y-0.5) ).rgb;
        finalCol = tex;

    }else if(u_mode == 2){ // GRADIENT
        float gradient = (1.-smoothstep(st.y, 0.5, counter/2.+0.5))+(1.-smoothstep(st.y, 0.5, 0.5-counter/2.));
        finalCol = vec3(gradient);

    }else if(u_mode == 3){ // Pulses
        float counterInv = 1.-counter;
        float gradientMask = 1.-((1.-smoothstep(st.y, 0.5, counterInv/2.+0.5))+(1.-smoothstep(st.y, 0.5, 0.5-counterInv/2.)));
        float pulses = (sin((distance(st.y, 0.5)+u_time/20.)*20.*TWO_PI)+1.)/2;
        finalCol = vec3(gradientMask*pulses);//floor(pulses*2.));

    }else if(u_mode == 4){ // lines
      float counterInv = (1.-counter)*2.;
      float gradientMask = 1.-((1.-smoothstep(st.y, 0.5, counterInv/2.+0.5))+(1.-smoothstep(st.y, 0.5, 0.5-counterInv/2.)));
      st -= vec2(0.5);
      st = scale(vec2(1.-fract(u_time/100.)))*st;
      st = rotate2d(-PI)*st;
      st = rotate2d( (fract(u_time/100.)-.5) * PI/6.)*st;
      st += vec2(0.5);
       float lines = lines(st);
      finalCol = vec3(gradientMask*lines);//floor(pulses*2.));

    }else if(u_mode == 5){ // Circles
        float circles = ring(st-vec2(st.x, 0.5), counter, 0.01);
        finalCol = vec3(circles);
    }





    gl_FragColor =  vec4(finalCol, brightness(finalCol));}
