#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "Gate.h"
#include "POV.h"
#include "Users.h"
#include "Pocket.h"
#include "ofxGrabCam.h"
#include "ofxSyphon.h"
#include "SyphonUtils.h"
#include "WavePovContent.h"
#include "WaveGateContent.h"
#include "WaveShaderContent.h"
#include "WavePositionalContent.h"
#include "TextureMix.h"
#include "ofxAutoReloadedShader.h"
#include "Wekinator.h"


#define VIEWER_HEIGHT 1.8

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
    void setupParameterGroup();
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
    ofParameterGroup guiGroup;

    ofParameterGroup paramGroup;
    ofParameter<bool> drawFloor;
    ofParameter<bool> drawGates;
    ofParameter<bool> drawSyphon;
    ofVec3f center = ofVec3f(0,VIEWER_HEIGHT, 40);
    
    
    ofParameterGroup paramsWekinator;
    ofParameter<float> in_1;
    ofParameter<float> in_2;
    ofParameter<float> in_3;
    ofParameter<float> in_4;

    ofParameterGroup wekinatorTestOut;
    ofParameter<float> out_1;
    ofParameter<float> out_2;
    ofParameter<float> out_3;
    
    // Gates
    vector<Gate> gates;
    
    // Syphon in & out
    SyphonClientDir syphonIn;
    SyphonFbo syphonOut;
    SyphonFbo syphonSimOut;
    
    // create content;
    WavePovContent contentPovFree;
    WavePovContent contentPovFront;
    WavePovContent contentPovBack;
    WaveGateContent contentGate;
    WaveShaderContent contentSmoke;
    WavePositionalContent contentImpulses;
        
    // mappings
    int mappingIndx;
    
    // viewPresets
    struct CameraPos{
        ofVec3f pos;
        string name;
    };
    vector<CameraPos> camPresets;
    int camPresetIndx;
    

    TextureMix textureMixer;
    
    Wekinator wekinator;
    
    // OSC
    ofxOscReceiver oscFromSensorFuse;
    
    void receiveFromSensorFuse();

    
    // Users
    map<int, Users> users;
    
    // Pockets
    

    vector <std::unique_ptr<Pocket>> pockets;

    

};

