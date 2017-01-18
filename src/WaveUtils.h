//
//  Utils.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 17/01/2017.
//
//

#ifndef Utils_h
#define Utils_h
enum waveOutputMappingType : int {
    POV_UV,
    POV_UV_NORMALIZED,
    TUBE
};

class WaveOutputGenerator{
public:
    
    
    ofFbo fbo;
    ofMesh mesh;
    ofTexture *texture;
    POV pov;
    vector<Gate> * gates;
    int mappingType = TUBE;
    
    void setPov(ofVec3f position){ pov.setPosition(position); };
    void setMappingType(int mappingType){ this->mappingType = mappingType; };
    
    
    WaveOutputGenerator(){
        
    }
    
    void setup(vector<Gate> * gates, ofVec3f povPosition, int mappingType){
        //this->pov = pov;
        this->gates = gates;
        this->mappingType = mappingType;
        pov.setPosition(povPosition);
        
        fbo.allocate(40, 1300);
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
    
    
    
    void update(){
        pov.update();
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
        fbo.draw(0,0);
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
                float dist = texture->getWidth()/39;
                for(int i = 0; i < 40; i++){
                    texCoords.push_back(ofVec2f(i*dist,texture->getHeight()*145/1590));
                    texCoords.push_back(ofVec2f(i*dist,texture->getHeight()*(145+120)/1590));
                    texCoords.push_back(ofVec2f(i*dist,texture->getHeight()*(145+120+530)/1590));
                    texCoords.push_back(ofVec2f(i*dist,texture->getHeight()*(145+120+530+530)/1590));
                    texCoords.push_back(ofVec2f(i*dist,texture->getHeight()*(145+120+530+530+120)/1590));
                }
                break;
        }
        
        mesh.clearTexCoords();
        mesh.addTexCoords(texCoords);
    }
    
    
    void createMesh(){
        for(int i = 0; i <= 40; i++){
            mesh.addVertex(ofVec2f(i,0));
            mesh.addVertex(ofVec2f(i,120));
            mesh.addVertex(ofVec2f(i,650));
            mesh.addVertex(ofVec2f(i,1180));
            mesh.addVertex(ofVec2f(i, 1300));
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


class UpDownCounter{
public:
    float value = 0;
    float increase = 0.005;
    
    bool activated = false;
    bool isUp = false;
    
    UpDownCounter(){}
    
    void activate(){
        activated = true;
        isUp = true;
    }
    
    void update(){
        if(activated){
            if(isUp) value += increase;
            if(value > 1) isUp = false;
            if(!isUp) value -= increase;
            if(value <= 0){
                value = 0;
                activated = false;
            }
        }
    }
    
    float get(){
        return value;
    }
};

class WaveSlitGenerator{
public:
    ofFbo fbo;
    ofImage image;
    UpDownCounter counters[40];
    ofShader shader;
    
    void setup(ofImage image){
        this->image = image;
        
        fbo.allocate(40, 1300);
        fbo.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fbo.end();
        
        shader.load("shaders/slit");
    }
    
    void activate(int index){
        counters[index].activate();
    }
    
    void update(){
        for(auto& c : counters){
            c.update();
        }
        fbo.begin();
        {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ofSetColor(255);
            ofFill();
            for(int i = 0; i < 40; i++){
                ofDrawRectangle(i-0.5, 0, 1, counters[i].get()*650);
                ofDrawRectangle(i-0.5, 1300, 1, -counters[i].get()*650);
                
                // ofDrawLine(i, 0, i, counters[i].get()*1300);
            }
            
//            shader.begin();
//            {
//                shader.setUniform2f("u_resolution", fbo.getWidth(), fbo.getHeight());
////                shader.setUniform1f("u_time", counter); //tempo p nr 1
////                shader.setUniform1f("u_density", density);
////                shader.setUniform1f("u_contrast", contrast);
////                shader.setUniform1f("u_H", hue);
////                shader.setUniform1f("u_S", saturation);
////                shader.setUniform1f("u_B", 1.);
////                shader.setUniform1f("u_direction", direction);
////                shader.setUniform1f("u_mix", mix);
//                
//                
//                shader.setUniformTexture("txtCont", image.getTexture(), 0);
//                
//                image.draw(0,0);
//                
////                ofSetColor(255,255,255);
////                ofFill();
////                ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
//            }
//            shader.end();
            
        }
        fbo.end();
    }
    
    void draw(){
        fbo.draw(0,0);
    }
};

#endif /* Utils_h */
