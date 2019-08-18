#pragma once
#include "ofMain.h"
#include "BirdManager.hpp"

class TextManager
{
public:
	TextManager();
    TextManager(BirdManager* b);
	void draw();
    void drawPoly();
	~TextManager();
	void addLetter(char c);

	//Font
	ofTrueTypeFont font;
	int size;
    
    //Polyline
    std::vector<ofPolyline> listOfPoly;
    
    //BirdManager
    BirdManager* birdmanager;

	ofVec2f startPoint;
	string text;



	
};

