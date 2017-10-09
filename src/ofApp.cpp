#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//set a background colour
	ofBackground(253, 246, 227);	

	//load the image to work with
	image.loadImage("theImage.jpg");
	
	//an objectifier to create segments from the image
	Objectifier o; 

	//segments is a vector of pointers to Segment objects
	segments = o.objectify(image);
}

//--------------------------------------------------------------
void ofApp::update(){

	//checking to see if mouse was pressed on a segment
	if(selecting){
	
		for(int i = 0; i < segments.size(); i++){

			//if mouse within bounds of segment
			if(clickPosX > segments[i]->getX() && clickPosX < (segments[i]->getX()+segments[i]->getWidth()) && clickPosY > segments[i]->getY() && clickPosY < (segments[i]->getY() + segments[i]->getHeight())){

				selectedSegment = i;
				selected = true;

				break;
			}
		}//finished checking if this mouse press was on a segment
		
		selecting = false;
	}//close selecting

	if(selected){
	
		segments[selectedSegment] -> move((mouseX - clickPosX), (mouseY - clickPosY));

		//update mouseposition within segment for this move
		clickPosX = mouseX;
		clickPosY = mouseY;
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	//draw all the segments
	for(auto & s: segments){

		s -> draw();
	}

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	clickPosX = x;
	clickPosY = y;

	selecting = true;

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

	selecting = false;
	selected = false;
}

//--------------------------------------------------------------
ofApp::~ofApp(){

	//delete the segments from dynamic memory
	for(auto & s: segments){

		delete s;
		s = nullptr;
	}
}

