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
    void draw();
    ofVec2f isInside( ofPoint p);
    
    bool isEditing;
    vector<ofPolyline> listOfPoly;
    
    
    
    
    
};

#endif /* PolyBackground_hpp */
