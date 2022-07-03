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
    save.addListener(this, &PolyBackground::saveToXml);
    
    //Parameters
    pg = _pg;
    pg->setName("polyBg");
    pg->add(currentPolylineSelected.set("selection", -1, -1, (POLYBACKGROUND_NUM-1)));
    pg->add(currentPointSelected.set("p_selection", -1, -1, 10));
    pg->add(isDraw.set("draw", false));
    pg->add(isAddObstacle.set("add obstacle", false));
    pg->add(clearButton.set("clear", false));
    pg->add(save.set("save", false));
    
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
    
    //Open Xml file first
    // If doesn't exist of wrong file, then create randomness
    if(!loadXml()){
        int fixNumberOfPolyline = POLYBACKGROUND_NUM;
        listOfPolyline.clear();
        for(int i=0; i<fixNumberOfPolyline; i++){
            listOfPolyline.push_back(createRandomPolyline());
        }
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
ofPolyline PolyBackground::getPolyBackground(int i) {

	if (i >= 0 && i < listOfPolyline.size()) {

		return listOfPolyline.at(i);
	}
	else {
		listOfPolyline.at(0);
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
    
    //Number of Point per polyline is const value
    int fixPointNumber = POINTPERPOLYLINE_NUM;
    
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

//------------------------------------------------------------
void PolyBackground::saveToXml(bool & isSave){
    
    if(isSave){
        
        xml.clear();
        xml.addTag("polybackground");
        xml.pushTag("polybackground");
        
        for( int pIndex = 0; pIndex<POLYBACKGROUND_NUM; pIndex++){
            
            xml.addTag("polyline");
            xml.pushTag("polyline", pIndex);
            
            for( int i=0; i<POINTPERPOLYLINE_NUM; i++){
                xml.addTag("point");
                xml.pushTag("point", i);
                xml.addValue("x", listOfPolyline.at(pIndex)[i].x);
                xml.addValue("y", listOfPolyline.at(pIndex)[i].y);
                xml.popTag();
            }
            xml.popTag();
        }
        
        xml.popTag();
        
        xml.save(xml_file);
        
    }
        
    
}

//------------------------------------------------------------
bool PolyBackground::loadXml(){
    
    bool isOk = xml.loadFile(xml_file);
    if(!isOk){
        return false;
    }else{
        
        listOfPolyline.clear();
        xml.pushTag("polybackground");
        int nbPolyline = xml.getNumTags("polyline");
        for( int pIndex = 0; pIndex<nbPolyline; pIndex++){
            
            xml.pushTag("polyline", pIndex);
            ofPolyline poly;
            int nbPoint = xml.getNumTags("point");
            
            for( int p=0; p<nbPoint; p++){
                xml.pushTag("point", p);
                int x = xml.getValue("x", 300);
                int y = xml.getValue("y", 300);
                poly.addVertex(x, y);
                xml.popTag();
                
            }
            
            poly.close();
            listOfPolyline.push_back(poly);
            xml.popTag();
            
            
        }
        xml.popTag();
        
        
    }
    
    
    return true;
}
