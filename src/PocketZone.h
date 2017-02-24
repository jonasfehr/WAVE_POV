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
    
    float timeOfActivation = 0;
    
    ofParameter<bool> deactivate;
    
    int oldMillis = 0;
    
    ofxOscSender * oscSender;
    
    int index = 0;

    
    PocketZone(){};
    
    void setup(string channelName, int index, int start, int stop, ofTexture *texture, ofxOscSender * oscSender){
        this->name = channelName;
        this->index = index;
        this->start = start;
        this->stop = stop;
        this->texture = texture;
        this->oscSender = oscSender;
        
        fbo.allocate(120,1300, GL_RGBA32F_ARB);
        
        shader.load("shaders/masking");
        
        inputType = INPUT_EXTERNAL;
        
    };
    
    void setup(string channelName, int index, int start, int stop, string shaderName, ofVec2f shaderSize, ofxOscSender * oscSender){
        this->name = channelName;
        this->index = index;
        this->start = start;
        this->stop = stop;
        this->oscSender = oscSender;

        
        fbo.allocate(120,1300, GL_RGBA32F_ARB);
        
        shader.load("shaders/PocketZoneMasking");
        inputShader.load("shaders/"+shaderName);
        
        fboShader.allocate(shaderSize.x, shaderSize.y, GL_RGB);
        fboShader.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fboShader.end();
        
        inputType = INPUT_SHADER;
        
        
        this->texture = &fboShader.getTexture();
        
        setupParameterGroup(name);
        
    };
    
    void update(){
        
        if(isActive && alpha < 1.) alpha += 0.1;
        else if( !isActive && alpha > 0) alpha -= 0.01;
        
        
        if(!deactivate){
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
                    shader.setUniformTexture("tex0", *texture, 0);
                    shader.setUniform2f("texResolution", texture->getWidth(), texture->getHeight());
                    
                    shader.setUniform2f("iResolution", fbo.getWidth(), fbo.getHeight());
                    shader.setUniform1f("iGlobalTime", ofGetElapsedTimef() );
                    shader.setUniform1f("u_opacity", alpha);
                    shader.setUniform1f("u_center", ((start+stop)/2.)/40.);
                    shader.setUniform1f("u_width", abs(stop-start)/39.);
                    
                    
                    
                    ofSetColor(255,255,255);
                    ofFill();
                    ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
                }
                shader.end();
                
            }
            fbo.end();
            
            


        }
        // if nothing is happening for more than 30 secs, turn off and change start stop

        if(ofGetElapsedTimef() - timeOfActivation > 30){
            isActive = false;
            
            start = (int)ofRandom(2, 39);
            stop = (int)ofRandom(start-10, start+10);
            if(stop > 40) stop = 40;
            else if(stop < 1 ) stop = 1;
            
            timeOfActivation = ofGetElapsedTimef();
            
            cout << "Zone " << start << " - " << stop << endl;
        }
        
        
        // SEND OSC
        if(ofGetElapsedTimeMillis()/50 != oldMillis && !deactivate){
            ofxOscMessage m;
            m.setAddress("/PocketZone/"+ofToString(index));
            m.addInt32Arg(isActive);
            if(isActive) m.addFloatArg(((start+stop)/2.)/40.*78.);
            oscSender->sendMessage(m);
            
            oldMillis = ofGetElapsedTimeMillis()/50;
        }
        
    }
    
    void setupParameterGroup(string name){
        parameterGroup.setName(name);
        parameterGroup.add(deactivate.set("deactivate", false));
    }
    
    void gateActivated(int g){
        if(g == start && !isActive){
            isActive = true;
            timeOfActivation = ofGetElapsedTimef();
        }
        if(g == stop){
            isActive = false;
            
            start = (int)ofRandom(2, 39);
            stop = (int)ofRandom(start-10, start+10);
            if(stop > 40) stop = 40;
            else if(stop < 1 ) stop = 1;
            
            timeOfActivation = ofGetElapsedTimef();
            
            cout << "Zone " << start << " - " << stop << endl;            
        }
    };
    
    
    enum pocketZoneInputType : int {
        INPUT_EXTERNAL = 1,
        INPUT_SHADER = 2,
    };
};



#endif /* PocketZone_h */
