#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Setup cam
    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    ofEnableDepthTest();
    
    cam.setDistance(100);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Calculate in

}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    ofRotateX(ofRadToDeg(.5));
    ofRotateY(ofRadToDeg(-.5));
        
    ofSetColor(255,0,0);
    ofFill();
    ofDrawBox(30);
    ofNoFill();
    ofSetColor(0);
    ofDrawBox(30);
    
    ofPushMatrix();
    ofTranslate(0,0,20);
    ofSetColor(0,0,255);
    ofFill();
    ofDrawBox(5);
    ofNoFill();
    ofSetColor(0);
    ofDrawBox(5);
    ofPopMatrix();
    cam.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
