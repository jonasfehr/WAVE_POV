uniform vec2 iResolution;
uniform float iGlobalTime;

uniform sampler2DRect tex0;
uniform vec2 texResolution;

uniform int u_mode;



float circle(in vec2 _st, in float _radius, in float _blur){
    vec2 dist = _st-vec2(0.5);
	return 1.-smoothstep(_radius-(_radius*_blur),
                         _radius+(_radius*_blur),
                         dot(dist,dist)*4.0);
}

//  Function from Iñigo Quiles
//  www.iquilezles.org/www/articles/functions/functions.htm
float pcurve( float x, float a, float b ){
    float k = pow(a+b,a+b) / (pow(a,a)*pow(b,b));
    return k * pow( x, a ) * pow( 1.0-x, b );
}

void main (){
  vec4 dot = vec4(0.);

//  if(u_mode == 1){
    vec2 st = gl_TexCoord[0].st;
    float circle = circle(st,0.1, 0.75);
    dot = vec4(circle);

//  }else if(u_mode == 2){
//    vec4 dot = texture2DRect(tex0, gl_FragCoord.xy);//gl_TexCoord[0].st);
//
//  }else if(u_mode == 3){
//    	float u_soft = 0.1;
//    	float u_length = 0.5;
//    	float u_width = 0.5;
//      vec2 st = gl_TexCoord[0].st;
//      float cX = pcurve(st.x, (1.-u_width) *20. * u_soft, (1.-u_width)*20. * u_soft );
//      float cY = pcurve(st.y, (1.-u_length)*20. * u_soft, (1.-u_length)*20. * u_soft );
//      dot = vec4(cX*cY);
//  }

    gl_FragColor = dot * gl_Color;
}
