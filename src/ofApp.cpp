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
    contentPovFree.setup(&gates, camPresets[0].pos, &syphonIn.getTexture(), TUBE);
    
    //load images
    ofImage img;
    img.load("images/Pass_1.png");
    imgGateContent.push_back(img);
    img.load("images/Pass_2.png");
    imgGateContent.push_back(img);
    img.load("images/Pass_3.png");
    imgGateContent.push_back(img);
    img.load("images/Pass_4.png");
    imgGateContent.push_back(img);
    
    contentGate.setup("gate", &imgGateContent);
    
    img.load("images/img1.png");
    imgPosContent.push_back(img);
    img.load("images/img2.png");
    imgPosContent.push_back(img);
    img.load("images/img3.png");
    imgPosContent.push_back(img);
    
    contentPosGhosts.setup("Ghosts", &imgPosContent, &objects);

    
    contentShaderSmoke.setup("smokeNoise");
    contentShaderLines.setup("lines");
    
    
    // setup Syphon
    syphonIn.setup();
    syphonIn.find("Main View", "Modul8");
    syphonOut.setup("WaveForMapping", 120, 1300);
    syphonSimOut.setup("WaveSimulation", ofGetWidth(), ofGetHeight());
    
    // add POCKETS
    pocketZone_1.setup(10, 25, "electric", ofVec2f(512));
    pocketPov_1.setup(5., &gates, camPresets[0].pos, "electric");
    pocketPovPos_2.setup( 5., &gates, camPresets[0].pos, &objects);
    
    
    
    // setup Mixer
    textureMixer.addFboChannel(contentPovFree.getFboPtr(), "PovFree", BLEND_LIGHTEN);
    textureMixer.addFboChannel(contentShaderSmoke.getFboPtr(), "Smoke", BLEND_SCREEN);
    textureMixer.addFboChannel(contentShaderLines.getFboPtr(), "Lines", BLEND_SCREEN);
    textureMixer.addFboChannel(contentPosGhosts.getFboPtr(), "Ghosts", BLEND_ADD);
    textureMixer.addFboChannel(contentGate.getFboPtr(), "Gate", BLEND_ADD);
    textureMixer.addFboChannel(pocketPov_1.getFboPtr(), "PovPocket_1", BLEND_SOFT_LIGHT);
    textureMixer.addFboChannel(pocketPovPos_1.getFboPtr(), "PovPocketPos_1", BLEND_SOFT_LIGHT);
    textureMixer.addFboChannel(pocketZone_1.getFboPtr(), "PovZone_1", BLEND_ADD);
    
    setupParameterGroup();
    guiGroup.setName("General");
    guiGroup.add(paramGroup);
    
    ofParameterGroup paramsControls;
    paramsControls.setName("ContentControls");
    paramsControls.add(contentGate.parameterGroup);
    paramsControls.add(contentPosGhosts.parameterGroup);
    guiControls.setup( paramsControls );
    guiMixer.setup( *textureMixer.getPointerToParameterGroup() );
    guiWekinator.setup(paramsWekinator);
    
    
    wekinator.setup(&paramsWekinator, textureMixer.getVectorOfParameterSubgroups());
    
    
    guiGeneral.setup(guiGroup);
    
    
    guiGeneral.loadFromFile("settingsGeneral.xml");
    guiMixer.loadFromFile("settingsMixer.xml");
    guiControls.loadFromFile("settingsControls.xml");
    
    oscFromSensorFuse.setup(49162);
    

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
    

    contentPovFree.update();
    contentGate.update();
    contentShaderSmoke.update();
    contentShaderLines.update();
    contentPosGhosts.update();
    
    // UPDATE POCKETS
    pocketZone_1.update();
    pocketPov_1.update();
    pocketPovPos_2.update();
    
    
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
    //syphonLayerPreview.publish();
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
}

//--------------------------------------------------------------
void ofApp::drawGUI(){
    if(!hideGui){
        guiGeneral.draw();
        guiControls.setPosition(guiGeneral.getPosition().x, guiGeneral.getPosition().y + guiGeneral.getHeight() + 15);
        guiControls.draw();
        guiWekinator.setPosition(guiControls.getPosition().x, guiControls.getPosition().y + guiControls.getHeight() + 15);
        guiWekinator.draw();
        
        //right side
        guiMixer.setPosition(ofGetWidth()-guiMixer.getWidth()-15, 15);
        guiMixer.draw();

        string info;
        info += "FPS: " + ofToString(ofGetFrameRate());
        info += "\nPOV: " + ofToString(contentPovFree.pov.getPosition());
        info += "\nCam: " + ofToString(camera.getGlobalPosition());
        info += "\nCam: " + ofToString(camPresets[camPresetIndx].name);
        ofDrawBitmapStringHighlight(info, 15, ofGetHeight()-4*15);
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
    
    if(key == 'q'){
        guiGeneral.loadFromFile("settingsGeneral.xml");
        guiMixer.loadFromFile("settingsMixer.xml");
        guiControls.loadFromFile("settingsControls.xml");
    }
    if(key == 'w'){
        guiGeneral.saveToFile("settingsGeneral.xml");
        guiMixer.saveToFile("settingsMixer.xml");
        guiControls.saveToFile("settingsControls.xml");
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
            
            pocketZone_1.gateActivated(ofToInt(address[1]));
            
        }else if(address[0] == "User"){
            users[ofToInt(address[1])].updateValues(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
            
            //cout << "User #" << address[1] << " pos " << m.getArgAsFloat(0) << " life " << m.getArgAsFloat(1) << " vel " << m.getArgAsFloat(2) << endl;
            if( pocketPov_1.getMinLifespan() < m.getArgAsFloat(1) ){
                pocketPov_1.setUser(&users[ ofToInt(address[1]) ]);
            }
            
            if( pocketPovPos_1.getMinLifespan() < m.getArgAsFloat(1) ){
                pocketPovPos_1.setUser(&users[ ofToInt(address[1]) ]);
            }
            
        }else if(address[0] == "soundObject"){
            // Do something with SoundObjects id = address[1]
 
                objects[ofToInt(address[1])].setPosition(ofVec3f(m.getArgAsFloat(0),1.8, m.getArgAsFloat(1)));
            
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
