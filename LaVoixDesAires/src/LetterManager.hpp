#pragma once
#ifndef LetterManager_hpp
#define LetterManager_hpp

#include "ofMain.h"
#include "NicheManager.hpp"
#include "Letter.hpp"
#define MAX_LETTER 300
#define MAX_LETTER_PER_LINE 44


class LetterManager
{
public:
    LetterManager();
    LetterManager(NicheManager* b, ofParameterGroup* pg, int w, int h);
    ~LetterManager();
    
	void draw();
    void drawLetter();
    void drawBirds();
    void drawDebug();
    void update();
    void clear();
	
	void addLetter(int c);
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
    string msgFontName = "atwriter.ttf"; //TODO supprimer la distinction entre les 2
    string birdFontName = "atwriter.ttf"; // plus lieu d'être.
    
    int w, h;
    
    string msg;
    int currentLineCharacter;
    ofPoint msgPosition;
    ofPoint nextLetterPosition;
	float timeOfLastLetter[MAX_LETTER];
    float writingSpeed; // Change fly duration according to writing speed
	
    
    // C'est ici que tout se joue :
    // LIST OF LETTER .
    vector<Letter> listOfLetter;

    //BirdManager
    NicheManager* nicheManager;

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

#endif /* letterManager_hpp */
