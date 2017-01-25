//
//  Pocket.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 25/01/2017.
//
//

#ifndef Pocket_h
#define Pocket_h
enum BlendModes : int {
    POCKET_POV = 1,
    POCKET_ZONE = 2,
};

class Pocket{
public:
    bool isActive = false;
    
    Pocket( ){
    }
    
    virtual void update() = 0;
    
    void getIsActive(){ return isActive; }
    
};

class PocketPov : public Pocket{
public:
    float minLifeSpan = 0;
    
    PocketPov( float minLifeSpan ){
        this-> minLifeSpan = minLifeSpan;
    }
    
    void update(){
        // ACTIVATE
    }
    
    void getMinLifespan(){ return minLifeSpan; };
    
};

class PocketZone : public Pocket{
public:
    int start = 0, stop = 40;
    
    vector<int> activatedGates;
    
    PocketZone( int start, int stop){
        this->start = start;
        this->stop = stop;
    };
    
    void update(){
        for(auto & g : activatedGates){
            if(g == start) isActive = true;
            if(g == stop) isActive = false;
        }
    }
    
    void activatedGate(int gate){
        activatedGates.push_back(gate);
    };
    
};



#endif /* Pocket_h */
