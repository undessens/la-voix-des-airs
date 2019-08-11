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

#include <stdio.h>

class Bird
{
    
public:
    
    Bird();
    Bird( char letter, int order, int w, int h , PolyBackground* p);
    
    void update(ofPoint t);
    void drawBasic();
    void drawDebug(int l);
    void updateAttraction(ofPoint p);
    void getEjected(ofVec2f v);
    void updateEjection(ofPoint p);
	void applyForce(ofVec2f force);
	void flock(vector<Bird> Birds);
	ofVec2f separate(vector<Bird> Birds);
	ofVec2f  align(vector<Bird> Birds);
	ofVec2f  cohesion(vector<Bird> Birds);
	ofVec2f seek(ofVec2f);
	void borders();
    
    ofPoint pos;
    ofPoint origin;
    ofVec2f speed;
	ofVec2f acc;
    ofVec2f force;
    ofVec2f noisedForce;

	//max
	float maxForce;
	float maxSpeed;

	//Flocking
	float swt; //multiply these force
	float awt;
	float cwt;
    
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

