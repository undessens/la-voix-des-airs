//
//  bird_manager.cpp
//  LaVoixDesAires
//
//  Created by Admin Mac on 10/07/2019.
//

#include "BirdManager.hpp"

BirdManager::BirdManager(){
    
    setup();
    
}

//-------------------------------------------------------------
BirdManager::BirdManager(PolyBackground* poly,
                         ofParameterGroup* _pg,
                         int _w,
                         int _h,
                         int _screenW,
                         int _screenH
                         ){
    
    
    polyBg = poly;
    pg = _pg;
    w = _w;
    h = _h;
    screenW = _screenW;
    screenH = _screenH;
    setup();
    
}

//-------------------------------------------------------------
void BirdManager::setup(){
    
    listOfBird.clear();
	listOfBird.reserve(2000);
    
    //Add Listener
    debug.addListener(this,&BirdManager::setDebug);
    debugScale.addListener(this,&BirdManager::setDebugScale);
    stiffness.addListener(this, &BirdManager::setStiffness);
    damping.addListener(this, &BirdManager::setDamping);
	separation.addListener(this, &BirdManager::setSeparation);
	alignment.addListener(this, &BirdManager::setAlignment);
	cohesion.addListener(this, &BirdManager::setCohesion);
    targetAttraction.addListener(this, &BirdManager::setTargetAttraction);
	size.addListener(this, &BirdManager::setSize);
    maxSpeed.addListener(this, &BirdManager::setMaxSpeed);
    maxForce.addListener(this, &BirdManager::setMaxForce);
    flyDuration.addListener(this, &BirdManager::setFlyDuration);
    
    // GUI parameter
    //pg->setName("birdmanager");
    pg->add(debug.set("debug", 0, 0, 5));
    pg->add(debugScale.set("debugScale", 14, 1, 50));
    pg->add(nbBird.set("nbBird", 0, 0, 300));
    pg->add(size.set("size", 55, 2, 100));
    
    
    // Parameter to cartesian spring equation
	pg->add(separation.set("separation",0.1, 0, 1));
	pg->add(cohesion.set("cohesion",0.1, 0, 1));
	pg->add(alignment.set("alignment",0.1, 0, 1));
    pg->add(targetAttraction.set("target att", 5.1,0, 10.0 ));
    pg->add(maxSpeed.set("max speed", 5, 0.001, 15));
    pg->add(maxForce.set("max force", 2, 0.001, 2));
    pg->add(stiffness.set("stiffness", 1.0, 0.001, 2.0));
    pg->add(damping.set("damping", 0.05, 0.001, 4.0 ));
    pg->add(flyDuration.set("fly duration", 300, 0, 6000));
	pg->add(attractionActive.set("attraction", true));
	pg->add(attractionFrequence.set("att freq", 1.2, 0.01, 10.));
	pg->add(attractionRadius.set("att radius", 400, 50, h ));
	pg->add(attractionHeight.set("att Y", 168, 0, h));

    
    std::vector<string> modelsFilePaths = {
        "../../../model/Bird_Asset_lowPoly_300.fbx",
        //"../../../model/green_bird.fbx",
        "../../../model/blue_bird.fbx",
        //"../../../model/tropical_bird.fbx",
//        "../../../model/source_phoenix/fly.fbx"
    };
	//3D MODEL
     loadModels(modelsFilePaths);
    
    //loadModel("../../../model/Bird_Asset_lowPoly.fbx");
    //loadModel("../../../model/Bird_Asset.fbx");
    
    //TEXTURE ( CPU SAVE WHEN TARGET IN JOINED )
    birdImage.load("png_oiseau.png");
    birdTexture = birdImage.getTexture();
    

}

//-------------------------------------------------------------
void BirdManager::update(string msg){

	//Calculte msg size
    int sizeMsg = 0;
    std::vector<string> listOfWords = ofSplitString(msg, " ");
    
    for(vector<string>::iterator its = listOfWords.begin(); its< listOfWords.end(); its++)
    {
        sizeMsg += ofUTF8Length(*its);
    }
    
   // ofLog(OF_LOG_NOTICE, "SizeMsg : "+ofToString(sizeMsg));


	//Update attraction point
	float angle = ofGetElapsedTimef() * attractionFrequence;
	att = ofVec2f(attractionRadius*cos(angle) + w / 2, attractionRadius*sin(angle) + attractionHeight);
	if (ofRandom(1) > 0.99 && attractionActive) {
		attractionActive = false;
	}
	if (ofRandom(1) > 0.996 && !attractionActive) {
		attractionActive = true;
	}

	//Update bird One by one
	//For ----------------------------------------------------------------------------DO NOT INCREMENT in Case we delete
	for (vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end(); ) {

		int nbOfBirdLiving = 0;

		for (vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end(); it++)
		{
			//CHANGE STATE : go to target if necessary
			it->changeState(sizeMsg);
			// FLOCK : increasing accelation from forces ( interaction, target, mouse ... )
			it->flock(&(*itn), att, attractionActive);
			// UPDATE there forces to calculate pos, speed, flying time, flying distance
			it->update();
			// BORDERS : teleportation from left to right, up to bottom
			it->borders();
			//Check is bird is alive
			if (it->state != BIRD_DIE) {
				nbOfBirdLiving++;
			}


		}

		if (nbOfBirdLiving == 0) {
			// Kill the niché
			itn = listOfBird.erase(itn);
		}
		else {
			//Weird but true
			++itn;
		}

	}
    
}

//-------------------------------------------------------------
void BirdManager::draw(){
    
    //ofPushView();
    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
    for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
    {
		// IF bird is die : don't draw it
		if (it->state != BIRD_DIE) {
			drawModel(it);
		}
		

        //Draw line between Neighbour - letter - not used anymore
        //   if(it->pos.distance(it->neighbourLeft->pos) <200)
        //   {
        //      ofSetColor(240);
        //      ofSetLineWidth(1);
        //      ofDrawLine(it->pos,it->neighbourLeft->pos);
        // }

        
    }
    //ofPopView();
    
    
}
//-------------------------------------------------------------------------
void BirdManager::drawDebug(){
    
	if (debug > 0) {
		if (attractionActive) {
			ofFill();
			ofDrawCircle(att, 10);
		}
	}
	
	
	for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
    for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
        {
			if (it->state != BIRD_DIE) {
				ofNoFill();
				it->drawDebug();
			}

        }
}

//-------------------------------------------------------------------------
void BirdManager::drawModel(vector<Bird>::iterator it) {

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
    int modelId = 0;

    listOfModel[modelId][index].setRotation(1, 0, 0, angleRotateZ, angleRotateY);
    
    //FINAL TRANSLATE
    
    //ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    listOfModel[modelId][index].setScale((it->size)/1000.0, (it->size) /1000.0, (it->size) /1000.0);
    
    // Draw the kind of rendering you want
	listOfModel[modelId][index].drawFaces();
    //listOfModel[index].drawVertices();
    //listOfModel[index].drawWireframe();
	
	
	ofPopMatrix();

}

void BirdManager::loadModels(std::vector<string> filenames){
    for(int i=0; i < filenames.size(); i++){
        loadModel(i, filenames[i]);
    }
}
//-------------------------------------------------------------
void BirdManager::loadModel(int atIndex, string filename){
    ofLog() << "Loading model: " << filename ;
    for(int i=0; i< nbModelPose; i++){
        listOfModel[atIndex][i] = ofxAssimpModelLoader();
        listOfModel[atIndex][i].loadModel(filename);
        listOfModel[atIndex][i].setLoopStateForAllAnimations(OF_LOOP_NORMAL);
        //listOfModel[atIndex][i].setPosition(0, 10, -5);
        listOfModel[atIndex][i].setPosition(-5, 2, 0 );
        listOfModel[atIndex][i].setPausedForAllAnimations(true);
        listOfModel[atIndex][i].setPositionForAllAnimations((float)i/nbModelPose);
        listOfModel[atIndex][i].update();
    }
    
}

//-------------------------------------------------------------
void BirdManager::addBird(ofPolyline p){
    if(p.size()>0)
    {
        int nbNiche = listOfBird.size();
        vector<Bird> newNiche;
        
        for(int i=0; i<p.size(); i++){
            
            if(p[i].x < w && p[i].y < h){
                int randomSize = size + ofRandom(-15, 15);
                Bird newBird = Bird(polyBg, p[i], randomSize , w, h, screenW, screenH, stiffness, nbNiche, flyDuration,false);
                newNiche.push_back(newBird);
            }
        }
        
        listOfBird.push_back(newNiche);
        //Create neighbour now -- NOT USED ANYMORE
         /*for(int i=0; i<p.size(); i++){
          
             if(i>0){
             (listOfBird[nbNiche])[i].neighbourLeft = &((listOfBird[nbNiche])[i-1]);
             }
             if(i==0){
                (listOfBird[nbNiche])[i].neighbourLeft = &((listOfBird[nbNiche])[p.size() -1]);
             }
         }*/
    }
    nbBird += p.size();
}

//-------------------------------------------------------------
void BirdManager::killAll(){
    
    listOfBird.clear();
    nbBird = 0;
}

//-------------------------------------------------------------
void BirdManager::createInvicibleArmy() {

		vector<Bird> newNiche;
		int armySize = 20;

		for (int i = 0; i < armySize; i++) {
						
			int randomSize = size + ofRandom(-15, 15);
			Bird newBird = Bird(polyBg, ofVec2f(0,0), randomSize, w, h, screenW, screenH, stiffness, 100000, flyDuration, true);
			newNiche.push_back(newBird);
			
		}

		listOfBird.push_back(newNiche);

		nbBird++;
}

//-------------------------------------------------------------
void BirdManager::lastFlyAll() {

	//Instead of clear all, push all birds into a DIE ON BORDER Mode
	for (vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end(); itn++)
		for (vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end(); it++)
		{
			it->goDieOnBorder();
		}
	
}

//-------------------------------------------------------------
void BirdManager::setStiffness(float &f){
    
    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
        for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
        {
            it->stiffness = f;
        }
    
}

//-------------------------------------------------------------
void BirdManager::setDamping(float &f){
    
    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
        for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
        {
            it->damping = f;
        }
    
}



//-------------------------------------------------------------
void BirdManager::setDebug(int &i){
    
    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
        for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
        {
            it->debugLevel = i;
        }
    
}

//-------------------------------------------------------------
void BirdManager::setDebugScale(int &i){
    
    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
        for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
        {
            it->debugScale = i;
        }
    
}

//-------------------------------------------------------------
void BirdManager::setSeparation(float &i) {

    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
        for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
        {
            it->swt= i;
        }

}

//-------------------------------------------------------------
void BirdManager::setCohesion(float &i) {

    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
        for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
        {
            it->cwt = i;
        }

}

//-------------------------------------------------------------
void BirdManager::setAlignment(float &i) {

    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
        for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
        {
            it->awt = i;
        }

}

//-------------------------------------------------------------
void BirdManager::setTargetAttraction(float &i) {
    
    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
    for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
    {
        it->twt = i;
    }
    
}

//-------------------------------------------------------------
void BirdManager::setSize(int &i) {

	for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
    for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
	{
		it->size= i;
	}

}

//-------------------------------------------------------------
void BirdManager::setMaxSpeed(float &f) {
    
    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
    for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
    {
        it->maxSpeed= f;
    }
    
}

//-------------------------------------------------------------
void BirdManager::setMaxForce(float &f) {
    
    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
    for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
    {
        it->maxForce= f;
    }
    
}

//-------------------------------------------------------------
void BirdManager::setFlyDuration(int &i){
    
    for( vector<vector<Bird>>::iterator itn = listOfBird.begin(); itn < listOfBird.end() ; itn++)
        for( vector<Bird>::iterator it = (*itn).begin(); it < (*itn).end() ; it++)
        {
            it->flyingDuration = i;
        }
    
}


//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------


