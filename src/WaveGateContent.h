//
//  WaveGateContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 23/01/2017.
//
//

#ifndef WaveGateContent_h
#define WaveGateContent_h
#include "ofxAutoReloadedShader.h"


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

class WaveGateContent{
public:
    ofFbo fbo;
    ofImage image;
    UpDownCounter counters[40];
    ofxAutoReloadedShader shader;
    
    void setup(ofImage image, string name){
        this->image = image;
        
        fbo.allocate(40, 1300, GL_RGBA32F_ARB);
        fbo.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        fbo.end();
        
        shader.load("shaders/slit");
        
        setupGuiGroup(name);
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
            
            
            ofFloatImage imgCounters;
            imgCounters.allocate(40, 1, OF_IMAGE_GRAYSCALE);
            int i = 0;
            while ( i < imgCounters.getPixels().size() ) {
                imgCounters.getPixels()[i] = counters[i].get();
                i++;
            }
            imgCounters.update();
            
            shader.begin();
            {
                
                shader.setUniformTexture("texForSlit", image.getTexture(), 1);
                shader.setUniformTexture("texCounters", imgCounters.getTexture(), 2);
                
                shader.setUniform2f("u_resolution", fbo.getWidth(), fbo.getHeight());
                shader.setUniform2f("u_texResolution", image.getWidth(), image.getHeight());
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
    
    void draw(){
        fbo.draw(0,0, 120, 1300);
    }
    
    ofTexture getTexture(){
        return fbo.getTexture();
    }
    
    
    ofParameterGroup paramGroup;
    ofParameter<float> speed;
    ofParameter<float> mode;
    ofParameter<float> easing;

    
    void setupGuiGroup(string name){
        paramGroup.setName(name);
        paramGroup.add(speed.set("speed", 0.01, 0., 0.2));
        paramGroup.add(mode.set("mode", 1, 1., 2));
        paramGroup.add(easing.set("easing", 1, 1., 2));
    }
    
    ofParameterGroup* getPointerToParameterGroup(){ return &paramGroup; }
    
};

#endif /* WaveGateContent_h */
