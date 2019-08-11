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
BirdManager::BirdManager(PolyBackground* poly, ofParameterGroup* _pg){
    
    
    polyBg = poly;
    pg = _pg;
    setup();
    
}

//-------------------------------------------------------------
void BirdManager::setup(){
    
    listOfBird.clear();
    
    //Add Listener
    noiseDAmplitude.addListener(this, &BirdManager::setNoiseDAmplitude);
    noiseDFreq.addListener(this, &BirdManager::setNoiseDFreq);
    noiseSAmplitude.addListener(this, &BirdManager::setNoiseSAmplitude);
    noiseSFreq.addListener(this, &BirdManager::setNoiseSFreq);
    nbBird.addListener(this, &BirdManager::setNbBird);
    debug.addListener(this,&BirdManager::setDebug);
    debugScale.addListener(this,&BirdManager::setDebugScale);
    stiffness.addListener(this, &BirdManager::setStiffness);
    damping.addListener(this, &BirdManager::setDamping);
	separation.addListener(this, &BirdManager::setSeparation);
	alignment.addListener(this, &BirdManager::setAlignment);
	cohesion.addListener(this, &BirdManager::setCohesion);
	size.addListener(this, &BirdManager::setSize);
    
    // GUI parameter
    //pg->setName("birdmanager");
    pg->add(debug.set("debug", 0, 0, 5));
    pg->add(debugScale.set("debugScale", 14, 1, 50));
    pg->add(nbBird.set("nbBird", 0, 0, 300));
    pg->add(size.set("size", 10, 2, 300));
    
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

    pg->add(stiffness.set("stiffness", 0.05, 0.001, 2.0));
    pg->add(damping.set("damping", 0.05, 0.001, 4.0 ));

	for (int i = 0; i < nbBird; i++)
	{
		//addBird((char)60 + i, i+1);
	}
    
}

//-------------------------------------------------------------
void BirdManager::update(){

    ofPoint targetMouse = ofPoint(ofGetMouseX(), ofGetMouseY());
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
		it->flock(listOfBird);
		if (ofGetMousePressed()) {
			it->updateAttraction(ofPoint(targetMouse));
		}
		it->update(targetMouse);
		it->borders();

    }
    
}

//-------------------------------------------------------------
void BirdManager::draw(){
    
     ofPushView();
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
        it->drawBasic();
        //it->drawDebug(debug);
    }
    ofPopView();
    
    //DEBUG from manager
    if(debug > 0 ){
        //Write debug here
        //Write debug here
    }
    
}

//-------------------------------------------------------------
void BirdManager::addBird(char l, int order){
    
    Bird newBird = Bird(l, order, ofGetWidth(), ofGetHeight(), polyBg);
    listOfBird.push_back(newBird);
    
    nbBird = listOfBird.size();
    
}

//-------------------------------------------------------------
void BirdManager::newSequence(string word){
    
    
    for( int i = 0 ; i< word.size(); i++)
    {
        addBird( (char)word[i] ,i );
    }
    
}

//-------------------------------------------------------------
void BirdManager::killAll(){
    
    listOfBird.clear();
}

//-------------------------------------------------------------
void BirdManager::setNoiseDAmplitude(float &f){
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
        it->noiseDAmplitude = f;
    }
    
}

//-------------------------------------------------------------
void BirdManager::setNoiseSAmplitude(float &f){
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
        it->noiseSAmplitude = f;
    }
    
}

//-------------------------------------------------------------
void BirdManager::setNoiseDFreq(float &f){
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
        it->noiseDFreq = f;
    }
    
}

//-------------------------------------------------------------
void BirdManager::setNoiseSFreq(float &f){
    
    for( vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end() ; it++)
    {
        it->noiseSFreq = f;
    }
    
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
void BirdManager::setNbBird(int &nb){
    
    if(nb > listOfBird.size()){
        
        //Create new bird
        int nbToCreate = nb - listOfBird.size();
        for (int i = 0; i< nbToCreate ; i++){
            addBird((char)(61+ofRandom(24)), listOfBird.size()-1);
        }
        
    }
    else if(nb < listOfBird.size()){
        
        //Delete existing bird
        int nbToDelete = listOfBird.size() - nb;
        
        for(int i = 0; i<nbToDelete; i++){
            listOfBird.pop_back();
        }
        
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
void BirdManager::setSize(int &i) {

	for (vector<Bird>::iterator it = listOfBird.begin(); it < listOfBird.end(); it++)
	{
		it->size= i;
	}

}




//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------


