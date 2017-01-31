uniform vec2 iResolution;
uniform float iGlobalTime;




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

void main () {

//vec2 st = gl_FragCoord.xy / iResolution.xy;
//
// st.x *= 40.; // number ofGates
// st.x = floor(st.x)*266.66666;

// Coordinate normalize
    // vec2 pos = gl_TexCoord[0].st;
    // pos.x = pos.x/80.*iResolution.x;
    // pos.y = pos.y/14.*iResolution.y;//+iResolution.y/2.;




    	//vec4 dot = texture2D(tex, gl_TexCoord[0].st);

    	//vec4 dot = texture2D(tex, gl_TexCoord[0].st);

    	 vec2 st = gl_TexCoord[0].st;
      // st.x /= 40.; // number ofGates
      // st.x = floor(st.x);//*10400.;

    	float circle = circle(st,0.1, 0.75);

      	float u_soft = 0.1;
      	float u_length;
      	float u_width;
      	float u_symmetry;

    	float cX = pcurve(st.x, (1.-u_width)*20. *u_soft				, (1.-u_width)*20. * u_soft);
    	float cY = pcurve(st.y, (1.-u_length)*50. * u_soft * u_symmetry	, (1.-u_length)*50. * u_soft * (1.-u_symmetry) );

    	vec4 dot = vec4(circle);

    	//vec4 dot = vec4(vec3(circle(gl_TexCoord[0].st,0.9)),1.);

        gl_FragColor = dot * gl_Color;

    }
