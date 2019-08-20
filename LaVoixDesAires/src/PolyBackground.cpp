//
//  PolyBackground.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#include "PolyBackground.hpp"

PolyBackground::PolyBackground(){
    
    
}

PolyBackground::PolyBackground(ofParameterGroup* _pg, int _w, int _h){
    
    //Listener
    clearButton.addListener(this,&PolyBackground::clear);
    saveImage.addListener(this, &PolyBackground::saveFboToFile);
    
    //Parameters
    pg = _pg;
    pg->setName("polyBg");
    pg->add(isDraw.set("draw", false));
    pg->add(nbObstacle.set("nbObstacle", 5, 1, 10));
    pg->add(isAddObstacle.set("add obstacle", false));
    pg->add(radius.set("radius", 40, 1, 200));
    pg->add(clearButton.set("clear", false));
    pg->add(isPencil.set("use Pencil", false));
    pg->add(saveImage.set("save", false));
    
    //List of Point/obstacle
    listOfPoint.clear();
    
    //Geomtry
    w = _w;
    h = _h;
    //w = 100;
    //h = 100;
  
    
    //Fbo and image for saving
    fbo.allocate(w,h, GL_RGBA);
    fbo.begin();
    ofClear(255,255,255, 0);
    ofBackground(255);
    fbo.end();
}

void PolyBackground::addObstacle(ofVec2f p){
    
    listOfPoint.push_back(p);
    
}

void PolyBackground::draw(){
    
    ofSetLineWidth(1);
    if(isDraw)
    {
        for( vector<ofVec2f>::iterator it = listOfPoint.begin(); it < listOfPoint.end() ; it++)
        {
            ofSetColor(255);
            ofDrawCircle( *it, radius);
        }
    }
    
}

void PolyBackground::clear(bool & b){
    
    listOfPoint.clear();
    clearButton = false;

    
}

ofVec2f PolyBackground::getClosestPoint(ofVec2f p){
    
    ofVec2f ret = ofVec2f(0, 0);
    
    for( vector<ofVec2f>::iterator it = listOfPoint.begin(); it < listOfPoint.end() ; it++)
    {
        
        if(p.distance(*it)<radius)
        {
            ret = *it;
            break;
        }
        
    }
    
    return ret;
}

void PolyBackground::pencilOnFbo(){
    
    if(isPencil && ofGetMousePressed())
    {
        fbo.begin();
        ofSetColor(0, 0, 50);
        ofDrawCircle(ofGetMouseX() * (w*1.0) /ofGetWidth() , ofGetMouseY() * (h*1.0)/ofGetHeight() , 1);
        fbo.end();
    }
    
    
    
}

void PolyBackground::saveFboToFile(bool & b){
    
    if(b)
    {
        ofPixels pix;
        fbo.readToPixels(pix);
        img.setFromPixels(pix);
        img.save("fbo_pencil.png");
        //Wait 500ms just for user to see the check sign on save button
        ofSleepMillis(500);
        saveImage = false;
    }
}


