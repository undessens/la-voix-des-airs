#include "TextManager.h"

TextManager::TextManager()
{
}

TextManager::TextManager(BirdManager* b, ofParameterGroup* _pg)
{
    msg = "";
    msgPosition = ofPoint(400, 300);

//    Verdana 60pt bbox on 1440px
//    top-left x:219, y:282
//    down-right x:1207, y:349 ==> ~900
    fontSpacing = 70;
    fontSize= 60;
    fontDistSampling = 2.9;
    
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
    pg->add(drawMsgFill.set("Draw Fill", false));
    pg->add(drawMsgContour.set("Draw Contour", false));
    pg->add(gFontSize.set("Font Size", fontSize, 38, 150));
    pg->add(gFontSpacing.set("Font Spacing", fontSpacing, 10, 200));
	pg->add(gMsgPositionX.set("Pos X", msgPosition.x, 10, 1000));
	pg->add(gMsgPositionY.set("Pos Y", msgPosition.y, 10, 600));
	pg->add(gfontDistSampling.set("Char Sampling", fontDistSampling, 0.0, 7.0));

	for (int i = 0; i < MAX_LETTER; i++) {
		timeOfLastLetter[i] = 0.f;
	}
}


TextManager::~TextManager()
{
}

void TextManager::changeFontSampling(float &s) {
	fontDistSampling = s;
}

void TextManager::changeMsgPositionX(int &x) {
	msgPosition.x = x;
}

void TextManager::changeMsgPositionY(int &y) {
	msgPosition.y = y;
}


void TextManager::changeFontSize(int &newSize) {
	fontSize = newSize;
	msgFont.load(msgFontName, fontSize, true, true, true);
	birdFont.load(birdFontName, fontSize, true, true, true);
}

void TextManager::changeFontSpacing(int &newSpacing) {
	fontSpacing = newSpacing;
}


void TextManager::draw() {
	//DRAW only the last letter shortly

	if (msgPolys.size() > 0) 
	{
	
			ofFill();
			int k = 0;

			for (int t = (msgPolys.size()-1) ; t >= 0; t--) 
			{
		
				float duration = ofGetElapsedTimef() - timeOfLastLetter[t];
				if (duration < 2)
				{
					if (duration < 1)
					{
						ofSetColor(255);
					}
					else
					{
						ofSetColor(255 - (duration - 1.0)/1.0 * 255.0);
					}
					for (int i = 0; i < msgPolys[t].size(); i++)
					{
						ofVec3f p = msgPolys[t][i];
						ofDrawCircle(p, 3);
					}

				}
				else {
					break;
				}


			}

		


	}

		
}

bool comparePointX(ofPoint &a, ofPoint &b) {
	return a.x < b.x;
}

bool comparePointY(ofPoint &a, ofPoint &b) {
	return a.y < b.y;
}

std::vector<ofPoint> reducePointsBy(std::vector<ofPoint> points, string sense, float minSpaceDistance) {
	std::vector<ofPoint> result;
	if (sense == "x") {
		std::sort(points.begin(), points.end(), &comparePointX);
	}
	else {
		std::sort(points.begin(), points.end(), &comparePointY);
	}

	if (points.size() > 0) {
		ofPoint prevPoint = points[0];
		float currentDistance;
		for (int i = 1; i < points.size(); i++) {
			currentDistance = prevPoint.distance(points[i]);
			//        of90Log() << "Distance is " << currentDistance;
			if (currentDistance >= minSpaceDistance) {
				result.push_back(points[i]);
				prevPoint = points[i];
			}
		}
	}


	return result;
}

ofPolyline TextManager::createPolyline(int letter, ofVec2f letterPosition) {

	ofPolyline simplePoly;

	if (birdFont.isLoaded()) 
	{
		//Set stroke to create polyline
		std::vector<ofPath> dottedPaths = birdFont.getStringAsPoints(msg);
		if (dottedPaths.size())
		{
			ofPath dottedPath = dottedPaths[dottedPaths.size() - 1];
			dottedPath.setStrokeWidth(2);
			std::vector<ofPolyline> dottedOutline = dottedPath.getOutline();
			for (int i = 0; i < dottedOutline.size(); i++)
			{
				dottedOutline[i].translate(msgPosition);

				if (fontSize >= 36 && dottedOutline[i].size()) 
				{
					simplePoly.addVertex(dottedOutline[i][0]);
				}
				else 
				{
					for (int k = 0; k < dottedOutline[i].size(); k++) 
					{
						simplePoly.addVertex(dottedOutline[i][k]);
					}
				}

			}


			
		}
	}
	else {
		ofLog(OF_LOG_ERROR) << "No dotted font found on the system!";
	}
	return simplePoly;
}

ofPolyline TextManager::simplifyPolyline(ofPolyline input) {
	ofPolyline simplifiedPoly;

			std::vector<ofPoint> points;
			for (int k = 0; k < input.size(); k++)
			{
				points.push_back(input[k]);
			}

			if (points.size()) 
			{
				//Reduce point only if it is a letter
				points = reducePointsBy(points, "x", fontDistSampling);
				points = reducePointsBy(points, "y", fontDistSampling);
				
                for (auto& pPoint : points) 
				{
                    simplifiedPoly.addVertex(pPoint);
                }
            }

    return simplifiedPoly;
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
	if (prevMsgLength < MAX_LETTER)
	{


		string newLetter = ofUTF8ToString(letter);
		msg += newLetter;

		ofVec2f newLetterPosition = ofVec2f(msgPosition.x + (float)prevMsgLength*fontSpacing, msgPosition.y);

		ofLog() << "New letter received: " << newLetter;
		ofLog() << "New message size:" << prevMsgLength + 1;

		if (msgFont.isLoaded()) {
			addPathSimple();
			//        addPathWithCustomSpacing(letter, newLetterPosition);
		}
		else {
			ofLog(OF_LOG_ERROR) << "No message font on the system!";
		}

		//First create polyline
		//Then simplify if necessary
		//
		ofPolyline originalPoly = createPolyline(letter, newLetterPosition);
		ofPolyline simplifiedPoly = originalPoly;
		if ((letter > 47 && letter < 58) || (letter > 65 && letter < 91) || (letter > 96 && letter < 123))
		{
			//Simplify if need ( letter and not special caracter )
			simplifiedPoly = simplifyPolyline(originalPoly);
		}

		// Add to msg poly the original one
		msgPolys.push_back(originalPoly);

		//Check time from the added letter
		timeOfLastLetter[msgPolys.size() - 1] = ofGetElapsedTimef();

		// Add bird with the simplified one
		birdmanager->addBird(simplifiedPoly);

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
    
	// When clear the message, don't kill Birds, but push them into DIE ON BORDER MODE
	//birdmanager->killAll();
	birdmanager->lastFlyAll();
}
