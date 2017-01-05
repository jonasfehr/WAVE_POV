#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    setupGUI();
    
    ofBackground(80);
    ofEnableSmoothing();
    camera.setCursorDrawEnabled(true);
    camera.setGlobalPosition(31.3198, 28.45, -37.9426);
    
    // Setup plane
    pov = POV();
    
    // Add gates
    for(int i = 0; i < 40; i++){
        gates.push_back(Gate(ofVec3f(0,3.6,-40+(i*2)), &pov));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
    // POV
    if(povOrbit){
        pov.orbit(center);
    }

    pov.update();

    
    if(pov.hasMoved()){     // Only update if POV is moved
        
        // Gates
        for(auto& g : gates){
            g.update();
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    camera.begin();
    
    // Set pov camera
    if(povCamera){
        camera.setGlobalPosition(pov.position);
        camera.lookAt(center);
    }
    
    // Grid
    if(drawGrid){
        ofPushMatrix();
        ofDrawGrid(1.0f, 60.0f, showGridLabels, false, true, false);
        ofPopMatrix();
    }
    
    // POV
    if(drawPlane && !povCamera){
        pov.drawPovPoint();
    }
    
    // Plane
    if(drawPlane){
        pov.drawPlane();
    }
    
    // Draw gates
    if(drawGates){
        for(auto& g : gates){
            g.draw();
        }
    }
    
    // Point rays at pov
    if(drawRays){
        ofSetColor(155,100,100);
        for(auto& g : gates){
            for(auto& e : g.edges){
                ofDrawLine(e.ray.getStart(), e.ray.getEnd());
            }
        }
    }
    
    //
    ofxRay::Ray testRay = ofxRay::Ray(camera.getCursorWorld(),ofVec3f(1,0,0), false);
    testRay.draw();
    
    bool intersects;
    ofVec3f intersect;
    intersects = pov.plane.intersect(testRay, intersect);
    
    
    camera.end();
    
    if(intersects){ofDrawBitmapStringHighlight(ofToString(pov.projector.getScreenCoordinateOfWorldPosition(intersect)), ofPoint(15,ofGetHeight()-25));}
    
    // Draw GUI
    drawGUI();
}

//--------------------------------------------------------------
void ofApp::setupGUI(){
    gui.setup();
    gui.add(drawGrid.set("draw grid", true));
    gui.add(showGridLabels.set("Show grid labels: ", false));
    gui.add(drawGates.set("draw gates", true));
    gui.add(drawRays.set("draw rays", true));
    gui.add(drawPlane.set("draw plane", true));
    gui.add(povCamera.set("point of view", false));
    gui.add(povOrbit.set("pov orbit", false));
}

//--------------------------------------------------------------
void ofApp::drawGUI(){
    if(!hideGui){
        gui.draw();
        string info;
        info += "FPS: " + ofToString(ofGetFrameRate());
        info += "\nPOV: " + ofToString(pov.position);
        info += "\nCam: " + ofToString(camera.getGlobalPosition());
        ofDrawBitmapStringHighlight(info, 15, gui.getHeight()+25);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f' || key == 'F'){
        ofToggleFullscreen();
    }
    
    if(key == 'p' || key == 'P'){
        pov.position = camera.getCursorWorld();
    }
    
    if(key == 'g' || key == 'G'){
        hideGui = !hideGui;
    }
    
    if(key == 'o' || key == 'O'){
        povOrbit = !povOrbit;
    }
    
    if(key == ' '){
        povCamera = false;
        camera.setGlobalPosition(31.3198, 28.45, -37.9426);
        camera.lookAt(center);
    }
    
    if(key == '1'){
        drawGrid = !drawGrid;
    }
    
    if(key == '2'){
        drawGates = !drawGates;
    }
    
    if(key == '3'){
        drawRays = !drawRays;
    }
    
    if(key == '4'){
        drawPlane = !drawPlane;
    }
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
