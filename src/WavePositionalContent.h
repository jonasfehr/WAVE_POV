//
//  WavePositionalContent.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 24/01/2017.
//
//

#ifndef WavePositionalContent_h
#define WavePositionalContent_h

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


class WavePositionalContent{
public:
    ofFbo fbo;
    ofxAutoReloadedShader shader;
    
    // vector to store all values
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    
    ofVbo vbo;
    vector<Impulse> impulses;
    
    void setup(string name){
        
        fbo.allocate(40, 1300, GL_RGBA32F_ARB);
        fbo.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        fbo.end();
        
        shader.load("shaders/impulse");
        
        setupGuiGroup(name);
    }
    
    void activate(int index){
    }
    
    void addImpulse(int gate){
        Impulse impulse;
        float posX = gate*2.;
        impulse.setup(ofVec2f(posX, 100), ofVec2f(0., -speed*4.));
        
        impulses.push_back(impulse);
    }
    
    
    void update(){
        points.clear();
        sizes.clear();
        for (vector<Impulse>::iterator it=impulses.begin(); it!=impulses.end();)    {
            it->update();
            points.push_back(it->position+10);
            sizes.push_back(ofVec3f(size));
            
            if(it->position.y <= -size/2)
                it = impulses.erase(it);
            else
                ++it;
        }
        
        // upload the data to the vbo
        int total = (int)impulses.size();
        vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
        vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
        
        fbo.begin();
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
                
                
                shader.setUniform1f("u_soft", soft); //tempo p nr 1
                shader.setUniform1f("u_length", length);
                shader.setUniform1f("u_width", width);
                shader.setUniform1f("u_symmetry", symmetry);
                
                vbo.draw(GL_POINTS, 0, (int)impulses.size());
            }
            shader.end();
            
            ofDisablePointSprites();
            
        }
        fbo.end();
    }
    
    void draw(){
        fbo.draw(0,0, 120, 1300);
    }
    
    ofTexture getTexture(){
        return fbo.getTexture();
    }
    
    
    ofParameterGroup paramGroup;
    float size = 100;
    
    ofParameter<float> soft = 0.5;
    ofParameter<float> length = .1;
    ofParameter<float> width = 1.;
    ofParameter<float> symmetry = .5;
    ofParameter<float> speed = .5;
    
    void setupGuiGroup(string name){
        paramGroup.setName(name);
        paramGroup.add(soft.set("soft",0.5,0.,1.));
        paramGroup.add(length.set("length",0.5,0.,1.));
        paramGroup.add(width.set("width",0.5,0.,1.));
        paramGroup.add(symmetry.set("symmetry",0.5,0.,1.));
        paramGroup.add(speed.set("speed",0.5,0.,1.));
    }
    
    ofParameterGroup* getPointerToParameterGroup(){ return &paramGroup; }
    
};

#endif /* WavePositionalContent_h */
