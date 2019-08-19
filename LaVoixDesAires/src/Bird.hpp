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

class Bird
{
    
public:
    
    Bird();
    Bird( char letter, int order, int w, int h , PolyBackground* p, ofVec2f t);
    
    void update(ofPoint t);
    void drawBasic();
    void drawDebug(int l);
    
    void getEjected(ofVec2f v);
    void updateEjection(ofPoint p);
	void applyForce(ofVec2f force);
	void flock(vector<Bird>* Birds);
	ofVec2f separate(vector<Bird>* Birds);
	ofVec2f align(vector<Bird>* Birds);
	ofVec2f cohesion(vector<Bird>* Birds);
	ofVec2f seek(ofVec2f);
	ofVec2f goToTarget();
	ofVec2f attraction(ofPoint p);
	void borders();
    
    ofPoint pos;
    ofPoint origin;
	ofVec2f target;
    ofVec2f speed;
	ofVec2f acc;
    ofVec2f force;
    ofVec2f noisedForce;

	//max
	float maxForce;
	float maxSpeed;

	//Force multiplier
	float swt; //go to mouse
	float awt;  // align
	float cwt;	//cohesion
	float twt;	// go to target
    
    //Neighbours
    Bird* neighbourLeft;
    Bird* neighbourRight;

    
    //Environnement
    int w;
    int h;
    PolyBackground* polyBg;
    float eject_force ;     //force being ejected for an obstacle
    bool is_ejected;        // situation of ejection during the count
    ofVec2f eject_direction;
    
    // Noise stuff ... not really clear
    float size;
    
    float force_amp;
    
    ofVec2f noiseD;
    float noiseDAmplitude;   // Direction noise
    float noiseDFreq;
    
    float noiseS;
    float noiseSAmplitude;
    float noiseSFreq;
    
    float stiffness;  //force to join centroid
    float damping;
    
    //Letter stuff
    char letter;  // Final letter to display
    int order;
    
    //Debug stuff
    int debugLevel;
    int debugScale;
    
    
};



#endif /* bird_hpp */

