#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(50);

    videoPlayer.loadMovie(ofToDataPath("D01T01_Ros_sync_Center_Small.mp4"));
    videoPlayer.setLoopState(OF_LOOP_NORMAL);
    videoPlayer.play();

    ofSetWindowShape(videoPlayer.width * 2, videoPlayer.height * 2);

    std::string urlx = "http://api.piecemeta.com/streams/d1c05738-4b96-4548-837f-90dd9b37af08.json";
    std::string urly = "http://api.piecemeta.com/streams/f166001a-42db-48f0-8514-a6fd636eed6c.json";
    //std::string urlz = "http://api.piecemeta.com/streams/5c77f56b-8fe3-4344-85e3-52fe16f78272.json";

    if (!responsex.open(urlx))
    {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON";
    }
    if (!responsey.open(urly))
    {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON";
    }
    /*if (!responsez.open(urlz))
    {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON";
    }*/

    for (Json::ArrayIndex i = 0; i < responsex["frames"].size(); i++)
    {
        ofPoint p;
        p.x = responsex["frames"][i].asFloat();
        p.y = responsey["frames"][i].asFloat();
        points.push_back(p);
    }

    curFrame.addListener(this, &ofApp::curFrameChanged);

    gui.setup(); // most of the time you don't need a name
    gui.add(curFrame.setup("Current Frame", 0, 0, responsex["frames"].size()));
    gui.add(playSpeed.setup("Playback Speed", 1, 1, 10));
    gui.add(forceDuration.setup("Force Duration", 1000, 0, 5000));
    gui.add(forceIntensity.setup("Force Intensity", 1, 0, 100));
    gui.add(velocity.setup("Velocity", 0, 0, 1));
    gui.add(bFeedback.setup("Force Feedback", false));
    gui.add(bShowCircle.setup("Show Circle", true));

    changedTime = -100;
    
    spidar.init();
    spidar.open();

    kalman.init(1e-4, 1e-1);
}

void ofApp::update()
{
    curFrame = (curFrame + playSpeed) % points.size();
    videoPlayer.setFrame(curFrame / 2);
    videoPlayer.update();

    //flow.calcOpticalFlow(videoPlayer);

    ofPoint &p0 = points.at(curFrame);
    kalman.update(p0);
    kalman.getPrediction();
    ofPoint p = kalman.getPrediction();

    if(bFeedback)
        spidar.setForce(kalman.getVelocity() * forceIntensity, forceDuration);
    spidar.update();

    velocity = kalman.getVelocity().length();

    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void ofApp::curFrameChanged(int & frame)
{
    changedTime = ofGetElapsedTimef();
}

void ofApp::draw()
{
    ofBackground(0);

    ofSetColor(255);
    videoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofPushMatrix();
    ofScale(2, 2);
    //flow.draw();
    ofPopMatrix();

    ofPushMatrix();

    ofSetColor(255);
    if (!bHide) {
        gui.draw();
    }

    if (bShowCircle)
    {
        ofPushMatrix();
        ofTranslate(ofGetWidth() - 200, 0);
        spidar.draw();
        ofPopMatrix();

        ofFill();

        ofPoint p = kalman.getEstimation();
        p.x = ofMap(p.x, 0, 15, -ofGetWidth() / 2, ofGetWidth() / 2);
        p.y = ofMap(p.y, 0, 15, ofGetHeight() / 2, -ofGetHeight() / 4);
        ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
        ofSetColor(ofColor::salmon);
        ofCircle(p, 10);
        ofSetLineWidth(4);
        ofPoint dir = kalman.getVelocity();
        ofTranslate(p);
        ofSetColor(ofColor::floralWhite);
        ofLine(ofPoint(), dir * 2);

        ofPopMatrix();
    }
}

void ofApp::keyPressed(int key) {
    if (key == 'h') {
        bHide = !bHide;
    }
    else if (key == 'f') {
        ofToggleFullscreen();
    }
    else if (key == ' ') {
        color = ofColor(255);
    }
    else if (key == OF_KEY_RIGHT) {
        curFrame = (curFrame + 5) % points.size();
    }
    else if (key == OF_KEY_LEFT) {
        curFrame = (curFrame - 6) % points.size();
    }
}

void ofApp::mouseDragged(int x, int y, int button)
{

}

void ofApp::mousePressed(int x, int y, int button)
{

}

void ofApp::mouseReleased(int x, int y, int button)
{

}
