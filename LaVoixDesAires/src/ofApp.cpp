#include "ofApp.h"


int const ofApp::fps = 35;
//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetWindowTitle("La Voix des Airs");


	//FINAL DIMENSION - FINAL DIMENSION - FINAL DIMENSION
    final_w = 1280;
    final_h = 720;
    //FINAL DIMENSION - FINAL DIMENSION - FINAL DIMENSION
    
    
    ofSetFrameRate(ofApp::fps);

	// Background Image, ofPolyline ...
	//polyBackground = new PolyBackground(&pg_polyBackground, 2048, 768);  // Polybackground, not really used

	// OSC, Gui parameters
	pg_nicheManager.setName("birdmanager");

	//BirdManager

	nicheManager = new NicheManager( &pg_nicheManager, final_w, final_h, ofGetWidth(), ofGetHeight());
    
    // INVICIBLE ARMY : add this ...
    //nicheManager->createInvicibleArmy();

    // Text Manager
    letterManager = new LetterManager(nicheManager, &pg_letterManager, final_w , final_h);
    
    //Global parameter
    pg.setName("main");
    pg.add(color.set("color",ofColor(0, 0, 10)));
    pg.add(frameRate.set("frameRate", 35, 0, 50));
    pg.add(debug.set("debug", false));
    pg.add(fakeCursor.set("Fake cursor", false));
    pg.add(lightTopEnable.set("light Top Enable", true));
    pg.add(lightTopPosX.set("light X",final_w/2, 0, final_w));
    pg.add(lightTopPosY.set("light Y", 0, 0, final_h));
    pg.add(lightTopPosZ.set("light Z", 400, -400,400 ));
    pg.add(lightTopColor.set("Light Top Color", ofColor::whiteSmoke));
    pg.add(lightBottomEnable.set("light Bottom Enable", true));
    pg.add(lightBottomColor.set("Light Bottom Color", ofColor::darkorange));
	pg.add(lightBottomDirectionnal.set("Light Bot Directionnal", true));
    
    
    // Adding all parameter to GUI
    gui.setup(pg);
    gui.add(pg_nicheManager);
    //gui.add(pg_polyBackground);
    gui.add(pg_letterManager);
    sync.setup((ofParameterGroup&)gui.getParameter(), 6666, "localhost", 6667);
	isGuiVisible = true;

	//3D stuff
	ofDisableArbTex();

	//fbo allocation
	fbo.allocate(final_w, final_h, GL_RGBA);
    fboLetter.allocate(final_w, final_h, GL_RGBA);
    fboLetter.begin();
    ofClear(0,0,0,0);
    fboLetter.end();
    
    // WARPER
    warper.setup(0, 0, final_w*0.99, final_h*0.99);
    warper.deactivate();
    
	// OSC receiver
	osc_receiver.setup(12345);
    
    //Lighting
    ofSetSmoothLighting(true);
    



	// sender name for madmapper
#if defined(_WIN32)
	sender.init("My_Lovely_Birds");
#endif
#if defined(__APPLE__)
    //syphonServer.init  // INIT NEED ?
#endif
    
}

//--------------------------------------------------------------
void ofApp::update() {

    
    //birdManager->update( textManager->msg, &fboLetter);
    
    letterManager->update();
    
    //Force attraction quickly after new letter
//    float timeSpendFromLastLetter = ofGetElapsedTimef() - textManager->timeOfLastLetter[textManager->msg.size()-1];
//    if( timeSpendFromLastLetter < 1.2f && !birdManager->attractionActive  ){
//        birdManager->attractionActive = true;
//    }
    


    sync.update();

	
	while (osc_receiver.hasWaitingMessages()) {
		ofxOscMessage m; 
		osc_receiver.getNextMessage(m);
		if (m.getAddress() == "/debug") {
			letter = m.getArgAsInt32(0);
			ofLog() << "message : " << letter; 
			if (letter == '\n') {
                letterManager->addLetter(13);
			} else if( letter==224){
            // Only draw ASCII extended code
                letterManager->addLetter(97);
            }  else if(letter== 232 || letter== 233){
            // char "Ž" & ""
                letterManager->addLetter(101);
            } else if(letter =='#'){
                clear_all();
            }
			else {
				letterManager->addLetter(letter);
			}
			
		}
        else if(m.getAddress() == "/background"){
            int r = m.getArgAsFloat(0);
            int g = m.getArgAsFloat(1);
            int b = m.getArgAsFloat(2);
            color = ofColor(r,g,b);
        }else if(m.getAddress() == "/light"){
            int r = m.getArgAsFloat(0);
            int g = m.getArgAsFloat(1);
            int b = m.getArgAsFloat(2);
            lightTopColor = ofColor(r,g,b);
        }
        else if(m.getAddress() == "/showwarper"){
            int show = m.getArgAsInt(0);
            if(show==1) warper.activate();
            else warper.deactivate();
        }
        else if(m.getAddress() == "/showgui"){
            int show = m.getArgAsInt(0);
            if(show==1) isGuiVisible = true;
            else isGuiVisible = false;
        }
        else if(m.getAddress() == "/clear"){
            clear_all();
        }else if(m.getAddress() == "/bird/size"){
            nicheManager->size = m.getArgAsInt32(0);
        }else if(m.getAddress() == "/bird/duration"){
            nicheManager->flyDuration = m.getArgAsInt32(0);
        }else if(m.getAddress() == "/bird/line"){
            nicheManager->birdLineWidth = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/warper/topleft"){
            int x = m.getArgAsFloat(0)*final_w;
            int y = m.getArgAsFloat(1)*final_h;
            warper.setCorner(warper.TOP_LEFT, x, y);
        } else if(m.getAddress() == "/warper/topright"){
            int x = m.getArgAsFloat(0)*final_w;
            int y = m.getArgAsFloat(1)*final_h;
            warper.setCorner(warper.TOP_RIGHT, x, y);
        } else if(m.getAddress() == "/warper/bottomleft"){
            int x = m.getArgAsFloat(0)*final_w;
            int y = m.getArgAsFloat(1)*final_h;
            warper.setCorner(warper.BOTTOM_LEFT, x, y);
        } else if(m.getAddress() == "/warper/bottomright"){
            int x = m.getArgAsFloat(0)*final_w;
            int y = m.getArgAsFloat(1)*final_h;
            warper.setCorner(warper.BOTTOM_RIGHT, x, y);
        }
        

	}
    
    frameRate = ofGetFrameRate();
}

//--------------------------------------------------------------
void ofApp::draw() {
    
     ofBackground(0);

    //DRAW ON FBO
    fbo.begin();

    ofBackground(color);
    ofSetVerticalSync(true);

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    letterManager->drawLetter();
    
    ofDisableBlendMode();
    
    


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
        letterManager->drawBirds();
    
		// DISABLE LIGHT
		ofDisableDepthTest();
		ofDisableLighting();

		// BIRD MANAGER DEBUG
     
        
    
		ofSetColor(255);
		//letterManager->drawDebug();


		// Polybackground - draw center of point as a circle
		//polyBackground->draw();
		
    
        //Polybackground - draw the pencil mode
//        if (polyBackground->isPencil)
//        {
//            polyBackground->pencilOnFbo();
//            ofSetColor(255);
//            polyBackground->fbo.draw(0, 0);
//        }

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
			ofDrawBitmapStringHighlight("FrameRate : " + ofToString(ofGetFrameRate()), ofGetWidth() / 2, ofGetHeight() - 10);
            ofDrawBitmapStringHighlight("Writing Speed : " + ofToString(letterManager->writingSpeed), ofGetWidth() / 2, ofGetHeight()-20);

		}

		fbo.end();


    //DRAW FBO ON SCREEN THROUGH WARPER
    
//    warper.begin();
//    ofSetColor(255);
        fbo.draw(0, 0);
//    if (warper.isActive()) {
//        warper.draw();
//    }
//    warper.end();
    
    
    //SPOUT windows only
#if defined(_WIN32)
    sender.send(fbo.getTexture());
#endif
#if defined(__APPLE__)
    syphonServer.publishTexture(&fbo.getTexture());
#endif
    
    
    //GUI
	if (isGuiVisible) {
		gui.draw();
	}
    

}

//--------------------------------------------------------------
void ofApp::clear_all(){
    
    
    letterManager->clear();
    //TODO : is fbo letter still used ???
//    fboLetter.begin();
//    ofClear(0, 0, 0, 0);
//    fboLetter.end();
    
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    ofLog() << "Key code from keyboard : " << key;
    
	cout << ofToString(key);
	if(key == 8){
        // Clear when backspace  received
        clear_all();
    } else if( key>31 && key < 128){
        // Only draw ASCII extended code
       letterManager->addLetter(key);
    } else if( key==224){
        // Only draw ASCII extended code
        letterManager->addLetter(97);
    }  else if(key== 232 || key == 233){
        // char "Ž" & ""
        letterManager->addLetter(101);
	}else if(key == 3680 || key == 1 || key==3681 || key == 16 || key== 3587) {
		//Maj  CMD , DO NOTHING
        return;
	}else if(key == 3682) {
		//CTRL - undraw GUI
		isGuiVisible = !isGuiVisible;

    }else if ( key == 4){
        // ALT
        if(warper.isActive())warper.deactivate();
        else warper.activate();
    }else{
		
        //13 entree
		//ù 249
		letterManager->addLetter(key);
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
