#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
    //ofSetFrameRate(2);
    
    polyBackground = PolyBackground();
    
    birdManager = BirdManager(&polyBackground);
    birdManager.newSequence("oo");
    
}

//--------------------------------------------------------------
void ofApp::update(){

    birdManager.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(100);
    
    ofColor(255);
    ofNoFill();
    birdManager.draw(true);
    
    ofDrawBitmapStringHighlight("X: "+ofToString(ofGetMouseX() )+" - Y: "+ofToString(ofGetMouseY()), ofGetMouseX(), ofGetMouseY());
    
    polyBackground.draw();
    
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
