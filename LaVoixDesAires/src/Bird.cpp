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
           int _size,
           int _w,                   // Width on fbo surface
           int _h,                    // Height on fbo surface
           int _screenW,
           int _screenH,
           float _stiff,
           int _order,
           int _flyDuration
           ){
    //TODO  : order + total of bird . Like percetange position
    
    // Geometrie cartesian stuff
    pos = ofPoint(w/2, h/2);
    acc = ofVec2f(0, 0);
	speed = ofVec2f(ofRandom(-4+2, 4+2), ofRandom(-1, 1));
    stiffness = 0.5;
    damping = 0.5;
	target = _target;
    
    //State of life
    state = BIRD_FREE;
    isTargetJoined = false;
    
    //Time and distance
    flyingTime = 0;            // time of fly : use for join to target
    flyingDuration = _flyDuration;      // Flying time in FREE_BIRD state , before going in BIRD_GOTARGET state
    flyingDistance = 0;        //meter of fly : use for wing motion
    
    //Order ( nichée
    order = _order;
    
    // Max
    maxSpeed = 5;
    maxForce = 0.25;
    
    //FLOCK parameter
    swt = 0.01; //multiply these force
    awt = 0.01;
    cwt = 0.01;
    twt = 5.1;
    
    //Neighbour
    isNeighbour = false;
    
    //Environnement
    polyBg = p;
    eject_force = 10.0f;       //multiplier of the speed when ejected for obstacle.
    is_ejected = false;
    
    // Noise stuff ... not really clear
    size = _size;
   
    stiffness = _stiff;  //force to join centroid
    damping = 0.5;
               
    // Geometry for borders
    w = _w;
    h = _h;
    screenW = _screenW;
    screenH = _screenH;
    
    //Debug Level
    debugLevel = 0;
    debugScale = 10;
    
}
//-------------------------------------------------------------
void Bird::update(){
    
    //Update speed
	speed += acc;
    
    //Limit Speed
	speed.limit(maxSpeed); // MAX SPEED IF NECESSARY
    
    // Update flying distance : 1) acceleration 2) fight gravity 3) fight air
    flyingDistance += min( acc.length() * 500.0 , 5.0 ) ;
    if(speed.y < -0.1)
    {
        flyingDistance += abs(speed.y)  * 1;
    }
    if(!isTargetJoined)
    {
        flyingDistance += 1;    //increase anyway
    }
    else{
        flyingDistance += 0.3;
    }
    
    if(flyingDistance > 100){
            flyingDistance = (int)(flyingDistance) % 100;
    }
    
    
    //UPDATE TIME
    flyingTime += 1;

    //update pos
	pos += speed;
    
    // RESET SPEED
    if(isTargetJoined){
        speed = ofVec2f(0,0);
    }
    
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
    ofSetLineWidth(1);
     
}

void Bird::drawDebug(int level){
    
    //Debut Printing
    switch(level)
    {
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
            ofDrawBitmapStringHighlight("posX: "+ofToString(pos.x )+" - posY:"+ofToString(pos.y), pos.x, pos.y+15);
            if(is_ejected)
            {
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
void Bird::applyForce(ofVec2f f) {
	
	//Force limit or not ?
	//force.limit(maxForce);
	acc += f;
}


//-------------------------------------------------------------
void Bird::flock(vector<Bird>* birds) {

    switch (state) {
        case BIRD_FREE:
        {
            // CALCULATE INTERACTION FORCES
            ofVec2f sep = separate(birds);
            ofVec2f ali = align(birds);
            ofVec2f coh = cohesion(birds);
            //COEF
            sep *= swt; //multiply these force
            ali *= awt;
            coh *= cwt;
            // APPLY
            applyForce(sep);
            applyForce(ali);
            applyForce(coh);
            // INTERACTION WITH MOUSE
            if (ofGetMousePressed(2))
            {
                ofVec2f att = attraction(ofPoint(ofGetMouseX()*w/screenW, ofGetMouseY()*h/screenH));
                applyForce(att);
            }
            break;
        }
        case BIRD_GOTOTARGET:
        {
            ofVec2f tar = goToTarget();
            tar *= twt;
            tar.limit(maxForce);
            applyForce(tar);
            break;
        }
        default:
            break;
    }
    
	

}
//-------------------------------------------------------------
void Bird::changeState(int msgSize){
    
    if(state == BIRD_FREE)
    {
      
        if(msgSize > (order+3)){
            state = BIRD_GOTOTARGET;
        }else if(flyingTime > flyingDuration)
        {
            state = BIRD_GOTOTARGET;
        }
        
    }

    
    
}

//-------------------------------------------------------------
void Bird::borders() {

	if (pos.x < 0) pos.x = w;
	if (pos.y < 0) pos.y = h;
	if (pos.x > w) pos.x = 0;
	if (pos.y > h) pos.y = 0;

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
		if ((d > 0) && (d < desiredseparation))
        {
			ofVec2f diff = pos - (it->pos);
			diff.normalize();
			diff /= d;
			steer += diff;
			count++;
		}
	}

	if (count > 0)
    {

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
		if (d > 0 && d < neighbordist)
        {
			steer += it->speed;
			count++;
		}
	}
	if (count > 0)
    {
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
		if ((d > 0) &&( d < neighbordist))
        {
			sum += it->pos;
			count++;
		}
	}
	if (count > 0)
    {
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
    if(dist.length() < 200 && twt>0.5)
    {
        if(maxSpeed>2)maxSpeed *=0.995;
        if(maxSpeed>1.5)maxSpeed *=0.99;
        if(maxSpeed>1)maxSpeed *=0.98;
    }
    if(dist.length() < 2)
    {
        isTargetJoined = true;
    }
    dist *= stiffness;
    dist /= 500;
	return dist;

}
