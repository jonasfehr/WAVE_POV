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
    Gate(ofVec3f position, ofVec3f* pov, ofxRay::Plane* plane){
        this->top = ofVec3f(position.x, topHeight, position.z);;
        this->pov = pov;
        this->plane = plane;
        
        // TOP
        ofxRay::Ray topRay;
        topRay.setStart(this->top);
        topRay.setEnd(*pov);
        
        // Left Outer
        ofxRay::Ray leftOuterRay;
        ofVec3f leftOuterVec = topRay.getStart();
        leftOuterVec.y = bottomHeight;
        leftOuterVec.x = -outerWidth;
        leftOuterRay.setStart(leftOuterVec);
        leftOuterRay.setEnd(*pov);
        
        // Right outer
        ofxRay::Ray rightOuterRay;
        ofVec3f rightOuterVec = topRay.getStart();
        rightOuterVec.y = bottomHeight;
        rightOuterVec.x = outerWidth;
        rightOuterRay.setStart(rightOuterVec);
        rightOuterRay.setEnd(*pov);
        
        // Left Inner
        ofxRay::Ray leftInnerRay;
        ofVec3f leftInnerVec = topRay.getStart();
        leftInnerVec.y = bottomHeight;
        leftInnerVec.x = -innerWidth;
        leftInnerRay.setStart(leftInnerVec);
        leftInnerRay.setEnd(*pov);
        
        // Right Inner
        ofxRay::Ray rightInnerRay;
        ofVec3f rightInnerVec = topRay.getStart();
        rightInnerVec.y = bottomHeight;
        rightInnerVec.x = innerWidth;
        rightInnerRay.setStart(rightInnerVec);
        rightInnerRay.setEnd(*pov);
        
        
        // Add to rays
        rays.push_back(topRay);
        rays.push_back(leftOuterRay);
        rays.push_back(rightOuterRay);
        rays.push_back(rightInnerRay);
        rays.push_back(leftInnerRay);
        
        
        
        
    };
    void update(){
        for(auto& r : rays){
            intersects = plane->intersect(r, intersect);
//            this->intersection = intersect;
            r.setEnd(*pov);
        }
    };
    void draw(){
        for(auto& r : rays){
            ofDrawSphere(r.getStart(), 0.05);
            ofSetColor(ofColor::whiteSmoke);
//            ofDrawBitmapString(ofToString(intersect), r.getStart());
        }
        // Draw gate
        ofSetColor(ofColor::darkGray);
        ofDrawLine(rays.at(0).getStart(), rays.at(1).getStart());
        ofDrawLine(rays.at(0).getStart(), rays.at(2).getStart());
        ofDrawLine(rays.at(1).getStart(), rays.at(3).getStart());
        ofDrawLine(rays.at(2).getStart(), rays.at(4).getStart());
        
        
    };
    
    bool intersects;
    ofVec3f top;
    ofVec3f* pov;
    ofVec3f intersect;
    ofxRay::Plane* plane;
    vector<ofxRay::Ray> rays;
    
    // DIMENSIONS
    const float topHeight = 3.6f;
    const float bottomHeight = 0.3f;
    const float outerWidth = 2.4f;
    const float innerWidth = 1.1f;
    
    
    
};


#endif /* Gate_h */
