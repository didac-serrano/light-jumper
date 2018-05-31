#pragma once

#ifndef _PECA_BASE
#define _PECA_BASE

#include "ofMain.h"
#include "LIGHT_JUMPER_DEFINES.h"

enum estatsPeca{
    CANVIA_ESTAT,
    SETUP,
    APAREIX,
    ACTIVA,
    IDLE,
    TOCADA,
    DESAPAREIX,
    THE_END,
};

class pecaBase {
public:

    pecaBase();
    ~pecaBase();

    // APP
    void setup(int id, int numgrid, ofVec2f pos, int radi);
    void update(int total, ofVec2f pos[MAX_NUM_BLOBS]);
    void updatem(ofVec2f pos);
    void draw();

    bool isInside(float x, float y);

    // PEÇA
    int pecaId;
    int pecaNumGraella;
    float idleTimer;
    float tocadaTimer;
    ofVec2f pecaPos;

    bool bpecaActiva;
    int pecaRadiDeteccio;
    int puntuacioPeca;

    int pecaWidth, pecaHeight;
    float pecaWMeitat, pecaHMeitat;

    // ESTATS
    estatsPeca estatPeca;
    estatsPeca estatPecaNext;
    string estatPecaToString();

    // INFO HELP
    bool bdrawInfoHelp;
    void toggleDrawInfoHelp();
    void showDrawInfoHelp();
    void hideDrawInfoHelp();

protected:

};

#endif
