//
//  NicheManager.hpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 31/05/2022.
//

#ifndef NicheManager_hpp
#define NicheManager_hpp

#include <stdio.h>
#include "Niche.hpp"
#include "ofMain.h"

class NicheManager{
    
public:
    NicheManager();
    NicheManager( ofParameterGroup* pg,
                  int w,                  // Final width of fbo
                  int h,                   // Final height on fbo
                  int screenW,
                  int screenH
                 );
    void setup();
    int update(Niche &n); // passer sous forme de référence .. return si la forme si l'arrivée est atteinte.
    void updateAttractionPoint();
    void drawBirds(Niche &n);
    void drawSmallLetter(Niche n);
    vector<Niche> createNicheFromPolyline(vector<ofPolyline> start,vector<ofPolyline> end );
    Niche createEphemereNicheFromPolyline(ofPolyline p );
    Niche createInvicibleArmy();
    void addNeighbourFromNiche( Niche &n);
    PolyBackground* polyBg;
    
    //Geometry
    int w;
    int h;
    int screenW;
    int screenH;
    
    
    //GUI
    ofParameterGroup* pg;
    ofParameter<int> debug;
    ofParameter<int> debugScale;
    ofParameter<int> nbBird;
    ofParameter<int> size;
    ofParameter<int> model;
    ofParameter<float> birdLineWidth;
    ofParameter<int> birdDistanceLine;
    
    ofParameter<float> stiffness;
    ofParameter<float> damping;
    ofParameter<float> separation;
    ofParameter<float> alignment;
    ofParameter<float> cohesion;
    ofParameter<float> targetAttraction;
    ofParameter<float> maxSpeed;
    ofParameter<float> maxForce;
    ofParameter<int> flyDuration;
    ofParameter<int> adaptativeFlyDuration;
    ofParameter<bool> attractionActive;
    ofParameter<float> attractionFrequence;
    ofParameter<int> attractionRadius;
    ofParameter<int> attractionHeight;
    
    //GUI Setter to all instances
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
    
    //Attaction
    ofVec2f att;    //attractor point
    
    //3D MODEL
    static const int nbModelPose = 100;
    ofxAssimpModelLoader listOfModel[nbModelPose];
    
    void drawModel(vector<Bird>::iterator i);
    ofxAssimpModelLoader getModel(int distance);
    void loadModel(string filename);
    void loadModelFromList(int &i);

    
    //Texture of bird ( save CPU )
    ofImage birdImage;
    ofTexture birdTexture;
    
};

#endif /* NicheManager_hpp */
