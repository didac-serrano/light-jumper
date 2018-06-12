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
    int puntuacioJugador_tut1;

public:
    static Singleton* getInstance();
    void method();
    ~Singleton(){
        instanceFlag = false;
    }
    int getPuntuacioJugador(){ return puntuacioJugador; };
    void setPuntuacioJugador(int p){ puntuacioJugador = p; };
    int getPuntuacioJugador_tut1(){ return puntuacioJugador_tut1; };
    void setPuntuacioJugador_tut1(int p){ puntuacioJugador_tut1 = p; };

};
#endif
