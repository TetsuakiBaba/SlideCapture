#pragma once

#include "ofMain.h"
#include "ofxHomography.h"
#include "ofxSyphon.h"


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
    void setCornerVertex(int id, int x, int y);
    void aspectChanged(bool & _status);
    bool intervalUpdate(int _ms);
    void exit();
    

    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    int     bufferCounter;
    int     drawCounter;
    
    float smoothedVol;
    float scaledVol;

    
    ofVideoGrabber camera;
    ofImage image;
    
    ofPoint selectedCorners[4];
    ofPoint originalCorners[4];
    ofPoint distortedCorners[4];
    ofMatrix4x4 homography;
    bool is_dragging;
    int id_selected_corner;
    ofFbo fbo;
    ofPolyline p_selected;
    
    ofTexture tex;
    ofxSyphonServer server_slide;
    ofxSyphonServer server_full;
    
    ofTrueTypeFont font;

};
