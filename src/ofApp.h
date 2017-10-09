//This program takes a starting image and creates a selection
//of independent image segments from the edges detected
#pragma once

#include "ofMain.h"
#include "segment.h"
#include "objectifier.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		ofImage image;//original image

		vector<Segment *> segments;//vector of segments

		bool selecting, selected;//for moving around individual segments

		//keep track of previous frame's mouse position and the currently selected segment
		int clickPosX, clickPosY, selectedSegment;

		//destructor to free memory
		~ofApp();
		
};
