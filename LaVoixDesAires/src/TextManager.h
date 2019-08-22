#pragma once
#include "ofMain.h"
#include "BirdManager.hpp"

class TextManager
{
public:

    TextManager();
    TextManager(BirdManager* b, ofParameterGroup* pg);
    ~TextManager();
    
	void draw();
    void drawPoly();
    void clear();
	
	void addLetter(int c);
    void changeFontSize(int &newSize);
    void changeFontSpacing(int &newSpacing);

	//Font & Text
    int fontSize;
    int fontSpacing;
    ofTrueTypeFont msgFont;
    ofTrueTypeFont birdFont;
    
    string msg;
    ofVec2f msgPosition;
	
    //Polyline
    std::vector<ofPolyline> msgPolys;
    std::vector<ofPath> msgPaths;
    
    //BirdManager
    BirdManager* birdmanager;
    
    //Parameter
    ofParameter<bool> drawMsgFill;
    ofParameter<bool> drawMsgContour;
    ofParameter<int> gFontSize;
    ofParameter<int> gFontSpacing;
	
    //OfParameter group
    ofParameterGroup* pg;
	
};

