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
#include "ofxXmlSettings.h"

#define POLYBACKGROUND_NUM 8
#define POINTPERPOLYLINE_NUM 8

class PolyBackground{

public :
    
    PolyBackground();
    PolyBackground(ofParameterGroup* pg, int w, int h);
    void init();
    void draw();
    void clear(bool & b);
    int getClosestPoint(ofVec2f);
    bool isEditing;
    vector<ofPolyline> listOfPolyline;
    
    ofPolyline createRandomPolyline();
    void clickForMove(ofVec2f mouse);
    void clickForSelect(ofVec2f mouse);
    ofPolyline getCurrentSelected();

    
    
    //Selection radius
    int radius = 20;
    
    //Parameters Group
    ofParameterGroup* pg;
    ofParameter<int> currentPolylineSelected;
    ofParameter<int> currentPointSelected;
    ofParameter<bool> isDraw;
    ofParameter<bool> isAddObstacle;
    ofParameter<bool> clearButton;
    ofParameter<bool> save;
    
    //FBO image for pencil. Draw on it from mouse and save to file
    ofFbo fbo; //TODO : not used ?
    
    //Xml settings, save settings
    void saveToXml(bool & isSave);
    bool loadXml();
    ofxXmlSettings xml;
    const string xml_file = "polybackgroundXML.xml";
    
    
    
    //Geometry
    int w;
    int h;
    
    
    
    
    
};

#endif /* PolyBackground_hpp */
