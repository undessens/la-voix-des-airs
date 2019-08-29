#include "ofApp.h"


int const ofApp::fps = 35;
//--------------------------------------------------------------
void ofApp::setup() {


	//FINAL DIMENSION - FINAL DIMENSION - FINAL DIMENSION
    final_w = 2048;
    final_h = 768;
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
    textManager = new TextManager(birdManager, &pg_textManager);
    
    //Global parameter
    pg.setName("main");
    pg.add(color.set("color",5,0,255));
    pg.add(frameRate.set("frameRate", 35, 0, 50));
    pg.add(debug.set("debug", false));
    pg.add(fakeCursor.set("Fake cursor", false));
    pg.add(lightTopPosX.set("light X",final_w/2, 0, final_w));
    pg.add(lightTopPosY.set("light Y", 0, 0, final_h));
    pg.add(lightTopColor.set("Light Top Color", ofColor::whiteSmoke));
    pg.add(lightBottomColor.set("Light Bottom Color", ofColor::darkorange));
	pg.add(lightBottomDirectionnal.set("Light Bottom Directionnal", true));
    
    
    // Adding all parameter to GUI
    gui.setup(pg);
    gui.add(pg_birdManager);
    gui.add(pg_polyBackground);
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
		lightTop.setPosition(lightTopPosX, lightTopPosY, 0);
		lightTop.setDiffuseColor((ofColor)lightTopColor);
		lightTop.setSpecularColor((ofColor)lightTopColor);
		lightTop.enable();

		//LIGHTING ON BOTTOM
		lightBottom.setPosition(lightTopPosX, final_h / 2, 0);
		lightBottom.setDiffuseColor((ofColor)lightBottomColor);
		lightBottom.setSpecularColor((ofColor)lightBottomColor);
		if (lightBottomDirectionnal) {
			lightBottom.setDirectional();
		}
		lightBottom.enable();



		// BIRD MANAGER
		birdManager->draw();

		// DISABLE LIGHT
		ofDisableDepthTest();
		ofDisableLighting();

		// BIRD MANAGER DEBUG
		ofSetColor(230);
		birdManager->drawDebug();

		

		// Text Manager
		textManager->draw();
		textManager->drawPoly(); //debug function

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
			ofDrawBitmapStringHighlight("FrameRate : " + ofToString(ofGetFrameRate()), ofGetWidth() / 2, ofGetHeight());
		}

		fbo.end();


    //DRAW FBO ON SCREEN
    ofSetColor(255);
    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    
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
