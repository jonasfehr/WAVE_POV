//
//  WaveSlitContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 23/01/2017.
//
//

#ifndef WaveSlitContent_h
#define WaveSlitContent_h
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

class WaveSlitContent{
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
                shader.setUniform2f("u_resolution", fbo.getWidth(), fbo.getHeight());
                shader.setUniform2f("u_texResolution", image.getWidth(), image.getHeight());
                //                shader.setUniform1f("u_time", counter); //tempo p nr 1
                //                shader.setUniform1f("u_density", density);
                //                shader.setUniform1f("u_contrast", contrast);
                                shader.setUniform1f("u_H", hue);
                                shader.setUniform1f("u_S", saturation);
                                shader.setUniform1f("u_B", brightness);
                //                shader.setUniform1f("u_direction", direction);
                //                shader.setUniform1f("u_mix", mix);
                
                
                shader.setUniformTexture("texForSlit", image.getTexture(), 1);
                shader.setUniformTexture("texCounters", imgCounters.getTexture(), 2);
                
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
    ofParameter<float> hue;
    ofParameter<float> saturation;
    ofParameter<float> brightness;

    
    void setupGuiGroup(string name){
        paramGroup.setName(name);
        paramGroup.add(hue.set("hue", 0., 0., 1.));
        paramGroup.add(saturation.set("saturation", 0., 0., 1.));
        paramGroup.add(brightness.set("brightness", 1., 0., 1.));
    }
    
    ofParameterGroup getParameterGroup(){ return paramGroup; }
    
};

#endif /* WaveSlitContent_h */
