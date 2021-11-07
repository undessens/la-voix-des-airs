#include "ofApp.h"


int const ofApp::fps = 35;
//--------------------------------------------------------------
void ofApp::setup() {


	//FINAL DIMENSION - FINAL DIMENSION - FINAL DIMENSION
    final_w = 1600;
    final_h = 1000;
    //FINAL DIMENSION - FINAL DIMENSION - FINAL DIMENSION
    
    
    ofSetFrameRate(ofApp::fps);

	// Background Image, ofPolyline ...
	polyBackground = new PolyBackground(&pg_polyBackground, 2048, 768);

	// OSC, Gui parameters
	pg_birdManager.setName("birdmanager");

	//BirdManager
	birdManager = new BirdManager(polyBackground, &pg_birdManager, final_w, final_h, ofGetWidth(), ofGetHeight());
	birdManager->createInvicibleArmy();

    // Text Manager
    textManager = new TextManager(birdManager, &pg_textManager, final_w , final_h);
    
    //Global parameter
    pg.setName("main");
    pg.add(color.set("color",ofColor::darkGray));
    pg.add(frameRate.set("frameRate", 35, 0, 50));
    pg.add(debug.set("debug", false));
    pg.add(fakeCursor.set("Fake cursor", false));
    pg.add(lightTopEnable.set("light Top Enable", true));
    pg.add(lightTopPosX.set("light X",final_w/2, 0, final_w));
    pg.add(lightTopPosY.set("light Y", 0, 0, final_h));
    pg.add(lightTopPosZ.set("light Z", 150, -400,400 ));
    pg.add(lightTopColor.set("Light Top Color", ofColor::whiteSmoke));
    pg.add(lightBottomEnable.set("light Bottom Enable", true));
    pg.add(lightBottomColor.set("Light Bottom Color", ofColor::darkorange));
	pg.add(lightBottomDirectionnal.set("Light Bot Directionnal", true));
    
    
    // Adding all parameter to GUI
    gui.setup(pg);
    gui.add(pg_birdManager);
    //gui.add(pg_polyBackground);
    gui.add(pg_textManager);
    sync.setup((ofParameterGroup&)gui.getParameter(), 6666, "localhost", 6667);
	isGuiVisible = true;

	//3D stuff
	ofDisableArbTex();

	//fbo allocation
	fbo.allocate(final_w, final_h, GL_RGBA);
    

	// OSC receiver
	osc_receiver.setup(12345);
    
    //Lighting
    ofSetSmoothLighting(true);
    



	// sender name for madmapper
#if defined(_WIN32)
	sender.init("My_Lovely_Birds");
#endif
    
}

//--------------------------------------------------------------
void ofApp::update() {

	birdManager->update( textManager->msg);
    textManager->update();
    
    //Force attraction quickly after new letter
    float timeSpendFromLastLetter = ofGetElapsedTimef() - textManager-> timeOfLastLetter[textManager->msgPaths.size()-1];
    if( timeSpendFromLastLetter < 1.2f && !birdManager->attractionActive  ){
        birdManager->attractionActive = true;
    }
    

	// GUI update
	if (isGuiVisible) {
		sync.update();
	}
	

    //SPOUT windows only
#if defined(_WIN32)
    sender.send(fbo.getTexture());
#endif
	
	while (osc_receiver.hasWaitingMessages()) {
		ofxOscMessage m; 
		osc_receiver.getNextMessage(m);
		if (m.getAddress() == "/debug") {
			letter = m.getArgAsInt32(0);
			ofLog() << "message : " << letter; 
			if (letter == '\n') {
				textManager->clear();
			}
			else {
				textManager->addLetter(letter);
			}
			
		}
	}
    
    frameRate = ofGetFrameRate();
}

//--------------------------------------------------------------
void ofApp::draw() {

		//DRAW ON FBO
		fbo.begin();

		ofBackground(color);
		ofSetVerticalSync(true);

		//DRAW STATIC BIRD
	//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	//    fboStatic.draw(0,0);
	//    ofDisableBlendMode();

		// DRAW ACCORDING TO Z DEPTH and not code order
		ofEnableDepthTest();


#ifndef TARGET_PROGRAMMABLE_GL
		glShadeModel(GL_SMOOTH);
#endif // !TARGET_PROGRAMMABLE_GL

		//LIGHTING ON TOP
		ofEnableLighting();
		//ofEnableSeparateSpecularLight();
		lightTop.setPosition(lightTopPosX, lightTopPosY, lightTopPosZ);
		lightTop.setDiffuseColor((ofColor)lightTopColor);
		lightTop.setSpecularColor((ofColor)lightTopColor);
        if(lightTopEnable){
            lightTop.enable();
        }else{
            lightTop.disable();
        }
		

		//LIGHTING ON BOTTOM
		lightBottom.setPosition(lightTopPosX, final_h / 2, lightTopPosZ);
		lightBottom.setDiffuseColor((ofColor)lightBottomColor);
		lightBottom.setSpecularColor((ofColor)lightBottomColor);
		if (lightBottomDirectionnal) {
			lightBottom.setDirectional();
		}
        if(lightBottomEnable){
            lightBottom.enable();
        }else{
            lightBottom.disable();
        }
		

        // Text Manager
        textManager->draw();

		// BIRD MANAGER - over text
		birdManager->draw();
    

    

		// DISABLE LIGHT
		ofDisableDepthTest();
		ofDisableLighting();

		// BIRD MANAGER DEBUG
		ofSetColor(255);
		birdManager->drawDebug();

		

		

		// Polybackground - draw center of point as a circle
		polyBackground->draw();
		//Polybackground - draw the pencil mode
		if (polyBackground->isPencil)
		{
			polyBackground->pencilOnFbo();
			ofSetColor(255);
			polyBackground->fbo.draw(0, 0);
		}

		// Fake cursor
		if (fakeCursor) {

			ofVec2f cursor = ofVec2f(ofGetMouseX() * (final_w*1.0) / ofGetWidth(), ofGetMouseY() * (final_h*1.0) / ofGetHeight());

			ofSetColor(255, 0, 0);
			ofDrawCircle(cursor, 20);
			ofDrawLine(cursor.x - 30, cursor.y, cursor.x + 30, cursor.y);
			ofDrawLine(cursor.x, cursor.y - 30, cursor.x, cursor.y + 30);


		}

		//DEBUG PART

		if (debug) {
            ofSetColor(255);
            ofDrawCircle(final_w/2, final_h/2, 200);
			ofDrawBitmapStringHighlight("FrameRate : " + ofToString(ofGetFrameRate()), ofGetWidth() / 2, ofGetHeight() - 10);
            ofDrawBitmapStringHighlight("Writing Speed : " + ofToString(textManager->writingSpeed), ofGetWidth() / 2, ofGetHeight()-20);

		}

		fbo.end();


    //DRAW FBO ON SCREEN
    ofSetColor(255);
    
    fbo.draw(200, 50, 1280, 800 );
    
    
    //GUI
	if (isGuiVisible) {
		gui.draw();
	}
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    ofLog() << "Key code from keyboard : " << key;
    
	cout << ofToString(key);
	if(key == 8){
        // Clear when backspace  received
        textManager->clear();
    } else if( key>31 && key < 128){
        // Only draw ASCII extended code
       textManager->addLetter(key);
	}else if(key == 3680 || key== 1 ||key == 3681) {
		//Maj DO NOTHING
	}
	else  if (key == 3682) {
		//CTRL - undraw GUI
		isGuiVisible = !isGuiVisible;

	}else	{  
		
        //13 entree
		//ù 249
		textManager->addLetter(key);
		// à 224
		// ° à 176

	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    ofLog() << "Mouse position is x:" << x << ", y:" << y;

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
