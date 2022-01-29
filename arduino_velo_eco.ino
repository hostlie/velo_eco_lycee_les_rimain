/*
  Code réalisé par Axel Trac:
    -utilisation d'un Arduino Mega 2560
    -écran LCD 16/2
*/


#include <LiquidCrystal.h>   // import de la librairie de l'écran LCD
#include <EEPROM.h>   // import de la librairie du stockage de la variable best

// initialisation, on définit les ports pour RS, E et D4 à D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int vitesse = 200;
int WR_lue;
int valeur_calcule_lue;
//int WR = 30; // pour remettre le WR
int valeur_calcul_points = 0,069; // pour remettre le WR
int temps_de_charge = 0;
int sensorPin = A0;    // broche input du capteur de vitesse
int best_lue = 0;
int analogPin = A3; // potentiometer wiper (middle terminal) connected to analog pin 3
unsigned long MS;  // variable pour le chronomètre
unsigned long start;  // variable pour le chronomètre
int vitesse_pedalage_minimal = 3; // variable pour savoir la vitesse minimal du vélo pour recharger les téléphones
int chrono_on = 0;
int points = 0;
float millisec = 0;
int sec_par_sec;
int sec_maintenant;
int lcd_new_record = 0;

// Déclaration des fonctions



void setup()
{
  Serial.begin(9600);  // initialisation du Serial
  lcd.begin(16, 2);    // initialisation de l'écran LCD
  //EEPROM.put(0, WR);
  EEPROM.put(1, valeur_calcul_points);
}

void loop()
{

  // Code pour la récupération de la vitesse du vélo
  vitesse = analogRead(analogPin);
  vitesse = vitesse - 200;  // PROVISOIRE
  Serial.print("vitesse : ");
  Serial.print(vitesse);
  if (vitesse > vitesse_pedalage_minimal)
  {
    Serial.print(" km/h");
  }
  else
  {
    Serial.println(" km/h");
  }
  

  // Code d'affichage du LCD
  if (vitesse < 0)
  {
    lcd.clear();
    WR_lue = EEPROM.get(0, WR_lue);
    lcd.setCursor(0, 0);  // met le curseur de l'écran à la 1ère colonne et 1ère ligne
    lcd.print("0 km/h");
    lcd.setCursor(9, 0);  // met le curseur de l'écran à la 9ème colonne et 1ère ligne
    lcd.print("WR");
    lcd.setCursor(12, 0);  // met le curseur de l'écran à la 12ème colonne et 1ère ligne
    lcd.print(WR_lue);
    lcd.setCursor(0, 1);  // met le curseur de l'écran à la 1ère colonne et 2ème ligne
    lcd.print("Veuillez pedaler");
  }
  else
  {
    lcd.clear();
    WR_lue = EEPROM.get(0, WR_lue);
    lcd.setCursor(0, 0);// met le curseur de l'écran à la 1ère colonne et 2ème ligne
    lcd.print(vitesse);
    lcd.print(" km/h");
    lcd.setCursor(9, 0);  // met le curseur de l'écran à la 12ème colonne et 1ère ligne
    lcd.print("WR");
    lcd.setCursor(12, 0);  // met le curseur de l'écran à la 12ème colonne et 1ère ligne
    lcd.print(WR_lue);
    lcd.setCursor(0, 1);  // met le curseur de l'écran à la 1ère colonne et 2ème ligne
    lcd.print("TC ");
    lcd.setCursor(3, 1);  // met le curseur de l'écran à la 1ère colonne et 2ème ligne
    lcd.print(temps_de_charge);
    lcd.print(" min ");
    lcd.print("PT ");
    lcd.print(points);
  }


  // Code pour le comptage des points du score
  
  if (vitesse > vitesse_pedalage_minimal)
  {  
    if (chrono_on == 0)
    {
      start = millis();  // Démarrage du chronomètre
      chrono_on = chrono_on + 1;
    }
    MS = millis() - start; // calcule du temps (chronomètre)
    Serial.print(" | temps de pédalage : ");
    Serial.print ((MS / 1000) / 60);  // minutes
    Serial.print ("' ");
    Serial.print ((MS / 1000) % 60);  // secondes
    Serial.println(" s");
    sec_maintenant = (MS / 1000) % 60;
    
    if (sec_par_sec != sec_maintenant)
    {
      Serial.println("Execution toutes les secondes"); 
      points = points + (vitesse * 0.069);
      Serial.print("points : ");
      Serial.println(points);
    }
    sec_par_sec = (MS / 1000) % 60;

    // Code de mise à jour du meilleur record
     if (points > WR_lue)
    {
      EEPROM.put(0, points);
      if (lcd_new_record == 0)
      {
        lcd.clear();
        lcd.print("   Bravo !!!   ");
        lcd.setCursor(0, 1);  // met le curseur de l'écran à la 1ère colonne et 2ème ligne
        lcd.print(" Record Battue ");
        lcd_new_record = lcd_new_record + 1;
        delay(3000);
      }
      
    }
    
  }
  else
  {
    chrono_on = 0;  // permet de relancer le chronomètre quand on arrète et redémarre le pédalage 
    points = 0;
    lcd_new_record = 0;
  }
  
}
