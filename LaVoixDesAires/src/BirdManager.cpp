//
//  bird_manager.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#include "BirdManager.hpp"

BirdManager::BirdManager(){
    
    listOfBird.clear();
    
}

BirdManager::BirdManager(PolyBackground* poly){
    
    listOfBird.clear();
    polyBg = poly;
    
}

void BirdManager::update(){

    ofPoint targetMouse = ofPoint(ofGetMouseX(), ofGetMouseY());
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
        it->update(targetMouse);
    }
    
}

void BirdManager::draw(bool debug){
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
        it->drawBasic();
    }
    
    
    
}

void BirdManager::addBird(char l, int order){
    
    Bird newBird = Bird(l, order, ofGetWidth(), ofGetHeight(), polyBg);
    listOfBird.push_back(newBird);
    
}

void BirdManager::newSequence(string word){
    
    
    for( int i = 0 ; i< word.size(); i++)
    {
        addBird( (char)word[i] ,i );
    }
    
}


void BirdManager::killAll(){
    
    listOfBird.clear();
}
