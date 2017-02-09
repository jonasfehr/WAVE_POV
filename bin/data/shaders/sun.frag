uniform vec2 iResolution;
uniform float iGlobalTime;
uniform float intensity;

#define time iGlobalTime*0.15


float snoise(vec3 uv, float res)
{
	const vec3 s = vec3(1e0, 1e2, 1e3);

	uv *= res;

	vec3 uv0 = floor(mod(uv, res))*s;
	vec3 uv1 = floor(mod(uv+vec3(1.), res))*s;

	vec3 f = fract(uv); f = f*f*(3.0-2.0*f);

	vec4 v = vec4(uv0.x+uv0.y+uv0.z, uv1.x+uv0.y+uv0.z,
		      	  uv0.x+uv1.y+uv0.z, uv1.x+uv1.y+uv0.z);

	vec4 r = fract(sin(v*1e-1)*1e3);
	float r0 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

	r = fract(sin((v + uv1.z - uv0.z)*1e-1)*1e3);
	float r1 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

	return mix(r0, r1, f.z)*2.-1.;
}

float cubicIn(float t) {
    return t * t * t;
}

float cubicOut(float t) {
    float f = t - 1.0;
    return f * f * f + 1.0;
}

//Circular
float circularIn(float t) {
    return 1.0 - sqrt(1.0 - t * t);
}

void main( )
{
	vec2 p = -.5 + gl_FragCoord.xy / iResolution.xy;
	p.x *= iResolution.x/iResolution.y;

	float color = 3.0 - (3.*length(2.*p));

	vec3 coord = vec3(atan(p.x,p.y)/6.2832+.5, length(p)*.4, .5);

	for(int i = 1; i <= 7; i++)
	{
		float power = pow(2., float(i));
		color += (2.5 / power) * snoise(coord + vec3(0.,-time*.05, time*.01), power*64.);
	}

	vec2 st = gl_FragCoord.xy / iResolution.xy;

	float radius = 0.05-intensity/1000.;
	float ring = smoothstep(distance(st,vec2(0.5)),0.00001, radius);

	float fire_1 = pow(max(color,0.),2.)*0.4;
	float fire_2 = pow(max(color,0.),3.)*0.15;

	gl_FragColor = vec4( vec3(fire_2*ring*cubicIn(intensity)), 1.0);
}
