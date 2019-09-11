#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetDataPathRoot("../Resources/data/");
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    string str_device;
    for( int i = 0; i < camera.listDevices().size(); i++){
        str_device = str_device + ofToString(i)+" -- "+camera.listDevices()[i].deviceName+"\n";
    }
    string str_return = ofSystemTextBoxDialog("Select Video Device:\n"+str_device);
    camera.setDeviceID(ofToInt(str_return));
   
    camera.initGrabber(1920,1080);
   
    
    setCornerVertex(0, 20, 20);
    setCornerVertex(1, 400, 20);
    setCornerVertex(2, 400, 220);
    setCornerVertex(3, 20,220);
    for( int i = 0; i < 4; i++ ){
        p_selected.addVertex(selectedCorners[i].x, selectedCorners[i].y);
    }
    
    distortedCorners[0].set(0, 0);
    distortedCorners[1].set(camera.getWidth(), 0);
    distortedCorners[2].set(camera.getWidth(), camera.getHeight());
    distortedCorners[3].set(0, camera.getHeight());

    
    is_dragging = false;
    fbo.allocate(camera.getWidth(), camera.getHeight());
    server_slide.setName("Slide");
    server_full.setName("Full");
    font.load(ofToDataPath("DIN Alternate Bold.ttf"), 18);
    
}

bool ofApp::intervalUpdate(int _ms)
{
    static unsigned long int ms_previous = ofGetElapsedTimeMillis();
    if( (ofGetElapsedTimeMillis() - ms_previous) > _ms ){
        ms_previous = ofGetElapsedTimeMillis();
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("SlideCapture");
    ofSetWindowShape(camera.getWidth()+camera.getWidth()/4, camera.getHeight());
    camera.update();
    homography = ofxHomography::findHomography(originalCorners, distortedCorners);
    if( camera.isFrameNew() ){
        fbo.begin();
        ofSetColor(255);
        ofPushMatrix();
        ofMultMatrix(homography);
        camera.draw(0,0,camera.getWidth(), camera.getHeight());
        ofPopMatrix();
        fbo.end();
        /*
        if( intervalUpdate(500) ){
            fbo.readToPixels(pixels_screen);
            image_screen.setFromPixels(pixels_screen);
            image_ocr = image_screen;
            image_ocr.resize(camera.getWidth()/4, camera.getHeight()/4);
            image_ocr.setImageType(OF_IMAGE_GRAYSCALE);
            ocr.setImage(image_ocr.getPixels().getData(),
                         image_ocr.getWidth(), image_ocr.getHeight());
            ocr.update();
        }
         */
    }
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    camera.draw(0,0);
    fbo.draw(ofGetWidth()-camera.getWidth()/4,
             0,
             camera.getWidth()/4, camera.getHeight()/4);
    string str_debug = "Syphon Server Name: [SlideCapture] Slide\nResolution:" +
    ofToString(camera.getWidth())+" x "+ofToString(camera.getHeight())+"\n";

    font.drawString(str_debug, ofGetWidth()-camera.getWidth()/4+5, camera.getHeight()/4-40);
    camera.draw(ofGetWidth()-camera.getWidth()/4,
                camera.getHeight()/4,
                camera.getWidth()/4, camera.getHeight()/4);
    str_debug = "Syphon Server Name: [SlideCapture] Full\nResolution:" +
    ofToString(camera.getWidth())+" x "+ofToString(camera.getHeight())+"\n";
    
    font.drawString(str_debug, ofGetWidth()-camera.getWidth()/4+5, 2*camera.getHeight()/4-40);
    ofSetColor(255,0,0);

    // Draw Homography Corners
    {
        ofNoFill();
        ofSetColor(255,0,0,150);
        ofSetLineWidth(2.0);
        float r = 20.0;
        for(int i = 0; i < 4; i++ ){
            if( ofDist(mouseX, mouseY, selectedCorners[i].x, selectedCorners[i].y) <= r ){
                ofFill();
            }
            ofDrawCircle(selectedCorners[i].x,selectedCorners[i].y,r);
            ofNoFill();
        }
        
        
        ofSetPolyMode(OF_POLY_WINDING_NONZERO);
        ofBeginShape();
        for(int i = 0; i < 4; i++){
            ofVertex(selectedCorners[i].x, selectedCorners[i].y);
        }
        ofEndShape();
    }
  
    ofFill();
    server_slide.publishTexture(&fbo.getTexture());
    server_full.publishTexture(&camera.getTexture());
   
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

void ofApp::setCornerVertex(int id, int x, int y)
{
    selectedCorners[id].set(x,y);
    originalCorners[id].set(selectedCorners[id].x, selectedCorners[id].y);
    //set(selectedCorners[id].x, selectedCorners[id].y-720);
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if( is_dragging == true ){
        setCornerVertex(id_selected_corner, x,y);
    }
    
    p_selected.clear();
    for(int i = 0; i < 4; i++){
        p_selected.addVertex(selectedCorners[i].x, selectedCorners[i].y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for( int i = 0; i < 4; i++ ){
        if( ofDist(selectedCorners[i].x, selectedCorners[i].y, x,y) < 20 ){
            is_dragging = true;
            id_selected_corner = i;
        }
    }
    
    p_selected.clear();
    for(int i = 0; i < 4; i++){
        p_selected.addVertex(selectedCorners[i].x, selectedCorners[i].y);
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    if( is_dragging == true ){
        for( int i = 0; i < 4; i++ ){
            setCornerVertex(id_selected_corner, x,y);
        }
    }

    
    is_dragging = false;
    p_selected.clear();
    for(int i = 0; i < 4; i++){
        p_selected.addVertex(selectedCorners[i].x, selectedCorners[i].y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit()
{
}

