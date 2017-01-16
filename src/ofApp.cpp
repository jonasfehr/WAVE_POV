#include "ofApp.h"
ofVec3f viewDirection;
//--------------------------------------------------------------
void ofApp::setup(){
    setupGUI();
    
    ofBackground(80);
    ofEnableSmoothing();
    camera.setCursorDrawEnabled(true);
    camera.setGlobalPosition(31.3198, 28.45, -37.9426);
    camera.setFov(70);
    
    // Setup plane
    pov = POV();
    
    // Add gates
    for(int i = 0; i < 40; i++){
        gates.push_back(Gate(ofVec3f(0,0, i*2), &pov, i));
    }
    
    // Set start & end points to calculate pov direction
    eStartLeft  =  &gates.front().edges.at(1);
    eStartRight =  &gates.front().edges.at(3);
    eEndLeft  =  &gates.back().edges.at(1);
    eEndRight =  &gates.back().edges.at(3);
    eTopStart = &gates.front().edges.at(2);
    eTopEnd = &gates.back().edges.at(2);
    // some def
    eMaxLeft = eStartRight;
    eMaxRight = eEndRight;
    
    
    // Setup Syphon in
    dir.setup();
    sClient.setup();
    
    //register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);
    
    dirIdx = -1;
    
    outputFbo.allocate(40, 1300);
    outputFbo.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    outputFbo.end();
    sServer.setName("WaveForMapping");

    createOutputMesh();
    
    // create presets for camera
    CameraPos camPos;
    camPos.name = "in front";
    camPos.pos = ofVec3f(0, VIEWER_HEIGHT, -2);
    camPresets.push_back(camPos);
    
    camPos.name = "inside";
    camPos.pos = ofVec3f(0, VIEWER_HEIGHT, 30);
    camPresets.push_back(camPos);
    
    camPos.name = "from side";
    camPos.pos = ofVec3f(10, VIEWER_HEIGHT, 40);
    camPresets.push_back(camPos);
    
    camPos.name = "from back";
    camPos.pos = ofVec3f(0, VIEWER_HEIGHT, 82);
    camPresets.push_back(camPos);
    
    camPos.name = "off side";
    camPos.pos = ofVec3f(10, VIEWER_HEIGHT, -10);
    camPresets.push_back(camPos);


}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
   
    
    // calcualte the direction of the pov
    findMaxPoints();
    ofVec3f pLeft = eMaxLeft->pos;
    ofVec3f pRight = eMaxRight->pos;
    pLeft.y = pRight.y = VIEWER_HEIGHT;
    
    viewDirection = (pLeft.operator-(pov.position)).getNormalized() + (pRight.operator-(pov.position)).getNormalized();
    viewDirection.normalize();

    pov.update(viewDirection);

    // Gates
    for(auto& g : gates){
        g.update();
    }
    vector<ofVec2f> texCoords;
    
    switch(mappingIndx){
        case 0:
            for(auto& g : gates){
                for(auto& e : g.edges){
                    texCoords.push_back(ofVec2f(e.uv.x*sClient.getWidth(),(-e.uv.y+1)*sClient.getHeight()));
                }
            }
            break;
            
        case 1:
            
            for(auto& g : gates){
                for(auto& e : g.edges){
                    ofVec2f uv = e.uv;
                    uv.x = ofMap(uv.x, eMaxLeft->uv.x, eMaxRight->uv.x, 0, 1.);
                    if(eTopStart->uv.y < eTopEnd->uv.y ){
                        if(eMaxLeft->uv.y > eMaxRight->uv.y ){
                            uv.y = ofMap(uv.y, eMaxLeft->uv.y, eTopStart->uv.y, 0, 1);
                        } else {
                            uv.y = ofMap(uv.y, eMaxRight->uv.y, eTopStart->uv.y, 0, 1);
                        }
                    } else {
                        if(eMaxLeft->uv.y > eMaxRight->uv.y ){
                            uv.y = ofMap(uv.y, eMaxLeft->uv.y, eTopEnd->uv.y, 0, 1);
                        } else {
                            uv.y = ofMap(uv.y, eMaxRight->uv.y, eTopEnd->uv.y, 0, 1);
                        }                    }
                    texCoords.push_back(ofVec2f(uv.x*sClient.getWidth(),uv.y*sClient.getHeight()));
                }
            }
            break;
            
        case 2:
            float dist = sClient.getWidth()/39;
            for(int i = 0; i < 40; i++){
                texCoords.push_back(ofVec2f(i*dist,sClient.getHeight()*145/1590));
                texCoords.push_back(ofVec2f(i*dist,sClient.getHeight()*(145+120)/1590));
                texCoords.push_back(ofVec2f(i*dist,sClient.getHeight()*(145+120+530)/1590));
                texCoords.push_back(ofVec2f(i*dist,sClient.getHeight()*(145+120+530+530)/1590));
                texCoords.push_back(ofVec2f(i*dist,sClient.getHeight()*(145+120+530+530+120)/1590));
            }
            break;
    }
    
    
        outputMesh.clearTexCoords();
        outputMesh.addTexCoords(texCoords);
    
    
    // Create output to be mapped
        outputFbo.begin();
        {
            //ofDisableArbTex();
            sClient.getTexture().bind();
            {
                ofSetColor(255);
                outputMesh.draw();
            }
            sClient.getTexture().unbind();
        }
        outputFbo.end();
  //  }
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
            pov.drawPlane();
        }
        
        // Draw gates
        if(drawGates){
            for(auto& g : gates){
                //     g.draw();
            }
            outputFbo.getTexture().bind();
            for(auto& g : gates){
                g.drawMeshLed();
            }
            outputFbo.getTexture().unbind();
            for(auto& g : gates){
                g.drawMeshProfile();
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
        
        
    }
    camera.end();
    
    // Draw Syphon
    if(drawSyphon){
        ofPushMatrix();
        {
            float s = 0.2;
            ofTranslate(0, ofGetHeight()-sClient.getHeight()*s);
            ofScale(s,s, 0);
            
            drawSyphonIn();
            
            ofSetColor(ofColor::orange);
            vector<ofVec2f> pos = outputMesh.getTexCoords();
            for(auto& p : pos){
                ofDrawCircle(p.x, -p.y+sClient.getHeight(), 10);
            }
        }
        ofPopMatrix();
    }
    
    // Draw GUI
    drawGUI();
    
    
    // PUBLISH OUTPUT
    ofFill();
    sServer.publishTexture(&outputFbo.getTexture());
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
        info += "\nPOV: " + ofToString(pov.position);
        info += "\nCam: " + ofToString(camera.getGlobalPosition());
        info += "\nCam: " + ofToString(camPresets[camPresetIndx].name);
        ofDrawBitmapStringHighlight(info, 15, gui.getHeight()+25);
    }
}

//--------------------------------------------------------------
void ofApp::drawSyphonIn(){

        if(dir.isValidIndex(dirIdx)){
            ofSetColor(255);
            ofFill();
            sClient.draw(0, 0);
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
    }
    
    if(key == 'c'){
        camPresetIndx++;
        camPresetIndx = camPresetIndx%camPresets.size();
        camera.setGlobalPosition(camPresets[camPresetIndx].pos );
        camera.lookAt(center);
    }
    if(key == 'x'){
        pov.position = camera.getPosition();
    }
    
    if(key == 's'){
        //press any key to move through all available Syphon servers
        if (dir.size() > 0)
        {
            dirIdx++;
            if(dirIdx > dir.size() - 1)
                dirIdx = 0;
            
            sClient.set(dir.getDescription(dirIdx));
            string serverName = sClient.getServerName();
            string appName = sClient.getApplicationName();
            
            if(serverName == ""){
                serverName = "null";
            }
            if(appName == ""){
                appName = "null";
            }
            ofSetWindowTitle(serverName + ":" + appName);
        }
        else
        {
            ofSetWindowTitle("No Server");
        }

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
void ofApp::findMaxPoints(){
    
    // Get maxLeft and maxRight to calculate pov direction
    if(pov.position.z < eStartRight->pos.z){
        if(pov.position.x > eStartRight->pos.x){
            // in front on the right of the installation
            eMaxLeft = eStartLeft;
            eMaxRight = eEndRight;
        }
        else if(pov.position.x < eStartRight->pos.x && pov.position.x > eStartLeft->pos.x){
            // in front in the middle of the installation
            eMaxLeft = eStartLeft;
            eMaxRight = eStartRight;
        }
        else if(pov.position.x < eStartLeft->pos.x){
            // in front on the left of the installation
            eMaxLeft = eEndLeft;
            eMaxRight = eStartRight;
        }
    }
    else if(pov.position.z > eStartRight->pos.z && pov.position.z < eEndRight->pos.z){
        if(pov.position.x > eStartRight->pos.x){
            // on the right of the installation
            eMaxLeft = eStartRight;
            eMaxRight = eEndRight;
        }
        else if(pov.position.x < eStartRight->pos.x && pov.position.x > eStartLeft->pos.x){
            // inside the installation
            // HERE WE NEED AN OTHER MODUS -> SPHERICAL MAPPING OF UVs
        }
        else if(pov.position.x < eStartLeft->pos.x){
            // on the left of the installation
            eMaxLeft = eEndLeft;
            eMaxRight = eStartLeft;
        }
    }
    else if(pov.position.z > eEndRight->pos.z){
        if(pov.position.x > eStartRight->pos.x){
            // behind on the right of the installation
            eMaxLeft = eStartRight;
            eMaxRight = eEndLeft;
        }
        else if(pov.position.x < eStartRight->pos.x && pov.position.x > eStartLeft->pos.x){
            // behind in the middle of the installation
            eMaxLeft = eEndRight;
            eMaxRight = eEndLeft;
        }
        else if(pov.position.x < eStartLeft->pos.x){
            // behind on the left of the installation
            eMaxLeft = eEndRight;
            eMaxRight = eStartLeft;
        }
    }

}

//--------------------------------------------------------------
// functions for syphon directory's callbacks
void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

//--------------------------------------------------------------
void ofApp::createOutputMesh(){
    for(int i = 0; i <= 40; i++){
        outputMesh.addVertex(ofVec2f(i,0));
        outputMesh.addVertex(ofVec2f(i,120));
        outputMesh.addVertex(ofVec2f(i,650));
        outputMesh.addVertex(ofVec2f(i,1180));
        outputMesh.addVertex(ofVec2f(i, 1300));
    }
    for(int i = 0; i < 40*5; i+=5){
        outputMesh.addIndex(i+5);
        outputMesh.addIndex(i+0);
        outputMesh.addIndex(i+6);
        outputMesh.addIndex(i+6);
        outputMesh.addIndex(i+0);
        outputMesh.addIndex(i+1);
        
        outputMesh.addIndex(i+6);
        outputMesh.addIndex(i+1);
        outputMesh.addIndex(i+7);
        outputMesh.addIndex(i+7);
        outputMesh.addIndex(i+1);
        outputMesh.addIndex(i+2);
        
        outputMesh.addIndex(i+7);
        outputMesh.addIndex(i+2);
        outputMesh.addIndex(i+8);
        outputMesh.addIndex(i+8);
        outputMesh.addIndex(i+2);
        outputMesh.addIndex(i+3);
        
        outputMesh.addIndex(i+8);
        outputMesh.addIndex(i+3);
        outputMesh.addIndex(i+9);
        outputMesh.addIndex(i+9);
        outputMesh.addIndex(i+3);
        outputMesh.addIndex(i+4);
    }
}

