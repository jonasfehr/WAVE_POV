//
//  Gate.h
//  WAVE_POV
//
//  Created by Frederik Tollund Juutilainen on 22/12/16.
//
//

#ifndef Gate_h
#define Gate_h

#include "GeoLocation.h"
#include "Pov.h"

/*
 Gates gates gates
 */

class Gate{
public:
    Gate(GeoLocation location, double altitude, float rotation){
        this->location = location;
        this->altitude = altitude;
        this->rotation = rotation;
        
        // create 3d points based on location and rotation
        ofVec3d top;
        top.set(location.getLongitude(),location.getLatitude(),0);
        points.push_back(top);
    };
    
    void update(){};
    
    void draw(){
        for(auto& p : points){
            ofDrawSphere(p.x, p.y, p.z, 10);
        };
    
    };
    
    GeoLocation location;
    double altitude = 0;
    float rotation = 0;

    
    float height = 3.60;
    vector<ofVec3d> points;
    
};

#endif /* Gate_h */
