//
//  WaveGateContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 23/01/2017.
//
//

#ifndef WaveGateContent_h
#define WaveGateContent_h

#include "WaveContent.h"


class UpDownCounter{
public:
    float value = 0;
    float increase = 0.001;
    
    bool activated = false;
    bool isUp = false;
    
    UpDownCounter(){}
    
    void activate(){
        activated = true;
        isUp = true;
    }
    
    void update(){
        if(activated){
            if(isUp) value += increase;
            if(value > 1) isUp = false;
            if(!isUp) value -= increase;
            if(value <= 0){
                value = 0;
                activated = false;
            }
        }
    }
    
    float get(){
        return value;
    }
};

class WaveGateContent : public WaveContent{
public:
    vector<ofImage> *images;
    UpDownCounter counters[40];
    ofFloatImage imgCounters;
    
    ofParameter<float> speed;
    ofParameter<int> mode;
    ofParameter<int> easing;
    ofParameter<int> imageIndex;

    
    WaveGateContent(){};
    
    void setup(string channelName, vector<ofImage> *images){
        this->images = images;
        this->name = channelName;

        fbo.allocate(40, 1300, GL_RGBA32F_ARB);
        fbo.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fbo.end();
        
        shader.load("shaders/WaveGateContent");
        
        setupParameterGroup(name);
        
        imgCounters.allocate(40, 1, OF_IMAGE_GRAYSCALE);
        
    }
    
    void activate(int index){
        counters[index].activate();
    }
    
    void update(){
        for(auto& c : counters){
            c.increase = speed;
            c.update();
        }
        fbo.begin();
        {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            

            int i = 0;
            while ( i < imgCounters.getPixels().size() ) {
                imgCounters.getPixels()[i] = counters[i].get();
                i++;
            }
            imgCounters.update();
            
            shader.begin();
            {
                shader.setUniformTexture("texForSlit", images->at(imageIndex).getTexture(), 1);
                shader.setUniformTexture("texCounters", imgCounters.getTexture(), 2);
                
                shader.setUniform2f("u_resolution", fbo.getWidth(), fbo.getHeight());
                shader.setUniform2f("u_texResolution", images->at(imageIndex).getWidth(), images->at(imageIndex).getHeight());
                shader.setUniform1f("u_time", ofGetElapsedTimef());
                shader.setUniform1i("u_mode", (int)mode);
                shader.setUniform1i("u_easing", (int)easing);

                ofSetColor(255,255,255);
                ofFill();
                ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
            }
            shader.end();
            
        }
        fbo.end();
    }
    

    void setupParameterGroup(string name){
        parameterGroup.setName(name);
        parameterGroup.add(speed.set("speed", 0.01, 0., 0.1));
        parameterGroup.add(mode.set("mode", 1, 1, 2));
        parameterGroup.add(easing.set("easing", 1, 0, 2));
        parameterGroup.add(imageIndex.set("imageIndex", 0, 0, images->size()-1));
    }
    
    ofParameterGroup* getPointerToParameterGroup(){ return &parameterGroup; }


    
private:

};

#endif /* WaveGateContent_h */
