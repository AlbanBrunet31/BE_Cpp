#include <string>
using namespace std;

const int readTimeMax = 4000;    //Durée max pour la lecture de vitesse (ms) : au-dela, vitesse considerée nulle
const float angleEntrePale = 120.0; //Angle entre les pales de l'helice (deg), 180.0 si helice avec deux pales


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
    encoderRotatif(int CLK,int DT, string n):capteur(CLK,n){
        angle  = 0;
        clkState  = LOW;
        clkLast  = HIGH;
        pinCLK=CLK;
        pinDT=DT;
    }

    void readRotary() {
        // gestion position
        clkState = digitalRead(pinCLK);
        if ((clkLast == LOW) && (clkState == HIGH)) {//rotation de l'encoder : front montant
            if (digitalRead(pinDT) == HIGH) {
                angle=angle-18;
                if ( angle < 0 ) {
                    angle = angle+360;
                }
            }
            else {
                angle=angle+18;
                if (angle >= 360) {
                    angle = angle-360;;
                }
            }
        }
        clkLast = clkState;

    }

    int getAngle(){
        return angle;
    }


protected:
    int angle;
    bool clkState;
    bool clkLast;
    int pinCLK;
    int pinDT;
    
};

class anemometre : public capteur{
public:
    anemometre(const int p, string n):capteur(p,n){
        for (int i=0;i<2;i++){
            timeHigh[i]=0;
        }
        vitesse=0.0;
    }

    /*
    !Peut durer readTimeMax secondes
    *@brief Calcule la vitesse grâce au temps écoulé entre deux fronts montants (=deux passages de l'aiment)
    *@return float Vitesse (rpm), [0rpm , 300rpm]
    */
    float setVitesse(){    //Renvoi la vitesse de rotation de la girouette

        unsigned long startMillis = millis();
        unsigned long readTime = 0;
        int i=0;
        int etatCapteur = 0;
        int d =100;

        if (-vitesse+250>10){   //Calcul du delai d'attente entre deux fronts (fonction lineaire, dependant de la vitesse)
            d=-vitesse+250;
        }else{
            d=10;
        }

        while ((i<=1)&&(readTime<readTimeMax)){ 
            etatCapteur=digitalRead(pin);
            if (etatCapteur==0){ //Detection état "0" -> Coupure du signal IR
                timeHigh[i]=millis();
                delay(d); //Pour eviter de detecter plusieurs fois un meme front : ce delai depend de la vitesse de rotation
                i++;
            }
            readTime=millis()-startMillis; 
            delay(1);           
        }

        if (readTime>=readTimeMax){ //Duree maximale atteinte : Les pales ne tournent pas (ou pas assez vite)
            vitesse=0.0;
        }else{  
            float dt = (timeHigh[1]-timeHigh[0])/1000.0;    //dt en s          
            vitesse=60*(angleEntrePale)/(float)(360*dt);  //vitesse en rpm
        }
        
        return vitesse;
        
    }

    float readVitesse(){
        return vitesse;
    }

    /*
        *@brief Temps de mesure passé en argument
        *@return float : Vitesse moyenne mesurée (rpm)
    */
    float velAverage(unsigned long time){
        unsigned long startTime = millis();
        vector <float> v;


        while((millis()-startTime)<time){
            v.push_back(setVitesse());
        }

        //Vitesse moyenne
        //float velaverage = accumulate( v.begin(), v.end(), 0 )/ v.size();

        //Vitesse mediane : permet de diminuer l'impact des valeurs tres eloignees
        float velaverage=0.0;
        sort(v.begin(),v.end());
 
        int n = v.size();
    
        if(n%2 == 0)
        {
            int i = v.size()/2;
            velaverage = (v[i-1] + v[i])/2;
        }
    
        else{
            velaverage = v[v.size()/2];
        }

        return velaverage;
    }

protected:
    unsigned long timeHigh[2];
    float vitesse;
};