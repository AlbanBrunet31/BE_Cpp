#ifndef TEST_LCD_H
#define TEST_LCD_H


#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <Wire.h>

#include "rgb_lcd.h"

char strPos[3];
char txtPos[] = "Pos:";
char strVit[6];
char txtVit[] = " Vit:";

rgb_lcd initLcd(){
    rgb_lcd lcd = rgb_lcd();
    lcd.begin(16,2);
    lcd.setRGB(102, 204, 255);  //Changement de la couleur
    lcd.noBlink();    
    lcd.noCursor();   //Curseur invisible
    lcd.noBlinkLED(); //Arret du clignotement
    lcd.leftToRight();
    lcd.display();    //Debut de l'affichage
    lcd.clear();    //Efface et place le curseur en (0,0)
    return lcd;
}

void updateLcd(rgb_lcd lcd, int pos, float vit){    //Mise a jour de l'affichage position/vitesse
    lcd.clear();
    lcd.home();

    //On convertit les valeurs en string 
    sprintf(strPos, "%d", pos);    //int to string
    sprintf(strVit, "%d", (int)vit);    //float to string
    //On affiche la position
    lcd.print(txtPos);
    lcd.print(strPos);
    lcd.setCursor(8,0);
    //On affiche la vitesse
    lcd.print(txtVit);
    lcd.print(strVit);
}

void uptadeLcd(rgb_lcd lcd, float vit, bool pump){ //Mise a jour de l'affichage : changement d'etat de la pompe (ON/OFF)
    lcd.clear();
    lcd.home();
    lcd.print("Pump ");
    if (pump==true){
        lcd.setRGB(250, 100, 100);
        lcd.print("ON ");
    }else if (pump==false){
        lcd.setRGB(102, 204, 255);
        lcd.print("OFF ");
    }
    sprintf(strVit, "%d", (int)vit);    //float to string
    lcd.print("Vit:");
    lcd.print(strVit);
}

#endif