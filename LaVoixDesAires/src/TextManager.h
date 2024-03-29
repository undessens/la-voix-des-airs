#pragma once
#include "ofMain.h"
#include "BirdManager.hpp"
#include "Letter.hpp"
#define MAX_LETTER 300
#define MAX_LETTER_PER_LINE 44

class TextManager
{
public:
    TextManager();
    TextManager(BirdManager* b, ofParameterGroup* pg, int w, int h);
    ~TextManager();
    
	void draw();
    void update();
    void clear();
	
	void addLetter(int c);
	ofPolyline createPolyline(int letter, ofVec2f letterPosition);
    ofPolyline simplifyPolyline(ofPolyline p);
    void addPathWithCustomSpacing(int letter, ofVec2f position);
    ofPath createPathFromLetter(int letter, ofVec2f newPos);
    vector<ofPolyline> reduceDistanceSampling(ofPath path);
    ofPath createFilledPathFromLetter(int letter, ofVec2f newPos);
    ofRectangle getBoundingBoxOfPath(ofPath &path);

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
    //string msgFontName = "Raleway-Medium.ttf";
   // string msgFontName = "typo-writer.otf";
    //string birdFontName = "typo-writer.otf";
    string msgFontName = "atwriter.ttf";
    string birdFontName = "atwriter.ttf";
    
    int w, h;
    
    
    string msg;
    int currentLineCharacter;
    ofPoint msgPosition;
    ofPoint nextLetterPosition;
	float timeOfLastLetter[MAX_LETTER];
    float writingSpeed; // Change fly duration according to writing speed
	
    
    // Ici, un ofPath par lettre, donnés dans le position finale.
    std::vector<ofPath> msgPaths;
    // TODO : change to vector of letter.
    // Meme si en gros ce sont les dernires qui sont affichées
    Letter myLetter;
    
    
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
    ofParameter<float> zoomBigLetter;
    ofParameter<int> alphaBigLetter;
};

