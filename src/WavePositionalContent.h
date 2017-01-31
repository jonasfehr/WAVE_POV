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
#include "Objects.h"

class WavePositionalContent : public WaveContent{
public:
    ofFbo fboShader;
    ofMesh mesh;
    
    // vector to store all values
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    
    ofVbo vbo;
    
    // Positions
    map<int, User> * objects;
    
    // parameters
    ofParameter<int> mode;
    ofParameter<int> imageIndex;

    vector<ofImage> *images;

    
    WavePositionalContent(){}
    
    
    
    
    // SETUP
//    void setup(string name, vector<ofImage> *images, map<int, User> *objects){
//        this->images = images;
//        this->name = name;
//        this->objects = objects;
//        fbo.allocate(120, 1300, GL_RGBA32F_ARB);
//        fboShader.allocate(78*130, 10*130, GL_RGBA32F_ARB);
//        
//        shader.load("shaders/WavePositionalContent");
//        
//        setupParameterGroup(name);
//        
//        createMesh();
//        
//    }
    
    void setup(string name, vector<ofImage> *images, map<int, User> *users){
        this->images = images;
        this->name = name;
        this->objects = users;
        fbo.allocate(120, 1300, GL_RGBA32F_ARB);
        fboShader.allocate(78*130, 10*130, GL_RGBA32F_ARB);
        
        shader.load("shaders/WavePositionalContent");
        
        setupParameterGroup(name);
        
        createMesh();
        
    }
    
    
    // UPDATE
    void update(){
        points.clear();
        sizes.clear();
        
        
        for(auto & o : *objects){
            
            ofVec3f pos = o.second.position;
            ofVec3f size = o.second.size;
            pos.x = pos.x*130;
            pos.y = (pos.z+7.)*130;
            pos.z = 0;
            
            points.push_back(pos);
            sizes.push_back(size);
        }
        
        
        // upload the data to the vbo
        int total = (int)objects->size();
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
                    shader.setUniformTexture("texPoint", images->at(imageIndex).getTexture(), 1);
                    shader.setUniform2f("texResolution", images->at(imageIndex).getWidth(), images->at(imageIndex).getHeight());
                    
                    shader.setUniform2f("iResolution", fboShader.getWidth(), fboShader.getHeight());
                    shader.setUniform1f("iGlobalTime",     ofGetElapsedTimef() ) ;//counter);
                    shader.setUniform1i("u_mode", (int)mode);
                    
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
        parameterGroup.add(mode.set("mode", 1, 1, 2));
        parameterGroup.add(imageIndex.set("imageIndex", 0, 0, images->size()-1));
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
        
        float dist = fboShader.getTexture().getWidth()/39;
        for(int i = 0; i <= 40; i++){
            mesh.addTexCoord( ofVec2f( i*dist, 0. ) );
            mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.1 ) );
            mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.5 ) );
            mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight()*0.9 ) );
            mesh.addTexCoord( ofVec2f( i*dist, fboShader.getTexture().getHeight() ) );
        }
    }
    
        
};

#endif /* WavePositionalContent_h */
