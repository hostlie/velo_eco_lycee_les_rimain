/*
Code réalisé par Axel Trac:
    -utilisation d'un Arduino Mega 2560
    -écran LCD 16/2
*/


#include <LiquidCrystal.h>   // import de la librairie de l'écran LCD
#include <EEPROM.h>   // import de la librairie du stockege de la variable best

// initialisation, on définit les ports pour RS, E et D4 à D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int vitesse = 0;
int temps_de_charge = 0;
int sensorPin = A0;    // broche input du capteur de vitesse
int sensorValue = 0;
int best_lue = 0;
unsigned long MS;  // variable pour le chronomètre
unsigned long start;  // variable pour le chronomètre

int Vitesse(int sensorValue)   // Déclaration d'une fonction du calcule de vitesse du vélo
{

}

int Best(int sensorValue)   // Déclaration d'une fonction de la mise à jour du meilleur score
{
  EEPROM.put(0, sensorValue);
}


int Lcd_velo(int vitesse, int temps_de_charge)  // Déclaration d'une fonction de l'écran LCD
{

  while (true)
  {
    
    if (vitesse == 0)
    {
        lcd.print(vitesse);
        lcd.print(" km/h ");
        lcd.print("WR ");  
        lcd.print("0");
        lcd.setCursor(0, 1);// met le curseur de l'écran à la 1ère colonne et 2ème ligne 
        lcd.print("Veuillez pedaler");  
    }
    else
    {
        lcd.setCursor(0, 1);// met le curseur de l'écran à la 1ère colonne et 2ème ligne 
        lcd.print("TC : ");
        lcd.print(temps_de_charge);
        lcd.print(" min            ");
    }
  }
}


void setup() 
{
  Serial.begin(9600);  // initialisation du Serial 
  lcd.begin(16, 2);    // initialisation de l'écran LCD
  start=millis();
  EEPROM.get(0, best_lue);
  Serial.print("World Record = ");
  Serial.println(best_lue);
}

void loop() 
{
  Lcd_velo(vitesse, temps_de_charge);
  MS = millis()-start;  // calcule du temps (chronomètre)

  Serial.print ((MS / 1000) / 60);  // minutes
  Serial.print ("' ");
  Serial.println ((MS / 1000) % 60);  // secondes

  if (sensorValue > best_lue)
  {
    Best(sensorValue);
  }
}
