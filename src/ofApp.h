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
#include "CameraController.h"

//#include "PocketPov.h"
//#include "PocketZone.h"
//#include "WaveGateContent.h"
//#include "WaveShaderContent.h"
//#include "WavePositionalContent.h"


#define VIEWER_HEIGHT 1.73

#define NUMOFLAYERS_ORBIT 1
#define NUMOFLAYERS_FIXED 2


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

    // Gates
    vector<Gate> gates;
    
    // Syphon in & out
    ofxSyphonServerDirectory syphonDir;
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
    ofxSyphonClientDir syphonIn;
    ofxSyphonServer syphonOutLayers[NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED];
    ofFbo simulationFBO;

    SyphonToPovContent syphonInLayers[NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED];
    
    // mappings
    int mappingIndx;
    
    // viewPresets
    struct CameraPos{
        glm::vec3 pos;
        string name;
    };
    vector<CameraPos> camPresets;
    int camPresetIndx;
    string camPresetName;

    // OSC
    ofxOscReceiver oscLayerControl;
    void receiveLayerControl();

};

