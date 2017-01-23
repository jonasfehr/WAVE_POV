#include "ofApp.h"
ofVec3f viewDirection;
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(80);
    ofEnableSmoothing();
    //ofDisableArbTex();
    
    
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
    img.load("images/Pass_4.png");
    contentSlit.setup(img, "slit");
    contentSmoke.setup("smokeNoise");
    contentSmoke.update();
    
    mixShader.load("shaders/mixer");
    
    // setup Syphon
    syphonIn.setup();
    syphonIn.find("Main View", "Modul8");

    syphonOut.setup("WaveForMapping", 120, 1300);    
    syphonSimOut.setup("WaveSimulation", ofGetWidth(), ofGetHeight());
    
    
    
    
    textureMixer.addFboChannel(&contentPovFree.fbo, "PovFree");
    textureMixer.addFboChannel(&contentSlit.fbo, "slit");
    textureMixer.addFboChannel(&contentSmoke.fbo, "smoke");
    textureMixer.setup();
    
    setupParameterGroup();
    guiGroup.setName("ContentControl");
    guiGroup.add(paramGroup);
    ofParameterGroup generic = contentSlit.getParameterGroup();
    guiGroup.add( generic );
    generic = textureMixer.getParameterGroup();
    guiGroup.add( generic );
    gui.setup(guiGroup);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
    contentPovFree.setInputTexture(&syphonIn.getTexture());
    contentPovFree.update(&camera);
    
    contentPovFront.setInputTexture(&syphonIn.getTexture());
    contentPovFront.update(&camera);
    
    contentPovBack.setInputTexture(&syphonIn.getTexture());
    contentPovBack.update(&camera);
    
    contentSlit.update();
    contentSmoke.update();

    
    syphonOut.begin();
    {
        ofClear(0);
        ofSetColor(255);
        
//        mixShader.begin();
//        {
//        
//            mixShader.setUniform2f("iResolution", syphonOut.getWidth(), syphonOut.getHeight());
//            mixShader.setUniform1f("iGlobalTime", ofGetElapsedTimef()); //tempo p nr 1
////            mixShader.setUniform1f("u_density", 1.);
////            mixShader.setUniform1f("u_contrast", 0.5);
////            mixShader.setUniform1f("u_H", .5);
////            mixShader.setUniform1f("u_S", 1.);
////            mixShader.setUniform1f("u_B", 1.);
////            mixShader.setUniform1f("u_direction", 1.);
////            mixShader.setUniform1f("u_mix", 0.5);
//            
//            
//            mixShader.setUniformTexture("tex0", contentPovFree.getTexture(), 0);
//            mixShader.setUniformTexture("tex1", contentPovFront.getTexture(), 1);
//            mixShader.setUniformTexture("tex2", contentSlit.getTexture(), 2);
//            mixShader.setUniformTexture("tex3", contentSmoke.getTexture(), 3);
//            
//            
//            ofSetColor(255,255,255);
//            ofFill();
//            ofDrawRectangle(0, 0, syphonOut.getWidth(), syphonOut.getHeight());
//        }
//        mixShader.end();
        textureMixer.draw(0,0,syphonOut.getWidth(), syphonOut.getHeight());
    }
    syphonOut.end();
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    syphonSimOut.begin();
    {
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
    }
    syphonSimOut.end();
    ofSetColor(255);
    syphonSimOut.draw();
    
    // Draw Syphon
    if(drawSyphon){
        ofPushMatrix();
        {
//            float s = 0.2;
//            ofTranslate(0, ofGetHeight()-syphonIn.getHeight()*s);
//            ofScale(s,s, 0);
            ofSetColor(255,10);
            ofScale(ofGetWidth()/syphonIn.getWidth(), ofGetHeight()/syphonIn.getHeight());
            syphonIn.draw();
            
            ofSetColor(ofColor::orange);
            vector<ofVec2f> pos = contentPovFree.getTexCoords();
            for(auto& p : pos){
                ofDrawCircle(p.x, syphonIn.getHeight()-p.y, 2);
            }
        }
        ofPopMatrix();
    }
    
    // Draw GUI
    drawGUI();
    
    
    // PUBLISH OUTPUT
    syphonOut.publish();
    syphonSimOut.publish();
    
}

//--------------------------------------------------------------
void ofApp::setupParameterGroup(){
    paramGroup.setName("VIEWs");
    paramGroup.add(drawFloor.set("draw floor", true));
    paramGroup.add(drawGates.set("draw gates", true));
    paramGroup.add(drawRays.set("draw rays", false));
    paramGroup.add(drawPlane.set("draw plane", false));
    paramGroup.add(drawSyphon.set("draw syphon in", true));
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
        contentPovFree.update(&camera);
        camera.lookAt(contentPovFree.pov.viewDirection+camera.getPosition());

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
    syphonSimOut.fbo.allocate(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
