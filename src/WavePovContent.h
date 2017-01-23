//
//  WavePovContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 23/01/2017.
//
//

#ifndef WavePovContent_h
#define WavePovContent_h
enum waveOutputMappingType : int {
    POV_UV,
    POV_UV_NORMALIZED,
    TUBE
};

class WavePovContent{
public:
    
    
    ofFbo fbo;
    ofMesh mesh;
    ofTexture *texture;
    POV pov;
    vector<Gate> * gates;
    int mappingType = TUBE;
    
    void setPov(ofVec3f position){ pov.setPosition(position); };
    void setMappingType(int mappingType){ this->mappingType = mappingType; };
    
    
    WavePovContent(){
        
    }
    
    void setup(vector<Gate> * gates, ofVec3f povPosition, int mappingType){
        //this->pov = pov;
        this->gates = gates;
        this->mappingType = mappingType;
        pov.setPosition(povPosition);
        
        fbo.allocate(120, 1300, GL_RGB16);
        fbo.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fbo.end();
        
        createMesh();
        
        // setup POV
        for(auto& gate : *gates){
            pov.addEdge(gate.leftInnerEdge);
            pov.addEdge(gate.leftOuterEdge);
            pov.addEdge(gate.topEdge);
            pov.addEdge(gate.rightOuterEdge);
            pov.addEdge(gate.rightInnerEdge);
        }
        
    }
    
    
    
    void update(ofCamera * cam){
        pov.update(cam);
        calcMapping();
        
        fbo.begin();
        {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            texture->bind();
            {
                ofSetColor(255);
                mesh.draw();
            }
            texture->unbind();
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
    
    void calcMapping(){
        
        vector<ofVec2f> texCoords;
        
        switch(mappingType){
            case POV_UV:
                for(auto& e : pov.edges){
                    texCoords.push_back(ofVec2f(e.uv.x*texture->getWidth(), e.uv.y*texture->getHeight()));
                }
                break;
                
            case POV_UV_NORMALIZED:
                pov.normalizeUV();
                for(auto& e : pov.edges){
                    texCoords.push_back(ofVec2f(e.uv.x*texture->getWidth(), e.uv.y*texture->getHeight()));
                }
                break;
                
            case TUBE:
                float dist = 1/40;
                for(int i = 0; i <= 40; i++){
                    texCoords.push_back(ofVec2f(i*dist,145/1590));
                    texCoords.push_back(ofVec2f(i*dist,(145+120)/1590));
                    texCoords.push_back(ofVec2f(i*dist,(145+120+530)/1590));
                    texCoords.push_back(ofVec2f(i*dist,(145+120+530+530)/1590));
                    texCoords.push_back(ofVec2f(i*dist,(145+120+530+530+120)/1590));
                }
                break;
        }
        
        mesh.clearTexCoords();
        mesh.addTexCoords(texCoords);
    }
    
    
    void createMesh(){
        for(int i = 0; i <= 40; i++){
            mesh.addVertex(ofVec2f(i*3,0));
            mesh.addVertex(ofVec2f(i*3,120));
            mesh.addVertex(ofVec2f(i*3,650));
            mesh.addVertex(ofVec2f(i*3,1180));
            mesh.addVertex(ofVec2f(i*3,1300));
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
    
};

#endif /* WavePovContent_h */
