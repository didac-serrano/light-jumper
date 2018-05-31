#include "botoDonut.h"

botoDonut::botoDonut() {

}

botoDonut::~botoDonut() {

}

void botoDonut::setup(int _x, int _y, int _radius, ofColor c){
    botoBase::setup(_x,_y,_radius);
    // DONUT
    // ring: https://forum.openframeworks.cc/t/draw-an-arc-of-a-ring/12791/3
    curvaCarrega.close();
	curvaCarrega.setArcResolution(80);
	curvaCarrega.setFilled(true);
	// saltingOrange = ofColor(252, 129, 0);
	curvaCarrega.setColor(c);
}

void botoDonut::draw(){
    ofPushStyle();
    // boto
    ofSetColor(255,200);
    ofFill();
    ofCircle(botoX, botoY, botoRadi-10);
    // càrrega
    curvaCarrega.arc(botoX,botoY, botoRadi+16,botoRadi+16, -90,amtCarrega360+1-90);
	curvaCarrega.arcNegative(botoX,botoY, botoRadi+2,botoRadi+2, amtCarrega360+1-90,-90);
	curvaCarrega.draw();
	curvaCarrega.clear();

    botoBase::draw();
}
