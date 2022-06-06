//
//  Niche.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 31/05/2022.
//

#include "Niche.hpp"


Niche::Niche(){
    
}

//--------------------------------------------
void Niche::draw(){
    
    for (auto &bird : listOfBird) // access by reference to avoid copying
    {
        bird.drawBasic();
    }
    
    
}


