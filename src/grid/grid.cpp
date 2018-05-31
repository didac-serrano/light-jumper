#include "grid.h"

grid::grid(){
}

grid::~grid(){

}

//--------------------------------------------------------------
void grid::setup(){
    bdrawInfoHelp = false;

    numRows = NUM_ROWS;
    numCols = NUM_COLS;
    rectW = APP_WIDTH/(float)numCols;
    rectH = APP_HEIGT/(float)numRows;
    for(int i=0; i<numRows; i++){
        for(int j=0; j<numCols; j++){
            int pos = i*numRows+j;
            posicioPeca[pos] = ofVec2f(j*rectW+rectW*0.5, i*rectH+rectH*0.5);
        }
    }
    gridTotaActiva();
}

//--------------------------------------------------------------
void grid::gridTotaActiva(){
    for(int i=0; i<numRows*numCols; i++){
        brectGridActiu[i] = true;
    }
}

//--------------------------------------------------------------
void grid::gridTotaInactiva(){
    for(int i=0; i<numRows*numCols; i++){
        brectGridActiu[i] = false;
    }
}

//--------------------------------------------------------------
void grid::gridActivaExcepteMargesSupInfDretaEsq(){
    gridTotaActiva();
    desactivaFilaSuperior();
    desactivaFilaInferior();
    desactivaColumnaEsquerra();
    desactivaColumnaDreta();
}

//--------------------------------------------------------------
void grid::draw(){
    if(bdrawInfoHelp){

        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                int pos = i*numRows+j;
                if(brectGridActiu[pos]){
                    ofFill();
                    ofSetColor(200,50);
                    ofRect(j*rectW, i*rectH, rectW, rectH);
                }
                ofNoFill();
                ofSetColor(200);
                ofRect(j*rectW, i*rectH, rectW, rectH);
                ofDrawBitmapString("id: " + ofToString(pos), j*rectW+6, i*rectH+36);
                //ofDrawBitmapString("at (" + ofToString((int)posicioPeca[pos].x) + "," + ofToString((int)posicioPeca[pos].y) + ")", j*rectW+6, i*rectH+56);
            }
        }
    }
}

//--------------------------------------------------------------
ofVec2f grid::returnPosicioOfPeca(int num){
    return posicioPeca[num];
}

//--------------------------------------------------------------
void grid::changeRectGridActiu(int num){
    brectGridActiu[num] = !brectGridActiu[num];
}

//--------------------------------------------------------------
void grid::activaRectGridActiu(int num){
    brectGridActiu[num] = true;
}

//--------------------------------------------------------------
void grid::desactivaRectGridActiu(int num){
    brectGridActiu[num] = false;
}

//--------------------------------------------------------------
void grid::desactivaFilaSuperior(){
    brectGridActiu[0] = false;
    brectGridActiu[1] = false;
    brectGridActiu[2] = false;
    brectGridActiu[3] = false;
    brectGridActiu[4] = false;
    brectGridActiu[5] = false;
    brectGridActiu[6] = false;
    brectGridActiu[7] = false;
    brectGridActiu[8] = false;
}

//--------------------------------------------------------------
void grid::desactivaFilaInferior(){
    brectGridActiu[72] = false;
    brectGridActiu[73] = false;
    brectGridActiu[74] = false;
    brectGridActiu[75] = false;
    brectGridActiu[76] = false;
    brectGridActiu[77] = false;
    brectGridActiu[78] = false;
    brectGridActiu[79] = false;
    brectGridActiu[80] = false;
}

//--------------------------------------------------------------
void grid::desactivaColumnaEsquerra(){
    brectGridActiu[0] = false;
    brectGridActiu[9] = false;
    brectGridActiu[18] = false;
    brectGridActiu[27] = false;
    brectGridActiu[36] = false;
    brectGridActiu[45] = false;
    brectGridActiu[54] = false;
    brectGridActiu[63] = false;
    brectGridActiu[72] = false;

}

//--------------------------------------------------------------
void grid::desactivaColumnaDreta(){
    brectGridActiu[8] = false;
    brectGridActiu[17] = false;
    brectGridActiu[26] = false;
    brectGridActiu[35] = false;
    brectGridActiu[44] = false;
    brectGridActiu[53] = false;
    brectGridActiu[62] = false;
    brectGridActiu[71] = false;
    brectGridActiu[80] = false;

}

//--------------------------------------------------------------
void grid::desactivaFilaSuperior2(){
    brectGridActiu[9] = false;
    brectGridActiu[10] = false;
    brectGridActiu[11] = false;
    brectGridActiu[12] = false;
    brectGridActiu[13] = false;
    brectGridActiu[14] = false;
    brectGridActiu[15] = false;
    brectGridActiu[16] = false;
    brectGridActiu[17] = false;
}

//--------------------------------------------------------------
void grid::desactivaFilaSuperior3(){
    brectGridActiu[18] = false;
    brectGridActiu[19] = false;
    brectGridActiu[20] = false;
    brectGridActiu[21] = false;
    brectGridActiu[22] = false;
    brectGridActiu[23] = false;
    brectGridActiu[24] = false;
    brectGridActiu[25] = false;
    brectGridActiu[26] = false;
}

//--------------------------------------------------------------
void grid::activaFilaSuperior2(){
    brectGridActiu[10] = true;
    brectGridActiu[11] = true;
    brectGridActiu[12] = true;
    brectGridActiu[13] = true;
    brectGridActiu[14] = true;
    brectGridActiu[15] = true;
    brectGridActiu[16] = true;
}

//--------------------------------------------------------------
void grid::activaFilaSuperior3(){
    brectGridActiu[19] = true;
    brectGridActiu[20] = true;
    brectGridActiu[21] = true;
    brectGridActiu[22] = true;
    brectGridActiu[23] = true;
    brectGridActiu[24] = true;
    brectGridActiu[25] = true;
}
