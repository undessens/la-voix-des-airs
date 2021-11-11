//
//  bird.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#include "Bird.hpp"
#include "ofApp.h"

Bird::Bird(){
    
    
}


//-------------------------------------------------------------
Bird::Bird( PolyBackground* p ,
           ofVec2f _target,
           ofVec2f initialPos,
           int _size,
           int _w,                   // Width on fbo surface
           int _h,                    // Height on fbo surface
           int _screenW,
           int _screenH,
           float _stiff,
           int _order,
           int _flyDuration,
		   bool _isInvicible
           ){
    //TODO  : order + total of bird . Like percetange position
    
    // Geometrie cartesian stuff
    pos = initialPos;
    acc = ofVec2f(0, 0);
	randomSpeedFromCenter( 0);
	target = _target;
    
    
	isInvicible = _isInvicible;
    
    //State of life
    if(isInvicible){
        state = BIRD_FREE;
    }else{
        state = BIRD_WAITING;
    }
    
    //Time and distance
    flyingTime = 0;            // time of fly : use for join to target
    flyingDuration = _flyDuration;      // Flying time in FREE_BIRD state , before going in BIRD_GOTARGET state
    flyingDistance = 0;        //meter of fly : use for wing motion
    lastUpdateTime = ofGetElapsedTimef();
    
    //Order ( nichée
    order = _order;

    // Max
    maxSpeed = 15; //15 before 
    maxForce = 1.2; // 0.25before
    
    //FLOCK parameter
    swt = 0.7; //multiply these force
    awt = 0.1;
    cwt = 0.1;
    twt = 5.1;
    
    //Neighbour- not used anymore
    //isNeighbour = false;
    
    //Environnement
    polyBg = p;
    eject_force = 10.0f;       //multiplier of the speed when ejected for obstacle.
    is_ejected = false;
    
    // Noise stuff ... not really clear
    size = _size;
	initalSize = size;
    finalSize = 5;
   
    stiffness = _stiff;  //force to join centroid

               
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
    
    //Get Delta Time from last update
    float deltaTime = 1/(ofGetElapsedTimef() - lastUpdateTime);
	float theoricPeriod = 1.0 / ofApp::fps;
    lastUpdateTime = ofGetElapsedTimef();
    
    //Update speed
   // speed += acc  /(deltaTime * (ofApp::fps*1.0) );
	speed += acc / (deltaTime * (theoricPeriod));
    
    //Limit Speed
	speed.limit(maxSpeed); // MAX SPEED IF NECESSARY
    
    // Update flying distance : 1) acceleration 2) fight gravity 3) fight air
    flyingDistance += min( acc.length() * 500.0 , 5.0 ) ;
    if(speed.y < -0.1)
    {
        flyingDistance += abs(speed.y)  * 1;
    }
    
    if(state != BIRD_TARGETJOINED)
    {
        flyingDistance += 1;    //inscrease normal
    }
    else{
        flyingDistance += 0.3; //increase just a bit
    }
    
    if(flyingDistance > 100){
            flyingDistance = (int)(flyingDistance) % 100;
    }
    
    
    //UPDATE TIME
    flyingTime += 1;

    //update pos
    pos += speed /(deltaTime * (theoricPeriod));
    
    // RESET SPEED
    if(state == BIRD_TARGETJOINED){
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

void Bird::drawDebug(){
    
    //Debut Printing
    switch(debugLevel)
    {
        case 1 :
            ofSetLineWidth(0.5);
            ofDrawCircle(target.x,target.y,2);
            break;
        case 2 :
            ofDrawLine(pos, pos + speed * 10);
            break;
        case 3 :
            ofSetLineWidth(0.5);
            ofDrawCircle(pos, 1);
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
void Bird::flock(vector<Bird>* birds, ofVec2f attPoint, bool isAtt) {

    switch (state) {
        case BIRD_WAITING:
        {
            // CALCULATE INTERACTION FORCES
            ofVec2f sep = separate(birds);
            //ofVec2f ali = align(birds);
            //ofVec2f coh = cohesion(birds);
            //COEF
            sep *= swt; //multiply these force
            // ali *= awt;
            //coh *= cwt;
            // APPLY
            applyForce(sep);
            //applyForce(ali);
            //applyForce(coh);
            // attractive force
            // TODO : change this 40 with parameter value
            // NON :  il faut forcer le départ quand une nouvelle lettre est tapée
            break;
        }
        case BIRD_FREE:
        {
            // CALCULATE INTERACTION FORCES
            ofVec2f sep = separate(birds);
            //ofVec2f ali = align(birds);  // WHY THIS IS COMMENT
            ofVec2f coh = cohesion(birds);
            //COEF
            sep *= swt; //multiply these force
            // ali *= awt;
            coh *= cwt;
            // APPLY
            applyForce(sep);
            //applyForce(ali);
            applyForce(coh);
            // attractive force
            if (isAtt )
            {
                ofVec2f att = attraction(attPoint);
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
            if(size>finalSize){
                size -=0.30;
            }
            if(maxSpeed > 0.01  ){
                //maxSpeed -= 0.0003;
                maxSpeed *=0.998;
            }
            break;
        }
        case BIRD_TARGETJOINED:
        {
            debugLevel = 3;
            ofVec2f tar = goToTarget();
            tar *= twt;
            tar.limit(maxForce);
            applyForce(tar);
            if(size>finalSize){
                size -=0.9;
            }
			break;
        }
		case BIRD_DIEONBORDER:
		{
			//FORCE
			ofVec2f sep = separate(birds);
			//COEF
			sep *= swt; //multiply these force
			// APPLY
			applyForce(sep);

		}
        default:
            break;
    }
    
	

}
//-------------------------------------------------------------
void Bird::changeState(int msgSize){
    
    
    if(state ==  BIRD_WAITING && !isInvicible && flyingTime > 50){
        state = BIRD_FREE;
    }
    
    
    if(state == BIRD_FREE)
    {
      
        if(flyingTime > flyingDuration && !isInvicible)
        {
            state = BIRD_GOTOTARGET;

        }
        
    }

    
    
}

//-------------------------------------------------------------
void Bird::borders() {

	int isOut = 0;

	if (pos.x < 0 && state != BIRD_DIE) {
		pos.x = w;
		isOut++;
	}
	if (pos.y < 0 && state != BIRD_DIE) {
			pos.y = h;
			isOut++;
		}
	if (pos.x > w && state != BIRD_DIE) {
		pos.x = 0;
		isOut++;
	}
	if (pos.y > h && state != BIRD_DIE) {
		pos.y = 0;
		isOut++;
	}

	if (state == BIRD_DIEONBORDER && isOut > 0) {
		state = BIRD_DIE;
	}
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
	
	//if (maxForce > 0.25)maxForce *= 0.99;
	

    ofVec2f dist = (target - pos);
    if(dist.length() < 150 )
    {
       // if(maxSpeed>2)maxSpeed *=0.995;
       // if(maxSpeed>1.5)maxSpeed *=0.99;
       // if(maxSpeed>1)maxSpeed *=0.98;
		if (maxSpeed > 2)maxSpeed *= 0.995;
		if (maxSpeed > 1.5)maxSpeed *= 0.99;
		if (maxSpeed > 0.9)maxSpeed *= 0.98;
		maxForce = 0.25;

		


    }
    if(dist.length() < 2)
    {
        state = BIRD_TARGETJOINED;
    }
    //Stiffness does matter with goToTarget
	//dist *= stiffness;
    //dist /= 500;
	return dist;

}

// ------------------------------------------------------------ 
//			Change of state : after target pos, fly until cross a border
// ------------------------------------------------------------ -
void Bird::goDieOnBorder() {

	if (!isInvicible) {	//Invicible never die
		state = BIRD_DIEONBORDER;

		randomSpeed( 20);
		size = initalSize;
		debugLevel = 0;

		// Max
		maxSpeed = 45;
		maxForce = 5;

	}
}
//------------------------------------------------------------------
void Bird::randomSpeed(int s) {


	float angle = ofRandom(0.8, 0.8*PI) + PI/2;
	if (ofRandom(1) > 0.5) angle += PI;

	float r = ofRandom(s, s+10);
	ofVec2f direction = ofVec2f(cos(angle), sin(angle));


	speed = direction * r;
}
//------------------------------------------------------------------
void Bird::randomSpeedFromCenter(int s){
    
    ofPoint centroid = ofPoint(w/2, h/2);
    ofVec2f direction = centroid - pos ;
    direction = direction.normalize();
    
    float r = 1;
    speed = direction * r;
    
    
    
}
