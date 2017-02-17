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
  float lines = step(0.99, sin(p.x*TWO_PI*50.));

  return lines;
}


void main(  ) {
  vec2 st = gl_FragCoord.xy/iResolution.xy;
   float size = cubicIn(rippleSize*.99);//fract(iGlobalTime/10.));
   float mask = 1.-step(size, distance(st.x*2., 1.));

  st -= vec2(0.5);
  st = scale(vec2(1.-size))*st;
   st = rotate2d(-PI/6.)*st;
  //st = rotate2d( (fract(iGlobalTime/100.)-.5) * PI/6.)*st;
  st += vec2(0.5);



    // st = rotate2d( sin(fract(time)*TWO_PI)*10. ) * st;
 	 float lines = lines(st);//step(0.99, sin((1.-rippleSize)*distance(st.x,0.5)*TWO_PI*20.));
  gl_FragColor = vec4( vec3(mask*lines), 1.);
}
