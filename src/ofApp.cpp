#include "ofApp.h"
ofVec3f viewDirection;
//--------------------------------------------------------------
void ofApp::setup(){
    setupGUI();
    
    ofBackground(80);
    ofEnableSmoothing();
    camera.setCursorDrawEnabled(true);
    camera.setGlobalPosition(31.3198, 28.45, -37.9426);
    
    // Setup plane
    pov = POV();
    
    // Add gates
    for(int i = 0; i < 40; i++){
        gates.push_back(Gate(ofVec3f(0,3.6, i*2), &pov, i));
    }
    
    // Set start & end points fro pov direction
    pStartLeft  =  &gates.front().edges.at(1).pos;
    pStartRight =  &gates.front().edges.at(2).pos;
    pEndLeft  =  &gates.back().edges.at(1).pos;
    pEndRight =  &gates.back().edges.at(2).pos;
    
    // some def
    pMaxLeft = pStartRight;
    pMaxRight = pEndRight;
    
    
    // Setup Syphon in
    dir.setup();
    client.setup();
    
    //register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);
    
    dirIdx = -1;

}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
   
    
    // calcualte the direction of the pov
    findMaxPoints();
    viewDirection = (pMaxLeft->operator-(pov.position)).getNormalized() + (pMaxRight->operator-(pov.position)).getNormalized();
    viewDirection.normalize();
    
    // POV
    if(povOrbit){
        pov.orbit(center);
    }

    pov.update(viewDirection);

    
    if(pov.hasMoved()){     // Only update if POV is moved
        
        // Gates
        for(auto& g : gates){
            g.update();
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    camera.begin();
    
    
    // Set pov camera
    if(povCamera){
        camera.setGlobalPosition(pov.position);
        camera.lookAt(pov.position+viewDirection);
    }
    
    // Grid
    if(drawGrid){
        ofPushMatrix();
        ofDrawGrid(1.0f, 180.0f, showGridLabels, false, true, false);
        ofPopMatrix();
    }
    
    // POV
    if(drawPlane && !povCamera){
        pov.drawPovPoint();
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
        client.getTexture().bind();
        for(auto& g : gates){
            g.drawMesh();
        }
        client.getTexture().unbind();
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
    
    
    camera.end();
    
    // Draw Syphon
    if(drawSyphon){
        ofPushMatrix();
        {
            float s = 0.2;
            ofTranslate(0, ofGetHeight()-client.getHeight()*s);
            ofScale(s,s, 0);
            
            drawSyphonIn();
            
            ofSetColor(ofColor::orange);
            for(auto& g : gates){
                for(auto& e : g.edges){

                    float px = ofMap(e.uv.x, 0, 1, 0, client.getWidth());
                    float py = ofMap(e.uv.y, 0, 1, 0, client.getHeight());
                    ofDrawCircle(px, py, 10);
                }
            }
        }
        ofPopMatrix();
    }
    
    // Draw GUI
    drawGUI();
}

//--------------------------------------------------------------
void ofApp::setupGUI(){
    gui.setup();
    gui.add(drawGrid.set("draw grid", true));
    gui.add(showGridLabels.set("Show grid labels: ", false));
    gui.add(drawGates.set("draw gates", true));
    gui.add(drawRays.set("draw rays", true));
    gui.add(drawPlane.set("draw plane", true));
    gui.add(drawSyphon.set("draw syphon in", true));
    gui.add(povCamera.set("point of view", false));
    gui.add(povOrbit.set("pov orbit", false));
}

//--------------------------------------------------------------
void ofApp::drawGUI(){
    if(!hideGui){
        gui.draw();
        string info;
        info += "FPS: " + ofToString(ofGetFrameRate());
        info += "\nPOV: " + ofToString(pov.position);
        info += "\nCam: " + ofToString(camera.getGlobalPosition());
        ofDrawBitmapStringHighlight(info, 15, gui.getHeight()+25);
    }
}

//--------------------------------------------------------------
void ofApp::drawSyphonIn(){

        if(dir.isValidIndex(dirIdx)){
            ofSetColor(255);
            ofFill();
            client.draw(0, 0);
        }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f' || key == 'F'){
        ofToggleFullscreen();
    }
    
    if(key == 'p' || key == 'P'){
        pov.position = camera.getCursorWorld();
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
    
    if(key == 's'){
        //press any key to move through all available Syphon servers
        if (dir.size() > 0)
        {
            dirIdx++;
            if(dirIdx > dir.size() - 1)
                dirIdx = 0;
            
            client.set(dir.getDescription(dirIdx));
            string serverName = client.getServerName();
            string appName = client.getApplicationName();
            
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
    if(pov.position.z < pStartRight->z){
        if(pov.position.x > pStartRight->x){
            // in front on the right of the installation
            pMaxLeft = pStartLeft;
            pMaxRight = pEndRight;
        }
        else if(pov.position.x < pStartRight->x && pov.position.x > pStartLeft->x){
            // in front in the middle of the installation
            pMaxLeft = pStartLeft;
            pMaxRight = pStartRight;
        }
        else if(pov.position.x < pStartLeft->x){
            // in front on the left of the installation
            pMaxLeft = pEndLeft;
            pMaxRight = pStartRight;
        }
    }
    else if(pov.position.z > pStartRight->z && pov.position.z < pEndRight->z){
        if(pov.position.x > pStartRight->x){
            // on the right of the installation
            pMaxLeft = pStartRight;
            pMaxRight = pEndRight;
        }
        else if(pov.position.x < pStartRight->x && pov.position.x > pStartLeft->x){
            // inside the installation
            // HERE WE NEED AN OTHER MODUS -> SPHERICAL MAPPING OF UVs
        }
        else if(pov.position.x < pStartLeft->x){
            // on the left of the installation
            pMaxLeft = pEndLeft;
            pMaxRight = pStartLeft;
        }
    }
    else if(pov.position.z > pEndRight->z){
        if(pov.position.x > pStartRight->x){
            // behind on the right of the installation
            pMaxLeft = pStartRight;
            pMaxRight = pEndLeft;
        }
        else if(pov.position.x < pStartRight->x && pov.position.x > pStartLeft->x){
            // behind in the middle of the installation
            pMaxLeft = pEndRight;
            pMaxRight = pEndLeft;
        }
        else if(pov.position.x < pStartLeft->x){
            // behind on the left of the installation
            pMaxLeft = pEndRight;
            pMaxRight = pStartLeft;
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


