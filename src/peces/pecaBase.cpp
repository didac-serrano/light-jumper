#include "pecaBase.h"

pecaBase::pecaBase(){

}

pecaBase::~pecaBase(){

}

void pecaBase::setup(int id, int numgrid, ofVec2f pos, int radi){
    // PEÇA
    pecaId = id;
    pecaNumGraella = numgrid;
    pecaPos = pos;

    bpecaActiva = true;
    pecaRadiDeteccio = radi;

    puntuacioPeca = 0;
}

void pecaBase::update(int total, ofVec2f pos[MAX_NUM_BLOBS]){
    if(bpecaActiva){
        // DETECCIO
        for(int i=0; i<total; i++){
            if(estatPeca == IDLE){
                if(isInside(pos[i].x, pos[i].y)){
                    estatPeca = CANVIA_ESTAT;
                    estatPecaNext = TOCADA;
                }
            }
        }
    }
}

void pecaBase::updatem(ofVec2f pos){
    // MOUSE
    if(bpecaActiva){
        if(estatPeca == IDLE){
            if(isInside(pos.x, pos.y)){
                estatPeca = CANVIA_ESTAT;
                estatPecaNext = TOCADA;
            }
        }
    }
}

void pecaBase::draw(){
    // HELP
    if(bdrawInfoHelp){
        ofPushStyle();
        ofSetColor(255,255,0);
        // radi deteccio peça
        ofNoFill();
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio);
        ofDrawBitmapString(estatPecaToString(), pecaPos.x, pecaPos.y-10);
        ofDrawBitmapString(ofToString(pecaId), pecaPos.x, pecaPos.y);
        ofPopStyle();
    }

}

bool pecaBase::isInside(float x, float y){
    // RECT
    //return x >= pecaX-pecaWMeitat && x < pecaX+pecaWMeitat && y >= pecaY-pecaHMeitat && y < pecaY+pecaHMeitat ? true : false;
    // CERCLE
    return (pecaPos.distance(ofVec2f(x, y)) < pecaRadiDeteccio);
}

string pecaBase::estatPecaToString(){
    if(estatPeca == SETUP){
        return "SETUP";
    }
    else if(estatPeca == APAREIX){
        return "APAREIX";
    }
    else if(estatPeca == ACTIVA){
        return "ACTIVA";
    }
    else if(estatPeca == IDLE){
        return "IDLE";
    }
    else if(estatPeca == TOCADA){
        return "TOCADA";
    }
    else if(estatPeca == DESAPAREIX){
        return "DESAPAREIX";
    }
    else if(estatPeca == CANVIA_ESTAT){
        return "CANVIA_ESTAT";
    }
    else if(estatPeca == THE_END){
        return "THE_END";
    }
}

void pecaBase::toggleDrawInfoHelp(){
    bdrawInfoHelp = !bdrawInfoHelp;
}

void pecaBase::showDrawInfoHelp(){
    bdrawInfoHelp = true;
}

void pecaBase::hideDrawInfoHelp(){
    bdrawInfoHelp = false;
}
