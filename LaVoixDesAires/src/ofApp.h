#pragma once

#include "ofMain.h"
#include "BirdManager.hpp"
#include "Bird.hpp"
#include "Centroid.hpp"
#include "TrajectoryPlayer.hpp"
#include "PolyBackground.hpp"
#include "ofxGui.h"
#include "ofxOscParameterSync.h"
#include "TextManager.h"
#if defined(_WIN32)
#include "ofxSpout.h"
#endif


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        BirdManager* birdManager;
        PolyBackground* polyBackground;
		TextManager* textManager;
    
    
        //GUI
        ofxPanel gui;
        ofxOscParameterSync sync;
        ofParameterGroup pg;
        ofParameterGroup pg_birdManager;
        ofParameterGroup pg_polyBackground;
        ofParameterGroup pg_trajectoryPlayer;
        ofParameterGroup pg_textManager;
		bool isGuiVisible;
    
        //Parameters of main program
        ofParameter<ofColor> color;
        ofParameter<int> frameRate;
        ofParameter<bool> debug;
        ofParameter<bool> fakeCursor;
        ofParameter<bool> lightTopEnable;
        ofParameter<int> lightTopPosX;
        ofParameter<int> lightTopPosY;
        ofParameter<int> lightTopPosZ;
    
        ofParameter<ofColor> lightBottomColor;
        ofParameter<bool> lightBottomEnable;
        ofParameter<bool> lightBottomDirectionnal;
        ofParameter<ofColor> lightTopColor;

    
		//Lighting
		ofLight lightTop;
        ofLight lightBottom;
    
        //Final fbo size send to madmapper
        int final_w;
        int final_h;

		//FBO
		ofFbo fbo;          //Clear on every frame
        ofFbo fboStatic;    // NOT USED ; Draw of bird "joined Target"
    
    

		// Spout sender
#if defined(_WIN32)
		ofxSpout::Sender sender;
#endif
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// OSC receiver
		ofxOscReceiver osc_receiver; 

		// ASCII char from osc
		int32_t letter;
    
        //FPS
        static const int fps;

		
};


