//
//  bird.hpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#ifndef bird_hpp
#define bird_hpp
#include "ofMain.h"
#include "PolyBackground.hpp"
#include "ofxAssimpModelLoader.h"
#include <stdio.h>

#define BIRD_WAITING 0
#define BIRD_FREE 1
#define BIRD_GOTOTARGET 2
#define BIRD_TARGETJOINED 3
#define BIRD_DIEONBORDER 4
#define BIRD_DIE 5
#define LETTERS_ADDED_ON_FLY 2

class Bird
{
    
public:
    
    Bird();
    Bird( PolyBackground* p,
         ofVec2f t,
         ofVec2f i,
         int size,
         int w,
         int h,
         int screenW,
         int screenH,
         float stiffness,
         int order,
         int flyDuration,
		 bool isInvicible
         );
    
    void update();
    void drawBasic();
    void drawDebug(int l);
    
    void getEjected(ofVec2f v);
	void applyForce(ofVec2f force);
	void flock(vector<Bird>* Birds, ofVec2f, bool isAtt);
    void changeState();
	ofVec2f separate(vector<Bird>* Birds);
	ofVec2f align(vector<Bird>* Birds);
	ofVec2f cohesion(vector<Bird>* Birds);
	ofVec2f seek(ofVec2f);
	ofVec2f goToTarget();
	ofVec2f attraction(ofPoint p);
	void goDieOnBorder();
	void borders();
	void randomSpeed(int s);
    void randomSpeedFromCenter(int s);
    
    ofPoint pos;
    ofPoint origin;
	ofVec2f target;
    ofVec2f speed;
	ofVec2f acc;

    
    //State of life, 3 MODE
    // 0 BIRD_FREE = interact with other
    // 1 BIRD_GoToTarget = no interaction, gently join the target point
    // 2 BIRD_DieOnBorder = Free but die when the border is crossed
	// 3 BIRD_Die = Die
    int state;
	//Special birds that never die
	bool isInvicible;
    
    //Time and distance to fly
    unsigned long flyingTime;
    unsigned long flyingDuration;
    int waitingDuration; // Waiting time when it appears with the big letter
    float flyingDistance;
    float lastUpdateTime;
    
    //Nichée
    int order;

	//max
	float maxForce;
	float maxSpeed;

	//Force multiplier
	float swt; //separation
	float awt;  // align
	float cwt;	//cohesion
	float twt;	// go to target
    
    //Neighbours - NOT USED ANYMORE
    bool isNeighbour;
    Bird* neighbourLeft;

    //Environnement
    PolyBackground* polyBg;
    float eject_force ;     //force being ejected for an obstacle
    bool is_ejected;        // situation of ejection during the count
    ofVec2f eject_direction;
    
    // Size : decrease when going to target
	float initalSize;     //Initial
	float size;     //Initial
    float finalSize;
    
    //Attraction spring force
    float stiffness;  //force to join centroid
    float damping;
    
    //Geometry
    int w;
    int h;
    int screenW;
    int screenH;
    
    //Debug stuff
    int debugScale;
    
    
};



#endif /* bird_hpp */

