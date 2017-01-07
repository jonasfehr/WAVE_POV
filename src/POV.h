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
    void update(ofVec3f viewDirection){
        
        plane.setUp(ofVec3f(0,1,0)); // Trying to avoid weird rotation of plane
        ofVec3f normal = viewDirection;
        ofVec3f planeCenter = position + normal * 1.5;
        plane.setCenter(planeCenter);
        
        //normal.align(plane.getCenter()); // seems to make no difference ???
        plane.setNormal(normal);
        
        
        // Calculate the UV points
        
    };
    void orbit(ofVec3f pivot){
        position.rotate((ofGetFrameNum() % 360)*0.001, pivot, ofVec3f(0,1,0));
    };
    void draw(){};
    
    void drawPlane(){
        plane.draw();
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
    
    // POV
    ofVec3f position = ofVec3f(-50,1,0);
    ofVec3f lastPosition;
    
    
    // the following functions are inspired by https://www.shadertoy.com/view/MscSRr
    ofVec2f povProj(ofVec3f povPos, ofxRay::Plane _plane, ofVec3f p)
    {
        p = p-povPos;
        float cZ = p.dot(_plane.getNormal());
        float cX = p.dot(_plane.getRight());
        float cY = p.dot(_plane.getUp());
        return ofVec2f(cX/cZ,cY/cZ); // ofVec2f(cX,cY);//
    }

    ofVec3f rayPlaneIntersec(ofVec3f rayStart, ofVec3f rayEnd, ofVec3f planeOrigin, ofVec3f planeNormal)
    {
        float intersection = (planeOrigin-rayStart).dot(planeNormal) / ((rayEnd-rayStart).getNormalized()).dot(planeNormal);
        return (rayEnd-rayStart).getNormalized().operator*(intersection)+rayStart;
    }
    
    
    ofVec2f getUV(ofVec3f p){
        ofVec2f coord = povProj(position, plane, p)*-1;
        //coord.x = coord.x *-1;
        return (coord+1)/2;
    }
};

#endif /* POV_h */
