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
    glm::vec2 position;
    glm::vec2 speed;
    

    
    Impulse(){};
    
    void setup(glm::vec2 _position, glm::vec2 _speed){
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
    glm::vec3 position = glm::vec3(0.);
    glm::vec3 size = glm::vec3(400.);
    
    void setPosition(glm::vec3 position){ this->position = position; }
    void setSize(glm::vec3 size){ this->position = size; }
};


class WavePositionalContent : public WaveContent{
public:
    ofFbo fboShader;
    ofMesh mesh;
    
    // vector to store all values
    vector<glm::vec3> points;
    vector<glm::vec3> sizes;
    
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
            glm::vec3 newP = glm::vec3(it->position+10.0f,0.0f);
            points.push_back(newP);
            
            float size = 100;
            sizes.push_back(glm::vec3(size));
            
            if(it->position.y <= -size/2)
                it = impulses.erase(it);
            else
                ++it;
        }
        
        
        for(auto & p : positions){
            
            glm::vec3 pos = p.second.position;
            glm::vec3 size = p.second.size;
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
        impulse.setup(glm::vec2(posX, 100), glm::vec2(0., -speed*4.));
        
        impulses.push_back(impulse);
    }
    
    void updatePosition(int identifier, glm::vec2 pos){
        positions[identifier].setPosition(glm::vec3(pos.x, pos.y, 0.));
    }
    
    
    void createMesh(){
        
        float dist = fboShader.getTexture().getWidth()/39;
        
        for(int i = 0; i <= 40; i++){
            mesh.addVertex(glm::vec3(i*3, 0, 0));
            mesh.addVertex(glm::vec3(i*3, 120, 0));
            mesh.addVertex(glm::vec3(i*3, 650, 0));
            mesh.addVertex(glm::vec3(i*3, 1180, 0));
            mesh.addVertex(glm::vec3(i*3, 1300, 0));
            
            mesh.addTexCoord(glm::vec3( i*dist, fboShader.getTexture().getHeight() ,0) );
            mesh.addTexCoord(glm::vec3( i*dist, fboShader.getTexture().getHeight()*0.1 ,0) );
            mesh.addTexCoord(glm::vec3( i*dist, fboShader.getTexture().getHeight()*0.5 ,0) );
            mesh.addTexCoord(glm::vec3( i*dist, fboShader.getTexture().getHeight()*0.9 ,0) );
            mesh.addTexCoord(glm::vec3( i*dist, fboShader.getTexture().getHeight() ,0) );
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
