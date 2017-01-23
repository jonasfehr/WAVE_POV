#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "Gate.h"
#include "POV.h"
#include "ofxGrabCam.h"
#include "ofxSyphon.h"
#include "SyphonUtils.h"
#include "WaveUtils.h"
#include "ofxAutoReloadedShader.h"


#define VIEWER_HEIGHT 1.8

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
    void setupGUI();
    void drawGUI();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    // Cam
    ofxGrabCam camera;
    
    // GUI
    bool hideGui = false;
    ofxPanel gui;
    ofParameter<bool> drawFloor;
    ofParameter<bool> drawGates;
    ofParameter<bool> drawRays;
    ofParameter<bool> drawPlane;
    ofParameter<bool> drawSyphon;
    ofVec3f center = ofVec3f(0,VIEWER_HEIGHT, 40);

    
    // Gates
    vector<Gate> gates;
    
    // Syphon in & out
    SyphonClientDir syphonIn;
    SyphonFbo syphonOut;
    SyphonFbo syphonSimOut;
    
    // create content;
    WavePovGenerator contentPovFree;
    WavePovGenerator contentPovFront;
    WavePovGenerator contentPovBack;
    WaveSlitGenerator contentSlit;
    WaveShaderContent contentSmoke;
    
    ofxAutoReloadedShader mixShader;
    
    // mappings
    int mappingIndx;
    
    // viewPresets
    struct CameraPos{
        ofVec3f pos;
        string name;
    };
    vector<CameraPos> camPresets;
    int camPresetIndx;
    

    
};

