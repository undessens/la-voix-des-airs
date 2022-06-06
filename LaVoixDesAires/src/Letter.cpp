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
    bigLetter = BigLetter(listOfPolyline, w, h);
    //At this point, list of polyline is centered on screen.
    //Move it to final position
    for( auto &polyline : shape){
        polyline.translate(positionSmallLetter);
    }
    nicheManager = _nicheManager;
    listOfNiche = nicheManager->createNicheFromPolyline(bigLetter.listOfPolyline ,  listOfPolyline);

    
    
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
            
            break;
        case StateOfLetter::SMALL_LETTER:
            
            break;
            
        default:
            break;
    }
    
    
}

//--------------------------------------------------------------
void Letter::drawLetter(){
    
    switch (state) {
        case StateOfLetter::BIG_LETTER:
            bigLetter.drawBasic(1, 127);
            //bigLetter.drawDebug();
            break;
        case StateOfLetter::FLYING:
            bigLetter.drawDebug();
            break;
        case StateOfLetter::SMALL_LETTER:
            for (auto &niche : listOfNiche) // access by reference to avoid copying
            {
                nicheManager->drawSmallLetter(niche);
            }
            break;
            
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void Letter::update(){
    
    for (auto &niche : listOfNiche) // access by reference to avoid copying
    {
        nicheManager->update(niche);
    }
    
    bigLetter.update(&listOfNiche);
    
    

    
    
    
// MAJ du BIG LETTER en fonction de la poisiton des oiseaux.
//    for( int i =0; i<listOfPolyline.size(); i++){
//
//        ofPolyline newPolyline;
//
//        if(birdManager->listOfBird[associatedNichee].size()==listOfPolyline[i].size() ){
//
//            for(int j=0; j<listOfPolyline[i].size(); j++){
//
//                ofVec2f birdPos = birdManager->listOfBird[associatedNichee].at(j).pos;
//                ofPoint p = (listOfPolyline[i])[j];
//                newPolyline.addVertex(birdPos.x, birdPos.y);
//            }
//
//            //newPolyline.close();
//            listOfPolyline.at(i) = newPolyline;
//
//
//
//        }else{
//            cout << "Leter update ERROR, associated nichee and listOfPyline sizes does not match \n";
//        }
//
//    }
    
    
    
}
