//
//  PolyBackground.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#include "PolyBackground.hpp"

PolyBackground::PolyBackground(){
    
    
}

PolyBackground::PolyBackground(ofParameterGroup* _pg){
    
    //Listener
    nbObstacle.addListener(this, &PolyBackground::addObstacle);
    
    //Parameters
    pg = _pg;
    pg->setName("polyBg");
    pg->add(lineSize.set("lineSize", 1, 1, 5));
    pg->add(nbObstacle.set("nbObstacle", 5, 1, 10));
    
    listOfPoly.clear();
    
    
    createObstacle();

    
    
}

void PolyBackground::createObstacle(){
    
    //Construct fake obstacle
    
        ofRectangle r = ofRectangle(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()), ofRandom(50, 120), ofRandom(50, 120));
        
        ofPolyline p ;
        p.addVertex(r.x, r.y);
        p.addVertex(r.x + r.width, r.y);
        p.addVertex(r.x + r.width, r.y + r.height);
        p.addVertex(r.x , r.y + r.height);
        p.close();
        p = p.getResampledBySpacing(10);
        
        listOfPoly.push_back(p);
        nbObstacle++;
    
}

void PolyBackground::addObstacle(int &nb){
    
    if(listOfPoly.size() < nb){
        // Create no obstacle
        int nbToCreate = nb-listOfPoly.size();
        for (int i = 0 ; i<nbToCreate; i++){
            createObstacle();
        }
        
    }
    else if(listOfPoly.size()> nb){
        //delete obstacle
        int nbToDelete = listOfPoly.size()-nb;
        for( int i= 0 ; i<nbToDelete; i++){
            listOfPoly.pop_back();
            
        }
        
        
    }
    
    nbObstacle = listOfPoly.size();
    
    
}

void PolyBackground::draw(){
    
    ofSetLineWidth(lineSize);
    for( vector<ofPolyline>::iterator it = listOfPoly.begin(); it < listOfPoly.end() ; it++){
    
        ofSetColor(255);
        it->draw();
        
    }
    ofSetLineWidth(1);
        
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

