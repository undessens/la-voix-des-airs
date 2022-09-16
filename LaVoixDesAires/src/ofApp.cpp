#include "ofApp.h"


int const ofApp::fps = 35;
//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetWindowTitle("La Voix des Airs");


	//FINAL DIMENSION - FINAL DIMENSION - FINAL DIMENSION
    final_w = 1850;
    final_h = 1080;
    //FINAL DIMENSION - FINAL DIMENSION - FINAL DIMENSION
    
    
	// Background Image, ofPolyline ...
    pg_polyBackground.setName("polybackground");
	polyBackground = new PolyBackground(&pg_polyBackground, final_w, final_h);  // Polybackground, not really used

	// OSC, Gui parameters
	pg_nicheManager.setName("Nichemanager");

	//BirdManager

	nicheManager = new NicheManager( &pg_nicheManager, final_w, final_h, ofGetWidth(), ofGetHeight());
    
    // Text Manager
    letterManager = new LetterManager(nicheManager,polyBackground, &pg_letterManager, final_w , final_h);
    
    //Global parameter
    pg.setName("main");
    pg.add(color.set("color",ofColor(0, 0, 0)));
    pg.add(frameRate.set("frameRate", 50, 0, 60));
    pg.add(debug.set("debug", false));
    pg.add(fakeCursor.set("Fake cursor", false));
    pg.add(lightTopEnable.set("light Top Enable", true));
    pg.add(lightTopPosX.set("light X",final_w/2, 0, final_w));
    pg.add(lightTopPosY.set("light Y", 0, 0, final_h));
    pg.add(lightTopPosZ.set("light Z", 400, -400,400 ));
    pg.add(lightTopColor.set("Light Top Color", ofColor::white));
    pg.add(lightBottomEnable.set("light Bottom Enable", false));
    pg.add(lightBottomColor.set("Light Bottom Color", ofColor::darkorange));
	pg.add(lightBottomDirectionnal.set("Light Bot Directionnal", true));
    
    
    // Adding all parameter to GUI
    gui.setup(pg);
    gui.add(pg_nicheManager);
    gui.add(pg_polyBackground);
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
    warper.setup(0, 0, final_w, final_h);
    warper.load("warper.xml");
    warper.deactivate();

	// FULLSCREEN
	isFullscreen = false;
    
    //FPS
    ofSetFrameRate(frameRate);
    
	// OSC receiver + sender
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

    nicheManager->updateAttractionPoint();
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
			}
			// char ç
			else if (letter == 231) {
				// char "Ž" & ""
				letterManager->addLetter(99);
            } else if(letter =='#'){
                clear_all();
            }
			else {
				letterManager->addLetter(letter);
			}
			
		}
        else if(m.getAddress() == "/alldebug"){
            debug = m.getArgAsInt(0) == 1;
            nicheManager->debug = m.getArgAsInt(0);
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
        else if(m.getAddress() =="/bird/linedist"){
            nicheManager->birdDistanceLine = m.getArgAsFloat(0);
        }
        else if(m.getAddress() =="/bird/atty"){
            nicheManager->attractionHeight = m.getArgAsFloat(0);
        }
        else if(m.getAddress() =="/bird/attradius"){
            nicheManager->attractionRadius = m.getArgAsFloat(0);
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
		} else if (m.getAddress() == "/polybg/fire/1") {
			Niche n = nicheManager->createEphemereNicheFromPolyline(polyBackground->getPolyBackground(0));
			if (n.listOfBird.size() > 0) {
				letterManager->addTemporaryNiche(n);
			}
		}
		else if (m.getAddress() == "/polybg/fire/0") {
			Niche n = nicheManager->createEphemereNicheFromPolyline(polyBackground->getPolyBackground(0));
			if (n.listOfBird.size() > 0) {
				letterManager->addTemporaryNiche(n);
			}

		}
		else if (m.getAddress() == "/polybg/fire/1") {
			Niche n = nicheManager->createEphemereNicheFromPolyline(polyBackground->getPolyBackground(1));
			if (n.listOfBird.size() > 0) {
				letterManager->addTemporaryNiche(n);
			}

		}
		else if (m.getAddress() == "/polybg/fire/2") {
			Niche n = nicheManager->createEphemereNicheFromPolyline(polyBackground->getPolyBackground(2));
			if (n.listOfBird.size() > 0) {
				letterManager->addTemporaryNiche(n);
			}

		}
		else if (m.getAddress() == "/polybg/fire/3") {
			Niche n = nicheManager->createEphemereNicheFromPolyline(polyBackground->getPolyBackground(3));
			if (n.listOfBird.size() > 0) {
				letterManager->addTemporaryNiche(n);
			}

		}
		else if (m.getAddress() == "/polybg/fire/4") {
		Niche n = nicheManager->createEphemereNicheFromPolyline(polyBackground->getPolyBackground(4));
		if (n.listOfBird.size() > 0) {
			letterManager->addTemporaryNiche(n);
		}

		}
		else if (m.getAddress() == "/polybg/fire/5") {
			Niche n = nicheManager->createEphemereNicheFromPolyline(polyBackground->getPolyBackground(5));
			if (n.listOfBird.size() > 0) {
				letterManager->addTemporaryNiche(n);
			}

		}
		else if (m.getAddress() == "/polybg/fire/6") {
			Niche n = nicheManager->createEphemereNicheFromPolyline(polyBackground->getPolyBackground(6));
			if (n.listOfBird.size() > 0) {
				letterManager->addTemporaryNiche(n);
			}

		}
		else if (m.getAddress() == "/polybg/fire/7") {
			Niche n = nicheManager->createEphemereNicheFromPolyline(polyBackground->getPolyBackground(7));
			if (n.listOfBird.size() > 0) {
				letterManager->addTemporaryNiche(n);
			}

		}
		
        

	}
    
    //frameRate = ofGetFrameRate();
    ofSetFrameRate(frameRate);
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
    
        // Draw Centroid
//    ofSetColor(255, 0, 0);
//    ofDrawCircle(final_w/2, final_h/2, 30);
    
		// DISABLE LIGHT
		ofDisableDepthTest();
		ofDisableLighting();

		// BIRD MANAGER DEBUG
    if(nicheManager->debug>0){
        ofSetColor(255,0,0);
        if(nicheManager->attractionActive){
            ofFill();
        }else{
            ofNoFill();
        }
        ofDrawCircle(nicheManager->att.x, nicheManager->att.y, 10);
    }
     
        
    
		ofSetColor(255);
		letterManager->drawDebug();


		// Polybackground - draw center of point as a circle
		polyBackground->draw();
		
    
        //Polybackground - draw the pencil mode
//        if (polyBackground->isPencil)
//        {
//            polyBackground->pencilOnFbo();
//            ofSetColor(255);
//            polyBackground->fbo.draw(0, 0);
//        }

		// Fake cursor
		if (fakeCursor) {

			//ofVec2f cursor = ofVec2f(ofGetMouseX() * (final_w*1.0) / ofGetWidth(), ofGetMouseY() * (final_h*1.0) / ofGetHeight());
			ofVec2f cursor = ofVec2f(ofGetMouseX() , ofGetMouseY() );

			ofSetColor(255, 0, 0);
			ofDrawCircle(cursor, 5);
			ofDrawLine(cursor.x - 30, cursor.y, cursor.x + 30, cursor.y);
			ofDrawLine(cursor.x, cursor.y - 30, cursor.x, cursor.y + 30);


		}

		//DEBUG PART

		if (debug) {
            ofSetColor(255);
			ofDrawBitmapStringHighlight("FrameRate : " + ofToString(ofGetFrameRate()), final_w / 2, final_h - 40);
            ofDrawBitmapStringHighlight("Writing Speed : " + ofToString(letterManager->writingSpeed), final_w / 2, final_h-60);
		}

		fbo.end();


    //DRAW FBO ON SCREEN THROUGH WARPER
    
        warper.begin();
            ofSetColor(255);
            fbo.draw(0, 0);
            if (warper.isActive()) {
                warper.draw();
            }
        warper.end();
    
    
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
    }else if( key ==35){
        // # symbol : clear
        clear_all();
    }
    else if( key>31 && key < 128){
        // Only draw ASCII extended code
       letterManager->addLetter(key);
    } else if( key==224){
        // Only draw ASCII extended code
        letterManager->addLetter(97);
    }  else if(key== 232 || key == 233){
        // char "Ž" & ""
        letterManager->addLetter(key);
	}else if(key == OF_KEY_UP) {
		//ESCAPE
		isFullscreen = !isFullscreen;
		ofSetFullscreen(isFullscreen);
			
        return;
	}else if(key == 3682) {
		//CTRL - undraw GUI
		isGuiVisible = !isGuiVisible;

    }else if ( key == 4){
        // ALT
        if(warper.isActive()){
            warper.deactivate();
            warper.saveToXml(xml_warper);
            warper.saveToXml(xml_warper);
            ofLogNotice(xml_warper.toString());
            //xml.setName("war_settings");
            if (!xml_warper.save("warper.xml")) {
                ofLogError() << "Couldn't save points.xml";
            }
            
        }
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
    
    if(polyBackground->isDraw){
        if(polyBackground->currentPolylineSelected>=0 ){
            polyBackground->clickForMove(ofVec2f(x, y));
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    ofLog() << "Mouse position is x:" << x << ", y:" << y;
    
    if(polyBackground->isDraw){
        if(polyBackground->currentPolylineSelected>=0 ){
            polyBackground->clickForSelect(ofVec2f(x, y));
        }
    }

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
