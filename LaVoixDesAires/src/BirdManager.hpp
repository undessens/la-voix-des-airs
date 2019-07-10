//
//  bird_manager.hpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#ifndef bird_manager_hpp
#define bird_manager_hpp
#include "Bird.hpp"
#include "PolyBackground.hpp"
#include "ofMain.h"

#include <stdio.h>

class BirdManager
{
    
public:
    
    BirdManager();
    BirdManager(PolyBackground* poly);
    void update();
    void draw( bool debug);
    void addBird(char letter, int order);
    void changeToLetter();
    void newSequence( string word  );
    void killAll();
    
    
    
    vector<Bird> listOfBird;
    PolyBackground* polyBg;
    
    
    
    
};

#endif /* bird_manager_hpp */
