//
//  SyphonForWave.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 16/01/2017.
//
//

#ifndef SyphonForWave_h
#define SyphonForWave_h
SyphonForWave(){
    ofxSyphonServer sServer;
    ofFbo outputFbo;
    
    ofMesh outputMesh;
    
    void setup(string name){
        outputFbo.allocate(40, 1300, GL_RGBA32F_ARB);
        outputFbo.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        outputFbo.end();
        sServer.setName(name);
        
        createOutputMesh();
    }
    
    
    void addTexCoords(){
        
    }
    
    void createOutputMesh(){
        for(int i = 0; i <= 40; i++){
            outputMesh.addVertex(ofVec2f(i,0));
            outputMesh.addVertex(ofVec2f(i,120));
            outputMesh.addVertex(ofVec2f(i,650));
            outputMesh.addVertex(ofVec2f(i,1180));
            outputMesh.addVertex(ofVec2f(i, 1300));
        }
        for(int i = 0; i < 40*5; i+=5){
            outputMesh.addIndex(i+5);
            outputMesh.addIndex(i+0);
            outputMesh.addIndex(i+6);
            outputMesh.addIndex(i+6);
            outputMesh.addIndex(i+0);
            outputMesh.addIndex(i+1);
            
            outputMesh.addIndex(i+6);
            outputMesh.addIndex(i+1);
            outputMesh.addIndex(i+7);
            outputMesh.addIndex(i+7);
            outputMesh.addIndex(i+1);
            outputMesh.addIndex(i+2);
            
            outputMesh.addIndex(i+7);
            outputMesh.addIndex(i+2);
            outputMesh.addIndex(i+8);
            outputMesh.addIndex(i+8);
            outputMesh.addIndex(i+2);
            outputMesh.addIndex(i+3);
            
            outputMesh.addIndex(i+8);
            outputMesh.addIndex(i+3);
            outputMesh.addIndex(i+9);
            outputMesh.addIndex(i+9);
            outputMesh.addIndex(i+3);
            outputMesh.addIndex(i+4);
        }
    }

    
    
}

#endif /* SyphonForWave_h */
