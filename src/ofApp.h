#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "ofxGrabCam.h"
#include "ofxSyphon.h"
#include "ofxAutoReloadedShader.h"
#include "ofxOsc.h"


#include "Gate.h"
#include "POV.h"
#include "ExternalObject.h"

#include "PocketPov.h"
#include "PocketZone.h"
#include "PocketPovPositionalContent.h"
#include "PocketGateReactiveContent.h"

#include "InputToWaveContent.h"
#include "WaveGateContent.h"
#include "WaveSlitContent.h"
#include "WaveShaderContent.h"
#include "WavePositionalContent.h"
#include "WaveGradientOnPositionContent.h"
#include "WaveRipplePovContent.h"
#include "WaveEffectContent.h"

#include "TextureMix.h"
#include "SyphonUtils.h"
#include "Wekinator.h"

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
    ofParameter<bool> drawSoundObjects;
    ofParameter<bool> autorunWekinator;
    ofVec3f center = ofVec3f(0,VIEWER_HEIGHT, 40);
    
    
    ofParameterGroup paramsWekinatorIn;
    ofParameter<float> in_1;
    ofParameter<float> in_2;

    ofParameterGroup paramsWekinatorOut;


    
    // Gates
    vector<Gate> gates;
    
    // Syphon in & out
    SyphonClientDir syphonIn;
    SyphonFbo syphonOut;

    // content
    InputToWaveContent contentPovFree;
    WaveShaderContent contentShaderSmoke;
//    WaveSlitContent contentSlit;
    WaveGateContent contentGate;
    WaveGradientOnPositionContent contentBeadsGradients;
    
    // Pockets
    PocketPovPositionalContent pocketPovPos_2;
    PocketGateReactiveContent pocketGateReactive_1;
    
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
    ofxOscReceiver oscFromWaveAudio;
    ofxOscReceiver oscFromWaveAudioParameters;
    ofxOscSender oscToWaveAudio;
    
    
    void receiveOSC();
    void sendOSC();
    
    int oldMillis = 0;

    
    
    // Images to be loaded
    vector<ofImage> imgGateContent;
    vector<ofImage> imgPosContent;
    
    // External Objects
    map<int, ExternalObject> users;
    map<int, ExternalObject> soundObjects;
    map<int, ExternalObject> beads;


    // presets
    int presetIndex = 1;
    void savePreset(int presetIndex);
    void loadPreset(int presetIndex);
    
    
};

