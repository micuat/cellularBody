#pragma once


#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxGui.h"
#include "ofxSpidarMouse.h"
#include "ofxCv.h"

class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void curFrameChanged(int & frame);
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    float changedTime;

    ofVideoPlayer videoPlayer;

    ofxJSONElement responsex, responsey, responsez;
    vector<ofPoint> points;
    ofxSpidarMouse spidar;

    ofxFloatSlider radius;
    ofxColorSlider color;
    ofxVec2Slider center;
    ofxIntSlider curFrame, playSpeed, forceDuration;
    ofxFloatSlider forceIntensity, velocity;
    ofxToggle bFeedback, bShowCircle;
    ofxButton twoCircles;
    ofxButton ringButton;
    ofxLabel screenSize;

    ofxPanel gui;

    ofxCv::KalmanPosition kalman;

    ofxCv::FlowPyrLK flow;

    bool bHide;

    ofApp() : bHide(false)
    {

    }
};
