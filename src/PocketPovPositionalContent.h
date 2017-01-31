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
#include "Objects.h"



class PocketPovPositionalContent : public Pocket{
public:
    float minLifeSpan = 0;
    User * user;
    InputToWaveContent povMappedContent;
    float timeOfActivation;
    float oldUserLifespan = 0;
    int userNotUpdatedSince;
    
    ofFbo fboContent;
    
    map<int, Objects> *objects;
    
    
    PocketPovPositionalContent(){};
    
    void setup( float minLifeSpan, vector<Gate> * gates, ofVec3f povPosition, map<int, Objects> *objects){
        this-> minLifeSpan = minLifeSpan;

        this->objects = objects;
        this->povMappedContent.setInvisible(0.);
        
        povMappedContent.setup(gates, povPosition, &fboContent.getTexture(), POV_UV);
        
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
                    for(auto & o : *objects){
                        
                        ofVec3f pos = o.second.position;
                        ofVec3f size = o.second.size;

                        ofSetColor(255);
                        ofDrawSphere(pos, size.x);
                        
                    }
                }
                povMappedContent.pov.end();
            }
            fboContent.end();
            
            
            
            
            
            povMappedContent.setInputTexture(&fboContent.getTexture());
            povMappedContent.update();
            // deactivate pocket if user is lost
            if(oldUserLifespan == user->getLifespan()) userNotUpdatedSince++;
            if(user->getLifespan() > oldUserLifespan) userNotUpdatedSince = 0;
            if(oldUserLifespan - user->getLifespan() > 0 || userNotUpdatedSince > 10){
                isActive = false;
                povMappedContent.setInvisible();
                oldUserLifespan = 0;
                return;
            }
            oldUserLifespan = user->getLifespan();
            
            povMappedContent.setVisible();
            ofVec3f newPos = ofVec3f(0, 1.72, user->getPosition());
            ofVec3f povCurrentPos = povMappedContent.getPovPtr()->getPosition();
            povMappedContent.getPovPtr()->setGlobalPosition(newPos*0.1+povCurrentPos*0.9); // proximate the users position to create a transition
            povMappedContent.getPovPtr()->lookAt(ofVec3f(0, 1.72, user->getPosition()+user->getVelocity()));
            
        }
        
    }
    
    float getMinLifespan(){ return minLifeSpan; };
    
    void setUser(User * user){
        this->user = user;
        isActive = true;
    };
    
    ofFbo* getFboPtr(){ return &povMappedContent.fbo; }
    
};

#endif /* PocketPovPositionalContent_h */
