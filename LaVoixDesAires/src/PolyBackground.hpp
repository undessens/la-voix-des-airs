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
    PolyBackground(ofParameterGroup* pg, int w, int h);
    void draw();
    void addObstacle(ofVec2f p);
    void clear(bool & b);
    ofVec2f getClosestPoint(ofVec2f);
    void saveFboToFile(bool & b);   //Save fbo to png image
    void pencilOnFbo();
    
    bool isEditing;
    vector<ofVec2f> listOfPoint;
    
    //Parameters Group
    ofParameterGroup* pg;
    ofParameter<int> nbObstacle;
    ofParameter<bool> isDraw;
    ofParameter<bool> isAddObstacle;
    ofParameter<bool> clearButton;
    ofParameter<int> radius;
    ofParameter<bool> isPencil;
    ofParameter<bool> saveImage;
    
    //FBO image for pencil. Draw on it from mouse and save to file
    ofFbo fbo;
    ofImage img;
    
    //Geometry
    int w;
    int h;
    
    
    
    
    
};

#endif /* PolyBackground_hpp */
