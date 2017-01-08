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
#include "POV.h"

class Gate{
public:
    Gate(ofVec3f position, POV* pov, int index){
        this->top = ofVec3f(position.x, topHeight, position.z);;
        this->pov = pov;
        this->index = index;
        
        // TOP
        Edge topEdge;
        topEdge.pos = this->top;
        topEdge.ray.setStart(topEdge.pos);
        topEdge.ray.setEnd(pov->position);
        
        
        // Left Outer
        Edge leftOuterEdge;
        leftOuterEdge.pos = topEdge.ray.getStart();
        leftOuterEdge.pos.y = bottomHeight;
        leftOuterEdge.pos.x = -outerWidth;
        leftOuterEdge.ray.setStart(leftOuterEdge.pos);
        leftOuterEdge.ray.setEnd(pov->position);
        
        // Right outer
        Edge rightOuterEdge;
        rightOuterEdge.pos = topEdge.ray.getStart();
        rightOuterEdge.pos.y = bottomHeight;
        rightOuterEdge.pos.x = outerWidth;
        rightOuterEdge.ray.setStart(rightOuterEdge.pos);
        rightOuterEdge.ray.setEnd(pov->position);
        
        // Left Inner
        Edge leftInnerEdge;
        leftInnerEdge.pos = topEdge.ray.getStart();
        leftInnerEdge.pos.y = bottomHeight;
        leftInnerEdge.pos.x = -innerWidth;
        leftInnerEdge.ray.setStart(leftInnerEdge.pos);
        leftInnerEdge.ray.setEnd(pov->position);
        
        // Right Inner
        Edge rightInnerEdge;
        rightInnerEdge.pos = topEdge.ray.getStart();
        rightInnerEdge.pos.y = bottomHeight;
        rightInnerEdge.pos.x = innerWidth;
        rightInnerEdge.ray.setStart(rightInnerEdge.pos);
        rightInnerEdge.ray.setEnd(pov->position);
        
        // Add to edges
        edges.push_back(topEdge);
        edges.push_back(leftOuterEdge);
        edges.push_back(rightOuterEdge);
        edges.push_back(rightInnerEdge);
        edges.push_back(leftInnerEdge);
        
        
        // create a meshLed to visualize the result
        
        float ledHeight = 0.021;
        float ledWidth = 0.012;
        float triOff = ledHeight*1.732;
        float profileHeight = 0.091;
        float profileWidth = 0.065;
        float profileOff = profileHeight*1.732;

        
        meshLed.addVertex(leftInnerEdge.pos+ofVec3f(0, 0, -ledWidth));
        meshLed.addVertex(leftInnerEdge.pos+ofVec3f(0, ledHeight, 0.));
        meshLed.addVertex(leftInnerEdge.pos+ofVec3f(0, 0, ledWidth));
        meshLed.addVertex(leftOuterEdge.pos+ofVec3f(0, 0, -ledWidth));
        meshLed.addVertex(leftOuterEdge.pos+ofVec3f(triOff, ledHeight, 0.));
        meshLed.addVertex(leftOuterEdge.pos+ofVec3f(0, 0, ledWidth));
        meshLed.addVertex(topEdge.pos+ofVec3f(0, 0, -ledWidth));
        meshLed.addVertex(topEdge.pos+ofVec3f(0, -ledHeight*2, 0.));
        meshLed.addVertex(topEdge.pos+ofVec3f(0, 0, ledWidth));
        meshLed.addVertex(rightOuterEdge.pos+ofVec3f(0, 0, -ledWidth));
        meshLed.addVertex(rightOuterEdge.pos+ofVec3f(-triOff, ledHeight, 0.));
        meshLed.addVertex(rightOuterEdge.pos+ofVec3f(0, 0, ledWidth));
        meshLed.addVertex(rightInnerEdge.pos+ofVec3f(0, 0, -ledWidth));
        meshLed.addVertex(rightInnerEdge.pos+ofVec3f(0, ledHeight, 0.));
        meshLed.addVertex(rightInnerEdge.pos+ofVec3f(0, 0, ledWidth));
        

        for(int i = 0; i < 12; i+=3){
            meshLed.addIndex(i+0);
            meshLed.addIndex(i+1);
            meshLed.addIndex(i+3);
            meshLed.addIndex(i+3);
            meshLed.addIndex(i+1);
            meshLed.addIndex(i+4);
        }

        for(int i = 0; i < 12; i+=3){
            meshLed.addIndex(i+2);
            meshLed.addIndex(i+1);
            meshLed.addIndex(i+5);
            meshLed.addIndex(i+5);
            meshLed.addIndex(i+1);
            meshLed.addIndex(i+4);
        }
        // endCaps
        meshLed.addIndex(0);
        meshLed.addIndex(1);
        meshLed.addIndex(2);
        meshLed.addIndex(12);
        meshLed.addIndex(13);
        meshLed.addIndex(14);
        
        
        
        meshProfile.addVertex(leftInnerEdge.pos+ofVec3f(0, 0, -ledWidth));
        meshProfile.addVertex(leftInnerEdge.pos+ofVec3f(0, -profileHeight, -profileWidth));
        meshProfile.addVertex(leftInnerEdge.pos+ofVec3f(0, -profileHeight, profileWidth));
        meshProfile.addVertex(leftInnerEdge.pos+ofVec3f(0, 0, ledWidth));

        meshProfile.addVertex(leftOuterEdge.pos+ofVec3f(0, 0, -ledWidth));
        meshProfile.addVertex(leftOuterEdge.pos+ofVec3f(-profileOff, -profileHeight, -profileWidth));
        meshProfile.addVertex(leftOuterEdge.pos+ofVec3f(-profileOff, -profileHeight, profileWidth));
        meshProfile.addVertex(leftOuterEdge.pos+ofVec3f(0, 0, ledWidth));
        
        meshProfile.addVertex(topEdge.pos+ofVec3f(0, 0, -ledWidth));
        meshProfile.addVertex(topEdge.pos+ofVec3f(0, profileHeight*2, -profileWidth));
        meshProfile.addVertex(topEdge.pos+ofVec3f(0, profileHeight*2, profileWidth));
        meshProfile.addVertex(topEdge.pos+ofVec3f(0, 0, ledWidth));
        
        meshProfile.addVertex(rightOuterEdge.pos+ofVec3f(0, 0, -ledWidth));
        meshProfile.addVertex(rightOuterEdge.pos+ofVec3f(profileOff, -profileHeight, -profileWidth));
        meshProfile.addVertex(rightOuterEdge.pos+ofVec3f(profileOff, -profileHeight, profileWidth));
        meshProfile.addVertex(rightOuterEdge.pos+ofVec3f(0, 0, ledWidth));
        
        meshProfile.addVertex(rightInnerEdge.pos+ofVec3f(0, 0, -ledWidth));
        meshProfile.addVertex(rightInnerEdge.pos+ofVec3f(0, -profileHeight, -profileWidth));
        meshProfile.addVertex(rightInnerEdge.pos+ofVec3f(0, -profileHeight, profileWidth));
        meshProfile.addVertex(rightInnerEdge.pos+ofVec3f(0, 0, ledWidth));
        
        
        for(int i = 0; i < 16; i+=4){
            meshProfile.addIndex(i+0);
            meshProfile.addIndex(i+1);
            meshProfile.addIndex(i+4);
            meshProfile.addIndex(i+4);
            meshProfile.addIndex(i+1);
            meshProfile.addIndex(i+5);
        }
        for(int i = 0; i < 16; i+=4){
            meshProfile.addIndex(i+1);
            meshProfile.addIndex(i+2);
            meshProfile.addIndex(i+5);
            meshProfile.addIndex(i+5);
            meshProfile.addIndex(i+2);
            meshProfile.addIndex(i+6);
        }
        for(int i = 0; i < 16; i+=4){
            meshProfile.addIndex(i+2);
            meshProfile.addIndex(i+3);
            meshProfile.addIndex(i+6);
            meshProfile.addIndex(i+6);
            meshProfile.addIndex(i+3);
            meshProfile.addIndex(i+7);
        }
        meshProfile.addIndex(1);
        meshProfile.addIndex(0);
        meshProfile.addIndex(2);
        meshProfile.addIndex(2);
        meshProfile.addIndex(0);
        meshProfile.addIndex(3);
        meshProfile.addIndex(17);
        meshProfile.addIndex(16);
        meshProfile.addIndex(18);
        meshProfile.addIndex(18);
        meshProfile.addIndex(16);
        meshProfile.addIndex(19);

        
        
    };
    void update(){
        vector<ofVec2f> texCoords;
        for(auto& e : edges){
            //e.intersects = pov->plane.intersect(e.ray, e.intersect);
            e.intersect = pov->rayPlaneIntersec(e.ray.getStart(), e.ray.getEnd(), pov->plane.getCenter(), pov->plane.getNormal());
            
            e.ray.setEnd(pov->position);
            e.uv = pov->getUV(e.ray.getStart());
            texCoords.push_back(ofVec2f(e.uv.x*1920,e.uv.y*1080));
            texCoords.push_back(ofVec2f(e.uv.x*1920,e.uv.y*1080));
            texCoords.push_back(ofVec2f(e.uv.x*1920,e.uv.y*1080));
        }
        meshLed.clearTexCoords();
        meshLed.addTexCoords(texCoords);
    };
    void draw(){
        int i = 0;
        for(auto& e : edges){
            ofSetColor(ofColor::black);
            ofDrawSphere(e.ray.getStart(), 0.05);
            
            ofSetColor(ofColor::orange);
            ofDrawSphere(e.intersect, 0.005);

           // ofDrawBitmapString(ofToString(e.uv), e.intersect);
            
            i++;
        }
        // Draw gate
        ofSetColor(ofColor::black);
        ofSetLineWidth(4);
        
        ofDrawLine(edges.at(0).ray.getStart(), edges.at(1).ray.getStart());
        ofDrawLine(edges.at(0).ray.getStart(), edges.at(2).ray.getStart());
        ofDrawLine(edges.at(1).ray.getStart(), edges.at(4).ray.getStart());
        
        ofDrawLine(edges.at(2).ray.getStart(), edges.at(3).ray.getStart());
        

        
        ofSetLineWidth(1);
    };
    
    void drawMeshLed(){
        ofSetColor(255);
        meshLed.draw();
    }
    void drawMeshProfile(){
        ofSetColor(0);
        meshProfile.draw();
    }
    ofVec3f top;
    POV* pov;
    
    struct Edge{
        bool intersects;
        ofVec3f intersect;
        ofxRay::Ray ray;
        ofVec3f pos;
        ofVec2f uv;
        
    };
    
    vector<Edge> edges;
    
    int index;
    
    ofMesh meshLed;
    ofMesh meshProfile;

    
    // DIMENSIONS
    const float topHeight = 3.6f;
    const float bottomHeight = 0.3f;
    const float outerWidth = 2.4f;
    const float innerWidth = 1.1f;
    
    
    
};


#endif /* Gate_h */
