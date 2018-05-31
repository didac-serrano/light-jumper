#include "pecaEmpty.h"

// the static event, or any static variable, must be initialized outside of the class definition.
ofEvent<int> pecaEmpty::actualitzaPunts = ofEvent<int>();

void pecaEmpty::setup(int _id, int _numgrid, ofVec2f _pos, int _radi, float _distT){
    pecaBase::setup(_id, _numgrid, _pos, _radi);
    puntuacioPeca = 1;

    // ESTAT
    estatPeca = SETUP;
    estatPecaNext = SETUP;
    bdrawInfoHelp = false;
    alfaPeca = 0;
    distanceInTime = _distT;

}

void pecaEmpty::init(int id, int numgrid, ofVec2f pos){
    pecaId = id;
    pecaNumGraella = numgrid;
    pecaPos = pos;
}

void pecaEmpty::update(int _total, ofVec2f _pos[MAX_NUM_BLOBS]){
    pecaBase::update(_total, _pos);

    // ESTATS PEÇA I RAIG
    if(estatPeca == CANVIA_ESTAT){
        estatPeca = estatPecaNext;
        if(estatPecaNext == SETUP){
        }
        else if(estatPecaNext == APAREIX){
            alfaPeca = 0;
        }
        else if(estatPecaNext == ACTIVA){
        }
        else if(estatPecaNext == IDLE){
        }
        else if(estatPecaNext == TOCADA){
            ofNotifyEvent(actualitzaPunts, puntuacioPeca);
            alfaPeca = 255;
        }
        else if(estatPecaNext == DESAPAREIX){
        }
    }
    else if(estatPeca == SETUP){

    }
    else if(estatPeca == APAREIX){
        alfaPeca +=3;
        if(alfaPeca >= 255){
            estatPeca = CANVIA_ESTAT;
            estatPecaNext = ACTIVA;
        }
    }
    else if(estatPeca == ACTIVA){
        estatPeca = CANVIA_ESTAT;
        estatPecaNext = IDLE;
    }
    else if(estatPeca == IDLE){
        idleTimer-=1;
        if(idleTimer<=0) {
            estatPeca = CANVIA_ESTAT;
            estatPecaNext = DESAPAREIX;
        }
    }
    else if(estatPeca == TOCADA){
        tocadaTimer-=1;
        if(tocadaTimer<=0) {
            estatPeca = CANVIA_ESTAT;
            estatPecaNext = DESAPAREIX;
        }
    }
    else if(estatPeca == DESAPAREIX){
        alfaPeca -=5;
        if(alfaPeca <= 0){
            estatPeca = CANVIA_ESTAT;
            estatPecaNext = THE_END;
        }
    }
    else if(estatPeca == THE_END){
        estatPeca = CANVIA_ESTAT;
        estatPecaNext = SETUP;
    }

}

void pecaEmpty::draw(){
    // PEÇA
    if(estatPeca == SETUP){

    }
    else if(estatPeca == APAREIX){
        ofPushStyle();
        ofSetColor(255,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio);
        ofSetColor(50,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio/3);
        ofPopStyle();
    }
    else if(estatPeca == ACTIVA){
        ofPushStyle();
        ofSetColor(255,255,0,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio);
        ofSetColor(50,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio/3);
        ofSetColor(255,255,0,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio/5);
        ofPopStyle();
    }
    else if(estatPeca == IDLE){
        ofPushStyle();
        ofSetColor(255,255*(idleTimer/IDLE_TIMER_PECA),0,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio);
        ofSetColor(50,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio/3);
        ofSetColor(255,255*(idleTimer/IDLE_TIMER_PECA),0,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio/5);
        ofPopStyle();
    }
    else if(estatPeca == TOCADA){
        ofPushStyle();
        ofSetColor(172,198,0,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio);
        ofSetColor(255,0,0,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio/3);
        ofSetColor(172,198,0,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio/5);
        ofPopStyle();
    }
    else if(estatPeca == DESAPAREIX){
        ofPushStyle();
        ofSetColor(255,alfaPeca);
        ofCircle(pecaPos.x, pecaPos.y, pecaRadiDeteccio);
        ofPopStyle();
    }

    pecaBase::draw();
}
