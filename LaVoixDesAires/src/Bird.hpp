//
//  bird.hpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#ifndef bird_hpp
#define bird_hpp
#include "ofMain.h"
#include "PolyBackground.hpp"

#include <stdio.h>

class Bird
{
    
public:
    
    Bird();
    Bird( char letter, int order, int w, int h , PolyBackground* p);
    
    void update(ofPoint t);
    void drawBasic();
    void updateAttraction(ofPoint p);
    void getEjected(ofVec2f v);
    void updateEjection(ofPoint p);
    
    ofPoint pos;
    ofPoint origin;
    ofVec2f speed;
    ofVec2f force;
    
    //Environnement
    int w;
    int h;
    PolyBackground* polyBg;
    float eject_force ;     //force being ejected for an obstacle
    bool is_ejected;        // situation of ejection during the count
    int eject_count;         // ejected until count is 0;
    ofVec2f eject_direction;
    
    float size;
    float noiseSpeed;  //noise difference between the target speed, and the real speed
    ofVec2f noisePos; // noise between the target position, and the real position
    float stiffness;  //force to join centroid
    float damping;
    
    //Letter stuff
    char letter;  // Final letter to display
    int order;
    
    //Debug stuff
    int debugLevel;
    
    
    
    
    
    
};



#endif /* bird_hpp */

