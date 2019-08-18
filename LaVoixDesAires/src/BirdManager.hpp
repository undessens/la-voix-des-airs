//
//  bird_manager.hpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#ifndef bird_manager_hpp
#define bird_manager_hpp
#include "Bird.hpp"
#include "PolyBackground.hpp"
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOscParameterSync.h"

#include <stdio.h>

class BirdManager
{
    
public:
    
    BirdManager();
    BirdManager(PolyBackground* poly, ofParameterGroup* pg);
    void setup();
    void update();
    void draw();
	void drawModel(vector<Bird>::iterator i);
    
    void changeToLetter();
    
    void killAll();
    
    //Manage instances
    void addBird(char letter, int order, ofVec2f t);
    void setNbBird( int &i);
    
    
    //Setter to all instances
    void setNoiseDAmplitude( float &f);
    void setNoiseDFreq( float &f);
    void setNoiseSAmplitude (float &f);
    void setNoiseSFreq( float &f);
    void setDebug(int &i);
    void setDebugScale(int &i);
    void setStiffness(float &f);
    void setDamping(float &f);
	void setSeparation(float &f);
	void setCohesion(float &f);
	void setAlignment(float &f);
	void setSize(int &f);
    
    vector<Bird> listOfBird;
    PolyBackground* polyBg;
    
    //GUI
    ofParameterGroup* pg;
    ofParameter<int> debug;
    ofParameter<int> debugScale;
    ofParameter<int> nbBird;
    ofParameter<int> size;
    ofParameter<float> noiseDAmplitude;   // Direction noise
    ofParameter<float> noiseDFreq;
    ofParameter<float> noiseSAmplitude;
    ofParameter<float> noiseSFreq;        // Speed Noise
    ofParameter<float> stiffness;
    ofParameter<float> damping;
	ofParameter<float> separation;
	ofParameter<float> alignment;
	ofParameter<float> cohesion;

	//3D MODEL
	ofxAssimpModelLoader model;

    
    
    
    
    
    
};

#endif /* bird_manager_hpp */
