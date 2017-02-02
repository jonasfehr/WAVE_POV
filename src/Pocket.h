//
//  Pocket.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 25/01/2017.
//
//

#ifndef Pocket_h
#define Pocket_h


class Pocket{
public:
    bool isActive = false;

    
    Pocket( ){
    }
    
    virtual void update() = 0;
    
    void getIsActive(){ return isActive; }


};

#endif /* Pocket_h */
