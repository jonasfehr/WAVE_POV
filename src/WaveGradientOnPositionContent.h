//
//  WaveGradientOnPositionContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 24/01/2017.
//
//

#ifndef WaveGradientOnPositionContent_h
#define WaveGradientOnPositionContent_h
#include "ofxAutoReloadedShader.h"
#include "ExternalObject.h"

///SHADER
#define STRINGIFY(x) #x

static string shader_uniformsHeader =
STRINGIFY(
          uniform vec2 iResolution;
          uniform float iGlobalTime;
          
          uniform float u_opacity;
          uniform int u_blendMode;
          );


static string shader_uniforms =
STRINGIFY(
          uniform float posX_$0;
          uniform float width_$0;
          );

static string shader_functions =
STRINGIFY(
          // functions
          vec3 hsv2rgb_smooth( in vec3 c ){
              vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
              rgb = rgb*rgb*(3.0-2.0*rgb);
              return c.z * mix( vec3(1.0), rgb, c.y);
          }
          
          vec3 hsv2rgb( in vec3 c ){
              vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
              return c.z * mix( vec3(1.0), rgb, c.y);
          }
          
          mat4 brightnessMatrix( float brightness ){
              return mat4( 1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          brightness, brightness, brightness, 1 );
          }
          
          mat4 contrastMatrix( float contrast ){
              float t = ( 1.0 - contrast ) / 2.0;
              
              return mat4( contrast, 0, 0, 0,
                          0, contrast, 0, 0,
                          0, 0, contrast, 0,
                          t, t, t, 1 );
              
          }
          
          mat4 saturationMatrix( float saturation ){
              vec3 luminance = vec3( 0.3086, 0.6094, 0.0820 );
              
              float oneMinusSat = 1.0 - saturation;
              
              vec3 red = vec3( luminance.x * oneMinusSat );
              red+= vec3( saturation, 0, 0 );
              
              vec3 green = vec3( luminance.y * oneMinusSat );
              green += vec3( 0, saturation, 0 );
              
              vec3 blue = vec3( luminance.z * oneMinusSat );
              blue += vec3( 0, 0, saturation );
              
              return mat4( red,     0,
                          green,   0,
                          blue,    0,
                          0, 0, 0, 1 );
          }
          
          
          //  Function from I√±igo Quiles
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
          
          // Overlay
          float blendOverlay(float base, float blend) {
              return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend));
          }
          
          vec3 blendOverlay(vec3 base, vec3 blend) {
              return vec3(blendOverlay(base.r,blend.r),blendOverlay(base.g,blend.g),blendOverlay(base.b,blend.b));
          }
          
          
          // BLEND FUNCTION
          vec4 blendMode( int mode, vec4 base, vec4 blend, float opacity ){
              if( mode == 1 ){
                  return vec4(blendAdd(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity), 1.);
                  
              }else if( mode == 2 ){
                  return vec4(blendMultiply(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity), 1.);
                  
              }else if( mode == 3 ){
                  return vec4(blendLighten(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity), 1.);
                  
              }else if( mode == 4 ){
                  return vec4(blendDarken(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity), 1.);
                  
              }else if( mode == 5 ){
                  return vec4(blendSubtract(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity), 1.);
                  
              }else if( mode == 6 ){
                  return vec4(blendScreen(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity), 1.);
                  
              }else if( mode == 7 ){
                  return vec4(blendAverage(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity), 1.);
                  
              }else if( mode == 8 ){
                  return vec4(blendSoftLight(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity), 1.);
                  
              }else if( mode == 9 ){
                  return vec4(blendOverlay(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity), 1.);
                  
              }else if( mode == 10 ){
                  return vec4(blend * blend.a * opacity + base*(1.-(blend.a*opacity)));
              }
          }
          
          // ---
          
          //          vec4 postProcessing(vec4 image, vec3 hsv, float contrast) {
          //              image = ((image - vec4(0.5)) * max(contrast+0.5, 0.0)) + vec4(0.5);
          //              image *= hsv2rgb(hsv);
          //              return image;
          //          }
          
          
          );


static string shader_mainHeader =
STRINGIFY(
          void main(){
              
              vec2 st = gl_FragCoord.xy / iResolution.xy;
              vec4 mixCol = vec4(0.);
              );
              
              
              static string shader_objectGradient =
              STRINGIFY(
                        
                        vec4 gradient_$0 = vec4(cubicPulse(posX_$0, width_$0,st.x));
                        
                        mixCol = blendMode( u_blendMode, mixCol, gradient_$0, u_opacity );
                        );
              
              static string shader_output =
              STRINGIFY(
                        gl_FragColor =  vec4(mixCol);
                        }
                        );
              
              
              // ---------------------------------------------
              
              class WaveGradientOnPositionContent : public WaveContent{
              public:
                  ofFbo fboShader;
                  ofMesh mesh;
                  
                  ofShader shader;
                  
                  // Positions
                  map<int, ExternalObject> * externalObjects;
                  
                  // parameters
                  ofParameter<int> blendMode;
                  ofParameter<float> opacity;
                  ofParameter<float> width;
                  
                  int oldExternalObjectSize = 0;
                  
                  WaveGradientOnPositionContent(){}
                  
                  
                  
                  void setup(string channelName, map<int, ExternalObject> * externalObjects){
                      this->name = channelName;
                      this->externalObjects = externalObjects;
                      
                      fbo.allocate(120, 1300, GL_RGBA32F_ARB);
                      fboShader.allocate(78*130, 10*130, GL_RGBA32F_ARB);
                      
                      
                      createMesh();
                      
                      parameterGroup.clear();
                      setupParameterGroup(name+"_gradients");

                      
                  }
                  
                  
                  // UPDATE
                  void update(){
                      // update shader if amount changes
                      if(externalObjects->size() != oldExternalObjectSize){
                          createShader();
                          oldExternalObjectSize = externalObjects->size();
                      }
                      
                      //        for(auto & o : *externalObjects){
                      //
                      //            ofVec3f pos = o.second.getPosition();
                      //            ofVec3f size = o.second.getSize();
                      //            pos.x = pos.x*130;
                      //            pos.y = (pos.z+7.)*130;
                      //            pos.z = 0;
                      //
                      //        }
                      
                      fbo.begin();
                      {
                          glClearColor(0.0, 0.0, 0.0, 0.0);
                          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                          shader.begin();
                          {
                              shader.setUniform2f("iResolution", fbo.getWidth(), fbo.getHeight());
                              shader.setUniform1f("iGlobalTime", ofGetElapsedTimef()); //tempo p nr 1
                              
                              shader.setUniform1f("u_opacity", opacity);
                              shader.setUniform1i("u_blendMode", blendMode);
                              
                              int i = 1;
                              for(auto & externalObject : *externalObjects){
                                  shader.setUniform1f("posX_"+ofToString(i), externalObject.second.getPosition().z/78.);
                                  shader.setUniform1f("width_"+ofToString(i), width);
                                  i++;
                              }
                              
                              ofSetColor(255,255,255);
                              ofFill();
                              ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
                              
                              
                          }
                          shader.end();
                          
                      }
                      fbo.end();
                      
                      
                      //        fbo.begin();
                      //        {
                      //            glClearColor(0.0, 0.0, 0.0, 0.0);
                      //            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                      //
                      //            fboShader.getTexture().bind();
                      //            {
                      //                ofSetColor(255);
                      //                mesh.draw();
                      //            }
                      //            fboShader.getTexture().unbind();
                      //
                      //        }
                      //        fbo.end();
                  }
                  
                  void setupParameterGroup(string name){
                      parameterGroup.setName(name);
                      parameterGroup.add(blendMode.set("blendMode", 0, 1, 10));
                      parameterGroup.add(opacity.set("opacity", 0, 0., 1));
                      parameterGroup.add(width.set("width", 0.1, 0., 1.));
                  }
                  
                  
                  void createShader(){

                      // GENERATE THE SHADER
                      stringstream shaderScript;
                      shaderScript << "#version 120" << endl;
                      shaderScript << shader_uniformsHeader << endl;
                      for(int i = 1; i <= externalObjects->size(); i++){
                          shaderScript << replaceAll(shader_uniforms, "$0", ofToString(i)) << endl;
                      }
                      shaderScript << shader_functions << endl;
                      shaderScript << shader_mainHeader << endl;
                      for(int i = 1; i <= externalObjects->size(); i++){
                          shaderScript << replaceAll(shader_objectGradient, "$0", ofToString(i)) << endl;
                      }
                      shaderScript << shader_output << endl;
                      
                      shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderScript.str());
                      shader.linkProgram();
                  }
                  
                  void createMesh(){
                      for(int i = 0; i <= 40; i++){
                          mesh.addVertex(ofVec2f(i*3,0));
                          mesh.addVertex(ofVec2f(i*3,120));
                          mesh.addVertex(ofVec2f(i*3,650));
                          mesh.addVertex(ofVec2f(i*3,1180));
                          mesh.addVertex(ofVec2f(i*3,1300));
                          
                          mesh.addVertex(ofVec2f((i*3+3),0));
                          mesh.addVertex(ofVec2f((i*3+3),120));
                          mesh.addVertex(ofVec2f((i*3+3),650));
                          mesh.addVertex(ofVec2f((i*3+3),1180));
                          mesh.addVertex(ofVec2f((i*3+3),1300));
                      }
                      for(int i = 0; i < 40*10; i+=10){
                          mesh.addIndex(i+5);
                          mesh.addIndex(i+0);
                          mesh.addIndex(i+6);
                          mesh.addIndex(i+6);
                          mesh.addIndex(i+0);
                          mesh.addIndex(i+1);
                          
                          mesh.addIndex(i+6);
                          mesh.addIndex(i+1);
                          mesh.addIndex(i+7);
                          mesh.addIndex(i+7);
                          mesh.addIndex(i+1);
                          mesh.addIndex(i+2);
                          
                          mesh.addIndex(i+7);
                          mesh.addIndex(i+2);
                          mesh.addIndex(i+8);
                          mesh.addIndex(i+8);
                          mesh.addIndex(i+2);
                          mesh.addIndex(i+3);
                          
                          mesh.addIndex(i+8);
                          mesh.addIndex(i+3);
                          mesh.addIndex(i+9);
                          mesh.addIndex(i+9);
                          mesh.addIndex(i+3);
                          mesh.addIndex(i+4);
                      }
                      
                      float dist = fboShader.getTexture().getWidth()/39;
                      for(int i = 0; i < 40; i++){
                          mesh.addTexCoord( ofVec2f( i*dist, 0. ) );
                          mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.1 ) );
                          mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.5 ) );
                          mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.9 ) );
                          mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight() ) );
                          
                          mesh.addTexCoord( ofVec2f( i*dist, 0. ) );
                          mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.1 ) );
                          mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.5 ) );
                          mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.9 ) );
                          mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight() ) );
                      }
                  }
                  
                  // function from http://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
                  string replaceAll(std::string str, const std::string& from, const std::string& to) {
                      if(from.empty())
                          return "";
                      size_t start_pos = 0;
                      while((start_pos = str.find(from, start_pos)) != std::string::npos) {
                          str.replace(start_pos, from.length(), to);
                          start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
                      }
                      return str;
                  }
                  
              };
              
              // ------------------------------------------------------
              
              
#endif /* WaveGradientOnPositionContent_h */
