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
#include "TextureToPovContent.h"

class PocketPov : public Pocket{
public:
    float minLifeSpan = 0;
    User * user;
    TextureToPovContent povMappedContent;
    float timeOfActivation;
    float oldUserLifespan = 0;
    int userNotUpdatedSince;
    
    PocketPov(){};
    
    void setup(string name, float minLifeSpan, vector<Gate> * gates, ofVec3f povPosition, ofTexture *texture){
        this-> minLifeSpan = minLifeSpan;
        this->povMappedContent = povMappedContent;
        this->povMappedContent.setInvisible(0.);
        
        povMappedContent.setup(name, gates, povPosition, texture, TUBE);
    }
    
//    void setup( float minLifeSpan, vector<Gate> * gates, ofVec3f povPosition, string shaderName){
//        this-> minLifeSpan = minLifeSpan;
//        this->povMappedContent = povMappedContent;
//        this->povMappedContent.setInvisible(0.);
//        
//        povMappedContent.setup(gates, povPosition, shaderName, ofVec2f(512), TUBE);
//    }
    
    void update(){
        
        
        // move pov if active
        if(isActive){
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

#endif /* PocketPov_h */
