//
//  Letter.hpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 31/10/2021.
//

#ifndef Letter_hpp
#define Letter_hpp

#include <stdio.h>
#include "ofMain.h"
#include "BirdManager.hpp"

class Letter
{
    
public:
    
    Letter();
    Letter(char text_letter, vector<ofPolyline> p, vector<int> n);
    
    void init();
    void update(BirdManager*);
    void drawBasic(float zoom, int alpha);
    void drawDebug();
    
    int iteration;
    int iterationMax;
    
    std::vector<ofPolyline> listOfPolyline;
    std::vector<int> listOfNiche;
    uint32_t text_letter;
    bool associatedToBirds;
    

    
    
};

#endif /* Letter_hpp */
