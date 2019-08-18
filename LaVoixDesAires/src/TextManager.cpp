#include "TextManager.h"

TextManager::TextManager()
{

}

TextManager::TextManager(BirdManager* b)
{
    size = 90;
    font.loadFont("verdana.ttf", size, true, false, true);
    startPoint = ofVec2f(300, 300);
    text = "";
    birdmanager = b;
    listOfPoly.clear();
    
}

TextManager::~TextManager()
{
}


void TextManager::draw() {

	//Font
	ofSetColor(ofColor::green);
	std::vector<ofPath> letters = font.getStringAsPoints(text);
	ofFill();

	ofSetLineWidth(3);
	for (vector<ofPath>::iterator it = letters.begin(); it < letters.end(); it++)
	{
		//Set stroke to create polyline
		it->setStrokeWidth(2);
		ofSetColor(ofColor::red);
		///DRAW OFPATH
//		it->draw(300, 200);
		std::vector<ofPolyline> outline = it->getOutline();
		for (vector<ofPolyline>::iterator ito = outline.begin(); ito < outline.end(); ito++) {

			ito->translate(startPoint);
			ito->simplify(0.5);
			ofPolyline simple = ito->getResampledBySpacing(10);

			for (int i = 0; i < simple.size(); i++) {
				ofSetColor(ofColor::blue);
				ofVec3f p = simple[i];
				ofNoFill();
				ofSetLineWidth(1);
				ofDrawCircle(ofVec2f(p.x, p.y), 1);

				if (i > 0) {

					ofDrawLine(simple[i], simple[i - 1]);

				}
				else {
					ofDrawLine(simple[i], simple[simple.size() - 1]);
				}

			}

		}

	}
}

void TextManager::addLetter(char c) {
	string newLetter = ofToString(c);
    text += newLetter;
    
    std::vector<ofPath> letterPath = font.getStringAsPoints(text);
    //Normally there one OfPath
    ofPath path = letterPath[letterPath.size()-1];
    
        //Set stroke to create polyline
    path.setStrokeWidth(2);
    std::vector<ofPolyline> outline = path.getOutline();
    for (vector<ofPolyline>::iterator ito = outline.begin(); ito < outline.end(); ito++) {
            
            ito->translate(startPoint);
            ito->simplify(0.5);
            ofPolyline simple = ito->getResampledBySpacing(10);
            
            //Add to List of Poly
            listOfPoly.push_back(simple);
            
            for (int i = 0; i < simple.size(); i++) {
                ofSetColor(ofColor::blue);
                ofVec3f p = simple[i];
                
                birdmanager->addBird(c, i, p);

                
            }
            
        }
        

    

}

void TextManager::drawPoly(){

    for (vector<ofPolyline>::iterator ito = listOfPoly.begin(); ito < listOfPoly.end(); ito++){
        
        for (int i = 0; i < ito->size(); i++) {
            ofSetColor(ofColor::blue);
            ofVec3f p = (*ito)[i];
            
            //Draw Cercle
            ofNoFill();
            ofSetLineWidth(1);
            ofDrawCircle(ofVec2f(p.x, p.y), 1);
            
            //Draw Line
            if (i > 0) {
                ofDrawLine((*ito)[i], (*ito)[i - 1]);
            }
            else {
                ofDrawLine((*ito)[i], (*ito)[ito->size() - 1]);
            }
            
        }
        
    }
    
}
