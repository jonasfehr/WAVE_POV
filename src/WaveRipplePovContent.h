//
//  WaveRipplePovContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 23/01/2017.
//
//

#ifndef WaveRipplePovContent_h
#define WaveRipplePovContent_h
#include "ofxAutoReloadedShader.h"
#include "ofxOscSender.h"



class WaveRipplePovContent : public WaveContent{
public:
    
    
    ofFbo fboShader;
    ofMesh mesh;
    ofTexture *texture;
    ofCamera pov;
    vector<Gate> * gates;
    
    
    
    ofxOscSender * oscSender;
    int index;

    
    ofParameter<float> possibility;
    ofParameter<float> increment;
    ofParameter<bool> deactivate;


    bool createRipple = false;
    float rippleSize = 0.;
    float timeLastActivation = 0;
    float waitTime = 10.;
    
    
    WaveRipplePovContent(){}
    
    
    void setup(string channelName, int index, vector<Gate> * gates, ofVec3f povPosition, ofxOscSender * oscSender){
        this->name = channelName;
        this->index = index;
        this->gates = gates;
        this->shader.load("shaders/WaveRipplesContent");
        this->oscSender = oscSender;
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
        
        fboShader.allocate(1024, 1024, GL_RGBA32F_ARB);
        fboShader.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fboShader.end();
        
        texture = &fboShader.getTexture();
        
        setupParameterGroup(name);
        
        timeLastActivation = ofRandom(0., 10.);
        waitTime = ofRandom(10., 30.)+index*5;
    }

    
    
    void setPov(ofCamera cam){
        pov = cam;
    }
    
    ofCamera* getPovPtr(){ return &pov; }
    
    void setPovPosition(ofVec3f pos){
        pov.setPosition(pos);
    }
    
    
    
    
    int oldMillis = 0;
    void update(){

        
        if(!createRipple && !deactivate){
            if( ofRandom(0., 1.) < possibility && ofGetElapsedTimef()-timeLastActivation > waitTime){
                rippleSize = 0.;
                createRipple = true;
                timeLastActivation = ofGetElapsedTimef();
                waitTime = ofRandom(30., 60.);
            }
            
        }else{
            rippleSize += increment;
            
            
            
            if(rippleSize >= 1.){
                createRipple = false;
            }
            
            // SEND OSC gate 1
            if(ofGetElapsedTimeMillis()/50 != oldMillis){
                ofxOscMessage m;
                m.setAddress("/Ripple/"+ofToString(index));
                m.addInt32Arg(createRipple);
                
                ofVec3f position = pov.getPosition() + pov.getLookAtDir().getNormalized().scale( 85.*( 1. - cubicOut(rippleSize) ) );
                m.addFloatArg(position.z);
                oscSender->sendMessage(m);
                
                oldMillis = ofGetElapsedTimeMillis()/50;
            }
        }
        
        
        
        
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
    
            fboShader.begin();
            {
                glClearColor(0.0, 0.0, 0.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                shader.begin();
                {
                    shader.setUniform2f("iResolution", fboShader.getWidth(), fboShader.getHeight());
                    shader.setUniform1f("iGlobalTime", ofGetElapsedTimef() ) ;//counter);
                    shader.setUniform1f("rippleSize", rippleSize );
                    
                    ofSetColor(255,255,255);
                    ofFill();
                    ofDrawRectangle(0, 0, fboShader.getWidth(), fboShader.getHeight());
                }
                shader.end();
            }
            fboShader.end();
            
            texture->bind();{
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
    
    ofFbo* getFboPtr(){ return &fbo; }
    
    
    void calcMapping(){
        
        vector<ofVec2f> texCoords;


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
    
    void setupParameterGroup(string name){
        parameterGroup.setName(name);
        parameterGroup.add(deactivate.set("deactivate", false));
        parameterGroup.add(possibility.set("possibility", 0.1, 0., 1.));
        parameterGroup.add(increment.set("increment", 0.01, 0.000001, 0.1));

    }
    
    float cubicOut(float t) {
        float f = t - 1.0;
        return f * f * f + 1.0;
    }
    
};

#endif /* WaveRipplePovContent_h */
