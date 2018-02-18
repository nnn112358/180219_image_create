#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include <iostream> 
#include <cmath>
#include <string>
#include <sstream>

#include <vector>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define CV_EXT "d.lib"
#else
#define CV_EXT ".lib"
#endif
#pragma comment(lib, "opencv_world320" CV_EXT) // OpenCV3.3.0ÇÃèÍçáÇÕÅA"opencv_core330"Ç…

using namespace std;
using namespace cv;



void img_info(Mat &img);
int int_random(int max);
double uniform_random(double min, double max);
double gaussian_random(double mean, double std);


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofParameter<string> screenSize;
		ofParameter<bool>	doFullScreen;
		ofImage image_out;
		ofxPanel gui;
		ofParameter<bool> bHide;
		ofxFloatSlider p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,p12,p13,p14,p15,p16,p17;

		Mat img_proc;
		Mat img_proc_scale;

		bool event_flg;

		void				setFullScreen(bool& _value) { ofSetFullscreen(_value); }


};
