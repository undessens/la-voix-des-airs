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
BirdManager::BirdManager(PolyBackground* poly, ofParameterGroup* _pg, int _w, int _h){
    
    
    polyBg = poly;
    pg = _pg;
    w = _w;
    h = _h;
    setup();
    
}

//-------------------------------------------------------------
void BirdManager::setup(){
    
    listOfBird.clear();
	listOfBird.reserve(200);
    
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
    
    // GUI parameter
    //pg->setName("birdmanager");
    pg->add(debug.set("debug", 0, 0, 5));
    pg->add(debugScale.set("debugScale", 14, 1, 50));
    pg->add(nbBird.set("nbBird", 0, 0, 300));
    pg->add(size.set("size", 50, 2, 500));
    
    // Direction Noise , bird change from left to right while reaching the target point
   // pg->add(noiseDAmplitude.set("noiseDAmplitude", 1.0, 0.0, 13.0));
    //pg->add(noiseDFreq.set("noiseDFreq", 0.01, 0.01, 10.0));
    
    // Speed Noise . Bird change its speed, while reaching the target point
    //pg->add(noiseSAmplitude.set("noiseSAmplitude", 0.0, 0.0, 13.0));
    //pg->add(noiseSFreq.set("noiseSFreq", 0.01, 0.01, 1.0));
    
    // Parameter to cartesian spring equation
	pg->add(separation.set("separation",0.1, 0, 1));
	pg->add(cohesion.set("cohesion",0.1, 0, 1));
	pg->add(alignment.set("alignment",0.1, 0, 1));
    pg->add(targetAttraction.set("target att", 0.0,0, 10.0 ));
    pg->add(maxSpeed.set("max speed", 5, 0.001, 15));
    pg->add(maxForce.set("max force", 0.25, 0.001, 0.8));

    pg->add(stiffness.set("stiffness", 0.05, 0.001, 2.0));
    pg->add(damping.set("damping", 0.05, 0.001, 4.0 ));

	for (int i = 0; i < nbBird; i++)
	{
		//addBird((char)60 + i, i+1);
	}
    
	//3D model

	model.loadModel("Bird_Asset.fbx", true);
	model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
	model.setPosition(0, 10, -5);
	model.resetAllAnimations();
	model.playAllAnimations();


}

//-------------------------------------------------------------
void BirdManager::update(){

    ofPoint targetMouse = ofPoint(ofGetMouseX(), ofGetMouseY());

	//3D MODEL
	model.update();
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
		it->flock(&listOfBird);
		it->update(targetMouse);
		it->borders();

    }
    
}

//-------------------------------------------------------------
void BirdManager::draw(){
    
    //ofPushView();
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
		drawModel(it);

        //Draw line between letter
        if(it->pos.distance(it->neighbourLeft->pos) <100) {
            ofDrawLine(it->pos,it->neighbourLeft->pos);
        }

        it->drawDebug(debug);
    }
    //ofPopView();
    
    
}
//-------------------------------------------------------------
void BirdManager::drawModel(vector<Bird>::iterator it) {

	ofPushMatrix();
	ofTranslate(it->pos.x, it->pos.y, 0);

	//ROTATE ON Y ( LEFT - RIGHT )
	
	float angleRotateY = 0;
	if (abs(it->speed.x) > 1) {
		angleRotateY = abs(it->speed.x) / it->speed.x * 90;
	}
	else {
		angleRotateY = it->speed.x / 1 * 90.0f;
	}

	// ROTATE ON Z ( before Y but need Y )
	float angleRotateZ = 0;
	float onY = it->speed.dot(ofVec2f(0, 1)) / it->speed.length();
	if (angleRotateY > 0) {
		angleRotateZ = onY * 90;
	}
	else
	{
		angleRotateZ = -onY * 90;
	}

	
	ofRotateDeg(angleRotateZ, 0, 0, 1);
	ofRotateDeg(angleRotateY, 0, 1, 0);

	
	model.setRotation(1, 0, 0, angleRotateZ, angleRotateY);
	//FINAL TRANSLATE
	//ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);

	model.setScale(size/1000.0, size /1000.0, size /1000.0);

	model.drawFaces();
	ofPopMatrix();

}


//-------------------------------------------------------------
void BirdManager::addBird(ofPolyline p){
    if(p.size()>0){
    
        int count = listOfBird.size();
        
        for(int i=0; i<p.size(); i++){
            
             Bird newBird = Bird(polyBg,p[i], size, w, h);
             listOfBird.push_back(newBird);
        }
        
        //Create neighbour now
         for(int i=0; i<p.size(); i++){
          
             if(i>0){
             listOfBird[count + i].neighbourLeft = &(listOfBird[count + i - 1]);
             }
             if(i==0){
                 listOfBird[count].neighbourLeft = &(listOfBird[count + p.size() -1]);
             }
             
         }
        
        
    }

}

//-------------------------------------------------------------
void BirdManager::killAll(){
    
    listOfBird.clear();
}

//-------------------------------------------------------------
void BirdManager::setStiffness(float &f){
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
        it->stiffness = f;
    }
    
}

//-------------------------------------------------------------
void BirdManager::setDamping(float &f){
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
        it->damping = f;
    }
    
}



//-------------------------------------------------------------
void BirdManager::setDebug(int &i){
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
        it->debugLevel = i;
    }
    
}

//-------------------------------------------------------------
void BirdManager::setDebugScale(int &i){
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
        it->debugScale = i;
    }
    
}

//-------------------------------------------------------------
void BirdManager::setSeparation(float &i) {

	for (vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end(); it++)
	{
		it->swt= i;
	}

}

//-------------------------------------------------------------
void BirdManager::setCohesion(float &i) {

	for (vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end(); it++)
	{
		it->cwt = i;
	}

}

//-------------------------------------------------------------
void BirdManager::setAlignment(float &i) {

	for (vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end(); it++)
	{
		it->awt = i;
	}

}

//-------------------------------------------------------------
void BirdManager::setTargetAttraction(float &i) {
    
    for (vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end(); it++)
    {
        it->twt = i;
    }
    
}

//-------------------------------------------------------------
void BirdManager::setSize(int &i) {

	for (vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end(); it++)
	{
		it->size= i;
	}

}

//-------------------------------------------------------------
void BirdManager::setMaxSpeed(float &f) {
    
    for (vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end(); it++)
    {
        it->maxSpeed= f;
    }
    
}

//-------------------------------------------------------------
void BirdManager::setMaxForce(float &f) {
    
    for (vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end(); it++)
    {
        it->maxForce= f;
    }
    
}



//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------


