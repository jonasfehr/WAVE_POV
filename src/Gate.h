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
        Edge topEdge;
        topEdge.ray.setStart(this->top);
        topEdge.ray.setEnd(*pov);
        
        // Left Outer
        Edge leftOuterEdge;
        leftOuterEdge.pos = topEdge.ray.getStart();
        leftOuterEdge.pos.y = bottomHeight;
        leftOuterEdge.pos.x = -outerWidth;
        leftOuterEdge.ray.setStart(leftOuterEdge.pos);
        leftOuterEdge.ray.setEnd(*pov);
        
        // Right outer
        Edge rightOuterEdge;
        rightOuterEdge.pos = topEdge.ray.getStart();
        rightOuterEdge.pos.y = bottomHeight;
        rightOuterEdge.pos.x = outerWidth;
        rightOuterEdge.ray.setStart(rightOuterEdge.pos);
        rightOuterEdge.ray.setEnd(*pov);
        
        // Left Inner
        Edge leftInnerEdge;
        leftInnerEdge.pos = topEdge.ray.getStart();
        leftInnerEdge.pos.y = bottomHeight;
        leftInnerEdge.pos.x = -innerWidth;
        leftInnerEdge.ray.setStart(leftInnerEdge.pos);
        leftInnerEdge.ray.setEnd(*pov);
        
        // Right Inner
        Edge rightInnerEdge;
        rightInnerEdge.pos = topEdge.ray.getStart();
        rightInnerEdge.pos.y = bottomHeight;
        rightInnerEdge.pos.x = innerWidth;
        rightInnerEdge.ray.setStart(rightInnerEdge.pos);
        rightInnerEdge.ray.setEnd(*pov);
        
        // Add to edges
        edges.push_back(topEdge);
        edges.push_back(leftOuterEdge);
        edges.push_back(rightOuterEdge);
        edges.push_back(rightInnerEdge);
        edges.push_back(leftInnerEdge);
        
    };
    void update(){
        for(auto& e : edges){
            e.intersects = plane->intersect(e.ray, e.intersect);
            e.ray.setEnd(*pov);
        }
    };
    void draw(){
        int i = 0;
        for(auto& e : edges){
            ofDrawSphere(e.ray.getStart(), 0.05);
            ofSetColor(ofColor::whiteSmoke);
            ofDrawBitmapString(ofToString(e.intersect), e.ray.getStart());
            i++;
        }
        // Draw gate
        ofSetColor(ofColor::darkGray);
        ofSetLineWidth(4);
        
        ofDrawLine(edges.at(0).ray.getStart(), edges.at(1).ray.getStart());
        ofDrawLine(edges.at(0).ray.getStart(), edges.at(2).ray.getStart());
        ofDrawLine(edges.at(1).ray.getStart(), edges.at(4).ray.getStart());
        
        ofDrawLine(edges.at(2).ray.getStart(), edges.at(3).ray.getStart());
        
        ofSetLineWidth(1);
    };
    
    ofVec3f top;
    ofVec3f* pov;
    ofxRay::Plane* plane;
    
    struct Edge{
        bool intersects;
        ofVec3f intersect;
        ofxRay::Ray ray;
        ofVec3f pos;
        
    };
    
    vector<Edge> edges;
    
    
    // DIMENSIONS
    const float topHeight = 3.6f;
    const float bottomHeight = 0.3f;
    const float outerWidth = 2.4f;
    const float innerWidth = 1.1f;
    
    
    
};


#endif /* Gate_h */
