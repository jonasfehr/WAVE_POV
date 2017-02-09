//
//  PocketGateReactiveContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 30/01/2017.
//
//

#ifndef PocketGateReactiveContent_h
#define PocketGateReactiveContent_h
#include "Pocket.h"
#include "WaveContent.h"

class PocketGateReactiveContent : public Pocket, public WaveContent{
public:
    ofFbo fboShader;
    ofMesh mesh;
    ofTexture *texture;
    
    ofxOscSender * oscSender;
    int oldMillis = 0;
    int index;
    
    ofParameter<float> increment;
    ofParameter<float> width;
    ofParameter<bool> deactivate;
    
    
    float effectPos = 0.;
    int reactiveGate = 0;
    bool isTarget = false;
    float autoMoveAtTime = 0;
    
    
    
    
    
    
    PocketGateReactiveContent(){};
    
    
    void setup(string channelName, int index, ofxOscSender * oscSender){
        this->name = channelName;
        this->index = index;
        this->oscSender = oscSender;
        
        this->shader.load("shaders/PocketGateReactiveContent");
        
        
        fbo.allocate(120, 1300, GL_RGBA32F_ARB);
        fbo.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fbo.end();
        
        
        fboShader.allocate(1024, 1024, GL_RGBA32F_ARB);
        fboShader.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fboShader.end();
        
        texture = &fboShader.getTexture();
        createMesh();
        
        setupParameterGroup(name);
        
        autoMoveAtTime = ofRandom(10., 30.)+index*5;
        
        reactiveGate = int(ofRandom(1., 40.));
        
        effectPos = reactiveGate;
        
    };
    
    void update(){
        
        if(!deactivate){
            
            if(!isTarget){
//                // move soft
//                if(effectPos > reactiveGate) effectPos -= increment;
//                else effectPos += increment;
//                
//                if(effectPos <= reactiveGate + increment && effectPos >= reactiveGate - increment){
//                    isTarget = true;
//                    autoMoveAtTime = ofRandom(10., 30.)+ofGetElapsedTimef();
//                    
//                }
                // jump
                    effectPos = reactiveGate;
                    isTarget = true;
                    autoMoveAtTime = ofRandom(10., 30.)+ofGetElapsedTimef();
                
                
                
            } else {
                // when at target and no reaction for some time
                if(autoMoveAtTime <= ofGetElapsedTimef()){
                    reactiveGate = int(ofRandom(1., 40.));
                    isTarget = false;
                }
                
            }
            
                        
            // SEND OSC gate 1
            if(ofGetElapsedTimeMillis()/50 != oldMillis){
                ofxOscMessage m;
                m.setAddress("/GateReactive/"+ofToString(index));
                m.addInt32Arg(isTarget);
                m.addFloatArg(effectPos*2.-2.);
                oscSender->sendMessage(m);
                
                oldMillis = ofGetElapsedTimeMillis()/50;
            }
            
//            fboShader.begin();
//            {
//                glClearColor(0.0, 0.0, 0.0, 0.0);
//                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//                
//                shader.begin();
//                {
//                    shader.setUniform2f("iResolution", fboShader.getWidth(), fboShader.getHeight());
//                    shader.setUniform1f("iGlobalTime", ofGetElapsedTimef() ) ;//counter);
//                    shader.setUniform1f("effectPos", (effectPos-1)/39. + 1./120. );
//                    shader.setUniform1f("width", width );
//                    
//                    cout << (effectPos-1)/39. + 1./120.  << endl;
//                    
//                    ofSetColor(255,255,255);
//                    ofFill();
//                    ofDrawRectangle(0, 0, fboShader.getWidth(), fboShader.getHeight());
//                }
//                shader.end();
//            }
//            fboShader.end();
            
            fbo.begin();
            {
                glClearColor(0.0, 0.0, 0.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                shader.begin();
                {
                    shader.setUniform2f("iResolution", fbo.getWidth(), fbo.getHeight());
                    shader.setUniform1f("iGlobalTime", ofGetElapsedTimef() ) ;//counter);
                    shader.setUniform1f("effectPos", (effectPos-1)/39. + 0.125 );
                    shader.setUniform1f("width", width );
                    
                    ofSetColor(255,255,255);
                    ofFill();
                    ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
                }
                shader.end();
                
//                texture->bind();{
//                    ofSetColor(255);
//                    mesh.draw();
//                }
//                texture->unbind();
                
            }
            fbo.end();
        }
    };
    
    void draw(){
        fbo.draw(0,0, 120, 1300);
    }
    
    
    
    void setInputTexture(ofTexture *texture){ this->texture = texture;}
    
    ofTexture getTexture(){ return fbo.getTexture(); }
    
    ofFbo* getFboPtr(){ return &fbo; }
    
    
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
        
        float dist = texture->getWidth()/39;
        for(int i = 0; i <= 40; i++){
            mesh.addTexCoord(ofVec2f(1+i*dist,texture->getHeight()*145/1590));
            mesh.addTexCoord(ofVec2f(1+i*dist,texture->getHeight()*(145+120)/1590));
            mesh.addTexCoord(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530)/1590));
            mesh.addTexCoord(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530+530)/1590));
            mesh.addTexCoord(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530+530+120)/1590));
            
            mesh.addTexCoord(ofVec2f(1+i*dist,texture->getHeight()*145/1590));
            mesh.addTexCoord(ofVec2f(1+i*dist,texture->getHeight()*(145+120)/1590));
            mesh.addTexCoord(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530)/1590));
            mesh.addTexCoord(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530+530)/1590));
            mesh.addTexCoord(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530+530+120)/1590));
            
        }
    }
    
    
    
    float cubicOut(float t) {
        float f = t - 1.0;
        return f * f * f + 1.0;
    }
    void setupParameterGroup(string name){
        parameterGroup.setName(name);
        parameterGroup.add(deactivate.set("deactivate", false));
        parameterGroup.add(width.set("width", 0.1, 0., 1.));
        parameterGroup.add(increment.set("increment", 0.01, 0.000001, 1/40.));
    }
    
    void gateActivated(int g){
        if(g == reactiveGate){
            reactiveGate = int(ofRandom(1., 40.));
            isTarget = false;
        }
    };
};



#endif /* PocketGateReactiveContent_h */
