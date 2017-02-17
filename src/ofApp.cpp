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
    camPos.lookAt = center;
    camPresets.push_back(camPos);
    
    camPos.name = "from back";
    camPos.pos = ofVec3f(0, VIEWER_HEIGHT, 85);
    camPos.lookAt = center;
    camPresets.push_back(camPos);
    
    camPos.name = "from side";
    camPos.pos = ofVec3f(10, VIEWER_HEIGHT, 40);
    camPos.lookAt = center;
    camPresets.push_back(camPos);

    
    camPos.name = "off side";
    camPos.pos = ofVec3f(10, VIEWER_HEIGHT, -10);
    camPos.lookAt = center;
    camPresets.push_back(camPos);
    
    camPos.name = "inside/lookBack";
    camPos.pos = ofVec3f(0, VIEWER_HEIGHT, 39);
    camPos.lookAt = camPos.pos+ofVec3f(0,0,1.);
    camPresets.push_back(camPos);
    
    camPos.name = "inside/lookFront";
    camPos.pos = ofVec3f(0, VIEWER_HEIGHT, 39);
    camPos.lookAt = camPos.pos+ofVec3f(0,0,-1.);
    camPresets.push_back(camPos);
    
    
    camera.setCursorDrawEnabled(true);
    camera.setFov(70);
    camera.setGlobalPosition(camPresets[camPresetIndx].pos );
    camera.lookAt(camPresets[camPresetIndx].lookAt);
    
    // SETUP THE GATES
    for(int i = 0; i < 40; i++){
        Gate gate = Gate(ofVec3f(0,0, i*2), i);
        gates.push_back(gate);
    }
    
    // setup content generators
//    contentPovSun.setup("PovSun", &gates, camPresets[0].pos, "sun", ofVec2f(1024), POV_UV);
//    contentPovSunBack.setup("PovSunBack", &gates, camPresets[1].pos, "sunback", ofVec2f(1024), POV_UV);
//    contentPovLinesTunnel.setup("PovLinesTunnel", &gates, camPresets[0].pos, "LinesTunnel", ofVec2f(1024), POV_UV);
    contentPovFree.setup("SyponInPovFree", &gates, camPresets[0].pos, &syphonIn.getTexture(), TUBE);
    contentPovFboFront.setup("povFboFront",&gates, camPresets[0].pos);
//    contentPovFboBack.setup("povFboBack",&gates, camPresets[5].pos);

//    contentShaderLines.setup("Lines", "lines");
//    contentBeadsGradients.setup("BeadsGradients", &beads);
    //    contentSoundObjectsGradients.setup("SoundObjGradients", &soundObjects);
    
    //load images
    ofImage img;
    img.load("images/iris_1.png");
    img.update();
    imgGateContent.push_back(img);
    img.load("images/iris_2.png");
    img.update();
    imgGateContent.push_back(img);
    img.load("images/iris_3.png");
    img.update();
    imgGateContent.push_back(img);
    img.load("images/Pass_1.png");
    img.update();
    imgGateContent.push_back(img);
    img.load("images/nova_2.png");
    img.update();
    imgGateContent.push_back(img);
    img.load("images/tree_2.png");
    img.update();
    imgGateContent.push_back(img);
    contentGate.setup("Gate", &imgGateContent);

    
//    contentShaderSmoke.setup("smokeNoise", "smokeNoise");
//    contentSlit.setup("Slit", contentShaderSmoke.getTexturePtr());

    
//    contentEffectFront.setup("contentEffectFront", 1, &oscToWaveAudio);
//    contentRipplePovBack.setup("contentRipplePovBack", 2, &gates, camPresets[1].pos, &oscToWaveAudio);


    //
    //    img.load("images/img1.png");
    //    imgPosContent.push_back(img);
    //    img.load("images/img2.png");
    //    imgPosContent.push_back(img);
    //    img.load("images/img3.png");
    //    imgPosContent.push_back(img);
    //
        contentPosGhosts.setup("Ghosts", &imgPosContent, &beads);
    
    
    
    
    // setup Syphon
    syphonIn.setup();
    syphonIn.find("Main View", "Modul8");
    syphonOut.setup("WaveForMapping", 120, 1300);
    //    syphonSimOut.setup("WaveSimulation", ofGetWidth(), ofGetHeight());
    
    // add POCKETS
//    pocketPov_1.setup("PocketPov", 1, &gates, camPresets[0].pos, "electric",  &oscToWaveAudio);
//    pocketPovPos_2.setup("PocketPovSoundObj" ,1,5., &gates, camPresets[0].pos, &soundObjects,&oscToWaveAudio);
//    pocketGateReactive_1.setup("PocketGateReactive", 1, &oscToWaveAudio);
    
    
    
    // setup Mixer
//    textureMixer.addFboChannel(contentShaderSmoke.getFboPtr(), contentShaderSmoke.getName(), BLEND_SCREEN);
    textureMixer.addFboChannel(contentPovFree.getFboPtr(), contentPovFree.getName(), BLEND_ADD);
    textureMixer.addFboChannel(contentPovFboFront.getFboPtr(), contentPovFboFront.getName(), BLEND_ADD);
//    textureMixer.addFboChannel(contentPovFboBack.getFboPtr(), contentPovFboBack.getName(), BLEND_ADD);
//    textureMixer.addFboChannel(contentSlit.getFboPtr(), contentSlit.getName(), BLEND_ADD);
//    textureMixer.addFboChannel(contentPovLinesTunnel.getFboPtr(), contentPovLinesTunnel.getName(), BLEND_ADD);
//    textureMixer.addFboChannel(contentPovSun.getFboPtr(), contentPovSun.getName(), BLEND_ADD);
//    textureMixer.addFboChannel(contentPovSunBack.getFboPtr(), contentPovSunBack.getName(), BLEND_ADD);
//    textureMixer.addFboChannel(contentBeadsGradients.getFboPtr(), contentBeadsGradients.getName(), BLEND_ADD);
    textureMixer.addFboChannel(contentGate.getFboPtr(), contentGate.getName(), BLEND_ADD);
//    textureMixer.addFboChannel(contentSoundObjectsGradients.getFboPtr(), contentSoundObjectsGradients.getName(), BLEND_ADD);
//
//    textureMixer.addFboChannel(contentRipplePovBack.getFboPtr(), contentRipplePovBack.getName(), BLEND_ADD);
    
    //    textureMixer.addFboChannel(contentShaderSmoke.getFboPtr(), "Smoke", BLEND_SCREEN);
        textureMixer.addFboChannel(contentPosGhosts.getFboPtr(), "Ghosts", BLEND_ADD);
    
//    textureMixer.addFboChannel(pocketPov_1.getFboPtr(), pocketPov_1.getName(), BLEND_SOFT_LIGHT);
//    textureMixer.addFboChannel(pocketPovPos_2.getFboPtr(), "PovPocketPos_1", BLEND_SOFT_LIGHT);
//    textureMixer.addFboChannel(pocketGateReactive_1.getFboPtr(), pocketGateReactive_1.getName(), BLEND_SOFT_LIGHT);

    //    textureMixer.addFboChannel(pocketZone_1.getFboPtr(), "PovZone_1", BLEND_ADD);
    
    setupParameterGroup();
    guiGroup.setName("General");
    guiGroup.add(paramGroup);
    
        ofParameterGroup paramsControls;
        paramsControls.setName("ContentControls");
//    paramsControls.add(contentSlit.parameterGroup);
//    paramsControls.add(contentBeadsGradients.parameterGroup);
//    paramsControls.add(contentSoundObjectsGradients.parameterGroup);
//    paramsControls.add(contentEffectFront.parameterGroup);
//    paramsControls.add(contentRipplePovBack.parameterGroup);
//    paramsControls.add(pocketPovPos_2.parameterGroup);
//    paramsControls.add(pocketGateReactive_1.parameterGroup);
    paramsControls.add(contentGate.parameterGroup);
        paramsControls.add(contentPosGhosts.parameterGroup);
        guiControls.setup( paramsControls );
    guiMixer.setup( *textureMixer.getPointerToParameterGroup() );
    
    
    // setup Wekinator
    paramsWekinatorOut.setName("WekinatorOut");
    for(auto & paramsTextureMixerChannel : textureMixer.getVectorOfParameterSubgroups()){
        paramsWekinatorOut.add(paramsTextureMixerChannel->get("saturation"));
        paramsWekinatorOut.add(paramsTextureMixerChannel->get("opacity"));

        
    }
    
    wekinator.setup(&paramsWekinatorIn, &paramsWekinatorOut);
    ofParameterGroup paramsWekinator;
    paramsWekinator.setName("Wekinator");
    paramsWekinator.add(paramsWekinatorIn);
    paramsWekinator.add(paramsWekinatorOut);
    guiWekinator.setup(paramsWekinator);
    guiWekinator.minimizeAll();
    
    
    guiGeneral.setup(guiGroup);
    
    loadPreset(presetIndex);
    
    
    // SETUP OSC CHANNELS
    oscFromSensorFuse.setup(49162);
    oscFromWaveAudio.setup(49164);
    oscToWaveAudio.setup("localhost", 49165);
    oscFromWaveAudioParameters.setup(49166);

//    oscSyncedParameters = paramsWekinatorIn;
    // see Tree_AI if return needed
//    ofAddListener(oscSyncedParameters.parameterChangedE(),this,&ofApp::parameterChanged);
    
    
    createDiamondMesh();
}


//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
    // RECEIVE DATA FROM OUTSIDE
    wekinator.update();
    receiveOSC();
    
    
    // UPDATE External Objects
    for(auto & eO : users){
        eO.second.update();
        
        if(eO.second.isDead()) {
            users.erase(eO.first);
            return;
        }
    }
    for(auto & eO : soundObjects){
        eO.second.update();
        
        if(eO.second.isDead()) {
            soundObjects.erase(eO.first);
            return;
        }
    }
    for(auto & eO : beads){
        eO.second.update();
        
        if(eO.second.isDead()) {
            beads.erase(eO.first);
            return;
        }
    }

    
    // UPDATE ALL THE CONTENT
//    contentPovSun.update();
//    contentPovSunBack.update();
//    contentPovLinesTunnel.update();
    contentPovFree.update();
//    contentEffectFront.update();
//    contentRipplePovBack.update();
//    contentShaderLines.update();
//    contentSlit.update();
//    contentBeadsGradients.update();
//    contentSoundObjectsGradients.update();
//        contentShaderSmoke.update();
    contentGate.update();
        contentPosGhosts.update();
    //
    //    // UPDATE POCKETS
    //    pocketZone_1.update();
//    pocketPov_1.update();
//    pocketPovPos_2.update();
//    pocketGateReactive_1.update();
    
    
    contentPovFboFront.begin();
    {

        ofEnableAlphaBlending();
        for(auto & o : soundObjects){
            
            ofVec3f pos = o.second.getPosition();
            ofVec3f size = o.second.getSize();
            pos.y = 2.;
            ofSetColor(255);
            ofNoFill();
            ofSetIcoSphereResolution(1);
            ofPushMatrix();
            ofTranslate(pos);
            ofRotateY(ofGetElapsedTimef()*5.);
            diamond.drawWireframe();
            ofPopMatrix();
   
        }
        ofDisableAlphaBlending();
    }
    contentPovFboFront.end();
//    
//    contentPovFboBack.begin();
//    {
//        
//        ofEnableAlphaBlending();
//        for(auto & o : soundObjects){
//            
//            ofVec3f pos = o.second.getPosition();
//            ofVec3f size = o.second.getSize();
//            pos.y = 2.;
//            ofSetColor(255);
//            ofNoFill();
//            ofSetIcoSphereResolution(1);
//            ofPushMatrix();
//            ofTranslate(pos);
//            ofRotateY(ofGetElapsedTimef()*5.);
//            diamond.drawWireframe();
//            ofPopMatrix();
//            
//        }
//        ofDisableAlphaBlending();
//    }
//    contentPovFboBack.end();
    
    // sendOSC
    // SEND OSC gate 1
//    if(ofGetElapsedTimeMillis()/50 != oldMillis){
//        ofxOscMessage m;
//        m.setAddress("/Sun");
//        m.addFloatArg(contentPovSun.getIntensity());
//        oscToWaveAudio.sendMessage(m);
//        
//        oldMillis = ofGetElapsedTimeMillis()/50;
//    }
    
    
    // CREATE THE OUTPUT TO MADMAPPER
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
    //    syphonSimOut.begin();
    //    {
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
        
        if(drawSoundObjects){
            for(auto & o : soundObjects){
                ofVec3f pos = o.second.getPosition();
                ofVec3f size = o.second.getSize();
                pos.y = 2.;
                
                ofSetColor(255);
                                        ofNoFill();
                ofSetIcoSphereResolution(1);
                ofPushMatrix();
                ofTranslate(pos);
                ofRotateY(ofGetElapsedTimef()*5.);
                diamond.drawWireframe();
                ofPopMatrix();
                
            }
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
    //    }
    //    syphonSimOut.end();
    //    ofSetColor(255);
    //    syphonSimOut.draw();
    
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
    //    syphonSimOut.publish();
    //syphonLayerPreview.publish();
}

//--------------------------------------------------------------
void ofApp::setupParameterGroup(){
    paramGroup.setName("VIEWs");
    paramGroup.add(drawFloor.set("draw floor", true));
    paramGroup.add(drawGates.set("draw gates", true));
    paramGroup.add(drawSyphon.set("draw syphon in", true));
    paramGroup.add(drawSoundObjects.set("drawSoundObjects", true));
    
    paramsWekinatorIn.setName("WekinatorInputs");
    paramsWekinatorIn.add(in_1.set("isDay", 0., 0., 1.));
    paramsWekinatorIn.add(in_2.set("isDvale", 0., 0., 1.));

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
        info += "\nPreset: " + ofToString(presetIndex);
        ofDrawBitmapStringHighlight(info, 15, ofGetHeight()-4*15);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'g' || key == 'G'){
        hideGui = !hideGui;
    }
    
    if(key == 'r'){
        if(!wekinator.bIsRunning) {
            wekinator.startRunning();
        }else if(wekinator.bIsRunning) {
            wekinator.stopRunning();
        }
    }
//
//    if(key == '2'){
//        wekinator.startRecording();
//    }
//    
//    if(key == '3'){
//        wekinator.train();
//    }
//    
//    if(key == '4'){
//        wekinator.deleteTraining();
//    }
//    
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
        //        contentSlit.activate(ofRandom(0,40));
    }
    if(key-48 > 0 && key-48 < 10){
        presetIndex = key-48;
    }
    
    if(key == 'q'){
        loadPreset(presetIndex);

    }
    if(key == 'w'){
        savePreset(presetIndex);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
//    if(key == '2'){
//        wekinator.stopRecording();
//    }
}
//--------------------------------------------------------------
void ofApp::savePreset(int index){
    guiGeneral.saveToFile("presets/settingsGeneral_"+ofToString(index)+".xml");
    guiMixer.saveToFile("presets/settingsMixer_"+ofToString(index)+".xml");
    guiControls.saveToFile("presets/settingsControls_"+ofToString(index)+".xml");
}
void ofApp::loadPreset(int index){
    guiGeneral.loadFromFile("presets/settingsGeneral_"+ofToString(index)+".xml");
    guiMixer.loadFromFile("presets/settingsMixer_"+ofToString(index)+".xml");
    guiControls.loadFromFile("presets/settingsControls_"+ofToString(index)+".xml");
}

// -------------------
void ofApp::createDiamondMesh(){ // create the diamonds
    diamond.addVertex(ofVec3f(0,1.5,0));
    diamond.addVertex(ofVec3f(0.5,0,0.5));
    diamond.addVertex(ofVec3f(0.5,0,-0.5));
    diamond.addVertex(ofVec3f(-0.5,0,-0.5));
    diamond.addVertex(ofVec3f(-0.5,0,0.5));
    diamond.addVertex(ofVec3f(0,-1.5,0));
    
    
    diamond.addIndex(1);
    diamond.addIndex(0);
    diamond.addIndex(2);
    
    diamond.addIndex(2);
    diamond.addIndex(0);
    diamond.addIndex(3);
    
    diamond.addIndex(3);
    diamond.addIndex(0);
    diamond.addIndex(4);
    
    diamond.addIndex(4);
    diamond.addIndex(0);
    diamond.addIndex(1);
    
    diamond.addIndex(1);
    diamond.addIndex(5);
    diamond.addIndex(2);
    
    diamond.addIndex(2);
    diamond.addIndex(5);
    diamond.addIndex(3);
    
    diamond.addIndex(3);
    diamond.addIndex(5);
    diamond.addIndex(4);
    
    diamond.addIndex(4);
    diamond.addIndex(5);
    diamond.addIndex(1);
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
    //    syphonSimOut.fbo.allocate(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::receiveOSC(){
    //PARSE OSC
    
    // from SensorFuse
    while(oscFromSensorFuse.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscFromSensorFuse.getNextMessage(&m);
        
        std::vector<std::string> address = ofSplitString(m.getAddress(),"/",true);
        
        
        // check for mouse moved message
        if(address[0] == "Gate"){
//            contentSlit.activate(ofToInt(address[1]));
            contentGate.activate(ofToInt(address[1]));
            
//            pocketZone_1.gateActivated(ofToInt(address[1]));
//            pocketGateReactive_1.gateActivated(ofToInt(address[1]));
            
        }else if(address[0] == "User"){
            users[ofToInt(address[1])].updateValuesFromUser(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
            
//            if( pocketPov_1.getMinLifespan() < m.getArgAsFloat(1) ){
//                pocketPov_1.setExternalObject(&users[ ofToInt(address[1]) ]);
//            }
            
//            if( pocketPovPos_2.getMinLifespan() < m.getArgAsFloat(1) ){
//                pocketPovPos_2.setExternalObject(&users[ ofToInt(address[1]) ]);
//            }
            
        }else if(address[0] == "SoundObject"){
            soundObjects[ofToInt(address[1])].updateValuesFromSoundObject(m.getArgAsFloat(0), m.getArgAsFloat(1));
            
        }else if(address[0] == "Bead"){
            beads[ofToInt(address[1])].updateValuesFromBead(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
            
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
    
    // from WaveAudio
    while(oscFromWaveAudio.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscFromWaveAudio.getNextMessage(&m);
        
        std::vector<std::string> address = ofSplitString(m.getAddress(),"/",true);
        
        

        
        // check for mouse moved message
        if(address[0] == "Bead"){
            beads[ofToInt(address[1])].updateValuesFromBead(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
            
        }else{
            // unrecognized message: display on the bottom of the screen
            string msg_string;
            msg_string = "fromWaveAudio: " + m.getAddress();
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
    while(oscFromWaveAudioParameters.hasWaitingMessages()){

        oscFromWaveAudioParameters.getParameter(paramsWekinatorIn);

    }
    
    
}
