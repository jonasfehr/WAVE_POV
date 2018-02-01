#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(80);
    ofEnableSmoothing();
    //ofDisableArbTex();
    
    
    // Add gates
    for(int i = 0; i < 40; i++){
        Gate gate = Gate(glm::vec3(0,0, i*2), i);
        gates.push_back(gate);
    }
    
    // create Mapping Image
//    vector<ofx::Mad3D::Fixture> fixtures;
//    for(auto & g : gates){
//        for(auto & f : g.fixtures)
//        {
//            fixtures.push_back(f);
//        }
//    }
//    mappingImg.setup(fixtures, 40, 1300);
//    mappingImg.save("mappingImg.png");
    
    // setup content generators
    //syphonInLayer_1.setup(&gates, camPresets[0].pos, &syphonIn.getTexture(), UV);
    
    // setup Syphon
//    syphonOut.setup("WaveForMapping", 120, 1300);
//    syphonSimOut.setup("WaveSimulation", ofGetWidth(), ofGetHeight());
    
    
    syphonDir.setup();
    ofAddListener(syphonDir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(syphonDir.events.serverRetired, this, &ofApp::serverRetired);
    
    for(int i = 0; i < NUMOFLAYERS_ORBIT; i++){
        syphonInLayers[i].setup("syphonIn_"+ofToString(i+1),&gates, &syphonDir, "syphonLayer_"+ofToString(i+1), "MadMapper", POV_UV_NORMALIZED, ORBIT_CAM);
    }
    for(int i = NUMOFLAYERS_ORBIT; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
        syphonInLayers[i].setup("syphonIn_"+ofToString(i+1),&gates, &syphonDir, "syphonLayer_"+ofToString(i+1), "MadMapper", POV_UV_NORMALIZED, FIXED_CAM);
    }
    
    for(int i = 0; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
        syphonOutLayers[i].setName("syphonIn_"+ofToString(i+1));
    }
    
    syphonIn.setup("Master", "MadMapper");

    //    syphonInLayer_1.setup("syphonIn_1",&gates, camPresets[0].pos, &syphonDir, "Modul8", "Main View", UV);
//    syphonInLayer_2.setup("syphonIn_2",&gates, camPresets[0].pos, &syphonDir, "Modul8", "Main View", UV);
//    syphonInLayer_3.setup("syphonIn_3",&gates, camPresets[0].pos, &syphonDir, "Modul8", "Main View", UV);
//    syphonInLayer_4.setup("syphonIn_4",&gates, camPresets[0].pos, &syphonDir, "Modul8", "Main View", UV);

    // create presets for camera
    CameraPos camPos;
    camPos.name = "in front";
    camPos.pos = glm::vec3(0, VIEWER_HEIGHT, -5);
    camPresets.push_back(camPos);
    
    camPos.name = "from back";
    camPos.pos = glm::vec3(0, VIEWER_HEIGHT, 85);
    camPresets.push_back(camPos);
    
    camPos.name = "from side";
    camPos.pos = glm::vec3(10, VIEWER_HEIGHT, 40);
    camPresets.push_back(camPos);
    
    camPos.name = "off side";
    camPos.pos = glm::vec3(10, VIEWER_HEIGHT, -10);
    camPresets.push_back(camPos);
    
    camPos.name = "inside";
    camPos.pos = glm::vec3(0, VIEWER_HEIGHT, 30);
    camPresets.push_back(camPos);

    
    camera.setCursorDrawEnabled(true);
    camera.setFov(70);
    camera.setGlobalPosition(camPresets[camPresetIndx].pos );
    camera.lookAt(center);
    camPresetName = camPresets[camPresetIndx].name;
    
    //load images
//    ofImage img;
//    img.load("images/Pass_1.png");
//    imgGateContent.push_back(img);
//    img.load("images/Pass_2.png");
//    imgGateContent.push_back(img);
//    img.load("images/Pass_3.png");
//    imgGateContent.push_back(img);
//    img.load("images/Pass_4.png");
//    imgGateContent.push_back(img);
    
//    contentGate.setup("gate", &imgGateContent);
    
//    contentShaderSmoke.setup("smokeNoise", "shaders/smokeNoise", 40, 1300, ofx::GpuMixer::SHADER_SHADERTOY);
//    contentShaderLines.setup("lines", "shaders/lines", 40, 1300, ofx::GpuMixer::SHADER_SHADERTOY);
//    contentPosGhosts.setup("Ghosts");
    
    
    // add POCKETS
//    pocketZone_1.setup(10, 25, "electric", ofVec2f(512));
//    pocketPov_1.setup(5., &gates, camPresets[0].pos, "electric");
    
    // setup Mixer
//    for(int i = 0; i < NUMOFLAYERS_ORBIT; i++){
//        mixer.addChannel(syphonInLayers[i], ofxGpuMixer::BLEND_ADD);
//    }

//    mixer.addChannel(contentShaderSmoke, ofxGpuMixer::BLEND_ADD);
//    mixer.addChannel(contentShaderLines, ofxGpuMixer::BLEND_ADD);
//    mixer.addChannel(pocketPov_1.getFboPtr(), "PovPocket_1", BLEND_SOFT_LIGHT);
//    mixer.addChannel(pocketZone_1.getFboPtr(), "PovZone_1", BLEND_ADD);
//    mixer.addChannel(contentPosGhosts.getFboPtr(), "Ghosts", BLEND_ADD);
//    mixer.addChannel(contentGate.getFboPtr(), "Gate", BLEND_ADD);
    
//    mixer.setup();
    
    setupParameterGroup();
    guiGroup.setName("General");
    guiGroup.add(paramGroup);
    
    paramsControls.setName("ContentControls");
    
    for(int i = 0; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
        paramsControls.add(syphonInLayers[i].getParameterGroup());

//        paramsControls.add(syphonInLayers[i].parameterGroup.get("inputSelect"));
//        paramsControls.add(syphonInLayers[i].parameterGroup.get("mappingMode"));
//        paramsControls.add(syphonInLayers[i].parameterGroup.get("speedRPM"));
    }
    
    guiControls.setup( paramsControls );
//    guiMixer.setup( mixer.getParameterGroup() );
//    guiWekinator.setup(paramsWekinator);
    
    
//    wekinator.setup(&paramsWekinator, mixer.getVectorOfParameterSubgroups());
    
    
    guiGeneral.setup(guiGroup);
    
    
    guiGeneral.loadFromFile("settingsGeneral.xml");
//    guiMixer.loadFromFile("settingsMixer.xml");
    guiControls.loadFromFile("settingsControls.xml");
    
//    oscFromSensorFuse.setup(49162);
    
//    oscLayerControl.setup(paramsControls, 49164, "localhost", 49165);
    oscLayerControl.setup(49166);
    
//    gateInfoIndx = 0;
    
//    artNode.setup();
//    artNode.sendPoll();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
    
//    wekinator.update();
//    receiveFromSensorFuse();
    receiveLayerControl();

//    for(auto & u : users){
//        u.second.update();
//
//        if(u.second.isDead()) {
//            users.erase(u.first);
//            return;
//        }
//    }
    
    for(int i = 0; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
        syphonInLayers[i].update();
    }

//    contentGate.update();
//    contentShaderSmoke.update();
//    contentShaderLines.update();
//    contentPosGhosts.update();
    
    // UPDATE POCKETS
//    pocketZone_1.update();
//    pocketPov_1.update();
    
    
//    syphonOut.begin();
//    {
//        ofClear(0);
//        ofSetColor(255);
//        mixer.draw(0,0,syphonOut.getWidth(), syphonOut.getHeight());
//    }
//    syphonOut.end();
    
    
//    // looking for artnet devices
//    if (artNode.readyFps(0.2)) { // send poll all 5 sec
//        artNode.sendPoll();
//        artNode.clearNodes();
//    }
//    artNode.update();
    
    if(camPresetIndx>=camPresets.size()){
        int index = camPresetIndx-camPresets.size();
        camera.setGlobalPosition(syphonInLayers[index].camera.getGlobalPosition());
        camera.lookAt(center);//syphonInLayers[index].camera.getGlobalPosition()+syphonInLayers[index].camera.getLookAtDir());
        camera.setFov(syphonInLayers[index].camera.getFov());
        camPresetName = syphonInLayers[index].name;
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    simulationFBO.begin();
    {
        camera.begin();
        {
            ofBackground(10);
            
            // Grid
            if(drawFloor){
                ofSetColor(20);
                ofPushMatrix();
                ofRotateXDeg(90);
                ofDrawPlane(0, 0, 1000, 1000);
                ofPopMatrix();
            }
            
            
            // Draw gates
            if(drawGates){
                syphonIn.getTexture().bind();
                {
                    for(auto& g : gates){
                        g.drawMeshLed();
                    }
                }
                syphonIn.getTexture().unbind();
                
                for(auto& g : gates){
                    g.drawMeshProfile();
                }
            }
            if(drawOrbiter&&camPresetIndx<camPresets.size()){
                for(int i = 0; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
                    syphonInLayers[i].camera.draw();
                }
            }
        }
        camera.end();
    }
    simulationFBO.end();
    ofSetColor(255);
    simulationFBO.draw(0,0);
    
    // Draw Syphon
    if(drawSyphon){
        for(int i = 0; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
            syphonInLayers[i].drawInputPreview(220, 10+i*160, 150, 150);
            ofDrawBitmapString(syphonInLayers[i].name, 220, 20+i*160);

        }
    }
    
    // Draw GUI
    drawGUI();
    
//    gates[gateInfoIndx].showFixtureInfo();
    
//    string info;
//    info += "NumberNodes  : " + ofToString(artNode.getNumNodes());
//    if(artNode.getNumNodes() > 0){
//        info += "\nIP           : " + artNode.getNodeIp(0);
//        auto node = artNode.getNode(0);
//        info += "\nSortName     : " + ofToString(node == NULL ? "-" : node->ShortName);
//        info += "\nStartChannel : " + ofToString(node == NULL ? "-" : node->LongName);
//        info += "\nStartAddress : " + ofToString(node == NULL ? 9999 : node->getPortAddress(0));
//    }
//    ofDrawBitmapStringHighlight(info, 345, ofGetHeight()-4*15);

    
    // PUBLISH OUTPUT
//    syphonOut.publish();
//    syphonSimOut.publish();
    //syphonLayerPreview.publish();
    
    for(int i = 0; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
        syphonOutLayers[i].publishTexture(&syphonInLayers[i].getTexture());
    }
}

//--------------------------------------------------------------
void ofApp::setupParameterGroup(){
    paramGroup.setName("VIEWs");
    paramGroup.add(drawFloor.set("draw floor", true));
    paramGroup.add(drawGates.set("draw gates", true));
    paramGroup.add(drawSyphon.set("draw syphon in", true));
    paramGroup.add(drawOrbiter.set("drawOrbiterCams", true));
    
    
//    paramsWekinator.setName("WekinatorInputs");
//    paramsWekinator.add(in_1.set("wekIn_1", 0., 0., 1.));
//    paramsWekinator.add(in_2.set("wekIn_2", 0., 0., 1.));
//    paramsWekinator.add(in_3.set("wekIn_3", 0., 0., 1.));
//    paramsWekinator.add(in_4.set("wekIn_4", 0., 0., 1.));
}

//--------------------------------------------------------------
void ofApp::drawGUI(){
    if(!hideGui){
        guiGeneral.draw();
        guiControls.setPosition(guiGeneral.getPosition().x, guiGeneral.getPosition().y + guiGeneral.getHeight() + 15);
        guiControls.draw();
//        guiWekinator.setPosition(guiControls.getPosition().x, guiControls.getPosition().y + guiControls.getHeight() + 15);
//        guiWekinator.draw();
        
        //right side
        guiMixer.setPosition(ofGetWidth()-guiMixer.getWidth()-15, 15);
        guiMixer.draw();

        string info;
        info += "FPS: " + ofToString(ofGetFrameRate());
//        info += "\nPOV: " + ofToString(syphonInLayer_1.pov.getPosition());
        info += "\nCam: " + ofToString(camera.getGlobalPosition());
        info += "\nCam: " + ofToString(camPresetName);
        ofDrawBitmapStringHighlight(info, 15, ofGetHeight()-4*15);
        
        
        // SyphonInput
        if(drawSyphon){
            syphonIn.draw(ofGetWidth()-syphonIn.getWidth()-5, 5);
        }
        
        string infoSyphon;
        infoSyphon += "SyphonIn Simulation";
        infoSyphon += "\nApp    : " + syphonIn.getApplicationName();
        infoSyphon += "\nServer : " + syphonIn.getServerName();
        ofDrawBitmapStringHighlight(infoSyphon, ofGetWidth()-160, ofGetHeight()-4*15);
        

    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'g' || key == 'G'){
        hideGui = !hideGui;
    }
    
//    if(key == '1'){
//        if(!wekinator.bIsRunning) {
//            wekinator.startRunning();
//        }else if(wekinator.bIsRunning) {
//            wekinator.stopRunning();
//        }
//    }
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
    
//    if(key == 'm'){
//        mappingIndx++;
//        mappingIndx = mappingIndx%4;
//    }

    if(key == 'i' || key == 's'){
        syphonIn.next();
    }

    if(key == 'c'){
        camPresetIndx++;
        camPresetIndx = camPresetIndx%(camPresets.size()+NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED);
        if(camPresetIndx<camPresets.size()){
        camera.setFov(70);
        camera.setGlobalPosition(camPresets[camPresetIndx].pos );
        camera.lookAt(center);
            camPresetName = camPresets[camPresetIndx].name;
        }
        
    }
//    if(key == 'x'){
//        syphonInLayer_1.setPov(camera);
//    }
    
    
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
    
//    if(key == 'o'){
//        gateInfoIndx--;
//        if(gateInfoIndx<0) gateInfoIndx = 39;
//    }
//    if(key == 'p'){
//        gateInfoIndx++;
//        if(gateInfoIndx>39) gateInfoIndx = 0;
//    }
//    if(key == 'l'){
//        // send new config to connected node
//        if((artNode.getNumNodes()>0) && (artNode.getNumNodes()<2) ){
//            artNode.setNodeAddress(0, gates[gateInfoIndx].fixtures[0].startUniverse, "G_"+ofToString(gateInfoIndx+1), "Gate_"+ofToString(gateInfoIndx+1));
//            artNode.sendPoll();
//            artNode.clearNodes();
//            cout << "readressed node" << endl;
//
//        } else cout << "more than one node connected node" << endl;
//
//
//    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == '2'){
//        wekinator.stopRecording();
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
//    syphonSimOut.fbo.allocate(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
//--------------------------------------------------------------
// Event handlers for syphonDir
void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg){
    for( auto& syphonDir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<syphonDir.serverName <<" | App Name: "<<syphonDir.appName;
    }
};
void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg){
    for( auto& syphonDir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<syphonDir.serverName <<" | App Name: "<<syphonDir.appName;
    }
};


//--------------------------------------------------------------
//void ofApp::receiveFromSensorFuse(){
//    //PARSE OSC
//    while(oscFromSensorFuse.hasWaitingMessages()){
//        // get the next message
//        ofxOscMessage m;
//        oscFromSensorFuse.getNextMessage(m);
//        
//        std::vector<std::string> address = ofSplitString(m.getAddress(),"/",true);
//        
//        
//        // check for mouse moved message
//        if(address[0] == "Gate"){
////            contentGate.activate(ofToInt(address[1]));
//            
////            pocketZone_1.gateActivated(ofToInt(address[1]));
//            
//        }else if(address[0] == "User"){
//            users[ofToInt(address[1])].updateValues(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
//            
//            //cout << "User #" << address[1] << " pos " << m.getArgAsFloat(0) << " life " << m.getArgAsFloat(1) << " vel " << m.getArgAsFloat(2) << endl;
////            if( pocketPov_1.getMinLifespan() < m.getArgAsFloat(1) ){
////                pocketPov_1.setUser(&users[ ofToInt(address[1]) ]);
////            }
//        }else if(address[0] == "soundObject"){
//            // Do something with SoundObjects id = address[1]
////            contentPosGhosts.updatePosition(ofToInt(address[1]), ofVec2f(m.getArgAsFloat(0), m.getArgAsFloat(1)));
//            
//            
//        }else{
//            // unrecognized message: display on the bottom of the screen
//            string msg_string;
//            msg_string = m.getAddress();
//            msg_string += ": ";
//            for(int i = 0; i < m.getNumArgs(); i++){
//                // get the argument type
//                msg_string += m.getArgTypeName(i);
//                msg_string += ":";
//                // display the argument - make sure we get the right type
//                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
//                    msg_string += ofToString(m.getArgAsInt32(i));
//                }
//                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
//                    msg_string += ofToString(m.getArgAsFloat(i));
//                }
//                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
//                    msg_string += m.getArgAsString(i);
//                }
//                else{
//                    msg_string += "unknown";
//                }
//            }
//            cout << msg_string << endl;
//        }
//        
//    }
//    
//    
//}
void ofApp::receiveLayerControl(){
    while(oscLayerControl.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscLayerControl.getNextMessage(m);
        
        std::vector<std::string> address = ofSplitString(m.getAddress(),"/",true);
        
        for(int i = 0; i < NUMOFLAYERS_ORBIT; i++){
            if(address[0] == "Layer_"+ofToString(i+1)){
                if(address[1] == "speed") syphonInLayers[i].camera.setSpeed(m.getArgAsFloat(0));
                if(address[1] == "viewerHeight") syphonInLayers[i].camera.setViewerHeight(m.getArgAsFloat(0));
                if(address[1] == "maxOrbitX") syphonInLayers[i].camera.setMaxOrbitCamX(m.getArgAsFloat(0));
                if(address[1] == "maxOrbitZ") syphonInLayers[i].camera.setMaxOrbitCamZ(m.getArgAsFloat(0));
                if(address[1] == "angle") syphonInLayers[i].camera.setAngle(m.getArgAsFloat(0));
                if(address[1] == "center") syphonInLayers[i].camera.setCenter(glm::vec3(m.getArgAsFloat(0),m.getArgAsFloat(1),m.getArgAsFloat(2)));
                return;
            }
        }
    }
}
