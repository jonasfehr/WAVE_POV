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
    ofCamera pov;
    vector<Gate> * gates;
    int mappingType = TUBE;
    
    void setMappingType(int mappingType){ this->mappingType = mappingType; };
    
    void setPov(ofCamera cam){
        pov = cam;
    }
    
    WavePovContent(){
        
    }
    
    void setup(vector<Gate> * gates, ofVec3f povPosition, int mappingType){
        //this->pov = pov;
        this->gates = gates;
        this->mappingType = mappingType;
        //pov.setPosition(povPosition);
        
        fbo.allocate(120, 1300, GL_RGB16);
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

    }
    
    
    
    void update(){
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
                for(auto& e : edges){
                    texCoords.push_back(ofVec2f(e.uv.x*texture->getWidth(), e.uv.y*texture->getHeight()));
                }
                break;
                
            case POV_UV_NORMALIZED:
                normalizeUV();
                for(auto& e : edges){
                    texCoords.push_back(ofVec2f(e.uv.x*texture->getWidth(), e.uv.y*texture->getHeight()));
                }
                break;
                
            case TUBE:
                float dist = texture->getWidth()/40;
                for(int i = 0; i <= 40; i++){
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
    
};

#endif /* WavePovContent_h */
