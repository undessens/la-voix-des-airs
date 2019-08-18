#pragma once
#include "ofMain.h"

class TextManager
{
public:
	TextManager();
	void draw();
	~TextManager();
	void addLetter(char c);


	//Font
	ofTrueTypeFont font;
	int size;

	ofVec2f startPoint;
	string text;



	
};

