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
    contentGate.setup(img, "gate");
    contentSmoke.setup("smokeNoise");
    
    contentImpulses.setup("impulses");
    
    
    // setup Syphon
    syphonIn.setup();
    syphonIn.find("Main View", "Modul8");

    syphonOut.setup("WaveForMapping", 120, 1300);    
    syphonSimOut.setup("WaveSimulation", ofGetWidth(), ofGetHeight());
    
    
    
    textureMixer.addFboChannel(&contentSmoke.fbo, "Smoke", BLEND_SCREEN);
    textureMixer.addFboChannel(&contentPovFree.fbo, "PovFree", BLEND_LIGHTEN);
    textureMixer.addFboChannel(&contentGate.fbo, "Gate", BLEND_ADD);
    
    setupParameterGroup();
    guiGroup.setName("ContentControl");
    guiGroup.add(paramGroup);
    guiGroup.add(paramsWekinator);
    // guiGroup.add(wekinatorTestOut);
    guiGroup.add( *contentGate.getPointerToParameterGroup() );
    guiGroup.add( *textureMixer.getPointerToParameterGroup() );
    
    wekinator.setup(&paramsWekinator, textureMixer.getVectorOfParameterSubgroups());

    
    gui.setup(guiGroup);
    
    gui.loadFromFile("settings.xml");
    
    oscFromSensorFuse.setup(49162);
    
    // ADD POCKETS
    std::unique_ptr<Pocket> p1 = std::unique_ptr<Pocket>(new PocketPov(100.));
    pockets.push_back(std::move(p1));
    std::unique_ptr<Pocket> p2 = std::unique_ptr<Pocket>(new PocketZone(10, 15));
    pockets.push_back(std::move(p2));
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
    wekinator.update();
    receiveFromSensorFuse();
    
    for(auto & u : users){
        u.second.update();
        
        if(u.second.isDead()) {
            users.erase(u.first);
            return;
        }
    }
    
    for(auto & p : pockets){
        p->update();
    }
    
    
    contentPovFree.setInputTexture(&syphonIn.getTexture());
    contentPovFree.update();
    
    contentPovFront.setInputTexture(&syphonIn.getTexture());
    contentPovFront.update();
    
    contentPovBack.setInputTexture(&syphonIn.getTexture());
    contentPovBack.update();
    
    contentGate.update();
    contentSmoke.update();

    
    syphonOut.begin();
    {
        ofClear(0);
        ofSetColor(255);
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
            float s = 0.2;
            ofTranslate(0, ofGetHeight()-syphonIn.getHeight()*s);
            ofScale(s,s, 0);
//            ofSetColor(255,10);
//            ofScale(ofGetWidth()/syphonIn.getWidth(), ofGetHeight()/syphonIn.getHeight());
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
    paramGroup.add(drawSyphon.set("draw syphon in", true));
    
    paramsWekinator.setName("WekinatorInputs");
    paramsWekinator.add(in_1.set("wekIn_1", 0., 0., 1.));
    paramsWekinator.add(in_2.set("wekIn_2", 0., 0., 1.));
    paramsWekinator.add(in_3.set("wekIn_3", 0., 0., 1.));
    paramsWekinator.add(in_4.set("wekIn_4", 0., 0., 1.));
    
    wekinatorTestOut.setName("WekinatorOutputs");
    wekinatorTestOut.add(out_1.set("wekOut_1", 0., 0., 1.));
    wekinatorTestOut.add(out_2.set("wekOut_2", 0., 0., 1.));
    wekinatorTestOut.add(out_3.set("wekOut_3", 0., 0., 1.));
}

//--------------------------------------------------------------
void ofApp::drawGUI(){
    if(!hideGui){
        gui.draw();
        string info;
        info += "FPS: " + ofToString(ofGetFrameRate());
        info += "\nPOV: " + ofToString(contentPovFree.pov.getPosition());
        info += "\nCam: " + ofToString(camera.getGlobalPosition());
        info += "\nCam: " + ofToString(camPresets[camPresetIndx].name);
        ofDrawBitmapStringHighlight(info, 15, gui.getHeight()+25);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'g' || key == 'G'){
        hideGui = !hideGui;
    }
    
    if(key == '1'){
        if(!wekinator.bIsRunning) {
            wekinator.startRunning();
        }else if(wekinator.bIsRunning) {
            wekinator.stopRunning();
        }
    }
    
    if(key == '2'){
        wekinator.startRecording();
    }
    
    if(key == '3'){
        wekinator.train();
    }
    
    if(key == '4'){
        wekinator.deleteTraining();
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
        contentPovFree.setPov(camera);
    }
    
    if(key == 's'){
        syphonIn.next();
    }
    
    if(key == 'p'){
        contentGate.activate(ofRandom(0,40));
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == '2'){
        wekinator.stopRecording();
    }
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
void ofApp::receiveFromSensorFuse(){
    //PARSE OSC
    while(oscFromSensorFuse.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscFromSensorFuse.getNextMessage(&m);
        
        std::vector<std::string> address = ofSplitString(m.getAddress(),"/",true);

        
        // check for mouse moved message
        if(address[0] == "Gate"){
            contentGate.activate(ofToInt(address[1]));
            
            // loop throug all pocketZone
            
            
        }else if(address[0] == "User"){
            users[ofToInt(address[1])].setPosition(m.getArgAsFloat(0));
            
            // loop throug all pocketPov

            
            
        }else{
            // unrecognized message: display on the bottom of the screen
            string msg_string;
            msg_string = m.getAddress();
            msg_string += ": ";
            for(int i = 0; i < m.getNumArgs(); i++){
                // get the argument type
                msg_string += m.getArgTypeName(i);
                msg_string += ":";
                // display the argument - make sure we get the right type
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msg_string += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                    msg_string += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                    msg_string += m.getArgAsString(i);
                }
                else{
                    msg_string += "unknown";
                }
            }
            cout << msg_string << endl;
        }
        
    }


}
