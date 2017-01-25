//
//  TextureMixShader.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 23/01/2017.
//
//

#ifndef TextureMixShader_h
#define TextureMixShader_h

#define STRINGIFY(x) #x

static string uniformsHeader =
STRINGIFY(
          uniform vec2 iResolution;
          uniform float iGlobalTime;
          );


static string uniforms =
STRINGIFY(
          uniform sampler2DRect tex$0;
          uniform float u_H_$0;
          uniform float u_S_$0;
          uniform float u_B_$0;
          uniform float u_contrast_$0;
          uniform float u_opacity_$0;
          uniform int u_blendMode_$0;
          uniform vec2 resolution_$0;
          );

static string functions =
STRINGIFY(
          // functions
          vec3 hsv2rgb_smooth( in vec3 c ){
              vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
              rgb = rgb*rgb*(3.0-2.0*rgb);
              return c.z * mix( vec3(1.0), rgb, c.y);}
          
          vec3 hsv2rgb( in vec3 c ){
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
          
          float mapNum(float s, float a1, float a2, float b1, float b2){
              return b1 + (s-a1)*(b2-b1)/(a2-a1);
          }
          
          
          // BLEND FUNCTIONS FROM https://github.com/jamieowen/glsl-blend
          
          // Add
          vec3 blendAdd(vec3 base, vec3 blend) {
              return min(base+blend,vec3(1.0));
          }
          
          // Multiply
          vec3 blendMultiply(vec3 base, vec3 blend) {
              return base*blend;
          }
          
          // Lighten
          vec3 blendLighten(vec3 base, vec3 blend) {
              return vec3(max(base.r,blend.r),max(base.g,blend.g),max(base.b,blend.b));
          }
          
          // Darken
          vec3 blendDarken(vec3 base, vec3 blend) {
              return vec3(min(base.r,blend.r),min(base.g,blend.g),min(base.b,blend.b));
          }
          
          // Subtract
          float blendSubtract(float base, float blend) {
              return max(base+blend-1.0,0.0);
          }
          
          vec3 blendSubtract(vec3 base, vec3 blend) {
              return max(base+blend-vec3(1.0),vec3(0.0));
          }
          
          // Screen
          float blendScreen(float base, float blend) {
              return 1.0-((1.0-base)*(1.0-blend));
          }
          
          vec3 blendScreen(vec3 base, vec3 blend) {
              return vec3(blendScreen(base.r,blend.r),blendScreen(base.g,blend.g),blendScreen(base.b,blend.b));
          }
          
          // Average
          vec3 blendAverage(vec3 base, vec3 blend) {
              return (base+blend)/2.0;
          }
          
          // SoftLight
          float blendSoftLight(float base, float blend) {
              return (blend<0.5)?(2.0*base*blend+base*base*(1.0-2.0*blend)):(sqrt(base)*(2.0*blend-1.0)+2.0*base*(1.0-blend));
          }
          
          vec3 blendSoftLight(vec3 base, vec3 blend) {
              return vec3(blendSoftLight(base.r,blend.r),blendSoftLight(base.g,blend.g),blendSoftLight(base.b,blend.b));
          }
          
          
          // BLEND FUNCTION
          vec3 blendMode( int mode, vec3 base, vec3 blend, float opacity ){
              
              if( mode == 1 ){
                  return (blendAdd(base, blend) * opacity + base * (1.0 - opacity));
                  
              }else if( mode == 2 ){
                  return (blendMultiply(base, blend) * opacity + base * (1.0 - opacity));
                  
              }else if( mode == 3 ){
                  return (blendLighten(base, blend) * opacity + base * (1.0 - opacity));
                  
              }else if( mode == 4 ){
                  return (blendDarken(base, blend) * opacity + base * (1.0 - opacity));
                  
              }else if( mode == 5 ){
                  return (blendSubtract(base, blend) * opacity + base * (1.0 - opacity));
                  
              }else if( mode == 6 ){
                  return (blendScreen(base, blend) * opacity + base * (1.0 - opacity));
                  
              }else if( mode == 7 ){
                  return (blendAverage(base, blend) * opacity + base * (1.0 - opacity));
                  
              }else if( mode == 8 ){
                  return (blendSoftLight(base, blend) * opacity + base * (1.0 - opacity));
              }
          }
          
          // ---
          
          vec3 postProcessing(vec3 image, vec3 hsv, float contrast) {
              image = ((image - vec3(0.5)) * max(contrast+0.5, 0.0)) + vec3(0.5);
              image *= hsv2rgb(hsv);
              return image;
          }


          );


static string mainHeader =
STRINGIFY(
          void main(){
    
              vec2 st = gl_FragCoord.xy / iResolution.xy;
              vec3 mixCol = vec3(0.);
          );
              
static string channel =
STRINGIFY(
          vec3 colTex$0 = texture2DRect(tex$0, resolution_$0 * st ).rgb;
          vec3 hsv_$0 = vec3( u_H_$0, u_S_$0, u_B_$0);
          colTex$0 = postProcessing(colTex$0, hsv_$0, u_contrast_$0);
          mixCol = blendMode( u_blendMode_$0, mixCol, colTex$0, u_opacity_$0 );
        );

static string output =
STRINGIFY(
              gl_FragColor =  vec4(mixCol,1.);
          }
          );

#endif /* TextureMixShader_h */
              
              
              // --- unused
              
//              float blendReflect(float base, float blend) {
//                  return (blend==1.0)?blend:min(base*base/(1.0-blend),1.0);
//              }
//              
//              vec3 blendReflect(vec3 base, vec3 blend) {
//                  return vec3(blendReflect(base.r,blend.r),blendReflect(base.g,blend.g),blendReflect(base.b,blend.b));
//              }
//              
//              vec3 blendReflect(vec3 base, vec3 blend, float opacity) {
//                  return (blendReflect(base, blend) * opacity + base * (1.0 - opacity));
//              }
//              
//              
//              vec3 blendExclusion(vec3 base, vec3 blend) {
//                  return base+blend-2.0*base*blend;
//              }
//              
//              vec3 blendExclusion(vec3 base, vec3 blend, float opacity) {
//                  return (blendExclusion(base, blend) * opacity + base * (1.0 - opacity));
//              }
//              
//              
//              float blendOverlay(float base, float blend) {
//                  return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend));
//              }
//              
//              vec3 blendOverlay(vec3 base, vec3 blend) {
//                  return vec3(blendOverlay(base.r,blend.r),blendOverlay(base.g,blend.g),blendOverlay(base.b,blend.b));
//              }
//              
//              vec3 blendOverlay(vec3 base, vec3 blend, float opacity) {
//                  return (blendOverlay(base, blend) * opacity + base * (1.0 - opacity));
//              }
//              
//              
//              float blendColorBurn(float base, float blend) {
//                  return (blend==0.0)?blend:max((1.0-((1.0-base)/blend)),0.0);
//              }
//              
//              vec3 blendColorBurn(vec3 base, vec3 blend) {
//                  return vec3(blendColorBurn(base.r,blend.r),blendColorBurn(base.g,blend.g),blendColorBurn(base.b,blend.b));
//              }
//              
//              vec3 blendColorBurn(vec3 base, vec3 blend, float opacity) {
//                  return (blendColorBurn(base, blend) * opacity + base * (1.0 - opacity));
//              }
//              float blendColorDodge(float base, float blend) {
//                  return (blend==1.0)?blend:min(base/(1.0-blend),1.0);
//              }
//              
//              vec3 blendColorDodge(vec3 base, vec3 blend) {
//                  return vec3(blendColorDodge(base.r,blend.r),blendColorDodge(base.g,blend.g),blendColorDodge(base.b,blend.b));
//              }
//              
//              vec3 blendColorDodge(vec3 base, vec3 blend, float opacity) {
//                  return (blendColorDodge(base, blend) * opacity + base * (1.0 - opacity));
//              }
//              
//              
//              float blendVividLight(float base, float blend) {
//                  return (blend<0.5)?blendColorBurn(base,(2.0*blend)):blendColorDodge(base,(2.0*(blend-0.5)));
//              }
//              
//              vec3 blendVividLight(vec3 base, vec3 blend) {
//                  return vec3(blendVividLight(base.r,blend.r),blendVividLight(base.g,blend.g),blendVividLight(base.b,blend.b));
//              }
//              
//              vec3 blendVividLight(vec3 base, vec3 blend, float opacity) {
//                  return (blendVividLight(base, blend) * opacity + base * (1.0 - opacity));
//              }
//              