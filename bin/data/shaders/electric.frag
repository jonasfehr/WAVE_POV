//Noise animation - Electric
//by nimitz (stormoid.com) (twitter: @stormoid)
// https://www.shadertoy.com/view/ldlXRS
//The domain is displaced by two fbm calls one for each axis.
//Turbulent fbm (aka ridged) is used for better effect.



#define time iGlobalTime*0.15
#define tau 6.2831853
//
uniform vec2 iResolution;
uniform float iGlobalTime;

mat2 makem2(in float theta){float c = cos(theta);float s = sin(theta);return mat2(c,-s,s,c);}


float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}
// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

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
float fbm(in vec2 p)
{
	float z=2.;
	float rz = 0.;
	vec2 bp = p;
	for (float i= 1.;i < 6.;i++)
	{
		rz+= abs((noise(p)-0.5)*2.)/z;
		z = z*2.;
		p = p*2.;
	}
	return rz;
}

float dualfbm(in vec2 p)
{
    //get two rotated fbm calls and displace the domain
	vec2 p2 = p*.7;
	vec2 basis = vec2(fbm(p2-time*1.6),fbm(p2+time*1.7));
	basis = (basis-.5)*.2;
	p += basis;

	//coloring
	return fbm(p*makem2(time*0.2));
}

float circ(vec2 p)
{
	float r = length(p);
	r = log(sqrt(r));
	return abs(mod(r*4.,tau)-3.14)*3.+.2;

}

void main( )
{
	//setup system
	vec2 p = gl_FragCoord.xy / iResolution.xy-0.5;
	p.x *= iResolution.x/iResolution.y;
	p*=50.;

    float rz = dualfbm(p);

	//rings
	p /= exp(mod(time*10.,3.14159));
	rz *= pow(abs((0.1-circ(p))),.9);//pow(abs((0.1-circ(p))),.9);

	//final color
	vec3 col = vec3(0.15)/rz;
	col=pow(abs(col),vec3(.99));
	gl_FragColor = vec4(col,1.);
}
