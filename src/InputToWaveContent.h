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
#include "ofxGpuMixer.h"


enum waveOutputMappingType : int {
    POV_UV,
    POV_UV_NORMALIZED,
    TUBE,
    UV
};



class InputToWaveContent: public ofx::GpuMixer::BasicChannel{
public:
    
    
//    ofFbo fbo;
    ofFbo fboShader;
    ofMesh mesh;
    ofTexture *texture;
    ofCamera pov;
    vector<Gate> * gates;
    int mappingType = TUBE;
    
    ofxAutoReloadedShader shader;
    
    bool isVisible = true;
    float alpha = 1.;
    
    int inputType = 0;
    
    InputToWaveContent(){}
    
    void setup(vector<Gate> * gates, glm::vec3 povPosition, ofTexture *texture, int mappingType){
        //this->pov = pov;
        this->gates = gates;
        this->mappingType = mappingType;
        this->texture = texture;
        //pov.setPosition(povPosition);
        
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
    
    void setup(vector<Gate> * gates, glm::vec3 povPosition, string shaderName, glm::vec2 shaderSize, int mappingType){
        //this->pov = pov;
        this->gates = gates;
        this->mappingType = mappingType;
        this->shader.load("shaders/"+shaderName);

        //pov.setPosition(povPosition);
        
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
            if(glm::dot( e.pos-pov.getPosition(), pov.getLookAtDir() ) > 0 ) coord.y = 1-coord.y;
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
    
    void setTexCoords(vector<glm::vec2> texCoords){
        mesh.clearTexCoords();
        mesh.addTexCoords(texCoords);
    }
    
    vector<glm::vec2> getTexCoords(){ return mesh.getTexCoords(); }
    
    void setInputTexture(ofTexture *texture){ this->texture = texture;}
            
    
    void calcMapping(){
        
        vector<glm::vec2> texCoords;
        
        float dist = texture->getWidth()/40;
        switch(mappingType){
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
    
    enum povInputType : int {
        INPUT_EXTERNAL = 1,
        INPUT_SHADER = 2,
    };
};

#endif /* InputToWaveContent_h */
