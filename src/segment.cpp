//segment.cpp

#include "segment.h"

Segment::Segment(ofImage img, ofVec2f pos, int wid, int high) : image(img), position(pos), width(wid), height(high){
}

//------------------------------------------------------

void Segment::draw(){

	//draw image at its currently defined position
	image.draw(position.x, position.y);
}

//-----------------------------------------------------
void Segment::move(int xValue, int yValue){

	//update position of segment being dragged
	position.x += xValue;
	position.y += yValue;
}

//--------------------------------------------------------
int Segment::getWidth(){

	return width;
}

//---------------------------------------------------------
int Segment::getHeight(){

	return height;
}

//-------------------------------------------------------
int Segment::getX(){

	return position.x;
}

//-----------------------------------------------------
int Segment::getY(){

	return position.y;
}
