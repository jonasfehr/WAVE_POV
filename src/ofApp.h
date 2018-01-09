#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "ofxGrabCam.h"
#include "ofxSyphon.h"
#include "ofxAutoReloadedShader.h"

#include "ofxSyphonUtils.h"
#include "ofxWekinator.h"
#include "ofxGpuMixer.h"
#include "ofxMad3D.h"
#include "ofxArtNode.h"

#include "Gate.h"
#include "User.h"

#include "PocketPov.h"
#include "PocketZone.h"

#include "InputToWaveContent.h"
#include "WaveGateContent.h"
#include "WaveShaderContent.h"
#include "WavePositionalContent.h"

//#include "TextureMix.h"

#define VIEWER_HEIGHT 1.73

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
    ofxPanel guiGeneral;
    ofxPanel guiMixer;
    ofxPanel guiControls;
    ofxPanel guiWekinator;

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

    
    // Gates
    vector<Gate> gates;
    
    // Syphon in & out
    ofxSyphonClientDir syphonIn;
    ofxSyphonFbo syphonOut;
    ofxSyphonFbo syphonLayerPreview;
    ofxSyphonFbo syphonSimOut;
    
    // create content;
    InputToWaveContent contentPovFree;
//    WaveGateContent contentGate;
    ofxGpuMixer::ShaderChannel contentShaderSmoke;
    ofxGpuMixer::ShaderChannel contentShaderLines;
//    WavePositionalContent contentPosGhosts;
    
    // mappings
    int mappingIndx;
    
    ofxMad3D::MappingImage mappingImg;
    ofxMad3D::CsvParser csvParser;

    // viewPresets
    struct CameraPos{
        ofVec3f pos;
        string name;
    };
    vector<CameraPos> camPresets;
    int camPresetIndx;
    int gateInfoIndx;


    ofxGpuMixer::Mixer mixer;
    
    ofxWekinator wekinator;
    
    // OSC
    ofxOscReceiver oscFromSensorFuse;
    
    void receiveFromSensorFuse();

    
    // Users
    map<int, User> users;
    
    // Pockets
    

    PocketZone pocketZone_1;
    PocketPov pocketPov_1;

    
    // Images to be loaded
    vector<ofImage> imgGateContent;
    vector<ofImage> imgPosContent;
    
    // NodeProgramming
    ofxArtNode artNode;

};

