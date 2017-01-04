//
//  Gate.h
//  WAVE_POV
//
//  Created by Frederik Tollund Juutilainen on 04/01/17.
//
//

#ifndef Gate_h
#define Gate_h

#include "ofxRay.h"

class Gate{
public:
    Gate(ofVec3f top, ofVec3f* pov, ofxRay::Plane* plane){
        this->top = top;
        this->pov = pov;
        this->plane = plane;
    
        ofxRay::Ray ray;
        ray.setStart(this->top);
        ray.setEnd(*pov);
        
        rays.push_back(ray);
    
    
    };
    void update(){
        for(auto& r : rays){
        intersects = plane->intersect(r, intersect);
//        this->intersection = intersect;
            r.setEnd(*pov);
        }
    };
    void draw(){
        ofDrawSphere(this->top, 0.1);
        ofSetColor(ofColor::whiteSmoke);
        ofDrawBitmapString(ofToString(intersect), this->top);
    };
    
    bool intersects;
    ofVec3f top;
    ofVec3f* pov;
    ofVec3f intersect;
    ofxRay::Plane* plane;
    vector<ofxRay::Ray> rays;

};


#endif /* Gate_h */
