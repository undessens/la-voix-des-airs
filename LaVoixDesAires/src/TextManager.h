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
    ofPolyline simplifyPolyline(int letter, ofVec2f letterPosition);
    void addPathWithCustomSpacing(int letter, ofVec2f position);
    void addPathSimple();

    void changeFontSize(int &newSize);
    void changeFontSpacing(int &newSpacing);
    void changeMsgPositionX(int &x);
    void changeMsgPositionY(int &y);
    void changeFontSampling(float &s);
    
	//Font & Text
    int fontSize;
    int fontSpacing;
    float fontDistSampling;
    ofTrueTypeFont msgFont ;
    ofTrueTypeFont birdFont;
    string msgFontName = "Raleway-Medium.ttf";
//    string msgFontName =  "verdana.ttf";
    string birdFontName = "Raleway-Dots.ttf";
    
    
    string msg;
    ofPoint msgPosition;
	
    //Polyline
    std::vector<ofPolyline> msgPolys;
    std::vector<ofPath> msgPaths;
    
    //BirdManager
    BirdManager* birdmanager;

    //OfParameter group
    ofParameterGroup* pg;
    
    //Parameter
    ofParameter<bool> drawMsgFill;
    ofParameter<bool> drawMsgContour;
    ofParameter<int> gFontSize;
    ofParameter<int> gFontSpacing;
    ofParameter<int> gMsgPositionX;
    ofParameter<int> gMsgPositionY;
    ofParameter<float> gfontDistSampling;
};

