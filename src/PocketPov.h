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

class PocketPov : public Pocket{
public:
    float minLifeSpan = 0;
    ExternalObject * externalObject;
    InputToWaveContent povMappedContent;
    float timeOfActivation;
    float oldexternalObjectLifespan = 0;
    int externalObjectNotUpdatedSince;
    string name;
    
    PocketPov(){};
    
    void setup(string channelName, float minLifeSpan, vector<Gate> * gates, ofVec3f povPosition, ofTexture *texture){
        this->name = channelName;
        this-> minLifeSpan = minLifeSpan;
        this->povMappedContent = povMappedContent;
        this->povMappedContent.setInvisible(0.);
        
        povMappedContent.setup(channelName, gates, povPosition, texture, POV_UV);
    }
    
    void setup(string channelName, float minLifeSpan, vector<Gate> * gates, ofVec3f povPosition, string shaderName){
        this->name = channelName;
        this-> minLifeSpan = minLifeSpan;
        this->povMappedContent = povMappedContent;
        this->povMappedContent.setInvisible(0.);
        
        povMappedContent.setup(channelName, gates, povPosition, shaderName, ofVec2f(512), POV_UV);
    }
    
    void update(){
        
        
        // move pov if active
        if(isActive){
            povMappedContent.update();
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
            
        }
        
    }
    
    float getMinLifespan(){ return minLifeSpan; };
    
    void setExternalObject(ExternalObject * externalObject){
        this->externalObject = externalObject;
        isActive = true;
    };
    
    ofFbo* getFboPtr(){ return &povMappedContent.fbo; }
    
    string getName(){ return name; }

};

#endif /* PocketPov_h */
