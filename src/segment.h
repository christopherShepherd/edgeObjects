//segment.h
//class to store an image and draw location together

#ifndef segment_h
#define segment_h

#include "ofMain.h"

class Segment{


	//position to draw this image
	ofVec2f position;

	//the image defining this section
	ofImage image;

	//pixel width and height of the image
	int width, height;

	public:
		Segment(ofImage img, ofVec2f pos, int wid, int high);

		//draw the segment
		void draw();

		//update the draw position of this segment
		void move(int xValue, int yValue); 

		//get values associated with this segment
		int getWidth();
		int getHeight();
		int getX();
		int getY();

};
#endif
