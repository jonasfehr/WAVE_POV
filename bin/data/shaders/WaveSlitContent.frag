#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect texForSlit;
uniform sampler2DRect texCounters;

//uniforms
//tempodivision 20]
uniform vec2 u_resolution;//100,1000;100,1000]
uniform vec2 u_texResolution;
uniform float u_time;
uniform int u_mode;




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

float random (in vec2 _st) {
    return fract( sin( dot( _st.xy, vec2(12.9898,78.233) ) ) * 43758.5453123);
}

float noise (in vec2 _st) {
    vec2 i = floor(_st);
    vec2 f = fract(_st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
    (c - a)* u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}


void main()
{

    vec2 st = gl_FragCoord.xy / u_resolution.xy;
    float blend = smoothstep( 0.05, 0.25,st.x);

    vec2 countersRes = vec2(40,1);
    float counter = texture2DRect(texCounters, gl_FragCoord.xx).r;

  //  counter = easing(easing, counter);

    vec3 finalCol = vec3(0);

        float n = noise(st*vec2(2., 5.)+vec2(u_time/2., 1.));
        n +=0.;
        n/=5.;
        // n=0.;
        vec2 coord = vec2( min(counter + n, 1.), min(st.y + n*.01, 1.));
        vec3 tex = texture2DRect(texForSlit, coord * vec2(u_texResolution.x, u_texResolution.y)).rgb;
        finalCol = tex*vec3(blend);






    gl_FragColor =  vec4(finalCol,1.);
  }
