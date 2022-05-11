#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <vector>
#include <bits/stdc++.h>

//Librairies perso
#include <capteur.h>
#include <pump.h>
#include <lcd.h>


using namespace std;

//IR (Rx et Tx)
const int pinIRreceiver = D3;   
const int pinIRtrans = D5;  

//Encodeur
const int pinCLK = D6;
const int pinDT = D7;

//Pompe
const int pinPump = D8;

//Ecran LCD
const int pinSDA = D2;
const int pinSCL = D1;
#define WIRE Wire
rgb_lcd lcd;


//Gestion de la Pompe
#define timeAverageVelocity 1000 //[ms]
float velocityThreshold = 80;

float velocity = 0.0;
int position = 0;

//Creation des objets
anemometre sensorIR(pinIRreceiver,"IRGirouette");       
encoderRotatif sensorRotation(pinCLK,pinDT,"EncoderGirouette");
pump waterPump(pinPump,"Pompe");



int gestionPompe(float v){      //Fonction qui gere l'etat de la pompe (ON/OFF) en fonction de la vitesse de rotation de l'helice

    if ((v>velocityThreshold) && (waterPump.getState()==false)){      //Vel > VelocityThreshold -> Pump ON
        uptadeLcd(lcd,v,true);
        return waterPump.setON();
        
    }else if ((v<=velocityThreshold) && waterPump.getState()==true){      //Vel < VelocityThreshold -> Pump OFF
        uptadeLcd(lcd,v,false);
        return waterPump.setOFF();
    }else{
        return -1;
    }
}

ICACHE_RAM_ATTR void isr(){     //Interruption declenchee a chaque mouvement de l'encodeur rotatif : calcul et affiche l'angle
    sensorRotation.readRotary();
    updateLcd(lcd,sensorRotation.getAngle(),velocity);
}

void setup()
{
    //IR
    pinMode(pinIRreceiver,INPUT);
    pinMode(pinIRtrans,OUTPUT);

    //Encoder
    pinMode(pinCLK,INPUT);
    attachInterrupt(digitalPinToInterrupt(pinCLK),isr,CHANGE);  //Mise en place de l'interruption en cas de mouvement de l'encodeur
    pinMode(pinDT,INPUT);

    //Water Pump
    pinMode(pinPump,OUTPUT);

    //LCD
    pinMode(pinSDA, OUTPUT);
    pinMode(pinSCL, OUTPUT);

    digitalWrite(pinIRtrans,HIGH);
    digitalWrite(pinPump,LOW);

    lcd=initLcd();  //Fonction definie dans lcd.h
}

 
void loop()
{
    velocity = sensorIR.velAverage(timeAverageVelocity);    //Calcul de la vitesse moyenne

    position= sensorRotation.getAngle();    //Recuperation de l'angle

    updateLcd(lcd,position,velocity);  //Affichage de la position et de la vitesse

    gestionPompe(velocity);     //Gestion de la pompe (ON/OFF)

}