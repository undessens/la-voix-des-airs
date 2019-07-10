//
//  bird.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#include "Bird.hpp"

Bird::Bird(){
    
    
}


//-------------------------------------------------------------
Bird::Bird(char _letter, int _order, int _w, int _h, PolyBackground* p ){
    //TODO  : order + total of bird . Like percetange position
    
    
    
    
    // Geometrie cartesian stuff
    pos = ofPoint(w/2, h/2);
    speed = ofVec2f(0, 0);
    stiffness = 0.05;
    damping = 0.5;
    
    //Letter stuff
    letter = _letter;
    order = _order;
    
    //Environnement
    polyBg = p;
    w = _w;
    h = _h;
    eject_force = 10.0f;       //multiplier of the speed when ejected for obstacle.
    
    
    //Debug Level
    debugLevel = 3;
    
    // Noisy feature
     noisePos = ofVec2f( ( ofRandom(20), ofRandom(20)));
    
    

    
}
//-------------------------------------------------------------
void Bird::update(ofPoint target){
    
    
    // Check environnement
    // is result is 0,0 , nothing is inside
    // is result > 0, get the vector is the direction of avoiding the obstacle
    ofVec2f result =  polyBg->isInside(pos + speed);
    
    if(result.length() > 0 ){  // if is inside an obstacle
        
        //Getting ejected
        getEjected(result );
    }

    
    //Check the situation
    
    if (is_ejected){
        
        updateEjection(target);
    }
    else
    {
        updateAttraction(target);
        
    }
    
    
    
    
    
    
    
    
    
}
//-------------------------------------------------------------
void Bird::drawBasic(){
    
    ofDrawCircle(pos.x, pos.y, 30);
    ofSetLineWidth(speed.length()*3);
    ofDrawLine(pos.x, pos.y, (pos + force*10 ).x , (pos + force*10).y);
    ofSetLineWidth(1);
    
    if(debugLevel > 0) {
    ofDrawBitmapStringHighlight("speedX: "+ofToString(speed.x )+" - Y:"+ofToString(speed.y), pos.x, pos.y+35);
    ofDrawBitmapStringHighlight("forceX: "+ofToString(force.x )+" - forceY:"+ofToString(force.y), pos.x, pos.y);
    ofDrawBitmapStringHighlight("posX: "+ofToString(pos.x )+" - posY:"+ofToString(pos.y), pos.x, pos.y+15);
    
        
        
        if(is_ejected){
            ofDrawBitmapStringHighlight("ejected", pos.x, pos.y+45);
            ofSetColor(ofColor::red);
           ofDrawLine(pos.x, pos.y, (pos + eject_direction*100 ).x , (pos + eject_direction*100).y);
        }
        
    }
    

    
    
    
}

//-------------------------------------------------------------
void Bird::getEjected(ofVec2f v ){
    
    is_ejected = true;
    eject_direction = v;
    eject_count = 10;
    
    
}

//-------------------------------------------------------------
void Bird::updateEjection(ofPoint target){
    
    
    if(eject_count<0){
        is_ejected = false;
    }else if(eject_count == 10){
        //change from eject direction;
        force = eject_direction  * eject_force;
        speed =  force * speed.length();
        
    }
    else{
        force = eject_direction*(10 - eject_count);
        speed = (speed + force )* 0.5;
    }
    pos = pos + speed;
    eject_count--;
    
}

//-------------------------------------------------------------
void Bird::updateAttraction(ofPoint target){
    
    // First calculate difference from the target
    ofPoint noisyTarget  = target + ofVec2f( 10 * ofSignedNoise(0.3 ), 10 *ofSignedNoise(0.2) );
    
    force = ( noisyTarget - pos )* stiffness;
    speed = (speed + force )*damping;
    
    // Check environnement
    // is result is 0,0 , nothing is inside
    // is result > 0, get the vector is the direction of avoiding the obstacle
    ofVec2f result =  polyBg->isInside(pos + speed);
    
    pos = pos + speed;
    
    
}

