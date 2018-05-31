#include "botoBase.h"

botoBase::botoBase() {

}

botoBase::~botoBase() {

}

void botoBase::setup(int _x, int _y, int _radius){
    // BOTÓ
    bdrawInfoHelp = false;
    botoSeleccionat = false;
    velocitatCarrega = 5;
    botoRadi = _radius;
    botoX = _x;
    botoY = _y;

    // CÀRREGA
    amtCarrega360 = 0;
}

void botoBase::init(){
    amtCarrega360 = 0;
    botoSeleccionat = false;
}

void botoBase::updatem(ofVec2f pos){
    // MOUSE
    actualitzaAmt(pos.x, pos.y);
}

void botoBase::update(int total, ofVec2f pos[MAX_NUM_BLOBS]){
    // DETECCIO
    for(int i=0; i<total; i++){
        actualitzaAmt(pos[i].x, pos[i].y);
    }
}

void botoBase::draw(){
    if(bdrawInfoHelp){
        drawHelp();
    }
}

void botoBase::drawHelp(){
    ofPushStyle();
    ofSetColor(255,255,0);
    // radi deteccio boto
    ofNoFill();
    ofCircle(botoX, botoY, botoRadi);
    // % carrega
    ofDrawBitmapString(ofToString((int)(100.0*amtCarrega360/360.0)) + "%", botoX,botoY+botoRadi+40);
    ofPopStyle();
}

void botoBase::actualitzaAmt(float _x, float _y){
    if(isInside(_x, _y)){
        amtCarrega360 += velocitatCarrega;
        if(amtCarrega360 >= 360){
            amtCarrega360 = 0;
            botoSeleccionat = true;
        }
    }
}

bool botoBase::isInside(float _x, float _y ){
    return (ofVec2f(_x, _y).distance(ofVec2f(botoX, botoY)) < botoRadi);
}

void botoBase::showDrawInfoHelp(){
    bdrawInfoHelp = true;
}

void botoBase::hideDrawInfoHelp(){
    bdrawInfoHelp = false;
}

void botoBase::toggleDrawInfoHelp(){
    bdrawInfoHelp = !bdrawInfoHelp;
}
