//
//  InputToWaveContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 23/01/2017.
//
//

#ifndef InputToWaveContent_h
#define InputToWaveContent_h
#include "ofxAutoReloadedShader.h"


enum waveOutputMappingType : int {
    POV_UV,
    POV_UV_NORMALIZED,
    TUBE
};



class InputToWaveContent{
public:
    
    
    ofFbo fbo;
    ofFbo fboShader;
    ofMesh mesh;
    ofTexture *texture;
    ofCamera pov;
    vector<Gate> * gates;
    int mappingType = TUBE;
    string name;
    
    float intensityNoise;
    
    ofxAutoReloadedShader shader;
    
    bool isVisible = true;
    float alpha = 1.;
    
    int inputType = 0;
    
    InputToWaveContent(){}
    
    void setup(string channelName, vector<Gate> * gates, ofVec3f povPosition, ofTexture *texture, int mappingType){
        this->name = channelName;
        this->gates = gates;
        this->mappingType = mappingType;
        this->texture = texture;
        pov.setPosition(povPosition);
        pov.lookAt(ofVec3f(0., 1.72, 39));

        
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
        
        inputType = INPUT_EXTERNAL;
    }
    
    void setup(string channelName, vector<Gate> * gates, ofVec3f povPosition, string shaderName, ofVec2f shaderSize, int mappingType){
        this->name = channelName;
        this->gates = gates;
        this->mappingType = mappingType;
        this->shader.load("shaders/"+shaderName);

        pov.setPosition(povPosition);        
        pov.lookAt(ofVec3f(0., 1.72, 39));

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
        
        fboShader.allocate(shaderSize.x, shaderSize.y, GL_RGB);
        fboShader.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fboShader.end();
        
        texture = &fboShader.getTexture();
        
        inputType = INPUT_SHADER;
    }

    
    void setMappingType(int mappingType){ this->mappingType = mappingType; };
    
    void setPov(ofCamera cam){
        pov = cam;
    }
    
    ofCamera* getPovPtr(){ return &pov; }
    
    void setPovPosition(ofVec3f pos){
        pov.setPosition(pos);
    }
    

    void setVisible(){ isVisible = true; }
    void setInvisible(){ isVisible = false; }
    void setInvisible(float alpha){
        isVisible = false;
        this->alpha = alpha;
    }

    
    
    
    
    void update(){
        if(isVisible && alpha < 1.) alpha += 0.1;
        else if( !isVisible && alpha > 0) alpha -= 0.01;
        
        // Calculate the UV points
        for(auto& e : edges){
            ofVec3f coord = pov.worldToScreen(e.pos);
            coord.x /= ofGetWidth();
            coord.y /= ofGetHeight();
            if((e.pos-pov.getPosition()).dot(pov.getLookAtDir())>0) coord.y = 1-coord.y;
            e.uv = coord;
        }
        
        calcMapping();
        
        fbo.begin();
        {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            switch (inputType) {
                    
                    // EXTERNAL TEXTURE
                case INPUT_EXTERNAL:
                    texture->bind();{
                        ofSetColor(255, alpha*255);
                        mesh.draw();
                    }
                    texture->unbind();
                    break;
                    
                    
                    // SHADER CALCULATES TEXTURE
                case INPUT_SHADER:
                    fboShader.begin();{
                        glClearColor(0.0, 0.0, 0.0, 0.0);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                        intensityNoise = ofNoise(ofGetElapsedTimef()/2.)/2+0.5;
                        
                        shader.begin();{
                            shader.setUniform2f("iResolution", fboShader.getWidth(), fboShader.getHeight());
                            shader.setUniform1f("iGlobalTime",     ofGetElapsedTimef() ) ;//counter);
                            shader.setUniform1f("intensity",     intensityNoise ) ;//counter);
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
                    break;
                    
                default:
                    break;
            }

        }
        fbo.end();
    }
    
    void draw(){
        fbo.draw(0,0, 120, 1300);
    }
    
    void setTexCoords(vector<ofVec2f> texCoords){
        mesh.clearTexCoords();
        mesh.addTexCoords(texCoords);
    }
    
    vector<ofVec2f> getTexCoords(){ return mesh.getTexCoords(); }
    
    void setInputTexture(ofTexture *texture){ this->texture = texture;}
    
    ofTexture getTexture(){ return fbo.getTexture(); }
    
    ofFbo* getFboPtr(){ return &fbo; }
    
    
    void calcMapping(){
        
        vector<ofVec2f> texCoords;
        
        switch(mappingType){
            case POV_UV:
                for(int i = 0; i < edges.size(); i+=5){
                    texCoords.push_back(ofVec2f(edges.at(i).uv.x*texture->getWidth(), edges.at(i).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+1).uv.x*texture->getWidth(), edges.at(i+1).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+2).uv.x*texture->getWidth(), edges.at(i+2).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+3).uv.x*texture->getWidth(), edges.at(i+3).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+4).uv.x*texture->getWidth(), edges.at(i+4).uv.y*texture->getHeight()));
                    
                    texCoords.push_back(ofVec2f(edges.at(i).uv.x*texture->getWidth(), edges.at(i).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+1).uv.x*texture->getWidth(), edges.at(i+1).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+2).uv.x*texture->getWidth(), edges.at(i+2).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+3).uv.x*texture->getWidth(), edges.at(i+3).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+4).uv.x*texture->getWidth(), edges.at(i+4).uv.y*texture->getHeight()));
                }
                break;
                
            case POV_UV_NORMALIZED:
                normalizeUV();
                for(int i = 0; i < edges.size(); i+=5){
                    texCoords.push_back(ofVec2f(edges.at(i).uv.x*texture->getWidth(), edges.at(i).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+1).uv.x*texture->getWidth(), edges.at(i+1).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+2).uv.x*texture->getWidth(), edges.at(i+2).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+3).uv.x*texture->getWidth(), edges.at(i+3).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+4).uv.x*texture->getWidth(), edges.at(i+4).uv.y*texture->getHeight()));
                    
                    texCoords.push_back(ofVec2f(edges.at(i).uv.x*texture->getWidth(), edges.at(i).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+1).uv.x*texture->getWidth(), edges.at(i+1).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+2).uv.x*texture->getWidth(), edges.at(i+2).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+3).uv.x*texture->getWidth(), edges.at(i+3).uv.y*texture->getHeight()));
                    texCoords.push_back(ofVec2f(edges.at(i+4).uv.x*texture->getWidth(), edges.at(i+4).uv.y*texture->getHeight()));
                }
                break;
                
            case TUBE:
                float dist = texture->getWidth()/39;
                for(int i = 0; i <= 40; i++){
                    texCoords.push_back(ofVec2f(1+i*dist,texture->getHeight()*145/1590));
                    texCoords.push_back(ofVec2f(1+i*dist,texture->getHeight()*(145+120)/1590));
                    texCoords.push_back(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530)/1590));
                    texCoords.push_back(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530+530)/1590));
                    texCoords.push_back(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530+530+120)/1590));
                    
                    texCoords.push_back(ofVec2f(1+i*dist,texture->getHeight()*145/1590));
                    texCoords.push_back(ofVec2f(1+i*dist,texture->getHeight()*(145+120)/1590));
                    texCoords.push_back(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530)/1590));
                    texCoords.push_back(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530+530)/1590));
                    texCoords.push_back(ofVec2f(1+i*dist,texture->getHeight()*(145+120+530+530+120)/1590));

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
            mesh.addVertex(ofVec2f(i*3,0));
            mesh.addVertex(ofVec2f(i*3,120));
            mesh.addVertex(ofVec2f(i*3,650));
            mesh.addVertex(ofVec2f(i*3,1180));
            mesh.addVertex(ofVec2f(i*3,1300));
            
            mesh.addVertex(ofVec2f((i*3+3),0));
            mesh.addVertex(ofVec2f((i*3+3),120));
            mesh.addVertex(ofVec2f((i*3+3),650));
            mesh.addVertex(ofVec2f((i*3+3),1180));
            mesh.addVertex(ofVec2f((i*3+3),1300));
        }
        for(int i = 0; i < 40*10; i+=10){
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
        ofVec3f intersect;
        ofVec3f pos;
        ofVec2f uv;
        
    };
    
    
    void addEdge(ofVec3f position){
        Edge edge;
        edge.pos = position;
        edges.push_back(edge);
    }
    
    vector <Edge> edges;
    
    enum povInputType : int {
        INPUT_EXTERNAL = 1,
        INPUT_SHADER = 2,
    };
    
    string getName(){ return name; }
    
    float getIntensity(){return intensityNoise;}

};

#endif /* InputToWaveContent_h */
