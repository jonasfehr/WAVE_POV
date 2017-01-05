#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "Gate.h"
#include "POV.h"
#include "ofxGrabCam.h"
#include "ofxRay.h"

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
    ofParameter<bool> drawGrid;
    ofParameter<bool> drawGates;
    ofParameter<bool> drawRays;
    ofParameter<bool> drawPlane;
    ofParameter<bool> povCamera;
    ofParameter<bool> povOrbit;
    ofParameter<bool> showGridLabels;
    ofVec3f center = ofVec3f(0,1,0);
    
    // POV
    POV pov;

    
    // Gates
    vector<Gate> gates;
};
