//
//  Users.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 24/01/2017.
//
//

#ifndef Users_h
#define Users_h
class Users{
public:
    float position;
    bool isActive;
    
    void setPosition(float position){
        this->position = position;
        life = 10;
    }
    
    void update(){
        life -= 1;
    }
    
    bool isDead(){
        return life <= 0;
    }
    
    float life = 0.;
};

#endif /* Users_h */
