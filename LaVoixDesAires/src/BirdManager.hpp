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
    void update(string msg);
    void draw();
    void drawDebug();
	void drawModel(vector<Bird>::iterator i);
    ofxAssimpModelLoader getModel(int distance);
    void loadModel(int atIndex, string filename);
    void loadModels(std::vector<string> filenames);
    
    void changeToLetter();
    
    //Manage instances
    int addBirdFromPolyline(ofPolyline target, ofPolyline initialPos);
    void setNbBird( int &i);
    void killAll();
	void lastFlyAll();
	void createInvicibleArmy();
    
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
	ofParameter<bool> attractionActive;
	ofParameter<float> attractionFrequence;
	ofParameter<int> attractionRadius;
	ofParameter<int> attractionHeight;

	//Attaction
	ofVec2f att;	//attractor point

	//3D MODEL
    static const int nbModelPose = 50;
    static const int nbModels = 4;
	ofxAssimpModelLoader listOfModel[nbModels][nbModelPose];
    
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
