#pragma once

#ifndef _PECA_EMPTY
#define _PECA_EMPTY

#include "ofMain.h"

#include "pecaBase.h"

class pecaEmpty : public pecaBase{
public:

    static ofEvent<int> actualitzaPunts;
    //static ofEvent<int> actualitzaPunts_tut1;


    virtual void setup(int _id, int _numgrid, ofVec2f _pos, int _radi, float _distT);
    void init(int id, int numgrid, ofVec2f pos);
    virtual void update(int _total, ofVec2f _pos[MAX_NUM_BLOBS]);
    virtual void draw();

    int alfaPeca;
    float distanceInTime;

protected:

};

#endif
