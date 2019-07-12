//
//  PolyBackground.hpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#ifndef PolyBackground_hpp
#define PolyBackground_hpp

#include <stdio.h>
#include "ofMain.h"

class PolyBackground{

public :
    
    PolyBackground();
    PolyBackground(ofParameterGroup* pg);
    void draw();
    void createObstacle();
    void addObstacle(int & nb);
    
    ofVec2f isInside( ofPoint p);
    
    bool isEditing;
    vector<ofPolyline> listOfPoly;
    
    
    //Parameters Group
    ofParameterGroup* pg;
    ofParameter<int> lineSize;
    ofParameter<int> nbObstacle;
    
    
    
    
    
};

#endif /* PolyBackground_hpp */
