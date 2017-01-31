
uniform vec2 iResolution;
uniform float iGlobalTime;
#define rnd(r) fract(4579.0 * sin(1957.0 * (r)))

void main()
{
    float t = iGlobalTime;
	vec2 pos = gl_FragCoord.xy/ iResolution.xy;
    vec4 col = vec4(0.0);

    for (float i = 0.0; i < 20.0; i += 1.0)
    {
        float r = (0.5 + 0.5 * rnd(i)) * 0.03;
        vec2 point = rnd(i + vec2(1.0, 6.0));
        vec2 veloc =  rnd(i + vec2(3.0, 4.0)) - 0.5;

        vec2 point_real = fract(point + veloc * t * 0.05);
        float dist = length(point_real - pos) / r;

        if (dist < 1.0)
        {
            col += 1.0 - dist;
        }
    }

    gl_FragColor = col;
}
