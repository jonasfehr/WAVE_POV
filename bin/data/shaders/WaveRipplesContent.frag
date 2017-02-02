// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
// http://patriciogonzalezvivo.com

uniform vec2 iResolution;
uniform float iGlobalTime;
uniform float rippleSize;
// float rippleSize = 0.1;
#define time iGlobalTime*0.03
#define TAU 6.2831853

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

float ring( vec2 _st, float _radius, float _stroke)
{
  float radius = _radius*2.;
   float thickness = _stroke;
   float ring = 1.-smoothstep(distance(_st,vec2(0.5)),radius, radius+thickness);
   ring += 1.-smoothstep(distance(_st,vec2(0.5)), radius, radius-thickness);
   return ring;
}

float circ(vec2 p)
{
	float r = length(p);
	r = log(sqrt(r));
	return abs(mod(r*4.,TAU)-3.14)*3.+.2;

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


void main(  ) {
    vec2 st = (gl_FragCoord.xy )/min(iResolution.x,iResolution.y);




    st*=20.; // ZOOM

    st.x += iGlobalTime/20.;

    vec3 color = vec3(1.);
    vec2 a = vec2(0.);
    vec2 b = vec2(0.);
    vec2 c = vec2(0.);

    a.x = fbm( st);
    a.y = fbm( st + vec2(1.0));

    b.x = fbm( st + 4. * a );
    b.y = fbm( st + 0.16*iGlobalTime);

    c.x = fbm( st + 7.0*b + vec2(10.7,.2)+ 0.215*iGlobalTime/10. );
    c.y = fbm( st + 3.944*b + vec2(.3,12.8)+ 0.16*iGlobalTime);

    float f = fbm(c);

//    color = mix(vec3(1.0,.0,.0), vec3(1.,.0,0.0), clamp((f*f),0.6, 1.));
//    color = mix(vec3(0.5,0.1,0.1), vec3(0.413,0.524,0.880), clamp(length(c.x),0., 1.));
  float contrast = 0.9;
  f = ((f - 0.5) * max(contrast+0.5, 0.0)) + 0.5;



// Create pulses
   vec2 stDot = gl_FragCoord.xy / iResolution.xy;

 	 float rings = ring( stDot, rippleSize, 0.05);


    vec3 finalColor = vec3(clamp(f*(rings), 0., 1.));

    gl_FragColor = vec4( finalColor, 1.);
}
