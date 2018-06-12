#ifndef _SALTING_SINGLETON
#define _SALTING_SINGLETON

#include <iostream>

using namespace std;

class Singleton{
private:
    static bool instanceFlag;
    static Singleton *single;

    Singleton(){
        //private constructor
    }

    int puntuacioJugador;

public:
    static Singleton* getInstance();
    void method();
    ~Singleton(){
        instanceFlag = false;
    }
    int getPuntuacioJugador(){ return puntuacioJugador; };
    void setPuntuacioJugador(int p){ puntuacioJugador = p; };

};
#endif
