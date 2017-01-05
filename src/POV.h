//
//  POV.h
//  WAVE_POV
//
//  Created by Frederik Tollund Juutilainen on 05/01/17.
//
//

#ifndef POV_h
#define POV_h

class POV{
public:
    POV(){
        plane = ofPlane(ofVec3f(1,0,0), ofVec3f(1,0,-1), ofVec3f(0,1,0), ofVec2f(1.6,0.9));
    };
    void update(){
        plane.setUp(ofVec3f(0,1,0)); // Trying to avoid weird rotation of plane
        ofVec3f planeCenter = position - position.getNormalized() * 1.5;
        plane.setCenter(planeCenter);
        
        ofVec3f normal = position;
        normal.align(plane.getCenter()); // seems to make no difference ???
        plane.setNormal(-normal);
        
        // project
        projector.setGlobalPosition(position);
        projector.lookAt(plane.getNormal());
    };
    void orbit(ofVec3f pivot){
        position.rotate((ofGetFrameNum() % 360)*0.001, pivot, ofVec3f(0,1,0));
    };
    void draw(){};
    
    void drawPlane(){
        plane.draw();
        projector.draw();
    };
    
    void drawPovPoint(){
        ofSetColor(ofColor::antiqueWhite);
        ofDrawSphere(position, 0.1);
    }
    
    bool hasMoved(){
        bool hasMoved = lastPosition != position;
        lastPosition = position;
        return hasMoved;
    }
    
    // Plane
    ofxRay::Plane plane;
    ofxRay::Projector projector;
    
    // POV
    ofVec3f position = ofVec3f(-50,1,0);
    ofVec3f lastPosition;
};

#endif /* POV_h */
