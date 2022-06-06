//
//  Letter.hpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 31/05/2022.
//

#ifndef Letter_hpp
#define Letter_hpp

#include <stdio.h>
#include "Niche.hpp"
#include "ofMain.h"
#include "BigLetter.hpp"
//#include "LetterManager.hpp"
#include "NicheManager.hpp"

class Letter
{
    
public:
    
    Letter();
    Letter(char _c, ofVec2f p, vector<ofPolyline> liste, NicheManager* nicheManager, int w, int h);
    
    enum class StateOfLetter { BIG_LETTER = 1, FLYING, SMALL_LETTER , NONE};
    StateOfLetter state;
    
    vector<Niche> listOfNiche;
    ofVec2f positionSmallLetter;
    ofVec2f positionBigLetter;
    void update();
    void draw();
    void drawLetter();
    void drawBirds();
    char c;
    int w;
    int h;
    
    
private:
    BigLetter bigLetter;
    NicheManager* nicheManager;
    vector<ofPolyline> listOfPolyline;
    
    
};

#endif /* Letter_hpp */
