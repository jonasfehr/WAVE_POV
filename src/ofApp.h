#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "ofxGrabCam.h"
#include "ofxSyphon.h"
#include "ofxAutoReloadedShader.h"
#include "ofxOscParameterSync.h"

#include "ofxSyphonUtils.h"
#include "ofxWekinator.h"
#include "ofxGpuMixer.h"
#include "ofxMad3D.h"
#include "ofxArtNode.h"

#include "Gate.h"
#include "User.h"

#include "TextureToPovContent.h"
#include "ShaderToPovContent.h"
#include "SyphonToPovContent.h"
#include "CameraOrbiter.h"

#include "PocketPov.h"
#include "PocketZone.h"
#include "WaveGateContent.h"
#include "WaveShaderContent.h"
#include "WavePositionalContent.h"


#define VIEWER_HEIGHT 1.73

#define NUMOFLAYERS 2


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
//    ofxPanel guiWekinator;

    ofParameterGroup guiGroup;
    
    ofParameterGroup paramsControls;


    ofParameterGroup paramGroup;
    ofParameter<bool> drawFloor;
    ofParameter<bool> drawGates;
    ofParameter<bool> drawSyphon;
    ofParameter<bool> drawOrbiter;
    ofVec3f center = ofVec3f(0,1.2+0.3, 39);
    
    
//    ofParameterGroup paramsWekinator;
//    ofParameter<float> in_1;
//    ofParameter<float> in_2;
//    ofParameter<float> in_3;
//    ofParameter<float> in_4;

    
    // Gates
    vector<Gate> gates;
    
    // Syphon in & out
    ofxSyphonServerDirectory syphonDir;
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
    ofxSyphonClientDir syphonIn;
    ofxSyphonFbo syphonOut;
    ofxSyphonFbo syphonLayerPreview;
    ofxSyphonFbo syphonSimOut;
    
    // create content;
    ofxGpuMixer::Mixer mixer;

    SyphonToPovContent syphonInLayers[NUMOFLAYERS];
    
//    WaveGateContent contentGate;
//    ofxGpuMixer::ShaderChannel contentShaderSmoke;
//    ofxGpuMixer::ShaderChannel contentShaderLines;
//    WavePositionalContent contentPosGhosts;
    
    // Pockets
//    PocketZone pocketZone_1;
//    PocketPov pocketPov_1;
    
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

    // Wekinator control
    //ofxWekinator wekinator;
    
    // OSC
    ofxOscReceiver oscFromSensorFuse;
    void receiveFromSensorFuse();
    
    ofxOscReceiver oscLayerControl;
    void receiveLayerControl();


    // Users
    map<int, User> users;
    
    // NodeProgramming
    ofxArtNode artNode;
};

