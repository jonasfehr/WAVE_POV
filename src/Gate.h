//
//  Gate.h
//  WAVE_POV
//
//  Created by Frederik Tollund Juutilainen on 04/01/17.
//
//

#ifndef Gate_h
#define Gate_h

#include "ofxMad3D.h"

#define LED_SHORT 117
#define LED_LONG 261

class Gate{
public:
    Gate(glm::vec3 position, int index){
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
        
        glm::vec3 centerLeft = glm::mix(leftOuterEdge, topEdge, 0.5);
        glm::vec3 centerRight = glm::mix(topEdge, rightOuterEdge, 0.5);
        
        
        
        // CreateFixtures
        ofxMad3D::Fixture newFixture;
        newFixture.name = "Gate_"+ofToString(index)+"LF";
        newFixture.type = "RGBW_Strip";
        newFixture.startPos = leftInnerEdge;
        newFixture.endPos = leftOuterEdge;
        newFixture.numLeds = LED_SHORT;
        newFixture.startUniverse = index*32+1;
        newFixture.startChannel = 1;
        newFixture.startUV = glm::vec2((float)(index/40.0f), (float) 0.0f);
        newFixture.endUV = glm::vec2((float)(index/40.0f), (float) LED_SHORT/1300.0f);
        fixtures.push_back(newFixture);
        
        newFixture.name = "Gate_"+ofToString(index)+"LB";
        newFixture.type = "RGBW_Strip";
        newFixture.startPos = centerLeft;
        newFixture.endPos = leftOuterEdge;
        newFixture.numLeds = LED_LONG;
        newFixture.startUniverse += 3;
        newFixture.startChannel = 1;
        newFixture.startUV = glm::vec2((float)(index/40.0f), (float)(650-3-LED_LONG)/1300.0f);
        newFixture.endUV = glm::vec2((float)(index/40.0f), (float)(650-3-LED_LONG-LED_LONG)/1300.0f);
        fixtures.push_back(newFixture);
       
        newFixture.name = "Gate_"+ofToString(index)+"LT";
        newFixture.type = "RGBW_Strip";
        newFixture.startPos = centerLeft;
        newFixture.endPos = topEdge;
        newFixture.numLeds = LED_LONG;
        newFixture.startUniverse += 3;
        newFixture.startChannel = 1;
        newFixture.startUV = glm::vec2((float)(index/40.0f), (float)(650-3-LED_LONG)/1300.0f);
        newFixture.endUV = glm::vec2((float)(index/40.0f), (float)(650-3)/1300.0f);
        fixtures.push_back(newFixture);
        
        newFixture.name = "Gate_"+ofToString(index)+"RT";
        newFixture.type = "RGBW_Strip";
        newFixture.startPos = centerRight;
        newFixture.endPos = topEdge;
        newFixture.numLeds = LED_LONG;
        newFixture.startUniverse += 3;
        newFixture.startChannel = 1;
        newFixture.startUV = glm::vec2((float)(index/40.0f), (float)(650+3+LED_LONG)/1300.0f);
        newFixture.endUV = glm::vec2((float)(index/40.0f), (float)(650+3)/1300.0f);
        fixtures.push_back(newFixture);
        
        newFixture.name = "Gate_"+ofToString(index)+"RB";
        newFixture.type = "RGBW_Strip";
        newFixture.startPos = centerRight;
        newFixture.endPos = rightOuterEdge;
        newFixture.numLeds = LED_LONG;
        newFixture.startUniverse += 3;
        newFixture.startChannel = 1;
        newFixture.startUV = glm::vec2((float)(index/40.0f), (float)(650+3+LED_LONG)/1300.0f);
        newFixture.endUV = glm::vec2((float)(index/40.0f), (float)(650+3+LED_LONG+LED_LONG)/1300.0f);
        fixtures.push_back(newFixture);
        
        newFixture.name = "Gate_"+ofToString(index)+"RF";
        newFixture.type = "RGBW_Strip";
        newFixture.startPos = rightInnerEdge;
        newFixture.endPos = rightOuterEdge;
        newFixture.numLeds = LED_SHORT;
        newFixture.startUniverse += 3;
        newFixture.startChannel = 1;
        newFixture.startUV = glm::vec2((float)(index/40.0f), (float)(1.0f));
        newFixture.endUV = glm::vec2((float)(index/40.0f), (float)(1300-LED_SHORT)/1300.0f);
        fixtures.push_back(newFixture);
        
        
        // create a meshLed to visualize the result
        
        float ledHeight = 0.021;
        float ledWidth = 0.012;
        float triOff = ledHeight*1.732;
        float profileHeight = 0.091;
        float profileWidth = 0.065;
        float profileOff = profileHeight*1.732;

        // create led mesh
        meshLed.addVertex(leftInnerEdge+glm::vec3(0, 0, -ledWidth));
        meshLed.addVertex(leftInnerEdge+glm::vec3(0, ledHeight, 0.));
        meshLed.addVertex(leftInnerEdge+glm::vec3(0, 0, ledWidth));
        
        meshLed.addVertex(leftOuterEdge+glm::vec3(0, 0, -ledWidth));
        meshLed.addVertex(leftOuterEdge+glm::vec3(triOff, ledHeight, 0.));
        meshLed.addVertex(leftOuterEdge+glm::vec3(0, 0, ledWidth));
        
        meshLed.addVertex(topEdge+glm::vec3(0, 0, -ledWidth));
        meshLed.addVertex(topEdge+glm::vec3(0, -ledHeight*2, 0.));
        meshLed.addVertex(topEdge+glm::vec3(0, 0, ledWidth));
        
        meshLed.addVertex(rightOuterEdge+glm::vec3(0, 0, -ledWidth));
        meshLed.addVertex(rightOuterEdge+glm::vec3(-triOff, ledHeight, 0.));
        meshLed.addVertex(rightOuterEdge+glm::vec3(0, 0, ledWidth));
        
        meshLed.addVertex(rightInnerEdge+glm::vec3(0, 0, -ledWidth));
        meshLed.addVertex(rightInnerEdge+glm::vec3(0, ledHeight, 0.));
        meshLed.addVertex(rightInnerEdge+glm::vec3(0, 0, ledWidth));
        

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
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 0));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 0));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 0));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 120));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 120));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 120));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 650));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 650));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 650));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 1180));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 1180));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 1180));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 1300));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 1300));
        meshLed.addTexCoord(glm::vec2((index*3+1.5), 1300));
        
        
        // create profile mesh
        meshProfile.addVertex(leftInnerEdge+glm::vec3(0, 0, -ledWidth));
        meshProfile.addVertex(leftInnerEdge+glm::vec3(0, -profileHeight, -profileWidth));
        meshProfile.addVertex(leftInnerEdge+glm::vec3(0, -profileHeight, profileWidth));
        meshProfile.addVertex(leftInnerEdge+glm::vec3(0, 0, ledWidth));

        meshProfile.addVertex(leftOuterEdge+glm::vec3(0, 0, -ledWidth));
        meshProfile.addVertex(leftOuterEdge+glm::vec3(-profileOff, -profileHeight, -profileWidth));
        meshProfile.addVertex(leftOuterEdge+glm::vec3(-profileOff, -profileHeight, profileWidth));
        meshProfile.addVertex(leftOuterEdge+glm::vec3(0, 0, ledWidth));
        
        meshProfile.addVertex(topEdge+glm::vec3(0, 0, -ledWidth));
        meshProfile.addVertex(topEdge+glm::vec3(0, profileHeight*2, -profileWidth));
        meshProfile.addVertex(topEdge+glm::vec3(0, profileHeight*2, profileWidth));
        meshProfile.addVertex(topEdge+glm::vec3(0, 0, ledWidth));
        
        meshProfile.addVertex(rightOuterEdge+glm::vec3(0, 0, -ledWidth));
        meshProfile.addVertex(rightOuterEdge+glm::vec3(profileOff, -profileHeight, -profileWidth));
        meshProfile.addVertex(rightOuterEdge+glm::vec3(profileOff, -profileHeight, profileWidth));
        meshProfile.addVertex(rightOuterEdge+glm::vec3(0, 0, ledWidth));
        
        meshProfile.addVertex(rightInnerEdge+glm::vec3(0, 0, -ledWidth));
        meshProfile.addVertex(rightInnerEdge+glm::vec3(0, -profileHeight, -profileWidth));
        meshProfile.addVertex(rightInnerEdge+glm::vec3(0, -profileHeight, profileWidth));
        meshProfile.addVertex(rightInnerEdge+glm::vec3(0, 0, ledWidth));
        
        
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
    
    void showFixtureInfo(){
        if(fixtures.size()>0){
            string info;
            info += "Gate          : " + ofToString(index+1);
            info += "\nStartUniverse : " + ofToString(fixtures[0].startUniverse);
            info += "\nStartChannel  : " + ofToString(fixtures[0].startChannel);
            info += "\n ";
            ofDrawBitmapStringHighlight(info, 15+150, ofGetHeight()-4*15);
        }
    }
    
    glm::vec3 pos;
    

    // Add to edges
    glm::vec3 topEdge;
    glm::vec3 leftOuterEdge;
    glm::vec3 rightOuterEdge;
    glm::vec3 leftInnerEdge;
    glm::vec3 rightInnerEdge;
    
    int index;
    
    ofMesh meshLed;
    ofMesh meshProfile;

    
    // DIMENSIONS
    const float topHeight = 3.6f;
    const float bottomHeight = 0.3f;
    const float outerWidth = 2.4f;
    const float innerWidth = 1.1f;
    
    vector<ofxMad3D::Fixture> fixtures;
};


#endif /* Gate_h */
