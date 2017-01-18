#include "ofApp.h"
ofVec3f viewDirection;
//--------------------------------------------------------------
void ofApp::setup(){
    setupGUI();
    
    ofBackground(80);
    ofEnableSmoothing();
    
    
    // create presets for camera
    CameraPos camPos;
    camPos.name = "in front";
    camPos.pos = ofVec3f(0, VIEWER_HEIGHT, -5);
    camPresets.push_back(camPos);
    
    camPos.name = "from back";
    camPos.pos = ofVec3f(0, VIEWER_HEIGHT, 85);
    camPresets.push_back(camPos);
    
    camPos.name = "from side";
    camPos.pos = ofVec3f(10, VIEWER_HEIGHT, 40);
    camPresets.push_back(camPos);
    
    camPos.name = "off side";
    camPos.pos = ofVec3f(10, VIEWER_HEIGHT, -10);
    camPresets.push_back(camPos);
    
    camPos.name = "inside";
    camPos.pos = ofVec3f(0, VIEWER_HEIGHT, 30);
    camPresets.push_back(camPos);
    
    
    camera.setCursorDrawEnabled(true);
    camera.setFov(70);
    camera.setGlobalPosition(camPresets[camPresetIndx].pos );
    camera.lookAt(center);
    
    // Add gates
    for(int i = 0; i < 40; i++){
        Gate gate = Gate(ofVec3f(0,0, i*2), i);
        gates.push_back(gate);
    }
    
    // setup content generators
    contentPovFree.setup(&gates, camPresets[0].pos, POV_UV);
    
    contentPovFront.setup(&gates, camPresets[0].pos, POV_UV);
    
    contentPovBack.setup(&gates, camPresets[1].pos, POV_UV);
    
    ofImage img;
    img.load("images/Pass_1.png");
    contentSlit.setup(img);
    
    
    // setup Syphon
    syphonIn.setup();
    syphonOut.setup("WaveForMapping");
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
    contentPovFree.setInputTexture(&syphonIn.getTexture());
    contentPovFree.update();
    
    contentPovFront.setInputTexture(&syphonIn.getTexture());
    contentPovFront.update();
    
    contentPovBack.setInputTexture(&syphonIn.getTexture());
    contentPovBack.update();
    
    contentSlit.update();
    
    
    syphonOut.begin();
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        ofClear(0);
        ofSetColor(255);
        contentPovFree.draw();
//        ofSetColor(255,100,0);
//        contentPovFront.draw();
//        ofSetColor(0,100,255);
//        contentPovBack.draw();
        ofSetColor(255);
        contentSlit.draw();
    }
    syphonOut.end();
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    camera.begin();
    {
        ofBackground(10);
        
        // Grid
        if(drawFloor){
            ofSetColor(20);
            ofPushMatrix();
            ofRotateX(90);
            ofDrawPlane(0, 0, 1000, 1000);
            ofPopMatrix();
        }
        
        // Plane
        if(drawPlane){
            contentPovFree.pov.drawPlane();
        }
        
        // Draw gates
        if(drawGates){
            syphonOut.getTexture().bind();
            {
                for(auto& g : gates){
                    g.drawMeshLed();
                }
            }
            syphonOut.getTexture().unbind();
            
            for(auto& g : gates){
                g.drawMeshProfile();
            }
        }
        
        // Point rays at pov
        if(drawRays){
            ofSetColor(155,100,100);
            for(auto& e : contentPovFree.pov.edges){
                ofDrawLine(e.pos, contentPovFree.pov.position);
            }
        }
    }
    camera.end();
    
    // Draw Syphon
    if(drawSyphon){
        ofPushMatrix();
        {
            float s = 0.2;
            ofTranslate(0, ofGetHeight()-syphonIn.getHeight()*s);
            ofScale(s,s, 0);
            syphonIn.draw();
            
            ofSetColor(ofColor::orange);
            vector<ofVec2f> pos = contentPovFree.getTexCoords();
            for(auto& p : pos){
                ofDrawCircle(p.x, p.y, 10);
            }
        }
        ofPopMatrix();
    }
    
    // Draw GUI
    drawGUI();
    
    
    // PUBLISH OUTPUT
    syphonOut.publish();
}

//--------------------------------------------------------------
void ofApp::setupGUI(){
    gui.setup();
    gui.add(drawFloor.set("draw floor", true));
    gui.add(drawGates.set("draw gates", true));
    gui.add(drawRays.set("draw rays", false));
    gui.add(drawPlane.set("draw plane", false));
    gui.add(drawSyphon.set("draw syphon in", true));
}

//--------------------------------------------------------------
void ofApp::drawGUI(){
    if(!hideGui){
        gui.draw();
        string info;
        info += "FPS: " + ofToString(ofGetFrameRate());
        info += "\nPOV: " + ofToString(contentPovFree.pov.position);
        info += "\nCam: " + ofToString(camera.getGlobalPosition());
        info += "\nCam: " + ofToString(camPresets[camPresetIndx].name);
        ofDrawBitmapStringHighlight(info, 15, gui.getHeight()+25);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f' || key == 'F'){
        ofToggleFullscreen();
    }
    
    if(key == 'g' || key == 'G'){
        hideGui = !hideGui;
    }
    
    if(key == '1'){
        drawFloor = !drawFloor;
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
    
    if(key == 'm'){
        mappingIndx++;
        mappingIndx = mappingIndx%3;
        contentPovFree.setMappingType(mappingIndx);
    }
    
    if(key == 'c'){
        camPresetIndx++;
        camPresetIndx = camPresetIndx%camPresets.size();
        camera.setGlobalPosition(camPresets[camPresetIndx].pos );
        camera.lookAt(center);
    }
    if(key == 'x'){
        contentPovFree.setPov(camera.getPosition());
    }
    
    if(key == 's'){
        syphonIn.next();
    }
    
    if(key == 'p'){
        contentSlit.activate(ofRandom(0,40));
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

//--------------------------------------------------------------
