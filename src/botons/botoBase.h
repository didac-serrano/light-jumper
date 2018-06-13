#pragma once

#ifndef _BOTO_BASE
#define _BOTO_BASE

#include "ofMain.h"
#include "LIGHT_JUMPER_DEFINES.h"

class botoBase {
public:

    botoBase();
    ~botoBase();

    void setup(int _x, int _y, int _radius);
    void update(int total, ofVec2f pos[MAX_NUM_BLOBS]);
    void updatem(ofVec2f pos);
    void draw();
    void drawHelp();
    void init();
    void actualitzaAmt(float _x, float _y);

    bool isInside(float _x, float _y);

    // BOTÓ
    bool botoSeleccionat;
    int botoRadi, botoX, botoY;
    float speed;
    float velocitatCarrega;
    float amtCarrega360 = 0;

    // INFO HELP
    bool bdrawInfoHelp;
    void showDrawInfoHelp();
    void hideDrawInfoHelp();
    void toggleDrawInfoHelp();

protected:

};

#endif
