//
//  WavePovFboContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 30/01/2017.
//
//

#ifndef WavePovFboContent_h
#define WavePovFboContent_h

#include "WaveContent.h"
#include "InputToWaveContent.h"



class WavePovFboContent : public WaveContent{
public:
    InputToWaveContent povMappedContent;    
    ofFbo fboContent;
    ofMesh diamond;

    
    WavePovFboContent(){};
    
    void setup(string channelName, vector<Gate> * gates, ofVec3f povPosition){
        this->name = channelName;
        
        fboContent.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);

        povMappedContent.setup(channelName, gates, povPosition, &fboContent.getTexture(), POV_UV);
        
        povMappedContent.pov.setNearClip(0);
        
        setupParameterGroup(name);
        
        createDiamondMesh();
        
    }
    
    
    void begin(){
        
        
        // move pov if active
        fboContent.begin();
        
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            povMappedContent.pov.begin();
        
    }
//                ofEnableAlphaBlending();
//                for(auto & o : *externalObjects){
//                    
//                    ofVec3f pos = o.second.getPosition();
//                    ofVec3f size = o.second.getSize();
//                    
//                    pos.y = 1.73;
//                    
//                    ofSetColor(255);
//                    ofNoFill();
//                    ofSetIcoSphereResolution(1);
//                    ofPushMatrix();
//                    ofTranslate(pos);
//                    ofRotateX(ofGetElapsedTimef()*10.);
//                    ofRotateY(ofGetElapsedTimef()*5.);
//                    ofDrawIcoSphere(ofVec3f(0.), 1.);
//                    //                        ofDrawCircle(0, 0, 1.);
//                    ofPopMatrix();
//                    
//                    
//                }
//                ofDisableAlphaBlending();
    void end(){
        
        povMappedContent.pov.end();
        
        fboContent.end();
        
        povMappedContent.setInputTexture(&fboContent.getTexture());
        povMappedContent.update();
        
    }
    
    
    ofFbo* getFboPtr(){ return &povMappedContent.fbo; }
    
    void setupParameterGroup(string name){
        parameterGroup.setName(name);
        
    }
    
    void createDiamondMesh(){ // create the diamonds
        diamond.addVertex(ofVec3f(0,1.5,0));
        diamond.addVertex(ofVec3f(0.5,0,0.5));
        diamond.addVertex(ofVec3f(0.5,0,-0.5));
        diamond.addVertex(ofVec3f(-0.5,0,-0.5));
        diamond.addVertex(ofVec3f(-0.5,0,0.5));
        diamond.addVertex(ofVec3f(0,-1.5,0));
        
        
        diamond.addIndex(1);
        diamond.addIndex(0);
        diamond.addIndex(2);
        
        diamond.addIndex(2);
        diamond.addIndex(0);
        diamond.addIndex(3);
        
        diamond.addIndex(3);
        diamond.addIndex(0);
        diamond.addIndex(4);
        
        diamond.addIndex(4);
        diamond.addIndex(0);
        diamond.addIndex(1);
        
        diamond.addIndex(1);
        diamond.addIndex(5);
        diamond.addIndex(2);
        
        diamond.addIndex(2);
        diamond.addIndex(5);
        diamond.addIndex(3);
        
        diamond.addIndex(3);
        diamond.addIndex(5);
        diamond.addIndex(4);
        
        diamond.addIndex(4);
        diamond.addIndex(5);
        diamond.addIndex(1);
    }
    
};

#endif /* WavePovFboContent_h */
