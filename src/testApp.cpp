#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	beats.loadSound("sounds/1085.mp3");
    beats.setVolume(0.75f);
	font.loadFont("GillSansStd-LightItalic.otf", 32);
	beats.setMultiPlay(false);
    
    
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    path = new ofxSuperPath();
    path->reset();
    
    marking = NULL;
    brush = NULL;
    paint = NULL;
    
}

//--------------------------------------------------------------
void testApp::update(){

	ofBackground(140,230,200);

	// update the sound playing system:
	ofSoundUpdate();
    
    bool killSome = false;
    if(ofGetFrameRate() < 50)
    {
        killSome = true;
    }
    
    if(marking != NULL)
    {
        marking->update();
    }
    
    for ( it=markings.begin() ; it < markings.end(); it++ )
    {
        
        ofxMarking *marking = *it;
        if(killSome)
        {
            marking->killLastParticle();
            if(marking->isDead)
            {
                delete marking;
                markings.erase(it);
            }
        }
        else
        {
            marking->update();
        }
    }

}
//--------------------------------------------------------------
void testApp::exit()
{
    for(int i = 0; i < markings.size(); i++)
    {
        delete markings[i];
    }
    markings.clear();
    
    if(marking != NULL)
    {
        delete marking;
    }
}
//--------------------------------------------------------------
void testApp::draw(){

	// draw the background colors:


	//---------------------------------- beats:
	if (beats.getIsPlaying()) ofSetHexColor(0xffffff);
	else ofSetHexColor(0x000000);
	font.drawString("shimmery shimmy !", 50,50);

	ofSetHexColor(0x000000);
	string tempStr = "click and drag\npct done: "+ofToString(beats.getPosition())+"\nspeed: " +ofToString(beats.getSpeed());
	ofDrawBitmapString(tempStr, 50,ofGetHeight()-50);

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    if(marking != NULL)
    {
        marking->draw();
    }
    
    for(int i = 0; i < markings.size(); i++)
    {
        markings[i]->draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	// continuously control the speed of the beat sample via drag,
	// when in the "beat" region:
	float widthStep = ofGetWidth() / 3.0f;
	if (x >= widthStep && x < widthStep*2){
		beats.setSpeed( 0.5f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*1.0f);
	}

        path->lineTo(x,y,0, ofFloatColor(0), ofGetFrameNum(), 0);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	float widthStep = ofGetWidth() / 3.0f;
	if (x < widthStep){
		synth.play();
		synth.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*10);
		synth.setPan(ofMap(x, 0, widthStep, -1, 1, true));
	} else if (x >= widthStep && x < widthStep*2){
		beats.play();
	} else {
		vocals.play();
		vocals.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*3);
		//map x within the last third of window to -1 to 1 ( for left / right panning )
		vocals.setPan( ofMap(x, widthStep*2, widthStep*3, -1, 1, true) );
	}
    
    path = new ofxSuperPath();
    paint = new ofxPaint(path, ofColor(255), 100);
    brush = new ofxFlockingBrush(path, paint);
    brush->setDynamic(true);
    brush->setSpeed(10);
    brush->setJitter(3.5);
    brush->setRefineShape(false);
    
    
    marking = new ofxMarking(path, paint, brush);
    path->reset();
    path->lineStart(x,y,0, ofFloatColor(0), ofGetFrameNum(), 0);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    path->lineEnd(x,y,0, ofGetFrameNum(), -1, 0);
    marking->pathFinished();
    markings.push_back(marking);
    marking = NULL;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
