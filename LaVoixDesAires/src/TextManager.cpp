#include "TextManager.h"

TextManager::TextManager()
{
}

TextManager::TextManager(BirdManager* b, ofParameterGroup* _pg)
{
    msg = "";
    msgPosition = ofPoint(300, 300);

//    Verdana 60pt bbox on 1440px
//    top-left x:219, y:282
//    down-right x:1207, y:349 ==> ~900
    fontSpacing = 70;
    fontSize= 38;
    fontDistSampling = 3.10;
    
//    msgFont.load("ttwpglot.ttf", fontSize, true, true, true);
//    msgFont.load("ralewayDots.ttf", fontSize, true, true, true);
//    birdFont.load(, fontSize, true, true, true);
    birdmanager = b;
    msgPolys.clear();
    
    
    gFontSize.addListener(this, &TextManager::changeFontSize);
    gFontSpacing.addListener(this, &TextManager::changeFontSpacing);
    gMsgPositionX.addListener(this, &TextManager::changeMsgPositionX);
    gMsgPositionY.addListener(this, &TextManager::changeMsgPositionY);
    gfontDistSampling.addListener(this, &TextManager::changeFontSampling);
    
    pg = _pg;
    pg->setName("Text Manager");
    pg->add(drawMsgFill.set("Draw Fill", true));
    pg->add(drawMsgContour.set("Draw Contour", true));
    pg->add(gFontSize.set("Font Size", fontSize, 38, 150));
    pg->add(gFontSpacing.set("Font Spacing", fontSpacing, 10, 200));
    pg->add(gMsgPositionX.set("Pos X", msgPosition.x, 10, 600));
    pg->add(gMsgPositionY.set("Pos Y", msgPosition.y, 10, 600));
    pg->add(gfontDistSampling.set("Char Sampling", fontDistSampling, 0.0, 7.0));
}


TextManager::~TextManager()
{
}

void TextManager::changeFontSampling(float &s){
    fontDistSampling = s;
}

void TextManager::changeMsgPositionX(int &x){
    msgPosition.x = x;
}

void TextManager::changeMsgPositionY(int &y){
    msgPosition.y = y;
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

std::vector<ofPoint> reducePointsBy(std::vector<ofPoint> points, string sense, float minSpaceDistance){
    std::vector<ofPoint> result;
    if (sense== "x") {
        std::sort(points.begin(), points.end(), &comparePointX);
    } else {
        std::sort(points.begin(), points.end(), &comparePointY);
    }
    
    if (points.size() > 0){
        ofPoint prevPoint = points[0];
        float currentDistance;
        for (int i=1; i < points.size(); i++) {
            currentDistance = prevPoint.distance( points[i]);
            //        of90Log() << "Distance is " << currentDistance;
            if(currentDistance >= minSpaceDistance ){
                result.push_back(points[i]);
                prevPoint = points[i];
            }
        }
    }

    
    return result;
}

ofPolyline TextManager::simplifyPolyline(int letter, ofVec2f letterPosition) {
    ofPolyline simplePoly;

    if(birdFont.isLoaded()){
        //Set stroke to create polyline
        std::vector<ofPath> dottedPaths = birdFont.getStringAsPoints(msg);
        if (dottedPaths.size()){
            ofPath dottedPath = dottedPaths[dottedPaths.size()-1];
            dottedPath.setStrokeWidth(2);
            std::vector<ofPolyline> dottedOutline = dottedPath.getOutline();
            std::vector<ofPoint> points ;
            for (int i=0; i < dottedOutline.size(); i++) {
                dottedOutline[i].translate(msgPosition);

                if(fontSize >= 36 && dottedOutline[i].size()){
                    points.push_back(dottedOutline[i][0]);
                } else {
                    for(int k=0; k < dottedOutline[i].size(); k++){
                        points.push_back(dottedOutline[i][k]);
                    }
                }
            }

            if(points.size()){
                ofLog() << "Original point size : " << points.size();
                points = reducePointsBy(points, "x", fontDistSampling);
                points = reducePointsBy(points, "y", fontDistSampling);
                ofLog() << "New Letter points size : " << points.size();
                for (auto& pPoint : points) {
                    simplePoly.addVertex(pPoint);
                }
            }
        }
    } else {
        ofLog(OF_LOG_ERROR) << "No dotted font found on the system!";
    }
    return simplePoly;
}

void TextManager::addPathWithCustomSpacing(int letter, ofVec2f position){
    ofPath letterPath = msgFont.getCharacterAsPoints(letter);
    letterPath.translate(position);
    msgPaths.push_back(letterPath);
}

void TextManager::addPathSimple(){
    std::vector<ofPath> lettersPaths = msgFont.getStringAsPoints(msg);
    if( lettersPaths.size()){
        ofPath letterPaths = lettersPaths.back();
        letterPaths.translate(msgPosition);
        msgPaths.push_back(letterPaths);
    }
}

void TextManager::addLetter(int letter) {
    
	int prevMsgLength = ofUTF8Length(msg);
    string newLetter = ofUTF8ToString(letter);
    msg += newLetter;
    
    ofVec2f newLetterPosition = ofVec2f(msgPosition.x + (float)prevMsgLength*fontSpacing, msgPosition.y);
    
    ofLog() << "New letter received: " << newLetter;
    ofLog() << "New message size:" << prevMsgLength + 1;
    
    if(msgFont.isLoaded()){
        addPathSimple();
//        addPathWithCustomSpacing(letter, newLetterPosition);
    } else {
        ofLog(OF_LOG_ERROR) << "No message font on the system!";
    }
    
    ofPolyline simplifiedPoly = simplifyPolyline(letter, newLetterPosition);
    msgPolys.push_back(simplifiedPoly);
    
    

    birdmanager->addBird(simplifiedPoly);
    
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
                ofSetColor(255,152, 0);
                ofDrawCircle(p, 3);
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
