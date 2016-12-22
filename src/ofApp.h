#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "Plane.h"
#include "Gate.h"
#include "Pov.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
    void setupGUI();

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
    
    vector<Gate> gates;
    Plane plane;
    Pov pov;
    
    // GUI
    bool bHideGui;
    ofxPanel gui;
    ofParameter<bool> drawGates;
    
    ofEasyCam cam;
		
};
