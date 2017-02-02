//
//  PocketPovPositionalContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 30/01/2017.
//
//

#ifndef PocketPovPositionalContent_h
#define PocketPovPositionalContent_h

#include "Pocket.h"
#include "InputToWaveContent.h"
#include "ExternalObject.h"



class PocketPovPositionalContent : public Pocket{
public:
    float minLifeSpan = 0;
    ExternalObject * externalObject;
    InputToWaveContent povMappedContent;
    float timeOfActivation;
    float oldExternalObjectLifespan = 0;
    int externalObjectNotUpdatedSince;
    string name;
    string getName(){ return name; }
    
    ofFbo fboContent;
    
    map<int, ExternalObject> *externalObjects;
    
    
    PocketPovPositionalContent(){};
    
    void setup(string channelName, float minLifeSpan, vector<Gate> * gates, ofVec3f povPosition, map<int, ExternalObject> *externalObjects){
        this->name = channelName;

        this-> minLifeSpan = minLifeSpan;

        this->externalObjects = externalObjects;
        this->povMappedContent.setInvisible(0.);
        
        povMappedContent.setup(channelName, gates, povPosition, &fboContent.getTexture(), POV_UV);
        
        fboContent.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);

    }
    
    
    void update(){
    
        // move pov if active
        if(isActive){
            
            fboContent.begin();
            {
                glClearColor(0.0, 0.0, 0.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
                povMappedContent.pov.begin();
                {
                    for(auto & o : *externalObjects){
                        
                        ofVec3f pos = o.second.getPosition();
                        ofVec3f size = o.second.getSize();

                        ofSetColor(255);
                        ofDrawSphere(pos, size.x);
                        
                    }
                }
                povMappedContent.pov.end();
            }
            fboContent.end();
            
            
            
            
            
            povMappedContent.setInputTexture(&fboContent.getTexture());
            povMappedContent.update();
            // deactivate pocket if externalObject is lost
            if(oldExternalObjectLifespan == externalObject->getLifespan()) externalObjectNotUpdatedSince++;
            if(externalObject->getLifespan() > oldExternalObjectLifespan) externalObjectNotUpdatedSince = 0;
            if(oldExternalObjectLifespan - externalObject->getLifespan() > 0 || externalObjectNotUpdatedSince > 10){
                isActive = false;
                povMappedContent.setInvisible();
                oldExternalObjectLifespan = 0;
                return;
            }
            oldExternalObjectLifespan = externalObject->getLifespan();
            
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
    
};

#endif /* PocketPovPositionalContent_h */
