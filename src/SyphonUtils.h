//
//  SyphonHelp.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 16/01/2017.
//
//
#include "ofxSyphon.h"

#ifndef SyphonHelp_h
#define SyphonHelp_h
class SyphonClientDir : public ofxSyphonClient{
public:
    ofxSyphonServerDirectory dir;
    int dirIdx;
    
    SyphonClientDir(){

    }
    
    void setup(){
        // Setup Syphon in
        dir.setup();
        ofxSyphonClient::setup();
        
        //register for our directory's callbacks
        ofAddListener(dir.events.serverAnnounced, this, &SyphonClientDir::serverAnnounced);
        ofAddListener(dir.events.serverRetired, this, &SyphonClientDir::serverRetired);
        
        dirIdx = -1;
    }
    
    void next(){
        //press any key to move through all available Syphon servers
        if (dir.size() > 0)
        {
            dirIdx++;
            if(dirIdx > dir.size() - 1)
                dirIdx = 0;
            
            set(dir.getDescription(dirIdx));
            string serverName = getServerName();
            string appName = getApplicationName();
            
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
    
    void previous(){
        //press any key to move through all available Syphon servers
        if (dir.size() > 0)
        {
            dirIdx--;
            if(dirIdx < 0)
                dirIdx = dir.size() - 1;
            
            set(dir.getDescription(dirIdx));
            string serverName = getServerName();
            string appName = getApplicationName();
            
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
    
    void draw(){
        if(dir.isValidIndex(dirIdx)){
            ofSetColor(255);
            ofFill();
            ofxSyphonClient::draw(0, 0);
        }
    }
    
    // Event handlers
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg){
        for( auto& dir : arg.servers ){
            ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        }
        dirIdx = 0;
    };
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg){
        for( auto& dir : arg.servers ){
            ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        }
        dirIdx = 0;
    };
};


class SyphonFbo : public ofxSyphonServer{
public:
    ofFbo fbo;
    
    ofMesh outputMesh;
    ofTexture *texture;
    
    SyphonFbo(){
        
    }
    
    void setup(string name){
        fbo.allocate(40, 1300);
        fbo.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fbo.end();
        ofxSyphonServer::setName(name);
        
    }
    
    
    
    void update(){
        fbo.begin();
        {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            texture->bind();
            {
                ofSetColor(255);
                outputMesh.draw();
            }
            texture->unbind();
        }
        fbo.end();
    }
    
    
    void begin(){
        fbo.begin();
    }
    
    void end(){
        fbo.end();
    }

    ofTexture getTexture(){
        return fbo.getTexture();
    }
    
    void publish(){
        ofFill();
        publishTexture(&fbo.getTexture());
    }
    
};


#endif /* SyphonHelp_h */
