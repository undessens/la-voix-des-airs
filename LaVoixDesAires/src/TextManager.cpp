#include "TextManager.h"

TextManager::TextManager()
{
	size = 90;
	font.loadFont("mono.ttf", size, true, false, true);
	startPoint = ofVec2f(300, 300);
	text = "hell";
	addLetter('o');

}


TextManager::~TextManager()
{
}


void TextManager::draw() {

	//Font
	ofSetColor(ofColor::green);
	font.drawString("hello", 300, 100);
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
	text += ofToString(c);

}