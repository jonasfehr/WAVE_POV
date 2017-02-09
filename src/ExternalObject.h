//
//  ExternalObject.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 31/01/2017.
//
//

#ifndef ExternalObject_h
#define ExternalObject_h
class ExternalObject{
public:
    
    ExternalObject(){};
    
    void updateValuesFromUser(float position, float lifespan, float velocity){
        this->position.z = position;
        this->lifespan = lifespan;
        this->velocity.z = velocity;
        
        lastUpdate = 0;
    }
    
    void updateValuesFromBead(float positionX, float distance,  float intensity ){
        this->position.z = positionX;
        this->position.y = intensity;
        
        lastUpdate = 0;
    }
    
    void updateValuesFromSoundObject(float posX, float posY){
        this->position.z = posX;
        this->position.x = posY;

        lastUpdate = 0;
    }
    
    void update(){
        lastUpdate -= 1;
    }
    
    bool isDead(){
        return lastUpdate < -10;
    }
    
    ofVec3f getPosition(){ return position; }
    void setPosition(ofVec3f position){ this->position = position; }

    float getLifespan(){ return lifespan; }
    
    ofVec3f getVelocity(){ return velocity; }
    
    ofVec3f getSize(){ return size; }
    void setSize(ofVec3f size){ this->position = size; }

    
private:
    float lastUpdate = 0.;
    
    ofVec3f position = ofVec3f(0.);
    float lifespan = 0.;
    ofVec3f velocity = ofVec3f(0.);
    ofVec3f size = ofVec3f(200);

    
    
    
};

#endif /* ExternalObject_h */
