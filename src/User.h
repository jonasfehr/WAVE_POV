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
        this->position.x = position;
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
    
    float getPosition(){ return position.x; }
    float getLifespan(){ return lifespan; }
    float getVelocity(){ return velocity; }
    
    float lastUpdate = 0.;
    
    ofVec3f position = ofVec3f(0.);
    ofVec3f size = ofVec3f(200);
    float lifespan = 0.;
    float velocity = 0.;
};

#endif /* User_h */
