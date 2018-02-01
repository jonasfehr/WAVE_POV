//
//  CameraORBIT_CAMer.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 14/01/2018.
//

#pragma once
#include "ofMain.h"

enum cameraMode : int {
    FIXED_CAM,
    ORBIT_CAM
};

class CameraController : public ofCamera{
public:
    ofParameterGroup parameterGroup;
    
    // ORBIT_CAMParameters
    ofParameter<float> speedRPM;
    ofParameter<float> MaxOrbitCamX;
    ofParameter<float> MaxOrbitCamZ;
    float theta;

    // FIXED_CAMParameters
    ofParameter<float> posY;
    ofParameter<float> posX;
    ofParameter<float> posZ;
    ofParameter<float> lookAtY;
    ofParameter<float> lookAtX;
    ofParameter<float> lookAtZ;
    ofParameter<float> fov;
    
    
    float oldPosX,oldPosY,oldPosZ;
    float oldLookAtX,oldLookAtY,oldLookAtZ;
    float oldFov;

    
    int cameraMode;
    
    string name;
    
    
    CameraController(){
        
    }
    
    void setup(string name, int cameraMode){
        this->name = name;
        this->cameraMode = cameraMode;
        
        theta = 0.0;
        
        speedRPM.set("speedRPM", 1., -10, 10);
        MaxOrbitCamX.set("MaxOrbitCamX", 100, 1, 100);
        MaxOrbitCamZ.set("MaxOrbitCamZ", 100, 1, 100);
     
        posY.set("posY", 1.72, 0, 10);
        posX.set("posX", 0, -10, 10);
        posZ.set("posZ", 100, -20, 100);
        lookAtY.set("lookAtY", 1.72, 0, 10);
        lookAtX.set("lookAtX", 0, -100, 100);
        lookAtZ.set("lookAtZ", 39, -20, 100);
        fov.set("fov", 70, 1, 179);
        
        setupParameterGroup();

        this->setVFlip(true);
        this->setGlobalPosition(glm::vec3(posX,posY,posZ));
        this->lookAt(glm::vec3(lookAtX,lookAtY,lookAtZ), glm::vec3(0,1,0));
        this->setFov(fov);
        
        oldPosX = posX;
        oldPosY = posY;
        oldPosZ = posZ;
        oldLookAtX = lookAtX;
        oldLookAtY = lookAtY;
        oldLookAtZ = lookAtZ;
        oldFov = fov;

    }
    
    void update(){
        if(cameraMode == ORBIT_CAM){
            theta +=  (TWO_PI/ (3600))* speedRPM;
            
            posX =lookAtX+cos(theta)*MaxOrbitCamX/2;
            posZ =lookAtZ+sin(theta)*MaxOrbitCamZ/2;
        }

        bool posChanged = oldPosX != posX || oldPosY != posY || oldPosZ != posZ;
        bool lookAtChanged = oldLookAtX != lookAtX || oldLookAtY != lookAtY || oldLookAtZ != lookAtZ;
        
        if(posChanged || lookAtChanged || oldFov!=fov ){
            this->setGlobalPosition(glm::vec3(posX,posY,posZ));
            this->lookAt(glm::vec3(lookAtX,lookAtY,lookAtZ), glm::vec3(0,1,0));
            this->setFov(fov);
            
            oldPosX = posX;
            oldPosY = posY;
            oldPosZ = posZ;
            oldLookAtX = lookAtX;
            oldLookAtY = lookAtY;
            oldLookAtZ = lookAtZ;
            oldFov = fov;
        }
        
    }
    
    void draw(){
        ofSetColor(ofColor::blue);
        ofPushMatrix();
        ofTranslate(getGlobalPosition());
        ofDrawSphere(0, 0, 0.5, 0.5);
        ofTranslate(0,1,0);
        ofDrawBitmapString(name,0,0,0);
        ofPopMatrix();
        
        drawFrustum();
    }
    
    void setSpeed(float speedRPM){ this->speedRPM = speedRPM; }
    float getSpeed(){ return speedRPM; }
    
    void setViewerHeight(float viewerHeight){ this->posY = viewerHeight; }
    float getViewerHeight(){ return posY; }
    
    void setMaxOrbitCamX(float MaxOrbitCamX){ this->MaxOrbitCamX = MaxOrbitCamX; }
    float getMaxOrbitCamX(){ return MaxOrbitCamX; }
    
    void setMaxOrbitCamZ(float MaxOrbitCamZ){ this->MaxOrbitCamZ = MaxOrbitCamZ; }
    float getMaxOrbitCamZ(){ return MaxOrbitCamZ; }
    
    void setAngle(float angle){ this->theta = glm::radians(angle); }
    //    float getAngle(){ return (glm::mod(glm::degrees(theta),360.0)); }
    
    void setLookAt(glm::vec3 lookAt){
        lookAtX = lookAt.x;
        lookAtY = lookAt.y;
        lookAtZ = lookAt.z;
    }
    
    glm::vec3 getLookAt(){
        return glm::vec3(lookAtX,lookAtY,lookAtZ);
    }
    
    void setPosition(glm::vec3 position){
        posX = position.x;
        posY = position.y;
        posZ = position.z;
    }
    
    glm::vec3 getPosition(){
        return glm::vec3(posX,posY,posZ);
    }
    
    
    
    
    void setFov(float fov){ this->fov = fov; }
    float getFov(){ return fov; }
    
    void setupParameterGroup(){
        switch(cameraMode){
            case ORBIT_CAM:
                parameterGroup.setName("ORBIT_CAM");
                parameterGroup.add(speedRPM);
                parameterGroup.add(posY);
                parameterGroup.add(MaxOrbitCamX);
                parameterGroup.add(MaxOrbitCamZ);
                break;
                
            case FIXED_CAM:
                parameterGroup.setName("FIXED_CAM");
                parameterGroup.add(posY);
                parameterGroup.add(posX);
                parameterGroup.add(posZ);
                parameterGroup.add(lookAtY);
                parameterGroup.add(lookAtX);
                parameterGroup.add(lookAtZ);
                parameterGroup.add(fov);
                break;
                
            default:
                break;
        }
    }
    
    ofParameterGroup& getParameterGroup(){ return parameterGroup; }
};

