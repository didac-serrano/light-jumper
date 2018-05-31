#pragma once

#ifndef _BOTO_DONUT
#define _BOTO_DONUT

#include "botoBase.h"

class botoDonut : public botoBase {
public:

    botoDonut();
    ~botoDonut();

    virtual void setup(int _x, int _y, int _radius, ofColor c);
    virtual void draw();

    // CARREGA CIRCULAR
    ofPath curvaCarrega;

protected:

};

#endif
