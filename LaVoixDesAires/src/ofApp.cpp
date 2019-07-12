#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
    //ofSetFrameRate(2);
    
    // Background Image, ofPolyline ...
    polyBackground = new PolyBackground(&pg_polyBackground);
    
    // OSC, Gui parameters
    pg_birdManager.setName("birdmanager");
    
    //BirdManager
    birdManager = new BirdManager( polyBackground, &pg_birdManager);
    birdManager->newSequence("o");
    
    
    //Adding all OSC parameter to gui
    pg.setName("main");
    pg.add(color.set("color",200,0,255));
    pg.add(frameRate.set("frameRate", 30, 0, 50));
    pg.add(debug.set("debug", false));
    
    gui.setup(pg);
    gui.add(pg_birdManager);
    gui.add(pg_polyBackground);
    sync.setup((ofParameterGroup&)gui.getParameter(), 6666, "localhost", 6667);
    
    
    
    
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
    
    //GUI
    gui.draw();
    
    ofColor(255);
    ofNoFill();
    birdManager->draw();
    
    polyBackground->draw();
    
    //DEBUG PART
    if(debug){
      
        ofDrawBitmapStringHighlight("X: "+ofToString(ofGetMouseX() )+" - Y: "+ofToString(ofGetMouseY()), ofGetMouseX(), ofGetMouseY());
        
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
