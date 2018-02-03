#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(80);
    ofEnableSmoothing();
    
    
    // Add gates
    for(int i = 0; i < 40; i++){
        Gate gate = Gate(glm::vec3(0,0, i*2), i);
        gates.push_back(gate);
    }
    
    syphonDir.setup();
    ofAddListener(syphonDir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(syphonDir.events.serverRetired, this, &ofApp::serverRetired);
    
    for(int i = 0; i < NUMOFLAYERS_ORBIT; i++){
        syphonInLayers[i].setup("syphonIn_"+ofToString(i+1),&gates, &syphonDir, "POV"+ofToString(i+1), "MadMapper", POV_UV_NORMALIZED, ORBIT_CAM);
    }
    for(int i = NUMOFLAYERS_ORBIT; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
        syphonInLayers[i].setup("syphonIn_"+ofToString(i+1),&gates, &syphonDir, "POV"+ofToString(i+1), "MadMapper", POV_UV_NORMALIZED, FIXED_CAM);
    }
    
    for(int i = 0; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
        syphonOutLayers[i].setName("syphonIn_"+ofToString(i+1));
    }
    
    syphonIn.setup("MASTER", "MadMapper");
    

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
    
  
    setupParameterGroup();
    guiGroup.setName("General");
    guiGroup.add(paramGroup);
    
    paramsControls.setName("ContentControls");
    
    for(int i = 0; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
        paramsControls.add(syphonInLayers[i].getParameterGroup());
    }
    
    guiControls.setup( paramsControls );
    guiGeneral.setup(guiGroup);
    guiGeneral.loadFromFile("settingsGeneral.xml");
    guiControls.loadFromFile("settingsControls.xml");
   
    oscLayerControl.setup(49166);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
    receiveLayerControl();
    
    for(int i = 0; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
        syphonInLayers[i].update();
    }
    
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

    
    // Draw Syphon
    if(drawSyphon){
        for(int i = 0; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
            syphonInLayers[i].drawInputPreview(220, 10+i*160, 150, 150);
            ofSetColor(255);
            string infoSyphon;
            infoSyphon += syphonInLayers[i].name;
            infoSyphon += "\n" + syphonInLayers[i].syphonClient.getApplicationName();
            infoSyphon += "\n" + syphonInLayers[i].syphonClient.getServerName();
            ofDrawBitmapString(infoSyphon, 220, 20+i*160);

        }
    }
    
    // Draw GUI
    drawGUI();
 
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
}

//--------------------------------------------------------------
void ofApp::drawGUI(){
    if(!hideGui){
        guiGeneral.draw();
        guiControls.setPosition(guiGeneral.getPosition().x, guiGeneral.getPosition().y + guiGeneral.getHeight() + 15);
        guiControls.draw();

        string info;
        info += "FPS: " + ofToString(ofGetFrameRate());
        info += "\nCam: " + ofToString(camera.getGlobalPosition());
        info += "\nCam: " + ofToString(camPresetName);
        ofDrawBitmapStringHighlight(info, 15, ofGetHeight()-4*15);
        
        
        // SyphonInput
        if(drawSyphon){
            syphonIn.draw(ofGetWidth()-syphonIn.getWidth()-10, 10);
        }
        
        string infoSyphon;
        infoSyphon += "Simulation";
        infoSyphon += "\n" + syphonIn.getApplicationName();
        infoSyphon += "\n" + syphonIn.getServerName();
        ofDrawBitmapString(infoSyphon, ofGetWidth()-syphonIn.getWidth()-10, 20);
        

    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'g' || key == 'G'){
        hideGui = !hideGui;
    }
    
    if(key >= '1' && key <= '9'){
        int index = key-48;
        if(index<camPresets.size()+NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED){
            camPresetIndx = index;
            if(camPresetIndx<camPresets.size()){
                camera.setFov(70);
                camera.setGlobalPosition(camPresets[camPresetIndx].pos );
                camera.lookAt(center);
                camPresetName = camPresets[camPresetIndx].name;
            }
        }
    }

    if(key == 'u'){
        syphonIn.next();
    }
    if(key == 'i'){
        syphonInLayers[0].next();
    }
    if(key == 'o'){
        syphonInLayers[1].next();
    }
    if(key == 'p'){
        syphonInLayers[2].next();
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
    
    if(key == 'l'){
        guiGeneral.loadFromFile("settingsGeneral.xml");
        guiControls.loadFromFile("settingsControls.xml");
    }
    if(key == 's'){
        guiGeneral.saveToFile("settingsGeneral.xml");
        guiControls.saveToFile("settingsControls.xml");
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

void ofApp::receiveLayerControl(){
    while(oscLayerControl.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscLayerControl.getNextMessage(m);
        
        std::vector<std::string> address = ofSplitString(m.getAddress(),"/",true);
        
        for(int i = 0; i < NUMOFLAYERS_ORBIT+NUMOFLAYERS_FIXED; i++){
            if(address[0] == "Layer_"+ofToString(i+1)){
                if(address[1] == "speed") syphonInLayers[i].camera.setSpeed(m.getArgAsFloat(0));
                if(address[1] == "viewerHeight") syphonInLayers[i].camera.setViewerHeight(m.getArgAsFloat(0));
                if(address[1] == "maxOrbitX") syphonInLayers[i].camera.setMaxOrbitCamX(m.getArgAsFloat(0));
                if(address[1] == "maxOrbitZ") syphonInLayers[i].camera.setMaxOrbitCamZ(m.getArgAsFloat(0));
                if(address[1] == "angle") syphonInLayers[i].camera.setAngle(m.getArgAsFloat(0));
                if(address[1] == "center" || address[1] == "lookAt"){
                    if(!(address.size()>2)){
                        syphonInLayers[i].camera.setLookAt(glm::vec3(m.getArgAsFloat(0),m.getArgAsFloat(1),m.getArgAsFloat(2)));
                    } else if( address[2] == "x" ){
                        syphonInLayers[i].camera.lookAtX = m.getArgAsFloat(0);
                    } else if( address[2] == "y" ){
                        syphonInLayers[i].camera.lookAtY = m.getArgAsFloat(0);
                    } else if( address[2] == "z" ){
                        syphonInLayers[i].camera.lookAtZ = m.getArgAsFloat(0);
                    }
                }
                if(address[1] == "position"){
                    if(!(address.size()>2)){
                        syphonInLayers[i].camera.setCamPos(glm::vec3(m.getArgAsFloat(0),m.getArgAsFloat(1),m.getArgAsFloat(2)));
                    } else if( address[2] == "x" ){
                        syphonInLayers[i].camera.posX = m.getArgAsFloat(0);
                    } else if( address[2] == "y" ){
                        syphonInLayers[i].camera.posY = m.getArgAsFloat(0);
                    } else if( address[2] == "z" ){
                        syphonInLayers[i].camera.posZ = m.getArgAsFloat(0);
                    }
                }
                if(address[1] == "fov") syphonInLayers[i].camera.setFov(m.getArgAsFloat(0));
            }
        }
    }
}
