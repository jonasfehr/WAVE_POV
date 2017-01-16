#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "Gate.h"
#include "POV.h"
#include "ofxGrabCam.h"
#include "ofxRay.h"
#include "ofxSyphon.h"

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
    
    // POV
    POV pov;

    
    // Gates
    vector<Gate> gates;
    
    // max Pos for viewDirection
    Gate::Edge *eStartRight, *eStartLeft, *eEndRight, *eEndLeft;
    Gate::Edge *eMaxLeft, *eMaxRight;
    Gate::Edge *eTopStart, *eTopEnd;
    
    void findMaxPoints();
    
    // Syphon in & out
    ofxSyphonServerDirectory dir;
    ofxSyphonClient sClient;
    int dirIdx;
    void drawSyphonIn();
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
    ofxSyphonServer sServer;
    ofFbo outputFbo;
    
    ofMesh outputMesh;
    void createOutputMesh();
    
    // mappings
    int mappingIndx;
    
    struct CameraPos{
        ofVec3f pos;
        string name;
    };
    vector<CameraPos> camPresets;
    int camPresetIndx;
    
};

