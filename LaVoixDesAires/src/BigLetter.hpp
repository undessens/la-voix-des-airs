//
//  Letter.hpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 31/10/2021.
//

#ifndef BigLetter_hpp
#define BigLetter_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Niche.hpp"

class BigLetter
{
    
public:
    
    BigLetter();
    BigLetter(vector<ofPolyline> p, int w, int h);
    
    void init();
    void update(vector<Niche>* listOfNiche);
    void drawBig(float zoom, int alpha);
    void drawSmall(float zoom, int alpha);
    void drawDebug();
    
    int iteration;
    int iterationMax; // duration in frame
    
    std::vector<ofPolyline> listOfPolyline;
    int scale;
    
    int w, h;

};

#endif /* BigLetter_hpp */
