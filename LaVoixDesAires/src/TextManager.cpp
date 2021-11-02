#include "TextManager.h"

TextManager::TextManager()
{
}

TextManager::TextManager(BirdManager* b, ofParameterGroup* _pg, int _w, int _h)
{
    msg = "";
    msgPosition = ofPoint(150, 300);
    
    w = _w;
    h = _h;

//    Verdana 60pt bbox on 1440px
//    top-left x:219, y:282
//    down-right x:1207, y:349 ==> ~900
    fontSpacing = 45;
    fontSize= 60;
    fontDistSampling = 5;
    changeFontSize(fontSize);
    
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
	pg->add(gfontDistSampling.set("Char Sampling", fontDistSampling, 1, 20));
    pg->add(zoomBigLetter.set("zoom big letter", 12 , 1, 20));
    pg->add(alphaBigLetter.set("alpha big letter", 245 , 0, 255));

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

void TextManager::update(){
    myLetter.update(birdmanager);
}


void TextManager::draw() {
	//DRAW only the last letter shortly
	if (msgPaths.size() > 0)
	{
	
			ofFill();
			int k = 0;

			for (int t = (msgPaths.size()-1) ; t >= 0; t--)
			{
		
				float duration = ofGetElapsedTimef() - timeOfLastLetter[t];
				
                //Draw the last letter, only for 2 seconds.
                if (duration < 2)
				{
					// 1st second, white only
                    if (duration < 1)
					{
						ofSetColor(255);
					}
                    // Then, fade to black.
					else
					{
						ofSetColor(255 - (duration - 1.0)/1.0 * 255.0);
					}
                    
                    if(drawMsgFill){
                        ofFill();
                    }else{
                        ofNoFill();
                    }
                    
                        msgPaths[t].draw();
//                    for (int i = 0; i < msgPolys[t].size(); i++)
//                    {
//                        ofVec3f p = msgPolys[t][i];
//                        ofDrawCircle(p, 3);
//                    }

				}
				else {
					break;
				}


			}


	}
    
    myLetter.drawBasic(zoomBigLetter, alphaBigLetter);
    
    

		
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

ofPath TextManager::createPathFromLetter( char letter, ofVec2f newLetterPosition){
    //NOT FILLED
    // permet de récuperer facilement un polyline avec la liste des points
    // Ne permet pas de bien déssiner le ofpath
    ofPath lastLetter = msgFont.getCharacterAsPoints(letter, true , false);
   
    lastLetter.translate(newLetterPosition);
    return lastLetter;
    
}

ofPath TextManager::createFilledPathFromLetter(char letter, ofVec2f newLetterPosition){
    // Ne permet pas de bien récuperer les polyline, mais permet de dessiner.
    ofPath lastLetter = msgFont.getCharacterAsPoints(letter, true , true);
    lastLetter.translate(newLetterPosition);
    return lastLetter;
    
}

vector<ofPolyline> TextManager::reduceDistanceSampling( ofPath path){
    //NOT FILLED
    // permet de récuperer facilement un polyline avec la liste des points
    // Ne permet pas de bien déssiner le ofpath
    vector<ofPolyline> listOfPoly;
    
    int size = path.getOutline().size();
    for(int i=0; i<size; i++){
        
        ofPolyline poly = path.getOutline()[i];
        poly = poly.getResampledBySpacing(fontDistSampling);
        listOfPoly.push_back(poly);
    }
    
    
    return listOfPoly;
    
}




void TextManager::addLetter(int letter) {
    
	int prevMsgLength = ofUTF8Length(msg);
    if(letter==13){
        msgPosition.y+= fontSize;
        msg.clear();

    }else{
        
        if (prevMsgLength < MAX_LETTER)
        {
            
            
            string newLetter = ofUTF8ToString(letter);
            msg += newLetter;
            
            ofVec2f newLetterPosition = ofVec2f(msgPosition.x + (float)prevMsgLength*fontSpacing, msgPosition.y);
            
            ofLog() << "New letter received: " << newLetter;
            ofLog() << "New message size:" << prevMsgLength + 1;
            
            ofPath pathLetterToBird, pathLetterToDraw;
            
            if (msgFont.isLoaded()) {
                pathLetterToBird = createPathFromLetter(msg.back(), newLetterPosition);
                pathLetterToDraw = createFilledPathFromLetter(msg.back(), newLetterPosition);
            }
            else {
                ofLog(OF_LOG_ERROR) << "No message font on the system!";
            }
            
            
            
            // Add bird polyline by polyline
            /*
             Plusieurs étape. On créé déja le path, puis on utilise reduceDistanceSampling pour limiter le nombre de point.
             On le transforme en vector<ofPolyline> car trop dur de modifier directement le ofpath
             Ensuite pour chaque polyline, on récréé un "BIG" polyline, destiné au lettrage, mais avec le meme nombre de point.
             
             */
            
            
            
            vector<int> listOfNiche;
            vector<ofPolyline> listOfPolyline = reduceDistanceSampling( pathLetterToBird);
            ofPolyline bigLetterPolyline;
            vector<ofPolyline> listOfBigPolyline;
            ofVec2f translation;
            
            for(int i=0; i<listOfPolyline.size(); i++){
                bigLetterPolyline = listOfPolyline[i];
                bigLetterPolyline.scale(zoomBigLetter, zoomBigLetter);
                if(i==0){
                    ofVec2f centroid = bigLetterPolyline.getCentroid2D();
                    ofRectangle boudingBox = bigLetterPolyline.getBoundingBox();
                    translation =  ofVec2f(w/2, h/2) - centroid;
                    translation.x -= boudingBox.width/4 + ofRandom(-500, 200);
                    translation.y +=  ofRandom(-200, 200);
                    
                }
                bigLetterPolyline.translate(translation);
                int niche = birdmanager->addBirdFromPolyline(listOfPolyline[i],bigLetterPolyline);
                listOfNiche.push_back(niche);
                listOfBigPolyline.push_back(bigLetterPolyline);
            }
            
            msgPaths.push_back(pathLetterToDraw);
            
            //Check time from the added letter
            if(msgPaths.size()>0){
                timeOfLastLetter[msgPaths.size() - 1] = ofGetElapsedTimef();
            }
            
            
            
            // Add big letter. Char, ofPath of the initial shape, listOfNiche = nichée associated to polyline
            
            /*
             if(lastLetter.getOutline().size()>0){
             lastLetter.scale(zoomBigLetter, zoomBigLetter);
             ofVec2f centroid = lastLetter.getOutline()[0].getCentroid2D();
             ofVec2f translation =  ofVec2f(ofGetWidth()/2, ofGetHeight()/2) - centroid ;
             lastLetter.translate(translation);
             }
             */
            
            
            myLetter =  Letter(letter, listOfBigPolyline, listOfNiche );
            
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
