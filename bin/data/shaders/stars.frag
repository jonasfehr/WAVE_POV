
// uniform vec2 iResolution;
// uniform float iGlobalTime;

#define time iGlobalTime*0.15


#define TWO_PI 6.28318530718


vec4 FAST32_hash_2D_Cell( vec2 gridcell )	//	generates 4 different random numbers for the single given cell point
{
    //	gridcell is assumed to be an integer coordinate
    const vec2 OFFSET = vec2( 26.0, 161.0 );
    const float DOMAIN = 71.0;
    const vec4 SOMELARGEFLOATS = vec4( 951.135664, 642.949883, 803.202459, 986.973274 );
    vec2 P = gridcell - floor(gridcell * ( 1.0 / DOMAIN )) * DOMAIN;
    P += OFFSET.xy;
    P *= P;
    return fract( (P.x * P.y) * ( 1.0 / SOMELARGEFLOATS.xyzw ) );
}

float Falloff_Xsq_C1( float xsq ) { xsq = 1.0 - xsq; return xsq*xsq; } // ( 1.0 - x*x )^2   ( Used by Humus for lighting falloff in Just Cause 2.  GPUPro 1 )

float Stars2D(	vec2 P,
                float probability_threshold,		//	probability a star will be drawn  ( 0.0->1.0 )
                float max_dimness,					//	the maximal dimness of a star ( 0.0->1.0   0.0 = all stars bright,  1.0 = maximum variation )
                float two_over_radius )				//	fixed radius for the stars.  radius range is 0.0->1.0  shader requires 2.0/radius as input.
{
    //	establish our grid cell and unit position
    vec2 Pi = floor(P);
    vec2 Pf = P - Pi;

    //	calculate the hash.
    //	( various hashing methods listed in order of speed )
    vec4 hash = FAST32_hash_2D_Cell( Pi );
    //vec4 hash = FAST32_hash_2D( Pi * 2.0 );		//	Need to multiply by 2.0 here because we want to use all 4 corners once per cell.  No sharing with other cells.  It helps if the hash function has an odd domain.
    //vec4 hash = BBS_hash_2D( Pi * 2.0 );
    //vec4 hash = SGPP_hash_2D( Pi * 2.0 );
    //vec4 hash = BBS_hash_hq_2D( Pi * 2.0 );

    //	user variables
    float VALUE = 1.0 - max_dimness * hash.z;

    //	calc the noise and return
    Pf *= two_over_radius;
    Pf -= ( two_over_radius - 1.0 );
    Pf += hash.xy * ( two_over_radius - 2.0 );
    return ( hash.w < probability_threshold ) ? ( Falloff_Xsq_C1( min( dot( Pf, Pf ), 1.0 ) ) * VALUE ) : 0.0;
}

float mapNum(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s-a1)*(b2-b1)/(a2-a1);
}
float random (in vec2 _st) {
    return fract( sin( dot( _st.xy, vec2(12.9898,78.233) ) ) * 43758.5453123);
}


void main()
{

    vec2 st = gl_FragCoord.xy / iResolution.xy;
    st.x *= iResolution.x/iResolution.y;

     st *= 10.12;
     //st.x += time*10.;
    // st.x = floor(st.x)/40.;



    vec2 p = st;
    float probability_threshold = .1;	//	probability a star will be drawn  ( 0.0->1.0 )
    float max_dimness = 0.5;					//	the maximal dimness of a star ( 0.0->1.0   0.0 = all stars bright,  1.0 = maximum variation )
    float two_over_radius = .5;      //	fixed radius for the stars.  radius range is 0.0->1.0  shader requires 2.0/radius as input.

    float stars = Stars2D( p, probability_threshold, max_dimness, two_over_radius);




    vec3 color = vec3(stars);


    gl_FragColor =  vec4(color, 1.);
}
