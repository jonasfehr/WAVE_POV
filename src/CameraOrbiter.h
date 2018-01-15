//
//  CameraOrbiter.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 14/01/2018.
//

#pragma once
#include "ofMain.h"


class CameraOrbiter : public ofCamera{
public:
    ofParameterGroup parameterGroup;

    ofParameter<float> speedRPM;
    ofParameter<float> viewerHeightY;
    ofParameter<float> maxOrbitX;
    ofParameter<float> maxOrbitZ;

    float fov;
    glm::vec3 center;
    glm::vec3 position;
    float theta;

  //  glm::vec3 position;
    
    string name;


    CameraOrbiter(){

    }
    
    void setup(string name){
        this->name = name;
        theta = 0.0;
        setupParameterGroup();
        
        center = glm::vec3(0,1.5,39.);
        
        this->setVFlip(true);
        this->setGlobalPosition(position);
        this->lookAt(center, glm::vec3(0,1,0));
        this->setFov(fov);
    }
    
    void update(){
        
        theta +=  (TWO_PI/ (3600))* speedRPM;
        glm::vec3 pos;
        glm::vec3 cent = center;

        pos.y = viewerHeightY;
        pos.x =cent.x+cos(theta)*maxOrbitX/2;
        pos.z =cent.z+sin(theta)*maxOrbitZ/2;
        
        position = pos;
        
        this->setGlobalPosition(position);
        this->lookAt(center, glm::vec3(0,1,0));
        this->setFov(fov);

        
    }
    
    void drawOrbiter(){
        ofSetColor(ofColor::blue);
        ofPushMatrix();
        ofTranslate(position);
        ofDrawSphere(0, 0, 0.5, 0.5);
        ofTranslate(0,1,0);
        ofDrawBitmapString(name,0,0,0);
        ofPopMatrix();
        
        drawFrustum();
    }
    
    void setSpeed(float speedRPM){ this->speedRPM = speedRPM; }
    float getSpeed(){ return speedRPM; }

    void setViewerHeight(float viewerHeight){ this->viewerHeightY = viewerHeight; }
    float getViewerHeight(){ return viewerHeightY; }

    void setMaxOrbitX(float maxOrbitX){ this->maxOrbitX = maxOrbitX; }
    float getMaxOrbitX(){ return maxOrbitX; }
    
    void setMaxOrbitZ(float maxOrbitZ){ this->maxOrbitZ = maxOrbitZ; }
    float getMaxOrbitZ(){ return maxOrbitZ; }
    
    void setAngle(float angle){ this->theta = glm::radians(angle); }
//    float getAngle(){ return (glm::mod(glm::degrees(theta),360.0)); }
    
    void setCenter(glm::vec3 center){ this->center = center;}
    glm::vec3 getCenter(){ return center; }
    
    void setFov(float fov){ this->fov = fov; }
    float getFov(){ return fov; }
    
    void setupParameterGroup(){
        parameterGroup.setName("orbitCam");
        parameterGroup.add(speedRPM.set("speedRPM", 1., -10, 10));
        parameterGroup.add(viewerHeightY.set("viewerHeight", 1.72, 0, 3.6));
        parameterGroup.add(maxOrbitX.set("maxOrbitX", 100, 1, 100));
        parameterGroup.add(maxOrbitZ.set("maxOrbitZ", 100, 1, 100));
//        parameterGroup.add(center.set("lookAt",ofVec3f(0,viewerHeightY,39),ofVec3f(-10,0,-10), ofVec3f(10,4,100) ));
//        parameterGroup.add(position.set("position",ofVec3f(0,viewerHeightY,-10),ofVec3f(-50,0,-50), ofVec3f(50,4,50) ));
//        parameterGroup.add(fov.set("fov", 70, 10, 180));

    }

    ofParameterGroup& getParameterGroup(){ return parameterGroup; }

    
};
