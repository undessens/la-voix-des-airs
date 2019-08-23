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
    BirdManager(PolyBackground* poly,
                ofParameterGroup* pg,
                int w,                  // Final width of fbo
                int h,                   // Final height on fbo
                int screenW,
                int screenH
                );
    void setup();
    void update();
    void draw();
	void drawModel(vector<Bird>::iterator i);
    ofxAssimpModelLoader getModel(int distance);
    void loadModel(string filename);
    
    void changeToLetter();
    
    //Manage instances
    void addBird(ofPolyline p);
    void setNbBird( int &i);
    void killAll();
    
    //Setter to all instances
    void setDebug(int &i);
    void setDebugScale(int &i);
    void setStiffness(float &f);
    void setDamping(float &f);
	void setSeparation(float &f);
	void setCohesion(float &f);
	void setAlignment(float &f);
    void setTargetAttraction(float &f);
    void setMaxSpeed(float &f);
    void setMaxForce(float &f);
	void setSize(int &f);
    void setFlyDuration(int &i);
    
    vector<vector<Bird>> listOfBird;
    PolyBackground* polyBg;
    
    //GUI
    ofParameterGroup* pg;
    ofParameter<int> debug;
    ofParameter<int> debugScale;
    ofParameter<int> nbBird;
    ofParameter<int> size;

    ofParameter<float> stiffness;
    ofParameter<float> damping;
	ofParameter<float> separation;
	ofParameter<float> alignment;
	ofParameter<float> cohesion;
    ofParameter<float> targetAttraction;
    ofParameter<float> maxSpeed;
    ofParameter<float> maxForce;
    ofParameter<int> flyDuration;

	//3D MODEL
    static const int nbModelPose = 50;
	ofxAssimpModelLoader listOfModel[nbModelPose];
    
    //Texture of bird ( save CPU )
    ofImage birdImage;
    ofTexture birdTexture;
    
    //Geometry
    int w;
    int h;
    int screenW;
    int screenH;

    
};

#endif /* bird_manager_hpp */
