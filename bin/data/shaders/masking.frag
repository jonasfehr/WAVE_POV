uniform vec2 iResolution;
uniform float iGlobalTime;

uniform sampler2DRect tex0;
uniform vec2 texResolution;

uniform float u_opacity;
uniform float u_center;
uniform float u_width;





//  Function from Iñigo Quiles
//  www.iquilezles.org/www/articles/functions/functions.htm
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




void main()
{

  vec2 st = gl_FragCoord.xy/iResolution;

    //float mask = cubicPulse(0.5,0.20,st.x);
    float mask = cubicPulse(u_center,u_width,st.x);

    vec3 colTex0 = texture2DRect(tex0, st*texResolution).rgb;
    vec3 finalColor = colTex0 * mask;// * u_opacity + colTex0 * (1.0 - u_opacity));



    gl_FragColor =  vec4(finalColor,1.);
}
