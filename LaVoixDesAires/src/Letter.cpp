//
//  Letter.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 31/05/2022.
//

#include "Letter.hpp"


//--------------------------------------------------------------
Letter::Letter(){
    
    positionBigLetter = ofVec2f(0, 0);
    state = StateOfLetter::NONE;
    
    /*
     Ancienne méthode pour créer le polyline a partir d'une lettre
     refait à partir d'ici.
     */
//    vector<int> listOfNiche;
//    vector<ofPolyline> listOfPolyline = reduceDistanceSampling( pathLetterToBird);
//    ofPolyline bigLetterPolyline;
//    vector<ofPolyline> listOfBigPolyline;
//    ofVec2f translation;
//
//    for(int i=0; i<listOfPolyline.size(); i++){
//        bigLetterPolyline = listOfPolyline[i];
//        bigLetterPolyline.scale(zoomBigLetter, zoomBigLetter);
//        if(i==0){
//            ofVec2f centroid = bigLetterPolyline.getCentroid2D();
//            ofRectangle boudingBox = bigLetterPolyline.getBoundingBox();
//            translation =  ofVec2f(w/2, h/2) - centroid;
//            translation.x -= 150 +ofRandom(- w/3, w/3); // RANDOMISATION DE LA POSITION
//            translation.y +=  ofRandom(-200, 200);
//
//
//        }
//        bigLetterPolyline.translate(translation);
//        //TODO : add writing speed in argument to ajust fly duration.
//        int niche = birdmanager->addBirdFromPolyline(listOfPolyline[i],bigLetterPolyline);
//        listOfNiche.push_back(niche);
//        listOfBigPolyline.push_back(bigLetterPolyline);
//    }
//
//    // NOT AN ERROR . PathLetterToDraw does not have Outline>0 . So it's easier to user pathLetterToBird instead.
//    if(pathLetterToBird.getOutline().size()>0){
//        msgPaths.push_back(pathLetterToDraw);
//    }
//
//    myLetter =  BigLetter(letter, listOfBigPolyline, listOfNiche );
    
    
    
}

//--------------------------------------------------------------
Letter::Letter(char _c, ofVec2f p, vector<ofPolyline> shape, NicheManager* _nicheManager, int _w, int _h){
    
    c = _c;
    positionSmallLetter = p;
    state = StateOfLetter::BIG_LETTER;
    listOfPolyline = shape;
    w = _w;
    h = _h;
    
    //At this point, list of polyline is 0,0 position
    //Move it to final position
    for( auto &polyline : listOfPolyline){
        polyline.translate(positionSmallLetter);
    }
    bigLetter = BigLetter(listOfPolyline, w, h);
    nicheManager = _nicheManager;
    
    listOfNiche = nicheManager->createNicheFromPolyline(bigLetter.listOfPolyline ,  listOfPolyline);
    for( auto &niche : listOfNiche){
        nicheManager->addNeighbourFromNiche(niche);
    }
    
}

//--------------------------------------------------------------
void Letter::draw(){
 
    drawLetter();
    drawBirds();
    
}

//--------------------------------------------------------------
void Letter::drawBirds(){
    
    switch (state) {
        case StateOfLetter::BIG_LETTER:
            for (auto &niche : listOfNiche) // access by reference to avoid copying
            {
                nicheManager->drawBirds(niche);
            }
        
            break;
        case StateOfLetter::FLYING:
            //bird
            for (auto &niche : listOfNiche) // access by reference to avoid copying
            {
                nicheManager->drawBirds(niche);
            }
            break;
        case StateOfLetter::SMALL_LETTER:
            for (auto &niche : listOfNiche) // access by reference to avoid copying
            {
                //nicheManager->drawBirds(niche); // Small letter, no birds anymore
            }
            break;
            
        default:
            break;
    }
    
    
}

//--------------------------------------------------------------
void Letter::drawLetter(){
    
    switch (state) {
        case StateOfLetter::BIG_LETTER:
            bigLetter.drawBig(1, 245);
            //bigLetter.drawDebug();
            bigLetter.drawFinal(120);
            break;
        case StateOfLetter::FLYING:
            //bigLetter.drawBasic(1, 127);
            //bigLetter.drawDebug();
            break;
        case StateOfLetter::SMALL_LETTER:
            // draw outline HERE
            //for (auto &niche : listOfNiche) // access by reference to avoid copying
            //{
            //nicheManager->drawSmallLetter(niche);
            //}
            // Draw fill Path here : CPU Consoming
            bigLetter.drawSmall(1, 245);
            break;
            
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void Letter::update(){
    
    /*
     #define BIRD_WAITING 0
     #define BIRD_FREE 1
     #define BIRD_GOTOTARGET 2
     #define BIRD_TARGETJOINED 3
     #define BIRD_DIEONBORDER 4
     #define BIRD_DIE 5
     */
    
    int minimalState = 100;
    //Update Birds in all situation first
    for (auto &niche : listOfNiche) // access by reference to avoid copying
    {
        minimalState = min(nicheManager->update(niche), minimalState);
    }
    
    switch (state) {
        case StateOfLetter::BIG_LETTER:
            bigLetter.update(&listOfNiche);
            if(bigLetter.iteration>bigLetter.iterationMax){
            //if (minimalState>= 1) { // accorder le changement par rapport aux oiseaux qui attendent, ou la lettre qui s'estompe ?
                state = StateOfLetter::FLYING;
            }
            break;
        case StateOfLetter::FLYING :
            //bigLetter.update(&listOfNiche);
            if(minimalState>= 3){
                state = StateOfLetter::SMALL_LETTER;
                bigLetter.update(&listOfNiche);
            }
            break;
        case StateOfLetter::SMALL_LETTER:
            bigLetter.update(&listOfNiche);
            if(minimalState>= 5){
                state = StateOfLetter::NONE;
            }
            break;
        
            
        default:
            break;
    }
    
    
    
    

    

    
    
    
}
