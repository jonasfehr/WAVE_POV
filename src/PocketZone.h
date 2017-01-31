//
//  PocketZone.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 30/01/2017.
//
//

#ifndef PocketZone_h
#define PocketZone_h
#include "Pocket.h"
#include "WaveContent.h"

class PocketZone : public Pocket, public WaveContent{
public:
    int start = 0, stop = 40;
    

    float alpha = 0.;
    
    
    ofTexture *texture;
    ofFbo fboShader;
    
    
    int inputType = 0;
    ofxAutoReloadedShader inputShader;

    vector<int> activatedGates;
    
    PocketZone(){};
    
    void setup( int start, int stop, ofTexture *texture){
        this->start = start;
        this->stop = stop;
        this->texture = texture;
        
        fbo.allocate(120,1300, GL_RGBA32F_ARB);
        
        shader.load("shaders/masking");
        
        inputType = INPUT_EXTERNAL;

    };
    
    void setup( int start, int stop, string shaderName, ofVec2f shaderSize){
        this->start = start;
        this->stop = stop;
                
        fbo.allocate(120,1300, GL_RGBA32F_ARB);
        
        shader.load("shaders/masking");
        inputShader.load("shaders/"+shaderName);
        
        fboShader.allocate(shaderSize.x, shaderSize.y, GL_RGB);
        fboShader.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fboShader.end();
        
        inputType = INPUT_SHADER;

        
        texture = &fboShader.getTexture();

    };
    
    void update(){

        if(isActive && alpha < 1.) alpha += 0.1;
        else if( !isActive && alpha > 0) alpha -= 0.01;
        
        
        
        // SHADER CALCULATES TEXTURE
        if(inputType == INPUT_SHADER){
            fboShader.begin();{
                glClearColor(0.0, 0.0, 0.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                inputShader.begin();{
                    inputShader.setUniform2f("iResolution", fboShader.getWidth(), fboShader.getHeight());
                    inputShader.setUniform1f("iGlobalTime", ofGetElapsedTimef() ) ;
                    ofSetColor(255,255,255);
                    ofFill();
                    ofDrawRectangle(0, 0, fboShader.getWidth(), fboShader.getHeight());
                }
                inputShader.end();
            }
            fboShader.end();
        }
    
        
        //Create content
        fbo.begin();
        {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            shader.begin();
            {
                shader.setUniformTexture("tex0", fboShader.getTexture(), 0);
                shader.setUniform2f("texResolution", texture->getWidth(), texture->getHeight());
                
                shader.setUniform2f("iResolution", fbo.getWidth(), fbo.getHeight());
                shader.setUniform1f("iGlobalTime", ofGetElapsedTimef() );
                shader.setUniform1f("u_opacity", alpha);
                shader.setUniform1f("u_center", ((start+stop)/2.)/78.);
                shader.setUniform1f("u_width", abs(start-stop)/78.);
                
                
                
                ofSetColor(255,255,255);
                ofFill();
                ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
            }
            shader.end();
            
        }
        fbo.end();
        

    
        
    }
    
    void gateActivated(int g){
        if(g == start) isActive = true;
        if(g == stop) isActive = false;
    };
    
    
    enum pocketZoneInputType : int {
        INPUT_EXTERNAL = 1,
        INPUT_SHADER = 2,
    };
};



#endif /* PocketZone_h */
