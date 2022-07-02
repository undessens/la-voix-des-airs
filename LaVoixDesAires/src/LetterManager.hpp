#pragma once
#ifndef LetterManager_hpp
#define LetterManager_hpp

#include "ofMain.h"
#include "NicheManager.hpp"
#include "Letter.hpp"
#include "Niche.hpp"
#include "ofxXmlSettings.h"

#define MAX_LETTER 300
#define MAX_LETTER_PER_LINE 44


class LetterManager
{
public:
    LetterManager();
    LetterManager(NicheManager* b, PolyBackground* p, ofParameterGroup* pg, int w, int h);
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
	void addTemporaryNiche(Niche n);

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
    ofRectangle rectWithoutLetter;
    ofxXmlSettings settingsRectWithoutLetter;
    void updateRectangle(int &x);
    void saveRectangle(bool &b);
    bool loadRectangle(string path);
    
    int w, h;
    
    string msg;
    int currentLineCharacter;
    ofPoint msgPosition;
    ofPoint nextLetterPosition;
	float timeOfLastLetter[MAX_LETTER];
    float writingSpeed; // Change fly duration according to writing speed
	
    
    // C'est ici que tout se joue :
    // LIST OF LETTER .
    vector<Letter*> listOfLetter;
    
    //Invicible army of bird
    Niche permanentNiche;
    
    //Shape to draw with bird
    vector<Niche> temporaryNiche;

    //BirdManager
    NicheManager* nicheManager;
    
    PolyBackground* polybackground;

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
    ofParameter<int> rectWithoutLetterX;
    ofParameter<int> rectWithoutLetterW;
    ofParameter<bool> saveRectangleWithoutLetter;
    
    //Setter to all instances. CAN NOT be in NicheManager
    void setDebug(int &i);
    void setDebugScale(int &i);
    void setStiffness(float &f);
    void setDamping(float &f);
    void setSeparation(float &f);
    void setCohesion(float &f);
    void setAlignment(float &f);
    void setTargetAttraction(float &f);
    void setMaxSpeed(float &f);
    void setMaxForce(float &f);
    void setSize(int &f);
    void setFlyDuration(int &i);
    
    
};

#endif /* letterManager_hpp */
