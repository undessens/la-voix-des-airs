//
//  NicheManager.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 31/05/2022.
//

#include "NicheManager.hpp"

NicheManager::NicheManager(){
    polyBg = new PolyBackground();
}

NicheManager::NicheManager(ofParameterGroup* _pg,
                           int _w,                  // Final width of fbo
                           int _h,                   // Final height on fbo
                           int _screenW,
                           int _screenH
                           ){
    polyBg = new PolyBackground();
    pg = _pg;
    w = _w;
    h = _h;
    screenH = _screenH;
    screenW = _screenW;
    
    setup();
}

//------------------------------------------------
void NicheManager::setup(){
    

    
    // GUI parameter
    //pg->setName("NicheManager");
    pg->add(debug.set("debug", 0, 0, 5));
    pg->add(debugScale.set("debugScale", 14, 1, 50));
    pg->add(nbBird.set("nbBird", 0, 0, 300));
    pg->add(size.set("size", 88, 2, 150));
    pg->add(birdLineWidth.set("line width", 1, 0.1, 5));
    pg->add(birdDistanceLine.set("line btwn birds", 30, 1, 800));
    
    
    // Parameter to cartesian spring equation
    pg->add(separation.set("separation",0.28, 0, 1));
    pg->add(cohesion.set("cohesion",0.05, 0, 1));
    pg->add(alignment.set("alignment",0.12, 0, 1));
    pg->add(targetAttraction.set("target att", 1.2,0, 10.0 ));
    pg->add(maxSpeed.set("max speed", 5, 0.001, 15));
    pg->add(maxForce.set("max force", 0.6, 0.001, 2));
    pg->add(stiffness.set("stiffness", 0.26, 0.001, 2.0));
    pg->add(damping.set("damping", 0.05, 0.001, 4.0 ));
    pg->add(flyDuration.set("fly duration", 300, 0, 400)); // NOT USED ANYMORE
    pg->add(adaptativeFlyDuration.set(" ada fly dur", 200, 0, 3000));
    pg->add(attractionActive.set("attraction", false));
    pg->add(attractionFrequence.set("att freq", 1.2, 0.01, 10.));
    pg->add(attractionRadius.set("att radius", 400, 50, h ));
    pg->add(attractionHeight.set("att Y", 168, 0, h));
    
    //3D MODEL
    loadModel("../../../model/Bird_Asset_lowPoly_300.fbx");
    //loadModel("../../../model/birds/bird_01_white.fbx");
     //loadModel("../../../model/birds/bird_02.fbx");   // ULtra aplati
    //loadModel("../../../model/birds/bird_03.fbx"); // il est pas sur le zeo. Texture_white.jpg manquant.
    //loadModel("../../../model/birds/bird_04.fbx"); //BIEN !
    //loadModel("../../../model/Bird_Asset.fbx");
    //loadModel("../../../model/bird_white_emission.fbx");
    //loadModel("../../../model/bird_01.fbx");
    
    //TEXTURE ( CPU SAVE WHEN TARGET IN JOINED ... NOT USED)
//    birdImage.load("png_oiseau.png");
//    birdTexture = birdImage.getTexture(
    
    
    //Attraction
    att = ofVec2f(w/2, h/2);
    
}


//-----------------------------------------------------------------------------------------------------
vector<Niche> NicheManager::createNicheFromPolyline(vector<ofPolyline> start, vector<ofPolyline> end){
    
    
    //TODO : vérifier que les tailles sont équivalentes
    
    vector<Niche> result;
    
    for(int i = 0; i<start.size(); i++){
        Niche newNiche;
        
        
        for(int j = 0; j<start[i].size(); j++){
            
            ofVec2f startPoint = start[i][j];
            ofVec2f endPoint = end[i][j];
            int randomSize = size + ofRandom(-15, 15);
            Bird newBird = Bird(polyBg, endPoint, startPoint, randomSize , w, h, screenW, screenH, stiffness, 0, adaptativeFlyDuration, false);
            newBird.awt = alignment;
            newBird.swt = separation;
            newBird.cwt  = cohesion;
            newBird.twt = targetAttraction;
            
            newNiche.listOfBird.push_back(newBird);
            int lastNiche = newNiche.listOfBird.size() - 1;
            
        }
        
        result.push_back(newNiche);
        

        
        
    }
    
    
    return result;
}


//---------------------------------------------------
void NicheManager::addNeighbourFromNiche(Niche &n){
    

    for(int i=0; i< n.listOfBird.size(); i++){
        
        if(i>0){
            (n.listOfBird[i].neighbourLeft = &(n.listOfBird)[i-1]);
        }
        if(i==0 && n.listOfBird.size()>0){
            (n.listOfBird[i].neighbourLeft = &(n.listOfBird)[n.listOfBird.size() -1]);
        }
    }
    
}

//---------------------------------------------------
int NicheManager::update(Niche &n){
    
        int nbOfBirdLiving = 0;
        bool targetJoined = true;
    int minimalStateOfBird = 100;
        
        for (vector<Bird>::iterator it = n.listOfBird.begin(); it < n.listOfBird.end(); it++)
        {
            //CHANGE STATE : go to target if necessary. From WAITING TO MOVE
            it->changeState();
            // FLOCK : increasing accelation from forces ( interaction, target, mouse ... )
            it->flock(&n.listOfBird, att, attractionActive);
            // UPDATE there forces to calculate pos, speed, flying time, flying distance
            it->update();
            // BORDERS : teleportation from left to right, up to bottom
            if(it->state == BIRD_FREE){
                it->borders();
            }
            
            //Check is bird is alive
            if (it->state != BIRD_DIE ) {
                nbOfBirdLiving++;
            }
            if(it->state != BIRD_TARGETJOINED ){
                targetJoined = false;
            }
            minimalStateOfBird = min(it->state, minimalStateOfBird);
            
            
        }
    
    /*
     targetJoined is not used, but minimalStateOfBird > BIRD_TARGETJOINED is similar
     
     */
    
    n.stateOfNiche = minimalStateOfBird;
    return minimalStateOfBird;
        

}


//-------------------------------------
void NicheManager::drawBirds(Niche &n){
    
    for( vector<Bird>::iterator it = n.listOfBird.begin(); it < n.listOfBird.end() ; it++)
    {
        // IF bird is die : don't draw it
        if (it->state != BIRD_DIE && it->size>6) {
            drawModel(it);
            if(debug>0){
                it->drawDebug(debug);
            }
        }
        
        
        //Draw line between Neighbour - Invicible birds does not have neighbour
        if(!it->isInvicible){
            if(it->pos.distance(it->neighbourLeft->pos) <birdDistanceLine   && it->state!=BIRD_DIEONBORDER)
            {
                ofSetColor(255);
                ofSetLineWidth(birdLineWidth);
                ofDrawLine(it->pos,it->neighbourLeft->pos);
            }
        }

        
        
    }
    
}



//-------------------------------------------------------------------------
void NicheManager::drawModel(vector<Bird>::iterator it) {
    
    //Change wings movement for alls animations
    int index = (it->flyingDistance)/100.0 * (nbModelPose - 1);
    ofPushMatrix();
    ofSetColor(255);
    ofTranslate(it->pos.x, it->pos.y, 0);
    
    float angleRotateY = 0;
    float angleRotateZ = 0;
    
    //Rotate bird if still moving
    
    if(it->state != BIRD_TARGETJOINED){
        //ROTATE ON Y ( LEFT - RIGHT )
        if (abs(it->speed.x) > 1) {
            angleRotateY = abs(it->speed.x) / it->speed.x * 90;
        } else {
            angleRotateY = it->speed.x / 1 * 90.0f;
        }
        
        // ROTATE ON Z ( before Y but need Y )
        
        float onY = it->speed.dot(ofVec2f(0, 1)) / it->speed.length();
        if (angleRotateY > 0)
        {
            angleRotateZ = onY * 90;
        } else {
            angleRotateZ = -onY * 90;
        }
        
    }
    
    
    
    ofRotateDeg(angleRotateZ, 0, 0, 1);
    ofRotateDeg(angleRotateY, 0, 1, 0);
    
    //    int modelId = it -> order % nbModels;
    
    listOfModel[index].setRotation(1, 0, 0, angleRotateZ, angleRotateY);
    
    //FINAL TRANSLATE
    
    //ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    listOfModel[index].setScale((it->size)/1000.0, (it->size) /1000.0, (it->size) /1000.0);
    
    // Draw the kind of rendering you want
    listOfModel[index].drawFaces();
    //listOfModel[index].drawVertices();
    //listOfModel[index].drawWireframe();
    
    ofPopMatrix();
    
}




//--------------------------------------
void NicheManager::drawSmallLetter(Niche n){
    
    // Dessin de la petite lettre
        // TARGET JOINED
            // Draw shape now on fbo.
            //Letter Shape is OK !!!
            string shape = "ok";
            for( vector<Bird>::iterator it = n.listOfBird.begin(); it < n.listOfBird.end() ; it++)
            {
                ofSetColor(255);
                ofSetLineWidth(birdLineWidth);
                ofDrawLine(it->pos,it->neighbourLeft->pos);
                it->state = BIRD_DIE; // TODO : not necessary
    
            }

    
    
    
    
}


//-------------------------------------------------------------
void NicheManager::loadModel(string filename){
    ofLog() << "Loading model: " << filename ;
    for(int i=0; i< nbModelPose; i++){
        listOfModel[i] = ofxAssimpModelLoader();
        listOfModel[i].loadModel(filename);
        listOfModel[i].setLoopStateForAllAnimations(OF_LOOP_NORMAL);
        //listOfModel[atIndex][i].setPosition(0, 10, -5);
        listOfModel[i].setPosition(-5, 2, 0 );
        listOfModel[i].setPausedForAllAnimations(true);
        listOfModel[i].setPositionForAllAnimations((float)i/nbModelPose);
        listOfModel[i].update();
    }
    
}

//-------------------------------------------------------------
Niche NicheManager::createInvicibleArmy(){
    
    Niche newNiche;
    int armySize = 20;
    
    for (int i = 0; i < armySize; i++) {
        
        int randomSize = size + ofRandom(-15, 15);
        Bird newBird = Bird(polyBg, ofVec2f(0,0), ofVec2f(ofRandom(0,w),h) , randomSize, w, h, screenW, screenH, stiffness, 0, flyDuration, true);
        newBird.awt = alignment;
        newBird.swt = separation;
        newBird.cwt  = cohesion;
        newBird.twt = targetAttraction;
        newNiche.listOfBird.push_back(newBird);
        nbBird++;
        
    }
    
    return newNiche;
    
}
