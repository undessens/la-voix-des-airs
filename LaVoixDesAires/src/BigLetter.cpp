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
    finalListOfPolyline = _path;
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
    
    scale +=ofRandom(15);
    ofVec2f randomPos = ofVec2f(ofRandom(-50, 50),ofRandom(-50, 50));
    

    for(auto &polyline : listOfPolyline){
        
        polyline.scale(scale, scale);
        
    }
    
    ofVec2f centroid = listOfPolyline[0].getCentroid2D();
    ofVec2f translate = ofVec2f(w/2, h/2) - centroid;
    for(auto &polyline : listOfPolyline){

        polyline.translate(ofVec2f(translate.x, translate.y));
        polyline.translate(randomPos);

    }
    
    
    

    
}

//-------------------------------------------------
void BigLetter::update(vector<Niche>* listOfNiche){
    
    
        for( int i =0; i<listOfPolyline.size(); i++){

            ofPolyline newPolyline;

            if(listOfPolyline[i].size() == listOfNiche->at(i).listOfBird.size() ){

                for(int j=0; j<listOfPolyline[i].size(); j++){

                    ofVec2f birdPos = listOfNiche->at(i).listOfBird.at(j).pos;
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

//-------------------------------------------------
void BigLetter::drawBig(float zoom, int alpha){
    
    if(iteration<iterationMax){

        ofPath path;
        
        for(int k = 0; k <(int)listOfPolyline.size(); k++){
            
            if( k!= 0){
                path.close();
                path.newSubPath();
            }
            
            ofPolyline simplePoly = (listOfPolyline[k]);
            
            for(int i = 0; i < (int)simplePoly.size(); i++){
                ofPoint p = ofPoint(simplePoly.getVertices()[i].x, simplePoly.getVertices()[i].y);
                path.curveTo(p);
                
            }
        }
        path.close();
        
        ofFill();
        ofPushMatrix();
        ofBlendMode(OF_BLENDMODE_ALPHA);
        ofEnableAlphaBlending();
        float alphaAttenuator = 1.0f - (iteration*1.0f / iterationMax);
        //float alphaAttenuator = 127;
        ofColor col = ofColor(255, 255, 255, alpha*alphaAttenuator);
        path.setColor(col);
        path.draw();
        ofDisableAlphaBlending();
        ofPopMatrix();
    }
    


    
}


//------------------------------------------------
// Exactly like draw Big, but alpha fix instead
void BigLetter::drawSmall(float zoom, int alpha){
    
        
        ofPath path;
        
        for(int k = 0; k <(int)listOfPolyline.size(); k++){
            
            if( k!= 0){
                path.close();
                path.newSubPath();
            }
            
            ofPolyline simplePoly = (listOfPolyline[k]);
            
            for(int i = 0; i < (int)simplePoly.size(); i++){
                ofPoint p = ofPoint(simplePoly.getVertices()[i].x, simplePoly.getVertices()[i].y);
                path.curveTo(p);
                
            }
        }
        path.close();
        
        ofFill();
        ofPushMatrix();
        ofBlendMode(OF_BLENDMODE_ALPHA);
        ofEnableAlphaBlending();
        //float alphaAttenuator = 127;
        ofColor col = ofColor(255, 255, 255, alpha);
        path.setColor(col);
        path.draw();
        ofDisableAlphaBlending();
        ofPopMatrix();
    
    
}

//------------------------------------------------
void BigLetter::drawFinal(int alpha){

        ofPath path;
        
        for(int k = 0; k <(int)finalListOfPolyline.size(); k++){
            
            if( k!= 0){
                path.close();
                path.newSubPath();
            }
            
            ofPolyline simplePoly = (finalListOfPolyline[k]);
            
            for(int i = 0; i < (int)simplePoly.size(); i++){
                ofPoint p = ofPoint(simplePoly.getVertices()[i].x, simplePoly.getVertices()[i].y);
                path.curveTo(p);
                
            }
        }
        path.close();
        
        ofFill();
        ofPushMatrix();
        ofBlendMode(OF_BLENDMODE_ALPHA);
        ofEnableAlphaBlending();
        ofColor col = ofColor(255, 255, 255, alpha);
        path.setColor(col);
        //path.setFilled(false);
        path.setStrokeColor(col);
        path.draw();
        ofDisableAlphaBlending();
        ofPopMatrix();
        
    
    
}


//-------------------------------------------------
void BigLetter::drawDebug(){
    
    ofSetColor(255, 0, 0);
    ofNoFill();
    for(int k = 0; k <(int)listOfPolyline.size(); k++){
        ofPolyline simplePoly = listOfPolyline[k];
        for(int i = 0; i < (int)simplePoly.size(); i++){
            ofPoint p = ofPoint(simplePoly.getVertices()[i].x, simplePoly.getVertices()[i].y);
            ofDrawCircle(p, 1);
            ofDrawBitmapString(ofToString(i), p.x - 15, p.y - 5);
        }
    }
    
}
