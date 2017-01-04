#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    setupGUI();
    
    // Add gates
    //    gates.push_back(Gate(GeoLocation(55.681089, 12.595234), 0, 0.0));
    //    gates.push_back(Gate(GeoLocation(55.681342, 12.595456), 0, 0.0));
    
    
    ofBackground(100, 100, 100);
    ofEnableSmoothing();
    camera.setCursorDrawEnabled(true);
    camera.setGlobalPosition(31.3198, 28.45, -37.9426);
    
    plane = ofPlane(ofVec3f(1,0,1), ofVec3f(1,0,-1), ofVec3f(0,1,0), ofVec2f(1.6,0.9));
    
    for(int i = 0; i < 40; i++){
        gates.push_back(Gate(ofVec3f(0,3.6,-40+(i*2)), &pov, &plane));
    }
    
    //    for(auto& g : gates){
    //        ofxRay::Ray ray;
    //        ray.setStart(g.top);
    //        ray.setEnd(pov);
    //
    //        intersectingRays.push_back(ray);
    //    }
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
    // POV
    if(povOrbit){
        pov.rotate((ofGetFrameNum() % 360)*0.001, center, ofVec3f(0,1,0));
    }
    
    if(lastPov != pov){     // Only update if POV is moved
        // Plane
        ofVec3f planeCenter = pov - pov.getNormalized() * 1.5;
        plane.setCenter(planeCenter);
        
        // Set normal for plane - FIXME: THIS DOESN'T WORK
        ofVec3f closest;
        double closestDist = std::numeric_limits<double>::max();
        
        for(auto& g : gates){
            double dist = ofDistSquared(g.top.x, g.top.y, g.top.z, pov.x, pov.y, pov.z);
            if(dist < closestDist){
                closestDist = dist;
                closest = g.top;
            }
        }
        
        ofVec3f normal = pov;
        normal.align(closest); // seems to make no difference ???
        plane.setNormal(-normal);
        
        // Gates
        for(auto& g : gates){
            g.update();
        }
        
        lastPov = pov;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    camera.begin();
    
    // Set pov camera
    if(povCamera){
        camera.setGlobalPosition(pov);
        camera.lookAt(center);
    }
    
    // Grid
    if(drawGrid){
        ofPushMatrix();
        ofSetColor(ofColor::darkSlateBlue);
        ofDrawGrid(1.0f, 60.0f, true, false, true, false);
        ofPopMatrix();
    }
    
    ofVec3f normal = pov;
    ofVec3f closest;
    double closestDist = std::numeric_limits<double>::max();
    
    for(auto& g : gates){
        double dist = ofDistSquared(g.top.x, g.top.y, g.top.z, pov.x, pov.y, pov.z);
        if(dist < closestDist){
            closestDist = dist;
            closest = g.top;
        }
    }
    
    ofDrawSphere(closest, 0.2);
    
    
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
            g.draw();
        }
    }
    
    // Point rays at pov
    if(drawRays){
        ofSetColor(155,100,100);
        for(auto& g : gates){
            for(auto& r : g.rays){
                ofDrawLine(r.getStart(), r.getEnd());
            }
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
