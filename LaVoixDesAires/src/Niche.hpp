//
//  Niche.hpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 31/05/2022.
//

#ifndef Niche_hpp
#define Niche_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Bird.hpp"
class Niche {
    
    
public:
    Niche();
    void draw();
    vector<Bird> listOfBird;
    
    
};

#endif /* Niche_hpp */
