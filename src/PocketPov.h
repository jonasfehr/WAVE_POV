//
//  PocketPov.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 30/01/2017.
//
//

#ifndef PocketPov_h
#define PocketPov_h

#include "Pocket.h"
#include "InputToWaveContent.h"
#include "ofxOscSender.h"

class PocketPov : public Pocket{
public:
    ExternalObject * externalObject;
    InputToWaveContent povMappedContent;
    float timeOfActivation = 0;
    float oldexternalObjectLifespan = 0;
    int externalObjectNotUpdatedSince;
    string name;
    int index = 0;
    ofxOscSender * oscSender;
    int oldMillis = 0;
    
    ofParameter<bool> deactivate;
    ofParameter<float> minLifeSpan;
    ofParameterGroup parameterGroup;

    
    
    
    PocketPov(){};
    
    void setup(string channelName, int index, vector<Gate> * gates, ofVec3f povPosition, ofTexture *texture, ofxOscSender * oscSender){
        this->name = channelName;
        this->index = index;
        this->povMappedContent = povMappedContent;
        this->povMappedContent.setInvisible(0.);
        this->oscSender = oscSender;
        
        povMappedContent.setup(channelName, gates, povPosition, texture, POV_UV);
        setupParameterGroup(name);

    }
    
    void setup(string channelName, int index, vector<Gate> * gates, ofVec3f povPosition, string shaderName, ofxOscSender * oscSender){
        this->name = channelName;
        this->index = index;
        this->povMappedContent = povMappedContent;
        this->povMappedContent.setInvisible(0.);
        this->oscSender = oscSender;

        povMappedContent.setup(channelName, gates, povPosition, shaderName, ofVec2f(512), POV_UV);
        setupParameterGroup(name);

    }
    
    void update(){

        povMappedContent.update();

        
        // move pov if active
        if(isActive && !deactivate){
            // deactivate pocket if externalObject is lost
            if(oldexternalObjectLifespan == externalObject->getLifespan()) externalObjectNotUpdatedSince++;
            if(externalObject->getLifespan() > oldexternalObjectLifespan) externalObjectNotUpdatedSince = 0;
            if(oldexternalObjectLifespan - externalObject->getLifespan() > 0 || externalObjectNotUpdatedSince > 10){
                isActive = false;
                povMappedContent.setInvisible();
                oldexternalObjectLifespan = 0;
                return;
            }
            oldexternalObjectLifespan = externalObject->getLifespan();
            
            
            
            povMappedContent.setVisible();
            ofVec3f newPos = ofVec3f(0, 1.72, externalObject->getPosition().z);
            ofVec3f povCurrentPos = povMappedContent.getPovPtr()->getPosition();
            povMappedContent.getPovPtr()->setGlobalPosition(newPos*0.1+povCurrentPos*0.9); // proximate the externalObjects position to create a transition
            povMappedContent.getPovPtr()->lookAt(ofVec3f(0, 1.72, externalObject->getPosition().z+externalObject->getVelocity().z));
            
        }else{
            povMappedContent.setInvisible();
        }
        
        // SEND OSC
        if(ofGetElapsedTimeMillis()/50 != oldMillis && !deactivate){
            ofxOscMessage m;
            m.setAddress("/PocketPov/"+ofToString(index));
            m.addInt32Arg(isActive);
            if(isActive) m.addFloatArg(externalObject->getPosition().z);
            oscSender->sendMessage(m);
            
            oldMillis = ofGetElapsedTimeMillis()/50;
        }
        
    }
    
    float getMinLifespan(){ return minLifeSpan; };
    
    void setExternalObject(ExternalObject * externalObject){
        this->externalObject = externalObject;
        isActive = true;
    };
    
    ofFbo* getFboPtr(){ return &povMappedContent.fbo; }
    
    string getName(){ return name; }
    
    void setupParameterGroup(string name){
        parameterGroup.setName(name);
        parameterGroup.add(deactivate.set("deactivate", false));
        parameterGroup.add(minLifeSpan.set("minLifeSpan", 5., 0., 10.));
    }

    
    ofParameterGroup* getPointerToParameterGroup(){ return &parameterGroup; }

};

#endif /* PocketPov_h */
