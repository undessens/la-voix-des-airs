#pragma once
#include "ofMain.h"
#include "BirdManager.hpp"

class TextManager
{
public:

    TextManager();
    TextManager(BirdManager* b, ofParameterGroup* pg);
	void draw();
    void drawPoly();
    void clear();
	~TextManager();
	void addLetter(int c);
    
    //OfParameter group
    ofParameterGroup* pg;

	//Font
	ofTrueTypeFont font ;
	int size;
    
    //Polyline
    std::vector<ofPolyline> listOfPoly;
    
    //BirdManager
    BirdManager* birdmanager;
    
    //Parameter
    ofParameter<bool> isDraw;

	ofVec2f startPoint;
	string text;



	
};

