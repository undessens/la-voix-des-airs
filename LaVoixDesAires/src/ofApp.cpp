#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
    ofSetFrameRate(60);
    
    // Background Image, ofPolyline ...
    polyBackground = new PolyBackground(&pg_polyBackground);
    
    // OSC, Gui parameters
    pg_birdManager.setName("birdmanager");
    
    //BirdManager
    birdManager = new BirdManager( polyBackground, &pg_birdManager);
    //birdManager->newSequence("o");
    
    
    //Adding all OSC parameter to gui
    pg.setName("main");
    pg.add(color.set("color",200,0,255));
    pg.add(frameRate.set("frameRate", 30, 0, 50));
    pg.add(debug.set("debug", false));
    
    gui.setup(pg);
    gui.add(pg_birdManager);
    gui.add(pg_polyBackground);
    sync.setup((ofParameterGroup&)gui.getParameter(), 6666, "localhost", 6667);
    
    //3D stuff
	ofDisableArbTex();
    
    
}

//--------------------------------------------------------------
void ofApp::update(){

    birdManager->update();
    
    // GUI update
    sync.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(color);
	ofSetVerticalSync(true);
    
    //GUI
    gui.draw();
    
    ofColor(255);
    ofNoFill();

	// LIGHT MANAGEMENT
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableDepthTest();
#ifndef TARGET_PROGRAMMABLE_GL
	glShadeModel(GL_SMOOTH);
#endif // !TARGET_PROGRAMMABLE_GL
	light.enable();
	//ofEnableSeparateSpecularLight();
	
	// BIRD MANAGER
    birdManager->draw();

	ofDisableDepthTest();
	light.disable();
	ofDisableLighting();
	ofDisableSeparateSpecularLight();



    
    //polyBackground->draw();
    
    //DEBUG PART
    if(debug){
      
        ofDrawBitmapStringHighlight("FrameRate : "+ofToString(ofGetFrameRate()), ofGetWidth()/2, ofGetHeight());
        
    }
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
