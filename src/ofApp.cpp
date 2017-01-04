#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    setupGUI();
    
    // Add gates
    //    gates.push_back(Gate(GeoLocation(55.681089, 12.595234), 0, 0.0));
    //    gates.push_back(Gate(GeoLocation(55.681342, 12.595456), 0, 0.0));
    
    
    ofBackground(100, 100, 100);
    ofEnableSmoothing();
    //	camera.setCursorDraw(true);
    camera.setCursorDrawEnabled(true);
    camera.setGlobalPosition(31.3198, 28.45, -37.9426);
    
    plane = ofPlane(ofVec3f(1,1,0), ofVec3f(1,0,-1), ofVec3f(0,1,0), ofVec2f(1.6,0.9));
    
    for(int i = 0; i < 40; i++){
        gates.push_back(ofVec3f(0,3.6,-40+(i*2)));
    }
    
    for(auto& g : gates){
        ofxRay::Ray ray;
        ray.setStart(g);
        ray.setEnd(pov);
        
        intersectingRays.push_back(ray);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
    // POV
    if(povOrbit){
        pov.rotate((ofGetFrameNum() % 360)*0.001, ofVec3f(0,0,0), ofVec3f(0,1,0));
    }
    
    // Only update if POV is moved
    if(lastPov != pov){
        // Plane
        float residual = 0.0f;
        plane.fitToPoints(gates, residual);
        ofVec3f planeCenter = pov;
        plane.setCenter(planeCenter);
        
        // Gates
        
        
        // Point rays at pov
        for(auto& r : intersectingRays){
            bool intersects;
            ofVec3f intersect;
            intersects = plane.intersect(r, intersect);
            r.setEnd(pov);
        }
        
        lastPov = pov;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    camera.begin();
    
    if(povCamera){
        camera.setGlobalPosition(pov);
    }
    
    // Grid
    if(drawGrid){
        ofPushMatrix();
        ofSetColor(155,100,100);
        ofDrawGrid(1.0f, 60.0f, true, false, true, false);
        ofPopMatrix();
    }
    
    // POV
    if(drawPlane && !povCamera){
        ofSetColor(ofColor::antiqueWhite);
        ofDrawSphere(pov, 0.1);
    }
    
    // Plane
    if(drawPlane){
        plane.draw();
    }
    
    // Draw gates
    if(drawGates){
        for(auto& g : gates){
            ofDrawSphere(g, 0.1);
        }
    }
    
    // Point rays at pov
    if(drawRays){
        for(auto& r : intersectingRays){
            
            //            r.draw();
            ofDrawLine(r.getStart(), r.getEnd());
        }
    }
    
    camera.end();
    
    // Draw GUI
    if(!hideGui){
        gui.draw();
        string info;
        info += "FPS: " + ofToString(ofGetFrameRate());
        info += "\nPOV: " + ofToString(pov);
        info += "\nCam: " + ofToString(camera.getGlobalPosition());
        ofDrawBitmapStringHighlight(info, 15, gui.getHeight()+25);
    }
}

//--------------------------------------------------------------
void ofApp::setupGUI(){
    gui.setup();
    gui.add(drawGrid.set("draw grid", true));
    gui.add(drawGates.set("draw gates", true));
    gui.add(drawRays.set("draw rays", true));
    gui.add(drawPlane.set("draw plane", true));
    gui.add(povCamera.set("point of view", false));
    gui.add(povOrbit.set("pov orbit", false));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f' || key == 'F'){
        ofToggleFullscreen();
    }
    
    if(key == 'p' || key == 'P'){
        pov = camera.getCursorWorld();
    }
    
    if(key == 'g' || key == 'G'){
        hideGui = !hideGui;
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
