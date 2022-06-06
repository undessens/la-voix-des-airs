//
//  Letter.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 31/10/2021.
//

#include "BigLetter.hpp"
//-------------------------------------------------
BigLetter::BigLetter(){
}


//-------------------------------------------------
BigLetter::BigLetter( vector<ofPolyline> _path,int _w,int _h){

    listOfPolyline = _path;
    iteration = 0;
    iterationMax = 80;
    scale = 10;
    w = _w;
    h = _h;
    init();

}

//-------------------------------------------------
void BigLetter::init(){
    
    bool vflip = true;
    bool filled = false;
    

    for(auto &polyline : listOfPolyline){
        
        polyline.scale(scale, scale);
        
    }
    
    ofVec2f centroid = listOfPolyline[0].getCentroid2D();
    ofVec2f translate = ofVec2f(w/2, h/2) - centroid;
    for(auto &polyline : listOfPolyline){

        polyline.translate(ofVec2f(translate.x, translate.y));

    }
    
    
    

    
}

//-------------------------------------------------
void BigLetter::update(vector<Niche>* listOfNiche){
    
    
    if(iteration<iterationMax){
//        for( int i =0; i<listOfPolyline.size(); i++){
//
//            ofPolyline newPolyline;
//
//            if(birdManager->listOfBird[associatedNichee].size()==listOfPolyline[i].size() ){
//
//                for(int j=0; j<listOfPolyline[i].size(); j++){
//
//                    ofVec2f birdPos = birdManager->listOfBird[associatedNichee].at(j).pos;
//                    ofPoint p = (listOfPolyline[i])[j];
//                    newPolyline.addVertex(birdPos.x, birdPos.y);
//                }
//
//                //newPolyline.close();
//                listOfPolyline.at(i) = newPolyline;
//
//
//
//            }else{
//                cout << "Leter update ERROR, associated nichee and listOfPyline sizes does not match \n";
//            }
//
//        }
        
        iteration++;
    }
    

    
    
}

//-------------------------------------------------
void BigLetter::drawBasic(float zoom, int alpha){
    
    
    if(iteration<iterationMax){
        ofFill();
        ofPushMatrix();
        ofBlendMode(OF_BLENDMODE_ALPHA);
        ofEnableAlphaBlending();
        
        float alphaAttenuator = 1.0f - (iteration*1.0f / iterationMax);
        ofSetColor(255, 255, 255, alpha*alphaAttenuator);
        ofTranslate(0, 0, 0);
        ofBeginShape();
        
        
        ofVec2f translation;
        
        for(int k = 0; k <(int)listOfPolyline.size(); k++){
            
            if( k!= 0)ofNextContour(true) ;
            
            ofPolyline simplePoly = (listOfPolyline[k]);
            
            for(int i = 0; i < (int)simplePoly.size(); i++){
                ofPoint p = ofPoint(simplePoly.getVertices()[i].x, simplePoly.getVertices()[i].y);
                ofCurveVertex(p.x, p.y);
            }
        }
        ofEndShape( true );
        ofDisableAlphaBlending();
        ofPopMatrix();
    }
    

    
}
//-------------------------------------------------
void BigLetter::drawDebug(){
    
    ofSetColor(255, 0, 0);
    ofFill();
    for(int k = 0; k <(int)listOfPolyline.size(); k++){
        ofPolyline simplePoly = listOfPolyline[k];
        for(int i = 0; i < (int)simplePoly.size(); i++){
            ofPoint p = ofPoint(simplePoly.getVertices()[i].x, simplePoly.getVertices()[i].y);
            ofDrawCircle(p, 5);
        }
    }
    
}
