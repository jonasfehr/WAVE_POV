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
    };
    
    void update(){};
    
    void draw(){};
    
    GeoLocation location; // longitude latitude
    double altitude = 0;
    float rotation = 0;
    
};

#endif /* Gate_h */
