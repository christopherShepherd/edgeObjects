//objectifier.cpp
//

#include "objectifier.h"

Objectifier::Objectifier(){

	//search vector to check surrounding pixels from the current one
	searches.push_back(ofVec2f(1, 0));
	searches.push_back(ofVec2f(1, 1));
	searches.push_back(ofVec2f(0, 1));
	searches.push_back(ofVec2f(-1, 1));
	searches.push_back(ofVec2f(-1, 0));
	searches.push_back(ofVec2f(-1, -1));
	searches.push_back(ofVec2f(0, -1));
	searches.push_back(ofVec2f(1, -1));

	searches.push_back(ofVec2f(2, 0));
	searches.push_back(ofVec2f(2, 1));
	searches.push_back(ofVec2f(2, 2));
	searches.push_back(ofVec2f(1, 2));
	searches.push_back(ofVec2f(0, 2));
	searches.push_back(ofVec2f(-1, 2));
	searches.push_back(ofVec2f(-2, 2));
	searches.push_back(ofVec2f(-2, 1));
	searches.push_back(ofVec2f(-2, 0));
	searches.push_back(ofVec2f(-2, -1));
	searches.push_back(ofVec2f(-2, -2));
	searches.push_back(ofVec2f(-1, -2));
	searches.push_back(ofVec2f(0, -2));
	searches.push_back(ofVec2f(1, -2));
	searches.push_back(ofVec2f(2, -2));
	searches.push_back(ofVec2f(2, 1));

}

//-----------------------------------------------------
vector<Segment*> Objectifier::objectify(ofImage img){
	
	//image width and height
	int width = img.getWidth();
	int height = img.getHeight();

	//grey image for edge detection
	ofImage imageGrey = img;
	imageGrey.setImageType(OF_IMAGE_GRAYSCALE);

	//holding image to place edge detected version of imageGrey
	ofImage filteredImage;
	filteredImage.allocate(width, height, OF_IMAGE_GRAYSCALE);

	//apply sobel filters on greyscale image
	detectEdges(imageGrey, filteredImage);

	//now have an image with edges highlighted.
	
	//keep track of discovered bright pixels
	vector<int> locations;

	//for every pixel in the image
	for(int y = 10; y < height-10; y++){
		for(int x = 10; x < width-10; x++){

			//get brightness
			float bright = filteredImage.getColor(x, y).getBrightness();

			//if brighter than a threshold
			if(bright > 100){
			
				//calculate where in the pixels array and see if it has already
				//been located
				int loc = x + y*width;

				//has this pixel been used in a segment already?
				bool noMatch = true;

				//check the pixels already used
				for(int i = 0; i < locations.size(); i++){

					if(loc == locations[i]){

						noMatch = false;
						break;
					}
				}

				//if still noMatch after checking those already found
				if(noMatch){

					//to remember we have found this pixel
					locations.push_back(loc);

					//a new object is a vector of points,
					//the pixel just found will be the start point
					ofVec2f startPixel(x, y);
					vector<ofVec2f> objNew;

					objNew.push_back(startPixel);

					//current position in the search pattern
					int searchNum = 0;

					//using this start pixel, chainsearch for nearby
					//bright pixels and add any found to this new object
					chainSearch(filteredImage, objNew, locations, startPixel, searches, searchNum);

					//once the whole object has been found
					//create a segment - push the pixels it covers to locations
					//start search again

					//ceateSegment() returns a pointer the the created segment
					Segment * seg = createSegment(img, objNew, locations, width);

					//push the pointer to the segments vector
					segments.push_back(seg);

				}
			}//end of brightness check
		}
	}//close for loop(pixels in the image)

	//objectifying of image finished
	return segments;
}

//-----------------------------------------------------------
void Objectifier::detectEdges(ofImage & img, ofImage & target){

	//sobelFilter detect horizontal lines
	double sobelX[3][3] = {{-1, 0, 1},
		               {-2, 0, 2},
			       {-1, 0, 1}};

	//vertical lines
	double sobelY[3][3] = {{1, 2, 1},
		               {0, 0, 0},
			       {-1, -2, -1}};

	//for each pixel in the image, convolve with the x and y kernels independently
	//the final pixel value = sqrt(i^2 + j^2)
	for(int x = 1; x < img.getWidth()-1; x++){
		for(int y = 1; y < img.getHeight()-1; y++){

			double i = (sobelX[0][0]*img.getColor(x-1, y-1).getBrightness()) + (sobelX[0][1]*img.getColor(x, y-1).getBrightness()) + (sobelX[0][2]*img.getColor(x+1, y-1).getBrightness()) + 
(sobelX[1][0]*img.getColor(x-1, y).getBrightness()) + (sobelX[1][1]*img.getColor(x, y).getBrightness()) + (sobelX[1][2]*img.getColor(x+1, y).getBrightness()) + 

(sobelX[2][0]*img.getColor(x-1, y+1).getBrightness()) + (sobelX[2][1]*img.getColor(x, y+1).getBrightness()) + (sobelX[2][2]*img.getColor(x+1, y+1).getBrightness());

			double j = (sobelY[0][0]*img.getColor(x-1, y-1).getBrightness()) + (sobelY[0][1]*img.getColor(x, y-1).getBrightness()) + (sobelY[0][2]*img.getColor(x+1, y-1).getBrightness()) + 
(sobelY[1][0]*img.getColor(x-1, y).getBrightness()) + (sobelY[1][1]*img.getColor(x, y).getBrightness()) + (sobelY[1][2]*img.getColor(x+1, y).getBrightness()) + 

(sobelY[2][0]*img.getColor(x-1, y+1).getBrightness()) + (sobelY[2][1]*img.getColor(x, y+1).getBrightness()) + (sobelY[2][2]*img.getColor(x+1, y+1).getBrightness());

			ofColor endVal = ofColor(sqrt((i*i)+(j*j)));

			target.setColor(x, y, endVal);
	
		}
	}

	//fix the changes to target image
	target.update();
}

//----------------------------------------------------------
void Objectifier::chainSearch(ofImage & img, vector<ofVec2f> & obs, vector<int> & locs, ofVec2f start, vector<ofVec2f> & searches, int & searchNum){


	//chainSearch looks for other bright pixels next to the one just found
	
	//current pixel to be checked
	ofVec2f current(start.x+searches[searchNum].x, start.y+searches[searchNum].y);

	//brightness of this pixel
	float bright = img.getColor(current.x, current.y).getBrightness();

	if(bright > 100){

		int loc = current.x + current.y*img.getWidth();
		bool noMatch = true;

		for(int i = 0; i < locs.size(); i++){

			if(loc == locs[i]){

				noMatch = false;
				break;
			}
		}

		if(noMatch){

			//if this pixel(current) is a new pixel, store its position
			obs.push_back(current);

			locs.push_back(loc);
			//reset the search position and check to see if there is a 
			//bright pixel next to this new one
			searchNum = 0;
			chainSearch(img, obs, locs, current, searches, searchNum);
		
		}
	}//close brightness check

	//pixel either wasn't bright or has already been found
	if(searchNum < 13){

		//if there are still surrounding pixels to search then search them
		searchNum++;
		chainSearch(img, obs, locs, start, searches, searchNum);
	}
		
	//if this pixel is dark and all other surrounding pixels have been checked
	//then all of the pixels defining this object have been found
}

//--------------------------------------------------------------------------------
Segment * Objectifier::createSegment(ofImage & img, vector<ofVec2f> & positions, vector<int> & locs, const int & width){

	//top left and bottom right most corners of the segment to be created 
	int xLeft, xRight;
	int yTop, yBottom;

	//for every point in this object, compare to find corner bounds
	for(int k = 0; k < positions.size(); k++){

		if(k == 0){

			xLeft = positions[k].x;
			xRight = positions[k].x;
			yBottom = positions[k].y;
			yTop = positions[k].y;
		}else{

			//assign the position as a corner if it satisfies the condition
			if(positions[k].x < xLeft){
				xLeft = positions[k].x;
			}else if(positions[k].x > xRight){
				xRight = positions[k].x;
			}

			if(positions[k].y < yTop){ 
				yTop = positions[k].y;
			}else if(positions[k].y > yBottom){
				yBottom = positions[k].y;
			}
		}
	}//end of position sort

	//update locations with segment coverage
	for(int y = yTop; y<=yBottom; y++){
		for(int x = xLeft; x<=xRight; x++){
		
			//prevents multiple selections over the same area
			int loc = x + y*width;
			locs.push_back(loc);
		}
	}
	

	//width and height of the current object
	int objWidth = xRight - xLeft;
	int objHeight = yBottom - yTop;

	//new image is an image section defined by the above
	ofImage objImage;
	objImage.cropFrom(img, xLeft, yTop, objWidth, objHeight);

	//vector to hold the position it should be drawn in
	ofVec2f imagePosition(xLeft, yTop);

	//create a pointer to a new Segment(in dynamic memory)
	Segment * seg = new Segment(objImage, imagePosition, objWidth, objHeight);

	//return the pointer
	return seg;

}

