// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
// http://patriciogonzalezvivo.com

 uniform vec2 iResolution;
 uniform float iGlobalTime;

#define PI 3.14159265359
#define TWO_PI 6.28318530718

float random (in vec2 _st) {
    return fract(sin(dot(_st.xy,
                         vec2(12.9898,78.233)))*
                 43758.5453123);
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

#define NUM_OCTAVES 2

float fbm ( in vec2 _st) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(20.0);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5),
                    -sin(0.5), cos(0.50));
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise(_st);
        _st = rot * _st * 2.2 + shift;
        a *= 0.5;
    }
    return v;
}

float triangle(vec2 st){
    // Number of sides of your shape
    int N = 3;

    // Angle and radius from the current pixel
    float a = atan(st.x,st.y);
    float r = TWO_PI/float(N);

    // Shaping function that modulate the distance
    float d = cos(floor(.5+a/r)*r-a)*length(st);

    return 1.0-smoothstep(.4,.41,d);
}

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

mat2 scale(vec2 _scale){
    return mat2(_scale.x,0.0,
                0.0,_scale.y);
}

#define numGates 40.

void main(  ) {
    vec2 st = gl_FragCoord.xy/iResolution.xy;

    vec2 stTri = st;
    stTri.x *= iResolution.x/iResolution.y;
    // Remap the space to -1. to 1.
    stTri = st *2.-1.;
    // translate center
    stTri += vec2(0.,-0.15);

    stTri = rotate2d( iGlobalTime*PI /10.) * stTri;
    stTri = scale(vec2(6.))*stTri;
    float triangle = triangle(stTri);

    st.y -= iGlobalTime/100.;
    st *= 20.;

    vec3 color = vec3(1.);
    vec2 a = vec2(0.);
    vec2 b = vec2(0.);
    vec2 c = vec2(60.,800.);

    a.x = fbm( st + 0.215*iGlobalTime );
    a.y = fbm( st + vec2(1.0));

    b.x = fbm( st + 4.*a);
    b.y = fbm( st);

    c.x = fbm( st + 7.0*b + vec2(10.7,.2)+ .715*iGlobalTime );
    c.y = fbm( st + 3.944*b + vec2(.3,12.8)+ 1.16*iGlobalTime);

    float f = fbm(st+b+c);

    // color = mix(vec3(0.445,0.002,0.419), vec3(1.000,0.467,0.174), clamp((f*f),0.2, 1.0));
    // color = mix(color, vec3(0.413,0.524,0.880), clamp(length(c.x),0.480, 0.92));

    vec3 finalColor = vec3(color*triangle*f);

    gl_FragColor = vec4( finalColor, 1.);//triangle);
}
