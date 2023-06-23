//#include <NewPing.h>

// definice pinů pro čidla
const int krajnivlevo = A3;
const int uprostredvlevo = A2;
const int uprostredvpravo = A1;
const int krajnivpravo = A0;

// promenne pro mereni vzdalenosti
int pTrig = 12;
int pEcho = 13;

//NewPing sonar(pTrig, pEcho); // Proměnná pro měření vzdálenosti

//unsigned int vzdalenost = sonar.ping_cm();

// definice pinů pro motory
int mLs = 7; //směr levý
int mLp = 5; //výkon levý
int mRs = 8; //směr pravý
int mRp = 6; //výkon pravý

// Citlivost senzorů čáry
const int prahkrajnivlevo = 400;
const int prahuprostredvlevo = 600;
const int prahuprostredvpravo = 600;
const int prahkrajnivpravo = 400;

// Konstanty PID regulátoru
const float Kp = 0.7;  // Proporční člen
//const float Ki = 0.0;  // Integrální člen
const float Kd = 0.1;  // Derivativní člen

// Požadovaná pozice
const float pozadovana_pozice = 524; //puvodní 524 pro 2 stredová čidla

// Proměnné PID regulátoru
float error_prev = 0;
//float integral = 0;

int rychlost;

void setup() {
  // sériový monitor
  Serial.begin(9600);

  // Nastavení pinů pro senzory čáry jako vstupy
  pinMode(krajnivlevo, INPUT);
  pinMode(uprostredvlevo, INPUT);
  pinMode(uprostredvpravo, INPUT);
  pinMode(krajnivpravo, INPUT);
  
  // dálkoměr
  pinMode(pTrig, OUTPUT);
  pinMode(pEcho, INPUT);
  
  
  // motory
  pinMode(mLs, OUTPUT);
  pinMode(mLp, OUTPUT);
  pinMode(mRs, OUTPUT);
  pinMode(mRp, OUTPUT);

}

void rovne() {
  digitalWrite(mLs, LOW);
  analogWrite(mLp, (1.2 * rychlost));
  digitalWrite(mRs, LOW);
  analogWrite(mRp, (1.2 * rychlost));
  }

void vlevo() {
  digitalWrite(mLs, HIGH);
  analogWrite(mLp, rychlost);
  digitalWrite(mRs, LOW);
  analogWrite(mRp, rychlost);
  }
  
void tocvlevo() {
  digitalWrite(mLs, HIGH);
  analogWrite(mLp, 0);
  digitalWrite(mRs, LOW);
  analogWrite(mRp, 255);
  }
  
void vpravo() {
  digitalWrite(mLs, LOW);
  analogWrite(mLp, rychlost);
  digitalWrite(mRs, HIGH);
  analogWrite(mRp, rychlost);
  }
  void tocvpravo() {
  digitalWrite(mLs, LOW);
  analogWrite(mLp, 200);
  digitalWrite(mRs, HIGH);
  analogWrite(mRp, 150);
  }
  
  void carakolmo() {
  stop();
  delay(150);
  digitalWrite(mLs, LOW);
  analogWrite(mLp, 0);
  digitalWrite(mRs, LOW);
  analogWrite(mRp, 200);
  delay(300);
  stop();
  delay(150);
  }
  
void stop() {
  digitalWrite(mLs, LOW);
  analogWrite(mLp, 0);
  digitalWrite(mRs, LOW);
  analogWrite(mRp, 0);
  }
  

void objet() {
  stop();
  delay(50);
  tocvlevo();
  delay(360);
  //jet();
  
  //delay(2000);
  //stop();
 //delay(50);
 //tocvpravo();
 // delay(500);
 // jet();
 // delay(2000);
 // stop();
// delay(50);
 // tocvpravo();
 // delay(500);
 // jet();
 // delay(1000);
//  stop();

}

void jet() {
  digitalWrite(mLs, LOW);
  analogWrite(mLp, 150);
  digitalWrite(mRs, LOW);
  analogWrite(mRp, 150);  
}
void back() {
  digitalWrite(mLs, HIGH);
  analogWrite(mLp, 100);
  digitalWrite(mRs, HIGH);
  analogWrite(mRp, 100); 
}


bool pojizdeni = false; // Proměnná pro sledování, zda byla část kódu provedena


void loop() {
  
  int sensor1Val = analogRead(krajnivlevo);
  int sensor2Val = analogRead(uprostredvlevo);
  int sensor3Val = analogRead(uprostredvpravo);
  int sensor4Val = analogRead(krajnivpravo);
    
	// Porovnání hodnot s prahovou hodnotou
  bool onLine1 = sensor1Val > prahkrajnivlevo;
  bool onLine2 = sensor2Val > prahuprostredvlevo;
  bool onLine3 = sensor3Val > prahuprostredvpravo;
  bool onLine4 = sensor4Val > prahkrajnivpravo;
  
  // Výpočet průměrné pozice z čidel
  float prumerna_pozice = (sensor1Val + sensor2Val + sensor3Val + sensor4Val) / 4.0;
  
   // Výpočet chyby
  float error = pozadovana_pozice - prumerna_pozice;
  
  // Výpočet PID členů
  float proporcialni = Kp * error;
  //integral += Ki * error;
  float derivacni = Kd * (error - error_prev); 
    
  // Aktualizace PID výstupu
  float vystup = proporcialni + derivacni; //+ integral;
  rychlost = constrain(vystup,200,255);

  // Uložení aktuální chyby pro další výpočet
  error_prev = error;
 
  float duration;
  float vzdalenost;

  digitalWrite(pTrig, LOW);
  delayMicroseconds(3);
  digitalWrite(pTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pTrig, LOW);
  duration = pulseIn(pEcho, HIGH);
  vzdalenost = duration * 0.034 / 2;
  
  //bool prekazka = true;
  //prekazka = (vzdalenost < 11 && vzdalenost > 10);
  
  	
    
	if (vzdalenost < 14 && vzdalenost > 10) { objet(); Serial.println("prekazka"); Serial.println(vzdalenost);}
	
	
	else if (!onLine1 && !onLine2 && !onLine3 && onLine4) {vpravo(); Serial.println("vpravo"); }
	
	else if (onLine1 && !onLine2 && !onLine3 && !onLine4) {vlevo(); Serial.println("vlevo");}
	
	else if (!onLine1 && onLine2 || onLine3 && !onLine4) {rovne(); Serial.println("rovne");}
	
	//else if (onLine1 && onLine2 && onLine3 && onLine4) {tocvlevo(); Serial.println("objetcarakolmo");}
	
	//else if (!pojizdeni) {jet(); Serial.println("pojizdeni"); pojizdeni = true; }
	
	
	delay(40);
	
   }
   
   
   
   
   
   