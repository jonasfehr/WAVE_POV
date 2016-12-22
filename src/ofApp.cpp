#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Setup cam
    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
//    ofEnableDepthTest();
    
//    cam.setDistance(100);

    setupGUI();
    
    // Add gates
    gates.push_back(Gate(GeoLocation(55.681089, 12.595234), 0, 0.0));
    gates.push_back(Gate(GeoLocation(55.681342, 12.595456), 0, 0.0));

}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    if(drawGates){
        for(auto g : gates){
            g.draw();
        }
    }


    
    cam.end();
    if(!bHideGui){gui.draw();}

}

//--------------------------------------------------------------
void ofApp::setupGUI(){
    bHideGui = false;
    gui.setup();
    gui.add(drawGates.set("draw gates", true));


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
