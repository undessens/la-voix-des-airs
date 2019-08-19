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
Bird::Bird( PolyBackground* p ,
           ofVec2f _target,
           int _size
           
           
           ){
    //TODO  : order + total of bird . Like percetange position
    
    // Geometrie cartesian stuff
    pos = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
    acc = ofVec2f(0, 0);
	speed = ofVec2f(ofRandom(-4+2, 4+2), ofRandom(-1, 1));
    stiffness = 0.5;
    damping = 0.5;
	target = _target;
    
    //Neighbour
    isNeighbour = false;
    
    //Environnement
    polyBg = p;
    eject_force = 10.0f;       //multiplier of the speed when ejected for obstacle.
    is_ejected = false;
    
    // Noise stuff ... not really clear
    size = _size;
   
    stiffness = 0.05;  //force to join centroid
    damping = 0.5;
    
	//FLOCK
	swt = 0.01; //multiply these force
	awt = 0.01;
	cwt = 0.01;
	twt = 0.0;

	// Max
	maxSpeed = 5;
	maxForce = 0.25;
    
    //Debug Level
    debugLevel = 0;
    debugScale = 10;
    
}
//-------------------------------------------------------------
void Bird::update(ofPoint target){
    
	speed += acc;
	speed.limit(maxSpeed); // MAX SPEED IF NECESSARY
	pos += speed;

	// OK but reset later, maybe after drawing
	acc = ofVec2f(0, 0);		// RESET ACCELERATION TO EACH CYCLE
    
}
//-------------------------------------------------------------
//                  DRAW
//-------------------------------------------------------------
void Bird::drawBasic(){
    
    //ofDrawCircle(pos.x, pos.y, 10);
	ofDrawArrow(pos, pos + speed*size);
    ofSetLineWidth(speed.length()*3);
    //ofDrawLine(pos.x, pos.y, (pos + force*10 ).x , (pos + force*10).y);
    ofSetLineWidth(1);
     
}

void Bird::drawDebug(int level){
    
    //Debut Printing
    switch(level){
        case 1 : ofSetColor(255);
            ofSetLineWidth(0.5);
            ofDrawCircle(target.x,target.y,2);
            break;
        case 2 :
            ofSetColor(ofColor::blue);
            ofDrawLine(pos, pos + speed * 10);
            break;
        case 9 :
            ofDrawBitmapStringHighlight("speedX: "+ofToString(speed.x )+" - Y:"+ofToString(speed.y), pos.x, pos.y+35);
            ofDrawBitmapStringHighlight("forceX: "+ofToString(force.x )+" - forceY:"+ofToString(force.y), pos.x, pos.y);
            ofDrawBitmapStringHighlight("posX: "+ofToString(pos.x )+" - posY:"+ofToString(pos.y), pos.x, pos.y+15);
            if(is_ejected){
                ofDrawBitmapStringHighlight("ejected", pos.x, pos.y+45);
                ofSetColor(ofColor::red);
                //ofDrawLine(pos.x, pos.y, (pos + eject_direction*100 ).x , (pos + eject_direction*100).y);
            }
        break;
        case 8 :


        break;
        
    }
    
}


//-------------------------------------------------------------
void Bird::getEjected(ofVec2f v ){
    
    is_ejected = true;
    eject_direction = v;
    
    
}

//-------------------------------------------------------------
void Bird::updateEjection(ofPoint target){
    
        //change from eject direction;
        //force = eject_direction  * eject_force;
        //speed =  force * speed.length();
    
        //pos = pos + speed;
    
}
//-------------------------------------------------------------
void Bird::applyForce(ofVec2f force) {
	
	//Force limit or not ?
	//force.limit(maxForce);
	acc += force;
}


//-------------------------------------------------------------
void Bird::flock(vector<Bird>* birds) {

	ofVec2f sep = separate(birds);
	ofVec2f ali = align(birds);
	ofVec2f coh = cohesion(birds);
	ofVec2f att = attraction(ofPoint(ofGetMouseX(), ofGetMouseY()));
	ofVec2f tar = goToTarget();

	sep *= swt; //multiply these force
	ali *= awt;
	coh *= cwt;
	if (!ofGetMousePressed(2)) {
		att *= 0;
	}
	tar *= twt;

	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
	applyForce(att);
	applyForce(tar);

}
//-------------------------------------------------------------
void Bird::borders() {

	if (pos.x < 0) pos.x = ofGetWidth();
	if (pos.y < 0) pos.y = ofGetHeight();
	if (pos.x > ofGetWidth()) pos.x = 0;
	if (pos.y > ofGetHeight()) pos.y = 0;

}
// ------------------------------------------------------------ 
//			FORCE
//-------------------------------------------------------------
ofVec2f Bird::separate(vector<Bird>* birds) {

	float desiredseparation = 25;
	ofVec2f steer = ofVec2f(0,0	);
	int count = 0;
	for (vector<Bird>::iterator it = (*birds).begin(); it < (*birds).end(); it++)
	{
		float d = pos.distance(it->pos);
		if ((d > 0) && (d < desiredseparation)) {
			ofVec2f diff = pos - (it->pos);
			diff.normalize();
			diff /= d;
			steer += diff;
			count++;
		}
	}

	if (count > 0) {

		steer /= ((float)count);
		steer.normalize();
		steer *= maxSpeed; // mutl with maxspeed
		steer = steer - speed;
		steer.limit(maxForce); //apply maxsforce to steer
	}

	return steer;

}
// ------------------------------------------------------------ 
//			FORCE
//-------------------------------------------------------------
ofVec2f Bird::align(vector<Bird> *birds) {
	float neighbordist = 25;
	ofVec2f steer = ofVec2f(0,0);
	int count = 0;
	for (vector<Bird>::iterator it = (*birds).begin(); it < (*birds).end(); it++)
	{
		float d = pos.distance(it->pos);
		if (d > 0 && d < neighbordist) {
			steer += it->speed;
			count++;
		}
	}
	if (count > 0) {
		steer /= (float)count;
		//IMplement Reynolds : Steering = Desired - Velocity
		steer.normalize();
		steer *= maxSpeed;		//MAX SPEED
		steer -= speed;
		steer.limit(maxForce);	 //MAX Force
	}

	return steer;
}
// ------------------------------------------------------------ 
//			FORCE
//-------------------------------------------------------------
ofVec2f Bird::cohesion(vector<Bird>* birds) {
	float neighbordist = 50;
	ofVec2f sum = ofVec2f(0, 0);
	int count = 0;
	for (vector<Bird>::iterator it = (*birds).begin(); it < (*birds).end(); it++)
	{
		float d = pos.distance(it->pos);
		if ((d > 0) &&( d < neighbordist)) {
			sum += it->pos;
			count++;
		}
	}
	if (count > 0) {
		sum /= (float)count*1.0;
		return seek(sum);
	}
	return sum;
}
// ------------------------------------------------------------ 
//			FORCE
// ------------------------------------------------------------ 
ofVec2f Bird::seek(ofVec2f TempTarget) {

	// Pointing from the position to the target
	ofVec2f desired = TempTarget - pos;

	// Normalize desired and scale to maximum speed
	desired.normalize();
	desired *= maxSpeed; //maxspeed
	ofVec2f steer = desired - speed;
	steer.limit(maxForce);

	return steer;

}
// ------------------------------------------------------------ 
//			FORCE
//-------------------------------------------------------------
ofVec2f Bird::attraction(ofPoint tempTarget) {

	// First calculate difference from the target
	ofVec2f f = (tempTarget - pos)* stiffness;
	f /= 500.0;
    f.limit(maxForce);
	return f;


}
// ------------------------------------------------------------ 
//			FORCE
// ------------------------------------------------------------ -
ofVec2f Bird::goToTarget() {
	
    ofVec2f dist = (target - pos);
    if(dist.length() < 200 && twt>0.5){
        if(maxSpeed>2)maxSpeed *=0.995;
        if(maxSpeed>1.5)maxSpeed *=0.99;
        if(maxSpeed>1)maxSpeed *=0.98;
        
     
    }
    if(dist.length() < 15 && twt>0.5){
        if(maxSpeed>0.2){
            maxSpeed *= 0.9;
        }
        // reduce all group parameter to zero
        // Keep the body fixe to the final 
        //cwt = 0;
        //awt = 0;
        //swt = 0;

        
    }
    dist *= stiffness;
    dist /= 500;
    dist.limit(maxForce);
    
	
	return dist;

}
