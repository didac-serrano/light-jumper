#pragma once

#ifndef _PECA_EMPTY
#define _PECA_EMPTY

#include "ofMain.h"

#include "pecaBase.h"

class pecaEmpty : public pecaBase{
public:

    static ofEvent<int> actualitzaPunts;

    virtual void setup(int _id, int _numgrid, ofVec2f _pos, int _radi, float _distT, float _angle, float _rot, ofVec2f _speed);
    void init(int id, int numgrid, ofVec2f pos);
    virtual void update(int _total, ofVec2f _pos[MAX_NUM_BLOBS], bool _onBeat);
    virtual void draw();

    int alfaPeca;
    float distanceInTime;
    float angle;
    float rot;
    float speedBonus;
    ofVec2f speed;

protected:

};

#endif
