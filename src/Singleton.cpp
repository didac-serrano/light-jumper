#include <stdio.h>
#include "Singleton.h"

bool Singleton::instanceFlag;
Singleton* Singleton::single = NULL;

Singleton* Singleton::getInstance()
{
    if(! instanceFlag){
        single = new Singleton();
        instanceFlag = true;
        return single;
    }
    else{
        return single;
    }
}

void Singleton::method(){
    // method of the singleton class
}
