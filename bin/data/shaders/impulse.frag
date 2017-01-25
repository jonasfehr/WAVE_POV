

uniform	float u_soft;
uniform	float u_length;
uniform	float u_width;
uniform	float u_symmetry;


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

void main (void) {



	//vec4 dot = texture2D(tex, gl_TexCoord[0].st);

	//vec4 dot = texture2D(tex, gl_TexCoord[0].st);

	vec2 st = gl_TexCoord[0].st;

	float circle = circle(st,0.5, 0.75);

	float cX = pcurve(st.x, (1.-u_width)*20. *u_soft				, (1.-u_width)*20. * u_soft);
	float cY = pcurve(st.y, (1.-u_length)*50. * u_soft * u_symmetry	, (1.-u_length)*50. * u_soft * (1.-u_symmetry) );
	
	vec4 dot = vec4((cX*cY));	

	//vec4 dot = vec4(vec3(circle(gl_TexCoord[0].st,0.9)),1.);
    
    gl_FragColor = dot * gl_Color;
    
}