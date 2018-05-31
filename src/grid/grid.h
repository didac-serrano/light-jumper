#pragma once

#ifndef _GRID
#define _GRID

#include "ofMain.h"
#include "LIGHT_JUMPER_DEFINES.h"

class grid {
public:

    grid();
    ~grid();

    void setup();
    void draw();

    int numRows;
    int numCols;
    float rectW, rectH;

    bool bdrawInfoHelp;
    void toggleDrawInfoHelp(){ bdrawInfoHelp = !bdrawInfoHelp; };
    void showDrawInfoHelp(){ bdrawInfoHelp = true; };
    void hideDrawInfoHelp(){ bdrawInfoHelp = false; };

    ofVec2f posicioPeca[NUM_ROWS*NUM_COLS];
    ofVec2f returnPosicioOfPeca(int num);

    bool brectGridActiu[NUM_ROWS*NUM_COLS];
    void changeRectGridActiu(int num);
    void activaRectGridActiu(int num);
    void desactivaRectGridActiu(int num);

    void desactivaFilaSuperior();
    void desactivaFilaInferior();
    void desactivaColumnaEsquerra();
    void desactivaColumnaDreta();
    void activaFilaSuperior2();
    void activaFilaSuperior3();
    void desactivaFilaSuperior2();
    void desactivaFilaSuperior3();

    void gridTotaActiva();
    void gridTotaInactiva();

    void gridActivaExcepteMargesSupInfDretaEsq();

protected:
};

#endif
