//
//  Utils.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 17/01/2017.
//
//

#ifndef WaveShaderContent_h
#define WaveShaderContent_h

#include "ofxAutoReloadedShader.h"
#include "WaveContent.h"


class WaveShaderContent  : public WaveContent{
public:

    //float counter = 0.;
    //float increment = 0.1;
    
    WaveShaderContent(){}
    void setup(string shaderName){
        
        fbo.allocate(40, 1300, GL_RGBA32F_ARB);
        fbo.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fbo.end();
        
        shader.load("shaders/"+shaderName);
    }
    
    
    void update(){
       // counter += increment;
        
        
        fbo.begin();
        {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shader.begin();
            {
                shader.setUniform2f("iResolution", fbo.getWidth(), fbo.getHeight());
                shader.setUniform1f("iGlobalTime",     ofGetElapsedTimef() ) ;//counter);
                //                shader.setUniform1f("u_density", density);
                //                shader.setUniform1f("u_contrast", contrast);
                //                shader.setUniform1f("u_H", hue);
                //                shader.setUniform1f("u_S", saturation);
                //                shader.setUniform1f("u_B", 1.);
                //                shader.setUniform1f("u_direction", direction);
                //                shader.setUniform1f("u_mix", mix);
                
                
                ofSetColor(255,255,255);
                ofFill();
                ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
            }
            shader.end();
            
        }
        fbo.end();
        
    }
    
 //   void setIncrement( float value ){ increment = value;}
    
};

#endif /* WaveShaderContent_h */
