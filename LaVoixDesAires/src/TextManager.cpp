#include "TextManager.h"

TextManager::TextManager()
{
}

TextManager::TextManager(BirdManager* b, ofParameterGroup* _pg)
{
    msg = "";
    msgPosition = ofVec2f(300, 300);
    
    fontSpacing = 70;
    fontSize= 70;
    
//    msgFont.load("ttwpglot.ttf", fontSize, true, true, true);
//    msgFont.load("ralewayDots.ttf", fontSize, true, true, true);
//    birdFont.load(, fontSize, true, true, true);
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
}


TextManager::~TextManager()
{
}


void TextManager::changeFontSize(int &newSize){
    fontSize = newSize;
    msgFont.load(msgFontName, fontSize, true, true, true);
    birdFont.load(birdFontName, fontSize, true, true, true);
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

bool comparePointX(ofPoint &a, ofPoint &b){
    return a.x < b.x;
}

bool comparePointY(ofPoint &a, ofPoint &b){
    return a.y < b.y;
}

ofPolyline TextManager::simplifyPolyline(int letter, ofVec2f letterPosition) {
    float minSamplingDistance = 5.0;
    ofPolyline simplePoly;
    
    if(birdFont.isLoaded()){
        //Set stroke to create polyline
        std::vector<ofPath> dottedPaths = birdFont.getStringAsPoints(msg);
        ofPath dottedPath = dottedPaths[dottedPaths.size()-1];
        dottedPath.setStrokeWidth(2);
        std::vector<ofPolyline> dottedOutline = dottedPath.getOutline();
        std::vector<ofPoint> points ;
        for (int i=0; i < dottedOutline.size(); i++) {
            if( dottedOutline[i].size()){
                dottedOutline[i].translate(msgPosition);
                points.push_back(dottedOutline[i][0]);
            }
        }
        
        if(points.size()){
            std::sort(points.begin(), points.end(), &comparePointY);
            std::sort(points.begin(), points.end(), &comparePointX);
            
            int letterPoints = 0;
            ofPoint prevPoint = points[0];
            simplePoly.addVertex(prevPoint);
            float distance;
            
            ofLog() << "Original point size : " << points.size();
            for (int i=1; i < points.size(); i++) {
                distance = prevPoint.distance( points[i]);
                //            ofLog() << "Distance is " << distance;
                if(distance >= minSamplingDistance){
                    //                    if(i%2){
                    simplePoly.addVertex(points[i]);
                    prevPoint = points[i];
                }
            }
            ofLog() << "New Letter points size : " << simplePoly.size();
        }
        
    } else {
        ofLog(OF_LOG_ERROR) << "No dotted font found on the system!";
    }
    
    return simplePoly;
}

void TextManager::addLetter(int letter) {
    
	int prevMsgLength = ofUTF8Length(msg);
    string newLetter = ofUTF8ToString(letter);
    msg += newLetter;
    
    ofVec2f newLetterPosition = ofVec2f(msgPosition.x + (float)prevMsgLength*fontSpacing, msgPosition.y);
    
    ofLog() << "New letter received: " << newLetter;
    ofLog() << "New message size:" << prevMsgLength + 1;
    
    if(msgFont.isLoaded()){
        ofPath letterPath = msgFont.getCharacterAsPoints(letter);
        letterPath.translate(newLetterPosition);
        msgPaths.push_back(letterPath);
    } else {
        ofLog(OF_LOG_ERROR) << "No message font on the system!";
    }
    
    ofPolyline simplifiedPoly = simplifyPolyline(letter, newLetterPosition);
    msgPolys.push_back(simplifiedPoly);
    
    

//    birdmanager->addBird(simplifiedPoly);f
    
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
//        ofSetColor(ofColor::blue);
        int k = 0;
        for (vector<ofPolyline>::iterator ito = msgPolys.begin(); ito < msgPolys.end(); ito++){
            ofNoFill();
//            ofVec2f center = (*ito)[0];
//            ofDrawCircle(center, 10);
            k+=1;
            for (int i = 0; i < ito->size(); i++) {
                ofVec3f p = (*ito)[i];
                
                //Draw Cercle
                ofSetLineWidth(1);
                ofSetColor(i*8,k*10, 255);
                ofDrawCircle(p, 4);
                //Draw Line
//                if (i > 0) {
//                    ofDrawLine((*ito)[i], (*ito)[i - 1]);
//                } else {
//                    ofDrawLine((*ito)[i], (*ito)[ito->size() - 1]);
//                }
            }

        }
    }
    
}

void TextManager::clear(){
    msg = "";
    msgPolys.clear();
    msgPaths.clear();
    birdmanager->killAll();
}
