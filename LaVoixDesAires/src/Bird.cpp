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
    pos = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
    acc = ofVec2f(0, 0);
	speed = ofVec2f(ofRandom(-6, 6), ofRandom(-6, 6));
    stiffness = 0.0005;
    damping = 0.5;
    
    //Environnement
    polyBg = p;
    w = _w;
    h = _h;
    eject_force = 10.0f;       //multiplier of the speed when ejected for obstacle.
    is_ejected = false;
    
    // Noise stuff ... not really clear
    size = 10;

    noiseDAmplitude=0;  // Direction noise
    noiseDFreq = 0.1;
    
    noiseSAmplitude = 0;
    noiseSFreq = 0.1;
    
    stiffness = 0.05;  //force to join centroid
    damping = 0.5;
    
    //Letter stuff
    letter = _letter;
    order = _order;

	//FLOCK
	swt = 0.01; //multiply these force
	awt = 0.01;
	cwt = 0.01;

	// Max
	maxSpeed = 5;
	maxForce = 0.25;
    
    //Debug Level
    debugLevel = 0;
    debugScale = 10;


    
    
    

    

    
}
//-------------------------------------------------------------
void Bird::update(ofPoint target){
    
    
    // Check environnement
    // is result is 0,0 , nothing is inside
    // is result > 0, get the vector is the direction of avoiding the obstacle
   // ofVec2f result =  polyBg->isInside(pos + speed);
    
   // if(result.length() > 0 ){  // if is inside an obstacle
        
        //Getting ejected
        //getEjected(result );
   // }



	speed += acc;
	speed.limit(maxSpeed); // MAX SPEED IF NECESSARY
	pos += speed;

	// OK but reset later, maybe after drawing
	acc = ofVec2f(0, 0);		// RESET ACCELERATION TO EACH CYCLE
    //updateAttraction(target);
    
    
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
        
        case 1 :
            ofDrawBitmapStringHighlight("speedX: "+ofToString(speed.x )+" - Y:"+ofToString(speed.y), pos.x, pos.y+35);
            ofDrawBitmapStringHighlight("forceX: "+ofToString(force.x )+" - forceY:"+ofToString(force.y), pos.x, pos.y);
            ofDrawBitmapStringHighlight("posX: "+ofToString(pos.x )+" - posY:"+ofToString(pos.y), pos.x, pos.y+15);
            if(is_ejected){
                ofDrawBitmapStringHighlight("ejected", pos.x, pos.y+45);
                ofSetColor(ofColor::red);
                //ofDrawLine(pos.x, pos.y, (pos + eject_direction*100 ).x , (pos + eject_direction*100).y);
            }
        break;
        case 2 :
            //Draw debug noise
            int scale = debugScale;
            ofSetLineWidth(1);
            ofSetColor(ofColor::yellow);
            ofDrawLine(pos.x, pos.y, (pos + force*scale ).x , (pos + force*scale).y);
            ofSetColor(ofColor::blue);
            ofVec2f force_normalize = force;
            force_normalize.normalize();
            ofVec2f noiseSpeed = pos + (force + force_normalize*noiseS)*scale;
            ofDrawLine((pos + force*scale ).x, (pos + force*scale).y, noiseSpeed.x , noiseSpeed.y);
            ofSetColor(ofColor::red);
            ofDrawLine( noiseSpeed.x , noiseSpeed.y, noiseSpeed.x + (noiseD*scale).x, noiseSpeed.y + (noiseD*scale).y  );
            //ofSetColor(ofColor::pink);
            //ofDrawLine(pos.x, pos.y, pos.x + noisedForce.x, pos.y + noisedForce.y);
            
            
            
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
void Bird::updateAttraction(ofPoint target){
    
    // First calculate difference from the target
    force = ( target - pos )* stiffness;
	force /= 500;
	applyForce(force);
    
    //Force is modified from to 2 kind of noise

    //NOISE SPEED
    // Noise direction is modifing the left, right direction of the bird
   // force_amp = force.length();
   // noiseS = noiseSAmplitude * ofSignedNoise( noiseSFreq * ofGetElapsedTimef());
    
   // cout << "\n"+ ofToString(noiseS);
    // Noise speed, is modifuing the attraction of the bird
   // ofVec2f force_dir = force;
   // force_dir = force_dir.normalize();
   // noiseD = force_dir.getPerpendicular() * noiseDAmplitude * ofSignedNoise(noiseDFreq * ofGetElapsedTimef());
    
   // noisedForce = ( force_amp + noiseS )*(force_dir + noiseD);
    

    
    
}
//-------------------------------------------------------------
void Bird::flock(vector<Bird> birds) {

	ofVec2f sep = separate(birds);
	ofVec2f ali = align(birds);
	ofVec2f coh = cohesion(birds);

	sep *= swt; //multiply these force
	ali *= awt;
	coh *= cwt;

	applyForce(sep);
	applyForce(ali);
	applyForce(coh);

}
//-------------------------------------------------------------
void Bird::borders() {

	if (pos.x < 0) pos.x = ofGetWidth();
	if (pos.y < 0) pos.y = ofGetHeight();
	if (pos.x > ofGetWidth()) pos.x = 0;
	if (pos.y > ofGetHeight()) pos.y = 0;

}
//-------------------------------------------------------------
ofVec2f Bird::separate(vector<Bird> birds) {

	float desiredseparation = 25;
	ofVec2f steer = ofVec2f(0,0	);
	int count = 0;
	for (vector<Bird>::iterator it = birds.begin(); it < birds.end(); it++)
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

//-------------------------------------------------------------
ofVec2f Bird::align(vector<Bird> birds) {
	float neighbordist = 25;
	ofVec2f steer = ofVec2f(0,0);
	int count = 0;
	for (vector<Bird>::iterator it = birds.begin(); it < birds.end(); it++)
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

//-------------------------------------------------------------
ofVec2f Bird::cohesion(vector<Bird> birds) {
	float neighbordist = 50;
	ofVec2f sum = ofVec2f(0, 0);
	int count = 0;
	for (vector<Bird>::iterator it = birds.begin(); it < birds.end(); it++)
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

ofVec2f Bird::seek(ofVec2f target) {

	// Pointing from the position to the target
	ofVec2f desired = target - pos;

	// Normalize desired and scale to maximum speed
	desired.normalize();
	desired *= maxSpeed; //maxspeed
	ofVec2f steer = desired - speed;
	steer.limit(maxForce);

	return steer;

}