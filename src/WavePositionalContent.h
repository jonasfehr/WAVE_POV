//
//  WavePositionalContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 24/01/2017.
//
//

#ifndef WavePositionalContent_h
#define WavePositionalContent_h
#include "ofxAutoReloadedShader.h"

class Impulse{
public:
    ofVec2f position;
    ofVec2f speed;
    

    
    Impulse(){};
    
    void setup(ofVec2f _position, ofVec2f _speed){
        position = _position;
        speed = _speed;
        //        dead = false;
    }
    
    void update(){
        position += speed;
        
    }
    void draw(){
        ofSetColor(255);
        ofDrawCircle(position, 10);
    };
    
    //    bool isDead(){ return dead;}
    //
    //    bool dead;
};

class Position{
public:
    ofVec3f position = ofVec3f(0.);
    ofVec3f size = ofVec3f(400.);
    
    void setPosition(ofVec3f position){ this->position = position; }
    void setSize(ofVec3f size){ this->position = size; }
};


class WavePositionalContent : public WaveContent{
public:
    ofFbo fboShader;
    ofMesh mesh;
    
    // vector to store all values
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    
    ofVbo vbo;
    
    // Positions
    vector<Impulse> impulses;
    map<int, Position> positions;
    
    ofParameter<float> soft = 0.5;
    ofParameter<float> length = .01;
    ofParameter<float> width = .001;
    ofParameter<float> symmetry = .5;
    ofParameter<float> speed = .5;
    
    
    WavePositionalContent(){}
    
    void setup(string name){
        
        fbo.allocate(120, 1300, GL_RGBA32F_ARB);
        fboShader.allocate(78*130, 14*130, GL_RGBA32F_ARB);
        
        shader.load("shaders/ptSpline");
        
        setupParameterGroup(name);
        
        createMesh();
        
    }
    
    void update(){
        points.clear();
        sizes.clear();
        for (vector<Impulse>::iterator it=impulses.begin(); it!=impulses.end();)    {
            it->update();
            points.push_back(it->position+10);
            
            float size = 100;
            sizes.push_back(ofVec3f(size));
            
            if(it->position.y <= -size/2)
                it = impulses.erase(it);
            else
                ++it;
        }
        
        
        for(auto & p : positions){
            
            ofVec3f pos = p.second.position;
            ofVec3f size = p.second.size;
            pos.x = pos.x*130;//pos.x/80.*fbo.getWidth();
            pos.y = (pos.y+7.)*130;//*130;//pos.y/14.*fbo.getHeight()+fbo.getHeight()/2;
            
            points.push_back(pos);
            sizes.push_back(size);
        }
        
        
        // upload the data to the vbo
        int total = (int)impulses.size()+positions.size();
        vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
        vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
        
        fbo.begin();{
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            fboShader.begin();
            {
                glClearColor(0.0, 0.0, 0.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                
                glDepthMask(GL_FALSE);
                
                ofSetColor(255);
                
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
                glBlendEquation(GL_MAX);
                ofEnablePointSprites();
                
                shader.begin();
                {
                    
                    shader.setUniform2f("iResolution", fboShader.getWidth(), fboShader.getHeight());
                    shader.setUniform1f("iGlobalTime",     ofGetElapsedTimef() ) ;//counter);
                    shader.setUniform1f("u_soft", soft); //tempo p nr 1
                    shader.setUniform1f("u_length", length);
                    shader.setUniform1f("u_width", width);
                    shader.setUniform1f("u_symmetry", symmetry);
                    
                    vbo.draw(GL_POINTS, 0, total);
                }
                shader.end();
                
                ofDisablePointSprites();
                
            }
            fboShader.end();
            
            fboShader.getTexture().bind();
            {
                ofSetColor(255);
                mesh.draw();
            }
            fboShader.getTexture().unbind();
            
        }
        fbo.end();
    }
    
    void setupParameterGroup(string name){
        parameterGroup.setName(name);
        parameterGroup.add(soft.set("soft",0.5,0.,1.));
        parameterGroup.add(length.set("length",0.05,0.,1.));
        parameterGroup.add(width.set("width",0.0005,0.,1.));
        parameterGroup.add(symmetry.set("symmetry",0.5,0.,1.));
        parameterGroup.add(speed.set("speed",0.5,0.,1.));
    }
    
    
    void addImpulse(int gate){
        Impulse impulse;
        float posX = gate * 2.;
        impulse.setup(ofVec2f(posX, 100), ofVec2f(0., -speed*4.));
        
        impulses.push_back(impulse);
    }
    
    void updatePosition(int identifier, ofVec2f pos){
        positions[identifier].setPosition(ofVec3f(pos.x, pos.y, 0.));
    }
    
    
    void createMesh(){
        
        float dist = fboShader.getTexture().getWidth()/39;
        
        for(int i = 0; i <= 40; i++){
            mesh.addVertex(ofVec2f(i*3,0));
            mesh.addVertex(ofVec2f(i*3,120));
            mesh.addVertex(ofVec2f(i*3,650));
            mesh.addVertex(ofVec2f(i*3,1180));
            mesh.addVertex(ofVec2f(i*3,1300));
            
            mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight() ) );
            mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.1 ) );
            mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.5 ) );
            mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.9 ) );
            mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight() ) );
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

#endif /* WavePositionalContent_h */
