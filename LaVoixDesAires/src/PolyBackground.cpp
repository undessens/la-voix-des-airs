//
//  PolyBackground.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#include "PolyBackground.hpp"


PolyBackground::PolyBackground(){
    
    int maxObstacle = 5;

    
    
    for(int i = 0; i< maxObstacle; i++){
        
        ofRectangle r = ofRectangle(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()), ofRandom(50, 120), ofRandom(50, 120));
        
        ofPolyline p ;
        p.addVertex(r.x, r.y);
        p.addVertex(r.x + r.width, r.y);
        p.addVertex(r.x + r.width, r.y + r.height);
        p.addVertex(r.x , r.y + r.height);
        p.close();
        p = p.getResampledBySpacing(10);
        
        listOfPoly.push_back(p);
        
    }
    
    
}

void PolyBackground::draw(){
    
    for( vector<ofPolyline>::iterator it = listOfPoly.begin(); it < listOfPoly.end() ; it++){
    
        ofSetColor(255);
        it->draw();
        
    }
        
}

ofVec2f PolyBackground::isInside( ofPoint p){
    
    for( vector<ofPolyline>::iterator it = listOfPoly.begin(); it < listOfPoly.end() ; it++){
        
        if(it->inside(p)){
            
            // Get the direction from the centroid
            ofVec2f return_direction = p - it->getCentroid2D() ;
            return_direction = return_direction.normalize();
            
            // Get the closest point
            float minLenght = 1000;
            int finalindex;
            
            for(int i=0 ; i<it->size() ; i++){
                
                float length = ( p.x - (*it)[i].x)*( p.x - (*it)[i].x) + ( p.y - (*it)[i].y)*( p.y - (*it)[i].y);
                
                if(length<minLenght){
                    
                    minLenght= length;
                    finalindex = i;
                }
                
            }
            
            ofVec2f normal = it->getNormalAtIndex(finalindex);
            ofVec2f diff = (return_direction - normal).normalize();
            ofVec2f finalDirection = (normal - diff).normalize();
            
            return finalDirection ;
            
        }
        
        
    }
    
    return ofVec2f(0,0);
    
}

