#include <string>
using namespace std;

const int readTimeMax = 4000;    //Durée max pour la lecture de vitesse (ms) : au-dela, vitesse considerée nulle
const float angleEntreAiments = 180.0; //Angle entre les aiments (deg), 360.0 si un seul aiment


class capteur{
public:
    capteur(){
        pin=0;
        nom="SansNom";
    }

    capteur(const int p, string n){
        pin=p;
        nom=n;
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

    int getPin(){
        return pin;
    }

protected:
    int pin;
    string nom;
};


class encoderRotatif : public capteur{
public:
    encoderRotatif(){

    }

protected:

};

class anemometre : public capteur{
public:
    anemometre(const int p, string n):capteur(p,n){
        for (int i=0;i<2;i++){
            timeHigh[i]=0;
        }
        vitesse=12.0;
    }

    /*
    !Peut durer readTimeMax secondes
    *@brief Calcule la vitesse grâce au temps écoulé entre deux fronts montants (=deux passages de l'aiment)
    *@return float Vitesse (tr/min)
    */
    float setVitesse(){    //Renvoi la vitesse de rotation de la girouette

        unsigned long startMillis = millis();
        unsigned long readTime = 0;
        int i=0;
        int etatPrecedentCapteur = 0;
        int etatCapteur = 0;

        while ((i<=1)&&(readTime<readTimeMax)){ 
            etatCapteur=digitalRead(pin);
            if (etatCapteur==1 && etatPrecedentCapteur==0){ //Detection front montant
                Serial.println("Front montant");
                timeHigh[i]=millis();
                i++;
            }
            etatPrecedentCapteur=etatCapteur;
            readTime=millis()-startMillis; 
            delay(1);           
        }

        if (readTime>=readTimeMax){ //Duree maximale atteinte
            Serial.println("readTime out");
            vitesse=0.0;
        }else{  
            float dt = (timeHigh[1]-timeHigh[0])/1000.0;    //dt en s
            Serial.print("dt (s) = ");
            Serial.println(dt);           
            vitesse=60*(angleEntreAiments)/(float)(360*dt); //vitesse en tr/min
        }
        
        return vitesse;
        
    }

    float readVitesse(){
        return vitesse;
    }

protected:
    unsigned long timeHigh[2];//+1
    float vitesse;
};