//objectifier.h
//class to detect edges of an image, and segment that image surrounding those edges

#ifndef objectifier_h
#define objectifier_h

#include "ofMain.h"
#include "segment.h"

class Objectifier{

	private:
		//edge detection - creates filtered version of img in target
		void detectEdges(ofImage & img, ofImage & target);

		//recursive - find a chain of bright pixels from a starting bright pixel
		//arguments reference -> image to search, points vector defining the new object, 
		//vector of already found pixels, start pixel position, the search pattern vector, searchnum(position in search pattern)
		void chainSearch(ofImage & img, vector<ofVec2f> & obs, vector<int> & locs, ofVec2f start, vector<ofVec2f> & searches, int & searchNum);

		//create a new Segment and return a pointer to that segment
		//arguments - image from which to crop segment, vector of detected bright pixels defining this segment, the locations vector holding all pixels currently used, width of provided image
		Segment * createSegment(ofImage & img, vector<ofVec2f> & positions, vector<int> & locs, const int & width);

		//defines a 5x5 search pattern surrounding a pixel
		vector<ofVec2f> searches;

		//vector of segment pointers which are finally returned by objectify
		vector<Segment*> segments;

	public:

		Objectifier();

		//the main 'run' function of objectifier
		//creates and returns an 'objectified' version of the input image
		//returning a vector of pointers to a collection of 'Segments'
		vector<Segment*> objectify(ofImage im);

};

#endif
