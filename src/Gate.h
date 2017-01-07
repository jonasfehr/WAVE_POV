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
        
        
        // create a mesh to visualize the result
        
        float ledHight = 0.05;
        float triOff = ledHight*0.866;
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        mesh.addVertex(leftInnerEdge.pos);
        mesh.addVertex(leftInnerEdge.pos+ofVec3f(0, ledHight, 0.));
        mesh.addVertex(leftOuterEdge.pos);
        mesh.addVertex(leftOuterEdge.pos+ofVec3f(triOff*2, ledHight, 0.));
        mesh.addVertex(topEdge.pos);
        mesh.addVertex(topEdge.pos+ofVec3f(0, -triOff, 0.));
        mesh.addVertex(rightOuterEdge.pos);
        mesh.addVertex(rightOuterEdge.pos+ofVec3f(-triOff*2, ledHight, 0.));
        mesh.addVertex(rightInnerEdge.pos);
        mesh.addVertex(rightInnerEdge.pos+ofVec3f(0, ledHight, 0.));

        
       
        for(int i = 0; i < 8; i+=2){
        mesh.addIndex(i+0);
        mesh.addIndex(i+1);
        mesh.addIndex(i+2);
        mesh.addIndex(i+2);
        mesh.addIndex(i+1);
        mesh.addIndex(i+3);
        }
        
        
        
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
        }
        mesh.clearTexCoords();
        mesh.addTexCoords(texCoords);
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
    
    void drawMesh(){
        ofSetColor(255);
        mesh.draw();
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
    
    ofMesh mesh;
    
    
    // DIMENSIONS
    const float topHeight = 3.6f;
    const float bottomHeight = 0.3f;
    const float outerWidth = 2.4f;
    const float innerWidth = 1.1f;
    
    
    
};


#endif /* Gate_h */
