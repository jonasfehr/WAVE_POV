//
//  WaveEffectContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 23/01/2017.
//
//

#ifndef WaveEffectContent_h
#define WaveEffectContent_h
#include "ofxAutoReloadedShader.h"
#include "ofxOscSender.h"



class WaveEffectContent : public WaveContent{
public:
    
    
    ofFbo fboShader;
    ofMesh mesh;
    ofTexture *texture;
    
    
    
    ofxOscSender * oscSender;
    int index;

    
    ofParameter<float> possibility;
    ofParameter<float> increment;
    ofParameter<bool> deactivate;


    bool createRipple = false;
    float effectPos = 0.;
    float timeLastActivation = 0;
    float waitTime = 10.;
    
    
    WaveEffectContent(){}
    
    
    void setup(string channelName, int index, ofxOscSender * oscSender){
        this->name = channelName;
        this->index = index;
        this->shader.load("shaders/WaveEffectContent");
        this->oscSender = oscSender;
        
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
        
        timeLastActivation = ofRandom(0., 10.);
        waitTime = ofRandom(10., 30.)+index*5;
    }

    
    
    
    
    int oldMillis = 0;
    void update(){

        
        if(!createRipple && !deactivate){
            if( ofRandom(0., 1.) < possibility && ofGetElapsedTimef()-timeLastActivation > waitTime){
                effectPos = 0.;
                createRipple = true;
                timeLastActivation = ofGetElapsedTimef();
                waitTime = ofRandom(30., 60.);
            }
            
        }else{
            effectPos += increment;
            
            
            
            if(effectPos >= 1.){
                createRipple = false;
            }
            
            // SEND OSC gate 1
            if(ofGetElapsedTimeMillis()/50 != oldMillis){
                ofxOscMessage m;
                m.setAddress("/Ripple/"+ofToString(index));
                m.addInt32Arg(createRipple);
                m.addFloatArg(effectPos);
                oscSender->sendMessage(m);
                
                oldMillis = ofGetElapsedTimeMillis()/50;
            }
        }
        
        

        
        fbo.begin();
        {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
            fboShader.begin();
            {
                glClearColor(0.0, 0.0, 0.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                shader.begin();
                {
                    shader.setUniform2f("iResolution", fboShader.getWidth(), fboShader.getHeight());
                    shader.setUniform1f("iGlobalTime", ofGetElapsedTimef() ) ;//counter);
                    shader.setUniform1f("effectPos", effectPos );
                    
                    ofSetColor(255,255,255);
                    ofFill();
                    ofDrawRectangle(0, 0, fboShader.getWidth(), fboShader.getHeight());
                }
                shader.end();
            }
            fboShader.end();
            
            texture->bind();{
                ofSetColor(255);
                mesh.draw();
            }
            texture->unbind();
            
        }
        fbo.end();
    }
    
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
    
    
    void setupParameterGroup(string name){
        parameterGroup.setName(name);
        parameterGroup.add(deactivate.set("deactivate", false));
        parameterGroup.add(possibility.set("possibility", 0.1, 0., 1.));
        parameterGroup.add(increment.set("increment", 0.01, 0.000001, 0.1));

    }
    
    float cubicOut(float t) {
        float f = t - 1.0;
        return f * f * f + 1.0;
    }
    
};

#endif /* WaveEffectContent_h */
