#include "TextManager.h"

TextManager::TextManager()
{
}

TextManager::TextManager(BirdManager* b, ofParameterGroup* _pg)
{
    msg = "";
    msgPosition = ofVec2f(300, 300);
    
    fontSpacing = 55;
    fontSize= 70;
//    msgFont.load("ttwpglot.ttf", fontSize, true, true, true);
    msgFont.load("immono.otf", fontSize, true, true, true);
    birdFont.load("ralewayDots.ttf", 55, true, true, true);
    birdmanager = b;
    msgPolys.clear();
    
    
    gFontSize.addListener(this, &TextManager::changeFontSize);
    gFontSpacing.addListener(this, &TextManager::changeFontSpacing);
    
    pg = _pg;
    pg->setName("Text Manager");
    pg->add(drawMsgFill.set("Draw Fill", true));
    pg->add(drawMsgContour.set("Draw Contour", true));
    pg->add(gFontSize.set("Font Size", fontSize, 10, 200));
    pg->add(gFontSpacing.set("Font Spacing", fontSpacing, 10, 200));
    addLetter(233);
}


TextManager::~TextManager()
{
}


void TextManager::changeFontSize(int &newSize){
    fontSize = newSize;
    msgFont.load("immono.otf", newSize, true, true, true);
}

void TextManager::changeFontSpacing(int &newSpacing){
    fontSpacing = newSpacing;
}


void TextManager::draw() {
	//Font
	ofSetColor(ofColor::green);
	std::vector<ofPath> letters = msgFont.getStringAsPoints(msg);
	ofFill();
}

void TextManager::addLetter(int letter) {
    
	int prevMsgLength = ofUTF8Length(msg);
    string newLetter = ofUTF8ToString(letter);
    
    ofVec2f newLetterPosition = ofVec2f(msgPosition.x + prevMsgLength*fontSpacing, msgPosition.y);
    msg += newLetter;
    
    ofLog() << "New letter received: " << newLetter;
    ofLog() << "New message size:" << prevMsgLength + 1;
    
    if(msgFont.isLoaded()){
        ofPath letterPath = msgFont.getCharacterAsPoints(letter);
        letterPath.translate(newLetterPosition);
        msgPaths.push_back(letterPath);
    } else {
        ofLog(OF_LOG_ERROR) << "No message font on the system!";
    }

    
    if(birdFont.isLoaded()){
        ofPath dottedPath = birdFont.getCharacterAsPoints(letter);
        //Set stroke to create polyline
        dottedPath.setStrokeWidth(2);
        dottedPath.translate(newLetterPosition);
        std::vector<ofPolyline> dottedOutline = dottedPath.getOutline();
        int letterPoints = 0;
    
        for (vector<ofPolyline>::iterator ito = dottedOutline.begin(); ito < dottedOutline.end(); ito++) {
            ofPolyline aPoly = *ito;
            aPoly.simplify(0.5);
            aPoly.getResampledBySpacing(10);
            letterPoints +=  aPoly.size();
            //Add to List of Poly
            msgPolys.push_back(aPoly);
            //            birdmanager->addBird(simple);
        }
        ofLog() << "Letter '"  << newLetter << "' points size : " << letterPoints;
    } else {
        ofLog(OF_LOG_ERROR) << "No dotted font found on the system!";
    }
    
}

void TextManager::drawPoly(){

    if(drawMsgFill){
        ofFill();
        ofSetColor(ofColor::black);
        for (vector<ofPath>::iterator ito = msgPaths.begin(); ito < msgPaths.end(); ito++){
            ito-> draw();
        }
    }
    
    if(drawMsgContour){
        ofSetColor(ofColor::blue);
        for (vector<ofPolyline>::iterator ito = msgPolys.begin(); ito < msgPolys.end(); ito++){
            ofNoFill();
            
            ofPoint center = ito->getCentroid2D();
            ito -> draw();
//            for (int i = 0; i < ito->size(); i++) {
//                ofVec3f p = (*ito)[i];
//
//                //Draw Cercle
//                ofFill();
//                ofSetLineWidth(1);
                ofDrawCircle(center, 10);
//
//                //Draw Line
//                if (i > 0) {
//                    ofDrawLine((*ito)[i], (*ito)[i - 1]);
//                } else {
//                    ofDrawLine((*ito)[i], (*ito)[ito->size() - 1]);
//                }
//            }

        }
    }
    
}

void TextManager::clear(){
    msg = "";
    msgPolys.clear();
    msgPaths.clear();
    birdmanager->killAll();
}
