#include "LetterManager.hpp"

LetterManager::LetterManager()
{
}

//--------------------------------------------------------------
LetterManager::LetterManager(NicheManager* b, PolyBackground* p, ofParameterGroup* _pg, int _w, int _h)
{
    msg = "";
    currentLineCharacter = 0;
    msgPosition = ofPoint(257, 377);
    nextLetterPosition = msgPosition;
    
    w = _w;
    h = _h;

//    Verdana 60pt bbox on 1440px
//    top-left x:219, y:282
//    down-right x:1207, y:349 ==> ~900
    fontSpacing = 23;
    fontSize= 40;
    fontDistSampling = 7.0;
    changeFontSize(fontSize);
    
    nicheManager = b;
    polybackground = p;
    
    nicheManager->stiffness.addListener(this, &LetterManager::setStiffness);
    nicheManager->debugScale.addListener(this, &LetterManager::setDebugScale);
    nicheManager->damping.addListener(this, &LetterManager::setDamping);
    nicheManager->separation.addListener(this, &LetterManager::setSeparation);
    nicheManager->cohesion.addListener(this, &LetterManager::setCohesion);
    nicheManager->alignment.addListener(this, &LetterManager::setAlignment);
    nicheManager->targetAttraction.addListener(this, &LetterManager::setTargetAttraction);
    nicheManager->maxSpeed.addListener(this, &LetterManager::setMaxSpeed);
    nicheManager->maxForce.addListener(this, &LetterManager::setMaxForce);
    nicheManager->size.addListener(this, &LetterManager::setSize);
    nicheManager->flyDuration.addListener(this, &LetterManager::setFlyDuration);
    
    permanentNiche = nicheManager->createInvicibleArmy();
    temporaryNiche.clear();

    gFontSize.addListener(this, &LetterManager::changeFontSize);
    gFontSpacing.addListener(this, &LetterManager::changeFontSpacing);
    gMsgPositionX.addListener(this, &LetterManager::changeMsgPositionX);
    gMsgPositionY.addListener(this, &LetterManager::changeMsgPositionY);
    gfontDistSampling.addListener(this, &LetterManager::changeFontSampling);
    borderLetter1X.addListener(this, &LetterManager::updateRectangle);
    borderLetter1W.addListener(this, &LetterManager::updateRectangle);
	borderLetter2X.addListener(this, &LetterManager::updateRectangle);
	borderLetter2W.addListener(this, &LetterManager::updateRectangle);
    saveRectangleWithoutLetter.addListener(this, &LetterManager::saveRectangle);
    
    pg = _pg;
    pg->setName("Text Manager");
    pg->add(drawMsgContour.set("Draw Contour", false));
    pg->add(drawMsgFill.set("Draw Fill", false));
    pg->add(gFontSize.set("Font Size", fontSize, 45, 150));
    pg->add(gFontSpacing.set("Font Spacing", fontSpacing, 10, 200));
	pg->add(gMsgPositionX.set("Pos X", msgPosition.x, 10, 1000));
	pg->add(gMsgPositionY.set("Pos Y", msgPosition.y, 10, 600));
	pg->add(gfontDistSampling.set("Char Sampling", fontDistSampling, 1, 20));
    pg->add(zoomBigLetter.set("zoom big letter", 8 , 1, 30));
    pg->add(alphaBigLetter.set("alpha big letter", 245 , 0, 255));
	pg->add(positionBigLetter.set("Y big letter", 810, 0, h));
    pg->add(borderLetter1X.set("rect letter X", 519 , 0, w));
    pg->add(borderLetter1W.set("rect letter W", 174, 0, w/2));
	pg->add(borderLetter2X.set("rect letter X", 1123, 0, w));
	pg->add(borderLetter2W.set("rect letter W", 146, 0, w / 2));
	pg->add(borderOffsetY.set("border offsetY", 21, 0, 25));
    pg->add(saveRectangleWithoutLetter.set("save rect", false));

    
    //WRITING SPEED . TIME OF LAST LETTER
	for (int i = 0; i < MAX_LETTER; i++) {
		timeOfLastLetter[i] = 0.f;
	}
    writingSpeed = 0;
    
    // RECTANGLE WITHOUT LETTER
    if(loadRectangle("rectanglewithoutletter.xml")){
        
    }else{
        borderLetter1 = ofRectangle(borderLetter1X, 0, borderLetter1W, h);
    }

	osc_sender.setup("10.0.1.104", 12340);
    
}

//--------------------------------------------------------------
LetterManager::~LetterManager()
{
}

//--------------------------------------------------------------
void LetterManager::changeFontSampling(float &s) {
	fontDistSampling = s;
}

//--------------------------------------------------------------
void LetterManager::changeMsgPositionX(int &x) {
	msgPosition.x = x;
	// If msg empty, update the new position in real time
	if (msg.size() == 0) {
		nextLetterPosition = msgPosition;
	}
}

//--------------------------------------------------------------
void LetterManager::changeMsgPositionY(int &y) {
	msgPosition.y = y;
	// If msg empty, update the new position in real time
	if (msg.size() == 0) {
		nextLetterPosition = msgPosition;
	}
}

//--------------------------------------------------------------
void LetterManager::changeFontSize(int &newSize) {
	fontSize = newSize;
	msgFont.load(msgFontName, fontSize, true, true, true);
	birdFont.load(birdFontName, fontSize, true, true, true);
}

//--------------------------------------------------------------
void LetterManager::changeFontSpacing(int &newSpacing) {
	fontSpacing = newSpacing;
}

//--------------------------------------------------------------
void LetterManager::update(){
    
    for (auto &letter : listOfLetter) // access by reference to avoid copying
    {
        letter->update();
    }
    

    nicheManager->update(permanentNiche);
	vector<Niche>::iterator itToErase;

    for(vector<Niche>::iterator it = temporaryNiche.begin() ;it!=temporaryNiche.end(); ){
        
        int miniState = nicheManager->update(*it);
        if(miniState>=5 ){ // Bird Die
            temporaryNiche.erase(it);
        }else{
            ++it;
        }
        
    }
    
    
    
}

//--------------------------------------------------------------
void LetterManager::draw() {
	//DRAW only the last letter shortly
	
    drawLetter();
    drawBirds();
    
		
}

//--------------------------------------------------------------
void LetterManager::drawLetter() {
    //DRAW only the last letter shortly
    
    //ofPushMatrix();
    for (auto &letter : listOfLetter) // access by reference to avoid copying
    {
        letter->drawLetter();
        
    }
    //ofPopMatrix();
    
    
}

//--------------------------------------------------------------
void LetterManager::drawBirds() {
    //DRAW only the last letter shortly
    
    ofPushMatrix();
    for (auto &letter : listOfLetter) // access by reference to avoid copying
    {
        letter->drawBirds();
        
    }
    
    nicheManager->drawBirds(permanentNiche);
    
    for(auto &niche : temporaryNiche){
        nicheManager->drawBirds(niche);
    }
    
    ofPopMatrix();
    
    
}

//--------------------------------------------------------------
void LetterManager::drawDebug() {

    if(nicheManager->debug>0){
        ofSetColor(0,255,0);
        ofNoFill();
        ofDrawCircle(nextLetterPosition.x, nextLetterPosition.y, 20);
        
        ofSetColor(ofColor::blue);
        ofDrawRectangle(borderLetter1);
		ofDrawRectangle(borderLetter2);
    }
    
    
    
}


//--------------------------------------------------------------
ofPath LetterManager::createPathFromLetter( int letter, ofVec2f newLetterPosition){
    //NOT FILLED
    // permet de récuperer facilement un polyline avec la liste des points
    // Ne permet pas de bien déssiner le ofpath
    ofPath lastLetter = msgFont.getCharacterAsPoints(letter, true , false);
   
    lastLetter.translate(newLetterPosition);
    return lastLetter;
    
}

//--------------------------------------------------------------
ofPath LetterManager::createFilledPathFromLetter(int letter, ofVec2f newLetterPosition){
    // Ne permet pas de bien récuperer les polyline, mais permet de dessiner.
    ofPath lastLetter = msgFont.getCharacterAsPoints(letter, true , true);
    lastLetter.translate(newLetterPosition);
    return lastLetter;
    
}

//--------------------------------------------------------------
ofRectangle LetterManager::getBoundingBoxOfPath(ofPath &path){
    
    ofRectangle rect;
    for(int i = 0; i<path.getOutline().size(); i++){
        ofRectangle b = path.getOutline().at(i).getBoundingBox();
        if( i==0) rect = b;
        else rect.growToInclude(b);
    }
    
    return rect;
    
}

//--------------------------------------------------------------
vector<ofPolyline> LetterManager::reduceDistanceSampling( ofPath path){
    // EN GROS : transforme un ofPath en vector<ofPolyline>, mais en plus, il
    // il faut une manip de ResampleBySpace.
    // permet de récuperer facilement un polyline avec la liste des points
    // Ne permet pas de bien déssiner le ofpath
    vector<ofPolyline> listOfPoly;
    
    int size = path.getOutline().size();
    for(int i=0; i<size; i++){
        
        ofPolyline poly = path.getOutline()[i];
        poly = poly.getResampledBySpacing(fontDistSampling);
        listOfPoly.push_back(poly);
    }
    
    return listOfPoly;
    
}

//--------------------------------------------------------------
void LetterManager::addLetter(int letter) {
    
	//Calculate speed of writing
    if(msg.length()>0){
        float newspeed = 1.0f / (ofGetElapsedTimef() - timeOfLastLetter[msg.length()-1]) ;
        float valueToAdd = newspeed - writingSpeed;
        writingSpeed = writingSpeed + 0.5 * valueToAdd;
        writingSpeed = newspeed;
        
        
        nicheManager->adaptativeFlyDuration = nicheManager->flyDuration * (( 1.0f / writingSpeed)/1.0f);
        if(nicheManager->adaptativeFlyDuration>nicheManager->flyDuration ){
            nicheManager->adaptativeFlyDuration = nicheManager->flyDuration;
        }

   }
    
    
    int prevMsgLength = ofUTF8Length(msg);
    if(letter==13){
        // RETOUR A LA LIGNE
        nextLetterPosition.y = nextLetterPosition.y + fontSize + 30;
        nextLetterPosition.x = msgPosition.x;
        currentLineCharacter = 0;

		// SEND OSC MESSAGE TO MAC
		ofxOscMessage m;
		m.setAddress("/back");
		m.addInt32Arg(0);
		osc_sender.sendMessage(m);
		m.clear();

    }else{
        // LETTRE NORMALE ( espace compris )
        if (prevMsgLength < MAX_LETTER && currentLineCharacter <= MAX_LETTER_PER_LINE)
        {
            
            
            string newLetter = ofUTF8ToString(letter);
            msg += newLetter;
            currentLineCharacter++;
            
            ofLog() << "New letter received: " << newLetter;
            ofLog() << "New message size:" << prevMsgLength + 1;

			
            
            // NOT a space and font is loaded
            if(letter != 32  && msgFont.isLoaded()){
                //NORMAL LETTER
                

                /******   CREATE OF_PATH FROM LETTER     ******/
                
                ofPath pathLetterToBird, pathLetterToDraw;
                pathLetterToBird = createPathFromLetter(letter, ofVec2f(0,0));
                pathLetterToDraw = createFilledPathFromLetter(letter, nextLetterPosition);
				ofRectangle rectOfLetter = getBoundingBoxOfPath(pathLetterToBird);
                
                 /******   CHECK IF NOT EMPTY ( special character for exemple )     ******/
                if(pathLetterToBird.getOutline().size()>0){
                    
                     /******   CONVERT OF_PATH TO VECTOR<POLYLINE>    ******/
                    vector<ofPolyline> listOfPolyline = reduceDistanceSampling( pathLetterToBird  );
                    
                    /******   CREATE NEW LETTER   ******/
					// 1. Translate horizontal letter if touching the  border
					
					// touching left border of 1st rectangle
					if (nextLetterPosition.x  < borderLetter1.x && (nextLetterPosition.x + rectOfLetter.width) > borderLetter1.x) {
						nextLetterPosition.x = borderLetter1.x + 5;
					}
					// touching right border of 1st rectangle
					if (nextLetterPosition.x  < (borderLetter1.x + borderLetter1.width) && (nextLetterPosition.x + rectOfLetter.width) >(borderLetter1.x + borderLetter1.width)) {
						nextLetterPosition.x = (borderLetter1.x + borderLetter1.width) + 5;
					}
					if (nextLetterPosition.x  < borderLetter2.x && (nextLetterPosition.x + rectOfLetter.width) > borderLetter2.x) {
						nextLetterPosition.x = borderLetter2.x + 5;
					}
					// touching right border of 1st rectangle
					if (nextLetterPosition.x  < (borderLetter2.x + borderLetter2.width) && (nextLetterPosition.x + rectOfLetter.width) >(borderLetter2.x + borderLetter2.width)) {
						nextLetterPosition.x = (borderLetter2.x + borderLetter2.width) + 5;
					}
					

					// 2. Translate vertically if inside the rectangle
					ofVec2f finalLetterPosition = nextLetterPosition;
					if ((nextLetterPosition.x) > borderLetter1.x && nextLetterPosition.x < (borderLetter1.x + borderLetter1.width)) {
						finalLetterPosition.y -= borderOffsetY;
					}
					if ((nextLetterPosition.x) > borderLetter2.x && nextLetterPosition.x < (borderLetter2.x + borderLetter2.width)) {
						finalLetterPosition.y -= borderOffsetY;
					}
					// 2. Call Constructor
					ofVec2f positionBL = ofVec2f(w / 2, positionBigLetter);
                    Letter* newLetter = new Letter(letter, finalLetterPosition, positionBL,listOfPolyline, nicheManager, w, h, zoomBigLetter);
                    listOfLetter.push_back(newLetter);
                    
                    /******   UPDATE CURSOR   ******/
                    
                    nextLetterPosition.x +=  rectOfLetter.width + 1; // space between letter
                    if(msg.size() != listOfLetter.size()){
                        ofLog(OF_LOG_ERROR) << " msg & listOfLetter sizes does not match";
                    }

					// SEND OSC MESSAGE TO MAC
					ofxOscMessage m;
					m.setAddress("/letter");
					m.addInt32Arg(0);
					osc_sender.sendMessage(m);
					m.clear();
                    
                }
                
            }else{
                //SPACE LETTER
                nextLetterPosition.x = nextLetterPosition.x + fontSpacing;
                
            }
            
            //Check time from the added letter. TODO : not used 
            if(msg.size()>0){
                timeOfLastLetter[msg.size() - 1] = ofGetElapsedTimef();
            }
            
            
        }
        
    }

    
}


//--------------------------------------------------------------
void LetterManager::addTemporaryNiche(Niche n) {

	temporaryNiche.push_back(n);
	nicheManager->addNeighbourFromNiche(temporaryNiche.back());
}

//--------------------------------------------------------------
void LetterManager::clear(){
    msg = "";
    currentLineCharacter = 0;
    
    for(auto letter : listOfLetter){
        delete letter;
    }

    listOfLetter.clear();
    nextLetterPosition = msgPosition;
    
	// When clear the message, don't kill Birds, but push them into DIE ON BORDER MODE
	//birdmanager->killAll();
    //nicheManager->lastFlyAll();
}


//--------------------------------------------------------------
void LetterManager::setDebug(int &i){
    //Debug is not is bird anymore but in NicheManager
}

//--------------------------------------------------------------
void LetterManager::setDebugScale(int &i){
    
    for(auto letter : listOfLetter){
        for(auto &niche : letter->listOfNiche){
            for(auto &bird : niche.listOfBird){
                bird.debugScale = i;
            }
        }
    }
    for(auto &bird : permanentNiche.listOfBird){
        bird.debugScale = i;
    }
}

//--------------------------------------------------------------
void LetterManager::setStiffness(float &f){
    for(auto letter : listOfLetter){
        for(auto &niche : letter->listOfNiche){
            for(auto &bird : niche.listOfBird){
                bird.stiffness = f;
            }
        }
    }
    for(auto &bird : permanentNiche.listOfBird){
        bird.stiffness = f;
    }
}
//--------------------------------------------------------------
void LetterManager::setDamping(float &f){
    for(auto letter : listOfLetter){
        for(auto &niche : letter->listOfNiche){
            for(auto &bird : niche.listOfBird){
                bird.damping = f;
            }
        }
    }
    for(auto &bird : permanentNiche.listOfBird){
        bird.damping = f;
    }

    
}
//--------------------------------------------------------------
void LetterManager::setSeparation(float &f){
    for(auto letter : listOfLetter){
        for(auto &niche : letter->listOfNiche){
            for(auto &bird : niche.listOfBird){
                bird.swt = f;
            }
        }
    }
    for(auto &bird : permanentNiche.listOfBird){
        bird.swt = f;
    }
}
//--------------------------------------------------------------
void LetterManager::setCohesion(float &f){
    for(auto letter : listOfLetter){
        for(auto &niche : letter->listOfNiche){
            for(auto &bird : niche.listOfBird){
                bird.cwt = f;
            }
        }
    }
    for(auto &bird : permanentNiche.listOfBird){
        bird.cwt = f;
    }
}
//--------------------------------------------------------------
void LetterManager::setAlignment(float &f){
    for(auto letter : listOfLetter){
        for(auto &niche : letter->listOfNiche){
            for(auto &bird : niche.listOfBird){
                bird.awt = f;
            }
        }
    }
    for(auto &bird : permanentNiche.listOfBird){
        bird.awt = f;
    }
}
//--------------------------------------------------------------
void LetterManager::setTargetAttraction(float &f){
    for(auto letter : listOfLetter){
        for(auto &niche : letter->listOfNiche){
            for(auto &bird : niche.listOfBird){
                bird.twt = f;
            }
        }
    }
    for(auto &bird : permanentNiche.listOfBird){
        bird.twt = f;
    }
}
//--------------------------------------------------------------
void LetterManager::setMaxSpeed(float &f){
    for(auto letter : listOfLetter){
        for(auto &niche : letter->listOfNiche){
            for(auto &bird : niche.listOfBird){
                bird.maxSpeed = f;
            }
        }
    }
    for(auto &bird : permanentNiche.listOfBird){
        bird.maxSpeed = f;
    }
}
//--------------------------------------------------------------
void LetterManager::setMaxForce(float &f){
    for(auto letter : listOfLetter){
        for(auto &niche : letter->listOfNiche){
            for(auto &bird : niche.listOfBird){
                bird.maxForce = f;
            }
        }
    }
    for(auto &bird : permanentNiche.listOfBird){
        bird.maxForce = f;
    }
}
//--------------------------------------------------------------
void LetterManager::setSize(int &f){
    for(auto letter : listOfLetter){
        for(auto &niche : letter->listOfNiche){
            for(auto &bird : niche.listOfBird){
                bird.size = f;
            }
        }
    }
    for(auto &bird : permanentNiche.listOfBird){
        bird.size = f;
    }
}
//--------------------------------------------------------------
void LetterManager::setFlyDuration(int &i){
    for(auto letter : listOfLetter){
        for(auto &niche : letter->listOfNiche){
            for(auto &bird : niche.listOfBird){
                bird.flyingDuration = i;
            }
        }
    }
    for(auto &bird : permanentNiche.listOfBird){
        bird.flyingDuration = i;
    }
}

//--------------------------------------------------------------
void LetterManager::updateRectangle(int &i){
    
	if (borderLetter2X < (borderLetter1X + borderLetter1W + 10)) {
		borderLetter2X = (borderLetter1X + borderLetter1W + 10);
	}
    borderLetter1 = ofRectangle(borderLetter1X, 0, borderLetter1W, h);
	borderLetter2 = ofRectangle(borderLetter2X, 0, borderLetter2W, h);
    
}

//--------------------------------------------------------------
void LetterManager::saveRectangle(bool &b){
    
    if(b){
        
        settingsRectWithoutLetter.clear();
        settingsRectWithoutLetter.addTag("rectangle");
        settingsRectWithoutLetter.pushTag("rectangle");
        settingsRectWithoutLetter.addValue("x", borderLetter1.x);
        settingsRectWithoutLetter.addValue("y", borderLetter1.y);
        settingsRectWithoutLetter.addValue("w", borderLetter1.width);
        settingsRectWithoutLetter.addValue("h", borderLetter1.height);
        settingsRectWithoutLetter.popTag();
        settingsRectWithoutLetter.save("rectanglewithoutletter.xml");
        
        
    }
    
}

//--------------------------------------------------------------
bool LetterManager::loadRectangle(string path){

    settingsRectWithoutLetter.clear();
    bool loadOk = settingsRectWithoutLetter.load(path);
    if(!loadOk){
        return false;
    }else{
        
        settingsRectWithoutLetter.pushTag("rectangle");
        int rx = settingsRectWithoutLetter.getValue("x", w/2);
        int ry = settingsRectWithoutLetter.getValue("y", 0);
        int rwidth = settingsRectWithoutLetter.getValue("w", w/4);
        int rheight = settingsRectWithoutLetter.getValue("x", h);
        settingsRectWithoutLetter.popTag();
        settingsRectWithoutLetter.clear();
        
        //rectWithoutLetter = ofRectangle(rx, ry, rwidth, rheight);
        //ofLog(OF_LOG_NOTICE, ofToString(rectWithoutLetter.getWidth()));
        //ofLog(OF_LOG_NOTICE, ofToString(rectWithoutLetter.width));
        
        borderLetter1W = rwidth;
        borderLetter1X = rx;
        
        
        return true;
        
    }
    
    
}

