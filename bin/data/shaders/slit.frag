#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect texForSlit;
uniform sampler2DRect texCounters;

//uniforms
//tempodivision 20]
uniform vec2 u_resolution;//100,1000;100,1000]
uniform vec2 u_texResolution;
uniform float u_time;//0.1,0.,1.]
uniform float u_density;//0.1,0.,1.]
uniform float u_contrast;//0.,0.,1.]
uniform float u_H;//0.,0.,1.]
uniform float u_S;//0.,0.,1.]
uniform float u_B;//0.,0.,1.]
uniform float u_direction;
uniform float u_mix;



vec3 hsv2rgb_smooth( in vec3 c ){
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 ); 
    rgb = rgb*rgb*(3.0-2.0*rgb); 
    return c.z * mix( vec3(1.0), rgb, c.y);}

vec3 hsv2rgb( in vec3 c )
{
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
    return c.z * mix( vec3(1.0), rgb, c.y);
}

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


void main()
{
    
    vec2 st = gl_FragCoord.xy / u_resolution.xy;

    vec2 countersRes = vec2(40,1);
    float alpha = texture2DRect(texCounters, gl_FragCoord.xx).r;

    vec2 coord = vec2( alpha, st.y);
  
    
    vec3 tex = texture2DRect(texForSlit, coord  * vec2(u_texResolution.x, u_texResolution.y-0.5) ).rgb;

    

//    vec3 hsv = vec3( u_H, u_S, u_B);
//    
//    vec3 rgbMul = hsv2rgb(hsv)*(noise * impulses);
//    vec3 rgbMax = hsv2rgb(hsv)*max(noise , impulses);
//    vec3 mixed = mix(rgbMul,rgbMax, u_mix);
//
//    vec3 impulsesColored = impulsesCol*(u_mix);
    
    vec3 finalCol = hsv2rgb(vec3(u_H, u_S, u_B)) * tex;

    gl_FragColor =  vec4(finalCol, brightness(tex));
}


