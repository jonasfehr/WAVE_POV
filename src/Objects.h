//
//  Objects.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 31/01/2017.
//
//

#ifndef Objects_h
#define Objects_h
class Objects{
public:
    ofVec3f position = ofVec3f(0.);
    ofVec3f size = ofVec3f(400.);
    
    void setPosition(ofVec3f position){ this->position = position; }
    void setSize(ofVec3f size){ this->position = size; }
};

#endif /* Objects_h */
