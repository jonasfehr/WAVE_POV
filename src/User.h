//
//  Users.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 24/01/2017.
//
//

#ifndef User_h
#define User_h
class User{
public:
    
    void updateValues(float position, float lifespan, float velocity){
        this->position = position;
        this->lifespan = lifespan;
        this->velocity = velocity;
        
        lastUpdate = 0;
    }
    
    void update(){
        lastUpdate -= 1;
    }
    
    bool isDead(){
        return lastUpdate > 10;
    }
    
    float getPosition(){ return position; }
    float getLifespan(){ return lifespan; }
    float getVelocity(){ return velocity; }
    
private:
    float lastUpdate = 0.;
    
    float position = 0.;
    float lifespan = 0.;
    float velocity = 0.;
};

#endif /* User_h */
