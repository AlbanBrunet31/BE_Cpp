#include <string>
#include <Arduino.h>

using namespace std;

class pump {

public: 
     pump(){
        pin=0;
        nom="SansNom";
        state=false;
    }

    pump(const int p, string n){
        pin=p;
        nom=n;
        state=false;
    }

    int setON(){
        digitalWrite(pin,HIGH);
        if (digitalRead(pin)==HIGH){
            state=true;
            return 0;
        }else{
            return -1;
        }
    }

    int setOFF(){
        digitalWrite(pin,LOW);
        if (digitalRead(pin)==LOW){
            state=false;
            return 0;
        }else{
            return -1;
        }
    }

    string getNom(){
        return nom;
    }

    int setNom(string n){
        nom=n;
        return 0;
    }

    int setPin(int p){
        pin=p;
        return 0;
    }

    bool getState(){
        return state;
    }


protected:
    int pin;
    string nom;
    bool state;
};