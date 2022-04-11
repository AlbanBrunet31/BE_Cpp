#include <Arduino.h>
#include <math.h>
#include <capteur.h>
#include <string>
using namespace std;

const int pinHallSensor = D1;   
anemometre sensorHall(pinHallSensor,"HallGirouette");
 
void setup()
{
    pinMode(pinHallSensor,INPUT);
    Serial.begin(115200);
    
   
}
 
void loop()
{
    delay(500);
    sensorHall.setVitesse();
    Serial.print("Vitesse : ");
    Serial.println(sensorHall.readVitesse());
}