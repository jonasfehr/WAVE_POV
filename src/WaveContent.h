//
//  WaveContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 31/01/2017.
//
//

#ifndef WaveContent_h
#define WaveContent_h
#include "ofxAutoReloadedShader.h"

class WaveContent{
public:
    ofFbo fbo;
    ofxAutoReloadedShader shader;
    ofParameterGroup parameterGroup;
    string name;
    
    WaveContent(){};
    
    virtual void setup(){};
    
    virtual void update(){};
    
    void draw(){
        fbo.draw(0,0, 120, 1300);
    }
    
    ofTexture getTexture(){ return fbo.getTexture(); }
    
    ofFbo* getFboPtr(){ return &fbo; }
    
    virtual void setupParameterGroup(string name){}
    
    ofParameterGroup* getPointerToParameterGroup(){ return &parameterGroup; }
    
    string getName(){ return name; }

    
private:
    
};


#endif /* WaveContent_h */
