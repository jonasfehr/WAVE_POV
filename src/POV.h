//
//  POV.h
//  WAVE_POV
//
//  Created by Frederik Tollund Juutilainen on 05/01/17.
//
//

#ifndef POV_h
#define POV_h


//class SimplePlane{
//public:
//    SimplePlane(){
//        this->center = ofVec3f(0,0,0);
//        this->normal = ofVec3f(0,0,-1);
//        this->up = ofVec3f(0,1,0);
//        this->right = ofVec3f(-1,0,0);
//    };
//    
//    void setup(ofVec3f center, ofVec3f normal, ofVec3f up, ofVec3f right){
//        this->center = center;
//        this->normal = normal;
//        this->up = up;
//        this->right = right;
//    }
//    
//    ofVec3f center;
//    ofVec3f normal;
//    ofVec3f up;
//    ofVec3f right;
//    
//    void recalcUp(){
//        up = ofVec3f(0,1,0) - normal * ofVec3f(0,1,0).dot(normal);
//        up.normalize();
//    }
//    void recalcRight(){
//        right = normal.getCrossed(up);
//        //right = right - normal * right.dot(normal);
//        right.normalize();
//    }
//    
//    void draw(){
//        ofPushMatrix();
//        ofTranslate(center);
//        ofSetColor(ofColor::red);
//        //ofDrawLine(ofVec3f(0), normal);
//        ofDrawArrow(ofVec3f(0), normal, 0.1);
//        ofSetColor(ofColor::orange);
//        //ofDrawLine(ofVec3f(0), up);
//        ofDrawArrow(ofVec3f(0), up, 0.1);
//        ofSetColor(ofColor::green);
//        //ofDrawLine(ofVec3f(0), right);
//        ofDrawArrow(ofVec3f(0), right, 0.1);
//        ofRotateY(ofVec3f(1,0,0).angle(right));
//
//        ofSetColor(ofColor::white, 50);
//        ofDrawGrid(1, 1, false, false, false, true);
//        ofPopMatrix();
//    }
//};
//
//
//class POV{
//public:
//    POV(){
//    };
//    
//    void update(ofCamera *cam){
//        
//        // recalc the Plane
//        viewDirection = calcViewDirection();
//        plane.center = position + viewDirection * 2.1105296161/2;
//        plane.normal = viewDirection * (2-2.1105296161/2);
//        plane.recalcUp();
//        plane.recalcRight();
//        
//        // Calculate the UV points
//        for(auto& e : edges){
//           // e.intersect = rayPlaneIntersec(e.pos, position, plane);
//           // e.uv = calcUVfromIntersect(e.intersect, plane);
//           // e.uv = calcUV(e.pos, plane);
//            ofVec3f coord = cam->worldToScreen(e.pos);
//            coord.x /= ofGetWidth();
//            coord.y /= ofGetHeight();
//           // coord = (coord * 2) -1;
//            coord.y = 1-coord.y;
//            e.uv = coord;
//        }
//    };
//
//    void draw(){};
//    
//    void drawPlane(){
//        plane.draw();
//    };
//    
//    void drawPovPoint(){
//        ofSetColor(ofColor::antiqueWhite);
//        ofDrawSphere(position, 0.1);
//    }
//    
//    bool hasMoved(){
//        bool hasMoved = lastPosition != position;
//        lastPosition = position;
//        return hasMoved;
//    }
//    
//    void orbit(ofVec3f pivot){
//        position.rotate((ofGetFrameNum() % 360)*0.001, pivot, ofVec3f(0,1,0));
//    };
//    
//    void setViewerHeight(float viewerHeight){ this->viewerHeight = viewerHeight; };
//    
//    
//    // ---   The functions to calculate the UV's and corresponding utilities
//    // the following two functions are inspired by https://www.shadertoy.com/view/MscSRr
//    ofVec2f povProj(ofVec3f povPos, SimplePlane plane, ofVec3f p)
//    {
//        p = p-povPos;
//        
//        float cZ = p.dot(plane.normal);
//        float cX = p.dot(plane.right);
//        float cY = p.dot(plane.up);
//        return ofVec2f(cX/cZ,cY/cZ);
//    }
//
//    ofVec3f rayPlaneIntersec(ofVec3f rayStart, ofVec3f rayEnd, SimplePlane plane)
//    {
//        float intersection = (plane.center-rayStart).dot(plane.normal) / ((rayEnd-rayStart).getNormalized()).dot(plane.normal);
//        return (rayEnd-rayStart).getNormalized().operator*(intersection)+rayStart;
//    }
//    
//    
//    ofVec2f calcUV(ofVec3f p, SimplePlane plane){
//        ofVec2f coord = povProj(position, plane, p);
//        //coord.y = -coord.y;
//        //coord.x = coord.x
//        return (coord+1)/2;
//    }
//    
//    ofVec2f calcUVfromIntersect(ofVec3f p, SimplePlane plane){
//        ofVec3f pointer = p-plane.center;
//        ofVec2f uv;
//        uv.x = pointer.dot(plane.right);
//        uv.y = pointer.dot(plane.up);
//        return uv;
//    }
//    
//    void normalizeUV(){
//        // find the extreme values
//        float xMin = 1;
//        float yMin = 1;
//        float xMax = 0;
//        float yMax = 0;
//        
//        for(auto& e : edges){
//            if(xMin > e.uv.x) xMin = e.uv.x;
//            if(yMin > e.uv.y) yMin = e.uv.y;
//            if(xMax < e.uv.x) xMax = e.uv.x;
//            if(yMax < e.uv.y) yMax = e.uv.y;
//        }
//        
//        // normalize the UVs
//        for(auto& e : edges){
//            e.uv.x = ofMap(e.uv.x, xMin, xMax, 0, 1);
//            e.uv.y = ofMap(e.uv.y, yMin, yMax, 0, 1);
//        }
//    }
//
//    ofVec3f calcViewDirection(){
//        // calculate the outherest point of the installation in order to calculate the view direction / normal
//        Edge * eStartLeft  = &edges.at(1);
//        Edge * eStartRight = &edges.at(3);
//        Edge * eStartTop   = &edges.at(2);
//        
//        Edge * eEndLeft    = &edges.at(edges.size()-3);
//        Edge * eEndRight   = &edges.at(edges.size()-1);
//        Edge * eEndTop     = &edges.at(edges.size()-2);
//        // some default def
//        Edge *eMaxLeft = eStartRight;
//        Edge *eMaxRight = eEndRight;
//        
//        // Get maxLeft and maxRight to calculate pov direction
//        if(position.z < eStartRight->pos.z){
//            if(position.x > eStartRight->pos.x){
//                // in front on the right of the installation
//                eMaxLeft = eStartLeft;
//                eMaxRight = eEndRight;
//            }
//            else if(position.x < eStartRight->pos.x && position.x > eStartLeft->pos.x){
//                // in front in the middle of the installation
//                eMaxLeft = eStartLeft;
//                eMaxRight = eStartRight;
//            }
//            else if(position.x < eStartLeft->pos.x){
//                // in front on the left of the installation
//                eMaxLeft = eEndLeft;
//                eMaxRight = eStartRight;
//            }
//        }
//        else if(position.z > eStartRight->pos.z && position.z < eEndRight->pos.z){
//            if(position.x > eStartRight->pos.x){
//                // on the right of the installation
//                eMaxLeft = eStartRight;
//                eMaxRight = eEndRight;
//            }
//            else if(position.x < eStartRight->pos.x && position.x > eStartLeft->pos.x){
//                // inside the installation
//                // HERE WE NEED AN OTHER MODUS -> SPHERICAL MAPPING OF UVs
//            }
//            else if(position.x < eStartLeft->pos.x){
//                // on the left of the installation
//                eMaxLeft = eEndLeft;
//                eMaxRight = eStartLeft;
//            }
//        }
//        else if(position.z > eEndRight->pos.z){
//            if(position.x > eStartRight->pos.x){
//                // behind on the right of the installation
//                eMaxLeft = eStartRight;
//                eMaxRight = eEndLeft;
//            }
//            else if(position.x < eStartRight->pos.x && position.x > eStartLeft->pos.x){
//                // behind in the middle of the installation
//                eMaxLeft = eEndRight;
//                eMaxRight = eEndLeft;
//            }
//            else if(position.x < eStartLeft->pos.x){
//                // behind on the left of the installation
//                eMaxLeft = eEndRight;
//                eMaxRight = eStartLeft;
//            }
//        }
//        
//        // calculate the view directions from the extreme points
//        ofVec3f pLeft = eMaxLeft->pos;
//        ofVec3f pRight = eMaxRight->pos;
//        pLeft.y = pRight.y = viewerHeight;
//        
//        ofVec3f viewDirection = (pLeft.operator-(position)).getNormalized() + (pRight.operator-(position)).getNormalized();
//        viewDirection.normalize();
//        
//        return viewDirection;
//    }
//    
//    // ---   EDGE's the container for the points where the UV's are calculated
//    struct Edge{
//        ofVec3f intersect;
//        ofVec3f pos;
//        ofVec2f uv;
//        
//    };
//    
//    
//    void addEdge(ofVec3f position){
//        Edge edge;
//        edge.pos = position;
//        edges.push_back(edge);
//    }
//    
//    vector <Edge> edges;
//    
//    
//    // ---   all the other variables
//    ofVec3f position = ofVec3f(-50,1.8,0);
//    void setPosition(ofVec3f position){ this->position = position; };
//
//    ofVec3f lastPosition;
//    float viewerHeight = 1.8;
//    ofVec3f viewDirection;
//    
//
//    SimplePlane plane;
//    
//};

#endif /* POV_h */
