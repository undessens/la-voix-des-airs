//
//  Letter.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 31/10/2021.
//

#include "Letter.hpp"

Letter::Letter(){
    text_letter = 'A';
    
}

Letter::Letter(char letter, vector<ofPolyline> _path, vector<int> n){
    text_letter = letter;
    listOfPolyline = _path;
    listOfNiche = n;
    iteration = 0;
    iterationMax = 80;
    init();
}
//-------------------------------------------------
void Letter::init(){
    
    bool vflip = true;
    bool filled = false;
    
    associatedToBirds = false;
    
    int size = (int)listOfPolyline.size();
    if(size != listOfNiche.size()){
        cout << "ERROR , Letter Init(), size of polyline and list of niche does not match \n";
    }
    
    
    

    
}

void Letter::update(BirdManager* birdManager){
    
    
    if(iteration<iterationMax){
        for( int i =0; i<listOfPolyline.size(); i++){
            
            int associatedNichee = listOfNiche[i];
            ofPolyline newPolyline;
            
            if(birdManager->listOfBird[associatedNichee].size()==listOfPolyline[i].size() ){
                
                for(int j=0; j<listOfPolyline[i].size(); j++){
                    
                    ofVec2f birdPos = birdManager->listOfBird[associatedNichee].at(j).pos;
                    ofPoint p = (listOfPolyline[i])[j];
                    newPolyline.addVertex(birdPos.x, birdPos.y);
                }
                
                //newPolyline.close();
                listOfPolyline.at(i) = newPolyline;
                
                
                
            }else{
                cout << "Leter update ERROR, associated nichee and listOfPyline sizes does not match \n";
            }

        }
        
        iteration++;
    }
    

    
    
}

//-------------------------------------------------
void Letter::drawBasic(float zoom, int alpha){
    
    
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
                //ofDrawCircle(p, 10);
            }
        }
        ofEndShape( true );
        ofDisableAlphaBlending();
        ofPopMatrix();
    }
    

    
}
//-------------------------------------------------
void Letter::drawDebug(){
    
    if(associatedToBirds){
        ofSetColor(255, 0, 0);
    }else{
        ofSetColor(0, 250, 0);
    }
    ofFill();
    ofPushMatrix();
    ofTranslate(200, 400, 0);
    ofBeginShape();
    for(int k = 0; k <(int)listOfPolyline.size(); k++){
        if( k!= 0)ofNextContour(true) ;
        ofPolyline simplePoly = listOfPolyline[k];
        for(int i = 0; i < (int)simplePoly.size(); i++){
            ofPoint p = ofPoint(simplePoly.getVertices()[i].x, simplePoly.getVertices()[i].y);
            ofVertex(p.x, p.y);
            ofDrawCircle(p, 5);
        }
    }
    ofEndShape( true );
    ofPopMatrix();
    
}
