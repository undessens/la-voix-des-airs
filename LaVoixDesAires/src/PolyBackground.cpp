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
    
    //Parameters
    pg = _pg;
    pg->setName("polyBg");
    pg->add(currentPolylineSelected.set("selection", -1, -1, POLYBACKGROUND_NUM));
    pg->add(currentPointSelected.set("p_selection", -1, -1, 10));
    pg->add(isDraw.set("draw", false));
    pg->add(isAddObstacle.set("add obstacle", false));
    pg->add(clearButton.set("clear", false));
    pg->add(saveImage.set("save", false));
    
    //List of Point/obstacle
    
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
    
    init();
}

//-----------------------------------------------------------
void PolyBackground::init(){
    
    int fixNumberOfPolyline = POLYBACKGROUND_NUM;
    listOfPolyline.clear();
    for(int i=0; i<fixNumberOfPolyline; i++){
        listOfPolyline.push_back(createRandomPolyline());
    }
    
}

//-----------------------------------------------------------
void PolyBackground::draw(){
    
    ofSetLineWidth(1);
    if(isDraw)
    {
        for( int i=0; i<listOfPolyline.size(); i++)
        {
            ofPolyline p = listOfPolyline[i];
            if(i==currentPolylineSelected){
                ofSetColor(0,0,200);
            }else{
                ofSetColor(127);
            }
            p.draw();
            for(int j=0; j<p.size(); j++){
                if(i==currentPolylineSelected && j==currentPointSelected){
                    ofSetColor(0,200,0);
                }else{
                    ofSetColor(127);
                }
                ofPoint polypoint = p[j];
                ofNoFill();
                ofDrawCircle(polypoint.x, polypoint.y, 10);
            }
        }
    }
    
}
//-----------------------------------------------------------
void PolyBackground::clear(bool & b){
    
    if(listOfPolyline.size()>0 && currentPolylineSelected>-1){
            listOfPolyline[currentPolylineSelected].clear();
    }
    

}
//-----------------------------------------------------------
void PolyBackground::clickForSelect(ofVec2f mouse){
    
    int closest_point = getClosestPoint(mouse);
    if(closest_point>=0){
        currentPointSelected = closest_point;
    }
}
//-----------------------------------------------------------
void PolyBackground::clickForMove(ofVec2f mouse){
    
    int closest_point = getClosestPoint(mouse);
    if(closest_point==currentPointSelected){	
        
        ofPolyline actual = listOfPolyline.at(currentPolylineSelected);
        ofPolyline newPolyline;
        for(int i=0; i<actual.size(); i++){
            if(i==currentPointSelected){
                newPolyline.addVertex(mouse.x, mouse.y);
            }else{
                newPolyline.addVertex(actual[i]);
            }
        }
        newPolyline.close();
        listOfPolyline.at(currentPolylineSelected) = newPolyline;
    }
    if(currentPointSelected == -1){
        ofVec2f translation = mouse -  listOfPolyline.at(currentPolylineSelected).getCentroid2D();
        
        listOfPolyline.at(currentPolylineSelected).translate(translation);
    }
    
}
//-----------------------------------------------------------
int PolyBackground::getClosestPoint(ofVec2f p){
    
    int ret = -1;
    
    if(currentPolylineSelected>=0 && currentPointSelected>= 0){
        for( int i=0; i<listOfPolyline[currentPolylineSelected].size(); i++)
        {
            ofVec2f point = listOfPolyline[currentPolylineSelected][i];
            if(p.distance(point)<radius)
            {
                return i;

            }
            
        }
        
    }
    
    return ret;
}



//-----------------------------------------------------------
ofPolyline PolyBackground::getCurrentSelected(){
    ofPolyline ret;
    
    if(currentPolylineSelected>=0){
        ret = listOfPolyline[currentPolylineSelected];
        ret = ret.getResampledByCount(20);
    }
    
    return ret;
    
}

//-----------------------------------------------------------
ofPolyline PolyBackground::createRandomPolyline(){
    
    int fixPointNumber = 5;
    
    ofPolyline newPoly;
    for(int i=0; i<fixPointNumber; i++){
        newPoly.addVertex(ofRandom(w), ofRandom(h));
    }
    newPoly.close();
    newPoly.scale(0.1, 0.1);
    ofRectangle r = newPoly.getBoundingBox();
    
    newPoly.translate( ofVec2f( ofVec2f(w/2, h/2) - r.getCenter()) );
    newPoly.translate(ofVec2f(ofRandom(-w/2, w/2), ofRandom(-h/2, h/2)));
    
    return newPoly;
    
}
