//
//  Gate.h
//  WAVE_POV
//
//  Created by Frederik Tollund Juutilainen on 04/01/17.
//
//

#ifndef Gate_h
#define Gate_h

class Gate{
public:
    Gate(ofVec3f position, int index){
        this->pos = position;
        this->index = index;
        
        // TOP
        topEdge = pos;
        topEdge.y = topHeight;
        
        // Left Outer
        leftOuterEdge = pos;
        leftOuterEdge.y = bottomHeight;
        leftOuterEdge.x = -outerWidth;
        
        // Right outer
        rightOuterEdge = pos;
        rightOuterEdge.y = bottomHeight;
        rightOuterEdge.x = outerWidth;
        
        // Left Inner
        leftInnerEdge = pos;
        leftInnerEdge.y = bottomHeight;
        leftInnerEdge.x = -innerWidth;
        
        // Right Inner
        rightInnerEdge = pos;
        rightInnerEdge.y = bottomHeight;
        rightInnerEdge.x = innerWidth;
        

        
        
        // create a meshLed to visualize the result
        
        float ledHeight = 0.021;
        float ledWidth = 0.012;
        float triOff = ledHeight*1.732;
        float profileHeight = 0.091;
        float profileWidth = 0.065;
        float profileOff = profileHeight*1.732;

        // create led mesh
        meshLed.addVertex(leftInnerEdge+ofVec3f(0, 0, -ledWidth));
        meshLed.addVertex(leftInnerEdge+ofVec3f(0, ledHeight, 0.));
        meshLed.addVertex(leftInnerEdge+ofVec3f(0, 0, ledWidth));
        
        meshLed.addVertex(leftOuterEdge+ofVec3f(0, 0, -ledWidth));
        meshLed.addVertex(leftOuterEdge+ofVec3f(triOff, ledHeight, 0.));
        meshLed.addVertex(leftOuterEdge+ofVec3f(0, 0, ledWidth));
        
        meshLed.addVertex(topEdge+ofVec3f(0, 0, -ledWidth));
        meshLed.addVertex(topEdge+ofVec3f(0, -ledHeight*2, 0.));
        meshLed.addVertex(topEdge+ofVec3f(0, 0, ledWidth));
        
        meshLed.addVertex(rightOuterEdge+ofVec3f(0, 0, -ledWidth));
        meshLed.addVertex(rightOuterEdge+ofVec3f(-triOff, ledHeight, 0.));
        meshLed.addVertex(rightOuterEdge+ofVec3f(0, 0, ledWidth));
        
        meshLed.addVertex(rightInnerEdge+ofVec3f(0, 0, -ledWidth));
        meshLed.addVertex(rightInnerEdge+ofVec3f(0, ledHeight, 0.));
        meshLed.addVertex(rightInnerEdge+ofVec3f(0, 0, ledWidth));
        

        for(int i = 0; i < 12; i+=3){
            meshLed.addIndex(i+0);
            meshLed.addIndex(i+1);
            meshLed.addIndex(i+3);
            meshLed.addIndex(i+3);
            meshLed.addIndex(i+1);
            meshLed.addIndex(i+4);
        }

        for(int i = 0; i < 12; i+=3){
            meshLed.addIndex(i+2);
            meshLed.addIndex(i+1);
            meshLed.addIndex(i+5);
            meshLed.addIndex(i+5);
            meshLed.addIndex(i+1);
            meshLed.addIndex(i+4);
        }
        // endCaps
        meshLed.addIndex(0);
        meshLed.addIndex(1);
        meshLed.addIndex(2);
        meshLed.addIndex(12);
        meshLed.addIndex(13);
        meshLed.addIndex(14);
        
        // TexCoords
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 0/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 0/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 0/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 120/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 120/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 120/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 650/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 650/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 650/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 1180/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 1180/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 1180/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 1300/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 1300/1300));
//        meshLed.addTexCoord(ofVec2f((index*3+1.5)/120, 1300/1300));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 0));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 0));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 0));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 120));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 120));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 120));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 650));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 650));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 650));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 1180));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 1180));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 1180));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 1300));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 1300));
        meshLed.addTexCoord(ofVec2f((index*3+1.5), 1300));
        
        
        // create profile mesh
        meshProfile.addVertex(leftInnerEdge+ofVec3f(0, 0, -ledWidth));
        meshProfile.addVertex(leftInnerEdge+ofVec3f(0, -profileHeight, -profileWidth));
        meshProfile.addVertex(leftInnerEdge+ofVec3f(0, -profileHeight, profileWidth));
        meshProfile.addVertex(leftInnerEdge+ofVec3f(0, 0, ledWidth));

        meshProfile.addVertex(leftOuterEdge+ofVec3f(0, 0, -ledWidth));
        meshProfile.addVertex(leftOuterEdge+ofVec3f(-profileOff, -profileHeight, -profileWidth));
        meshProfile.addVertex(leftOuterEdge+ofVec3f(-profileOff, -profileHeight, profileWidth));
        meshProfile.addVertex(leftOuterEdge+ofVec3f(0, 0, ledWidth));
        
        meshProfile.addVertex(topEdge+ofVec3f(0, 0, -ledWidth));
        meshProfile.addVertex(topEdge+ofVec3f(0, profileHeight*2, -profileWidth));
        meshProfile.addVertex(topEdge+ofVec3f(0, profileHeight*2, profileWidth));
        meshProfile.addVertex(topEdge+ofVec3f(0, 0, ledWidth));
        
        meshProfile.addVertex(rightOuterEdge+ofVec3f(0, 0, -ledWidth));
        meshProfile.addVertex(rightOuterEdge+ofVec3f(profileOff, -profileHeight, -profileWidth));
        meshProfile.addVertex(rightOuterEdge+ofVec3f(profileOff, -profileHeight, profileWidth));
        meshProfile.addVertex(rightOuterEdge+ofVec3f(0, 0, ledWidth));
        
        meshProfile.addVertex(rightInnerEdge+ofVec3f(0, 0, -ledWidth));
        meshProfile.addVertex(rightInnerEdge+ofVec3f(0, -profileHeight, -profileWidth));
        meshProfile.addVertex(rightInnerEdge+ofVec3f(0, -profileHeight, profileWidth));
        meshProfile.addVertex(rightInnerEdge+ofVec3f(0, 0, ledWidth));
        
        
        for(int i = 0; i < 16; i+=4){
            meshProfile.addIndex(i+0);
            meshProfile.addIndex(i+1);
            meshProfile.addIndex(i+4);
            meshProfile.addIndex(i+4);
            meshProfile.addIndex(i+1);
            meshProfile.addIndex(i+5);
        }
        for(int i = 0; i < 16; i+=4){
            meshProfile.addIndex(i+1);
            meshProfile.addIndex(i+2);
            meshProfile.addIndex(i+5);
            meshProfile.addIndex(i+5);
            meshProfile.addIndex(i+2);
            meshProfile.addIndex(i+6);
        }
        for(int i = 0; i < 16; i+=4){
            meshProfile.addIndex(i+2);
            meshProfile.addIndex(i+3);
            meshProfile.addIndex(i+6);
            meshProfile.addIndex(i+6);
            meshProfile.addIndex(i+3);
            meshProfile.addIndex(i+7);
        }
        meshProfile.addIndex(1);
        meshProfile.addIndex(0);
        meshProfile.addIndex(2);
        meshProfile.addIndex(2);
        meshProfile.addIndex(0);
        meshProfile.addIndex(3);
        meshProfile.addIndex(17);
        meshProfile.addIndex(16);
        meshProfile.addIndex(18);
        meshProfile.addIndex(18);
        meshProfile.addIndex(16);
        meshProfile.addIndex(19);

        
        
    };

    void draw(){
        drawMeshLed();
        drawMeshProfile();
    }
    
    void drawMeshLed(){
        ofSetColor(255);
        meshLed.draw();
    }
    void drawMeshProfile(){
        ofSetColor(0);
        meshProfile.draw();
    }
    
    ofVec3f pos;
    

    // Add to edges
    ofVec3f topEdge;
    ofVec3f leftOuterEdge;
    ofVec3f rightOuterEdge;
    ofVec3f leftInnerEdge;
    ofVec3f rightInnerEdge;
    
    int index;
    
    ofMesh meshLed;
    ofMesh meshProfile;

    
    // DIMENSIONS
    const float topHeight = 3.6f;
    const float bottomHeight = 0.3f;
    const float outerWidth = 2.4f;
    const float innerWidth = 1.1f;
};


#endif /* Gate_h */
