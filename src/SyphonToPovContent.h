//
//  SyphonToPovContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 09/01/2018.
//
//

#pragma once

#include "TextureToPovContent.h"
#include "ofxSyphon.h"

#include "CameraController.h"




class SyphonToPovContent: public TextureToPovContent{
public:
    
    ofxSyphonClient syphonClient;
    ofxSyphonServerDirectory *dir;

    ofParameter<int> inputSelect;
    ofParameter<int> mappingMode;

    
    ofParameter<float> speed;
    ofParameter<int> imageIndex;
    
    CameraController camera;
    
    glm::vec3 camPos;
    
    int cameraMode;
    
    SyphonToPovContent(){}
    
    void setup(string name, vector<Gate> * gates, ofxSyphonServerDirectory *dir, string serverName, string appName, int initialMappingMode, int cameraMode){
        
        this->dir = dir;
        

        syphonClient.setup();
        if(!syphonClient.isSetup()) cout << "isSetup";
        

        inputSelect.set("input", find(serverName, appName), 0, dir->size()-1);
        
        setupParameterGroup(name, initialMappingMode);
        
        camera.setup(name, cameraMode);
        
        TextureToPovContent::setup(name, gates, camera.getPosition(), &syphonClient.getTexture(), initialMappingMode);
        
        
        ofAddListener(parameterGroup.parameterChangedE(), this, &SyphonToPovContent::inputChange);
  
        ofAddListener(dir->events.serverAnnounced, this, &SyphonToPovContent::serverAnnounced);
        ofAddListener(dir->events.serverRetired, this, &SyphonToPovContent::serverRetired);


    }
    
    void update(){
        if(!syphonClient.isSetup()){
            cout << "got here";
            return;
        }
        camera.update();
        camPos = camera.getPosition();
        
        pov = camera;

        if(isVisible && alpha < 1.) alpha += 0.1;
        else if( !isVisible && alpha > 0) alpha -= 0.01;
        
        // Calculate the UV points
        for(auto& e : edges){
            glm::vec3 coord = pov.worldToScreen(e.pos);
            coord.x /= ofGetWidth();
            coord.y /= ofGetHeight();
            if(glm::dot( e.pos-pov.getPosition(), pov.getLookAtDir() ) > 0 ) coord.y = 1.-coord.y;
            e.uv = coord;
        }
        
        calcMapping();
        
        fbo.begin();
        {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            syphonClient.bind();{
                ofSetColor(255, alpha*255);
                mesh.draw();
            }
            syphonClient.unbind();
            
        }
        fbo.end();
    }
    
    int find(string serverName, string appName){
        
        for( int i = 0; i < dir->size(); i++){
           ofxSyphonServerDescription server = dir->getDescription(i);
            if(appName == server.appName && serverName == server.serverName){
                syphonClient.set(server);
                int indx = i;
                ofLogNotice("ofxSyphonServerDirectory found: "+serverName+" | "+appName);
                i = dir->size(); // jump out of the loop
                inputSelect = indx;
                return indx;
            }
        }
        ofLogNotice("ofxSyphonServerDirectory coudn't find: "+serverName+" | "+appName);
        if(dir->size()>0) return 0;
        return -1;
        
        
    }
    
    void next(){
//        if (dir.size() > 0)
//        {
//            dirIdx++;
//            if(dirIdx > dir.size() - 1)
//                dirIdx = 0;
//
//            set(dir.getDescription(dirIdx));
//        }
        
        int index = inputSelect;
        index++;
        index = index%(inputSelect.getMax()+1);
        inputSelect = index;
        syphonClient.set(dir->getDescription(inputSelect));
        setMappingMode(mappingMode);
    }
    
    void setupParameterGroup(string name, int mapping){
    
        parameterGroup.setName(name);
       // parameterGroup.add(inputSelect.set("input", dirIndx, 0, dir->size()-1));
        parameterGroup.add(mappingMode.set("mappingMode", mapping, 0, 3));

  //      parameterGroup.add(speed.set("speed", 0.01, 0., 0.2));
//        parameterGroup.add(mode.set("mode", 1, 1, 2));
//        parameterGroup.add(easing.set("easing", 1, 1, 2));
//        parameterGroup.add(imageIndex.set("imageIndex", 1, 1, image->size()));
        parameterGroup.add(camera.getParameterGroup());
    }
    
    
    // Event handlers
    void inputChange(ofAbstractParameter& e){
        syphonClient.set(dir->getDescription(inputSelect));
        setMappingMode(mappingMode);
    }
    
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg){
//        for( auto& dir : arg.servers ){
//            ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
//        }
        if(inputSelect>dir->size()) inputSelect = dir->size()-1;
        inputSelect.setMax(dir->size()-1);
        
    };
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg){
//        for( auto& dir : arg.servers ){
//            ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
//        }
        if(inputSelect>dir->size()) inputSelect = dir->size()-1;
        inputSelect.setMax(dir->size()-1);
    };
    

    
    

};

