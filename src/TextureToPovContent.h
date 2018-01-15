//
//  TextureToPovContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 09/01/2018.
//
//

#pragma once

#include "ofxAutoReloadedShader.h"
#include "ofxGpuMixer.h"
#include "ofxSyphon.h"


enum waveOutputmappingMode : int {
    POV_UV,
    POV_UV_NORMALIZED,
    TUBE,
    UV
};



class TextureToPovContent: public ofx::GpuMixer::BasicChannel{
public:
    
    
    //    ofFbo fbo;
    ofMesh mesh;
    ofTexture *texture;
    ofCamera pov;
    vector<Gate> * gates;
    
    bool isVisible = true;
    float alpha = 1.;
    
    int mappingMode;
    
    
    
    TextureToPovContent(){}
    
    
    void setup(string name, vector<Gate> * gates, glm::vec3 povPosition, ofTexture *texture, int initialMappingMode){
        
        this->name = name;
        this->gates = gates;
        this->mappingMode = initialMappingMode;
        
        // initialise fbo
        fbo.allocate(120, 1300, GL_RGBA32F_ARB);
        fbo.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fbo.end();
        
        createMesh();
        
        // setup POV
        for(auto& gate : *gates){
            addEdge(gate.leftInnerEdge);
            addEdge(gate.leftOuterEdge);
            addEdge(gate.topEdge);
            addEdge(gate.rightOuterEdge);
            addEdge(gate.rightInnerEdge);
        }
        pov.setPosition(povPosition);
        pov.setVFlip(true);
        
        this->texture = texture;
    }
    
    void setMappingMode(int mappingMode){ this->mappingMode = mappingMode; };
    
    void setPov(ofCamera cam){
        pov = cam;
    }
    
    ofCamera* getPovPtr(){ return &pov; }
    
    void setPovPosition(glm::vec3 pos){
        pov.setPosition(pos);
    }
    
    
    void setVisible(){ isVisible = true; }
    void setInvisible(){ isVisible = false; }
    void setInvisible(float alpha){
        isVisible = false;
        alpha = 0;
    }
    
    
    void update(){
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
            
            texture->bind();{
                ofSetColor(255, alpha*255);
                mesh.draw();
            }
            texture->unbind();
            
        }
        fbo.end();
    }
    
    void draw(){
        fbo.draw(0,0, 120, 1300);
    }
    
    void drawInputPreview(int x, int y, int w, int h){
        ofSetColor(255);
        texture->draw(x, y, w, h);
        
                ofPushMatrix();
                {
                    float s = 0.2;
                    ofTranslate(x,y);
                    ofScale(w/texture->getWidth(), h/texture->getHeight(), 0);
        
                    ofSetColor(ofColor::orange);
                    vector<glm::vec2> pos = getTexCoords();
                    for(auto& p : pos){
                        ofDrawCircle(p.x, p.y, 10*w/texture->getWidth());
                    }
                }
                ofPopMatrix();
    }
    
    void setTexCoords(vector<glm::vec2> texCoords){
        mesh.clearTexCoords();
        mesh.addTexCoords(texCoords);
    }
    
    vector<glm::vec2> getTexCoords(){ return mesh.getTexCoords(); }
    
    void setInputTexture(ofTexture *texture){ this->texture = texture;}
    
    void calcMapping(){
        
        vector<glm::vec2> texCoords;
        
        float dist = texture->getWidth()/40;
        switch(mappingMode){
            case POV_UV:
                for(auto& e : edges){
                    texCoords.push_back(glm::vec2(e.uv.x*texture->getWidth(), e.uv.y*texture->getHeight()));
                }
                break;
                
            case POV_UV_NORMALIZED:
                normalizeUV();
                for(auto& e : edges){
                    texCoords.push_back(glm::vec2(e.uv.x*texture->getWidth(), e.uv.y*texture->getHeight()));
                }
                break;
                
            case TUBE:
                for(int i = 0; i <= 40; i++){
                    texCoords.push_back(glm::vec2(i*dist,texture->getHeight()*145/1590));
                    texCoords.push_back(glm::vec2(i*dist,texture->getHeight()*(145+120)/1590));
                    texCoords.push_back(glm::vec2(i*dist,texture->getHeight()*(145+120+530)/1590));
                    texCoords.push_back(glm::vec2(i*dist,texture->getHeight()*(145+120+530+530)/1590));
                    texCoords.push_back(glm::vec2(i*dist,texture->getHeight()*(145+120+530+530+120)/1590));
                }
                break;
            case UV:
                for(int i = 0; i <= 40; i++){
                    texCoords.push_back(glm::vec2(i*dist,texture->getHeight()*0));
                    texCoords.push_back(glm::vec2(i*dist,texture->getHeight()*120/1300));
                    texCoords.push_back(glm::vec2(i*dist,texture->getHeight()*650/1300));
                    texCoords.push_back(glm::vec2(i*dist,texture->getHeight()*(650+530)/1300));
                    texCoords.push_back(glm::vec2(i*dist,texture->getHeight()*1));
                }
                break;
        }
        
        mesh.clearTexCoords();
        mesh.addTexCoords(texCoords);
    }
    
    void normalizeUV(){
        // find the extreme values
        float xMin = 1;
        float yMin = 1;
        float xMax = 0;
        float yMax = 0;
        
        for(auto& e : edges){
            if(xMin > e.uv.x) xMin = e.uv.x;
            if(yMin > e.uv.y) yMin = e.uv.y;
            if(xMax < e.uv.x) xMax = e.uv.x;
            if(yMax < e.uv.y) yMax = e.uv.y;
        }
        
        // normalize the UVs
        for(auto& e : edges){
            e.uv.x = ofMap(e.uv.x, xMin, xMax, 0, 1);
            e.uv.y = ofMap(e.uv.y, yMin, yMax, 0, 1);
        }
    }
    
    void createMesh(){
        for(int i = 0; i <= 40; i++){
            mesh.addVertex(ofPoint(i*3,0));
            mesh.addVertex(ofPoint(i*3,120));
            mesh.addVertex(ofPoint(i*3,650));
            mesh.addVertex(ofPoint(i*3,1180));
            mesh.addVertex(ofPoint(i*3,1300));
        }
        for(int i = 0; i < 40*5; i+=5){
            mesh.addIndex(i+5);
            mesh.addIndex(i+0);
            mesh.addIndex(i+6);
            mesh.addIndex(i+6);
            mesh.addIndex(i+0);
            mesh.addIndex(i+1);
            
            mesh.addIndex(i+6);
            mesh.addIndex(i+1);
            mesh.addIndex(i+7);
            mesh.addIndex(i+7);
            mesh.addIndex(i+1);
            mesh.addIndex(i+2);
            
            mesh.addIndex(i+7);
            mesh.addIndex(i+2);
            mesh.addIndex(i+8);
            mesh.addIndex(i+8);
            mesh.addIndex(i+2);
            mesh.addIndex(i+3);
            
            mesh.addIndex(i+8);
            mesh.addIndex(i+3);
            mesh.addIndex(i+9);
            mesh.addIndex(i+9);
            mesh.addIndex(i+3);
            mesh.addIndex(i+4);
        }
    }
    
    // ---   EDGE's the container for the points where the UV's are calculated
    struct Edge{
        glm::vec3 intersect;
        glm::vec3 pos;
        glm::vec2 uv;
        
    };
    
    
    void addEdge(glm::vec3 position){
        Edge edge;
        edge.pos = position;
        edges.push_back(edge);
    }
    
    vector <Edge> edges;
    
};

