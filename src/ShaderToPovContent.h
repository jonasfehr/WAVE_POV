//
//  ShaderToPovContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 09/01/2018.
//
//

#pragma once

#include "TextureToPovContent.h"
#include "ofxAutoReloadedShader.h"

class ShaderToPovContent: public TextureToPovContent{
public:
    
    ofFbo fboShader;
    ofxAutoReloadedShader shader;
    
    
    
    ShaderToPovContent(){}
    
    void setup(string name, vector<Gate> * gates, glm::vec3 povPosition, string shaderName, glm::vec2 shaderSize, int initialMappingMode){
        

        this->shader.load(shaderName);
        
        fboShader.allocate(shaderSize.x, shaderSize.y, GL_RGB);
        fboShader.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fboShader.end();
        
        TextureToPovContent::setup(name, gates, povPosition, &fboShader.getTexture(), initialMappingMode);
    }
    
    void update(){
        if(isVisible && alpha < 1.) alpha += 0.1;
        else if( !isVisible && alpha > 0) alpha -= 0.01;
        
        // Calculate the UV points
        for(auto& e : edges){
            glm::vec3 coord = pov.worldToScreen(e.pos);
            coord.x /= ofGetWidth();
            coord.y /= ofGetHeight();
            if(glm::dot( e.pos-pov.getPosition(), pov.getLookAtDir() ) > 0 ) coord.y = 1-coord.y;
            e.uv = coord;
        }
        
        calcMapping();
        
        fbo.begin();
        {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
           
            fboShader.begin();{
                glClearColor(0.0, 0.0, 0.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                shader.begin();{
                    shader.setUniform2f("iResolution", fboShader.getWidth(), fboShader.getHeight());
                    shader.setUniform1f("iGlobalTime",     ofGetElapsedTimef() ) ;//counter);
                    ofSetColor(255,255,255);
                    ofFill();
                    ofDrawRectangle(0, 0, fboShader.getWidth(), fboShader.getHeight());
                }
                shader.end();
            }
            fboShader.end();
            
            texture->bind();{
                ofSetColor(255, alpha*255);
                mesh.draw();
            }
            texture->unbind();

        }
        fbo.end();
    }
};


