// Déclaration des variables et des constantes
int PhotoR = A0;  // entrée analogique de la photorésistance
int FinCHaut = 9; // entrée du fin de course HAUT. Une broche sur la masse l'autre sur l'arduino
int FinCBas = 12; // entrée du fin de course BAS. Une broche sur la masse l'autre sur l'arduino
// int D2 = 2;       // montée
// int D3 = 3;       // descente

// PWM ports for BTS7960 motor driver module
#define RPWM 10
#define LPWM 11
#define PWM1 5
#define PWM2 6

void motor_cw() {             //CLOCK WISE
  digitalWrite (LPWM, LOW);
  digitalWrite (RPWM, HIGH);
  analogWrite (PWM1, 255); // 0- 255
  analogWrite (PWM2, 255); // 0- 255
  Serial.println ("MOTOR RUNS CW");
}
void motor_ccw() {             //COUNTER CLOCK WISE
  digitalWrite (LPWM, HIGH);
  digitalWrite (RPWM, LOW);
  analogWrite (PWM1, 255);
  analogWrite (PWM2, 255);
  Serial.println ("MOTOR RUNS CCW");
}

void motor_stop() {         //motor stop
  digitalWrite (LPWM, LOW);
  digitalWrite (RPWM, LOW);
  analogWrite (PWM1, 0);
  analogWrite (PWM2, 0);
  Serial.println ("STOP");
}


//variable pour stocker la valeur lue après conversion
int PhotoRLue;

void setup()
{
  // démarrage la liaison série entre entrée analogique et ordi
  Serial.begin(115200);

  Serial.println("START");
  pinMode (RPWM, OUTPUT);
  pinMode (LPWM, OUTPUT);
  pinMode (PWM1, OUTPUT);
  pinMode (PWM2, OUTPUT);

  //Déclaration des contacts fin de course en entrée avec utilisation de la fonction PULLUP interne
  pinMode(FinCHaut, INPUT_PULLUP); //Les Pull Up sont des résistances internes à l'arduino.
  //Donc de base lorsque le boutton n'est pas appuyé on lit un état haut (5V = niveau logique 1)
  pinMode(FinCBas, INPUT_PULLUP);
}

void loop()
{
  bool motorRunning = false;
  
  PhotoRLue = analogRead(PhotoR);
  //Valeur comprise entre 0 et 1024
  //on affiche la valeur lue sur la liaison série
  Serial.print("PhotoRLue = ");
  Serial.print(PhotoRLue);
  //on saute une ligne entre deux affichages
  Serial.println();
/*
  delay(2000);
  motor_cw();

  delay(5000);
  motor_stop();

  delay(5000);
  motor_ccw();
*/

//TODO: creer fonction "fermeture" et "ouverture" 
// idéalement relais vers driver moteur ?
  
  if (PhotoRLue > 800 & digitalRead(FinCBas) == 1)         //Condition : Detection de la nuit et la porte est ouverte
  //Alors on ferme la porte
  {
    //Tant que la porte n'est pas fermée, et que le moteur ne tourne pas et que le contacteur Haut n'est pas enclanché, le moteur tourne
    //
    while(digitalRead(FinCBas) == 1 && !(digitalRead(FinCHaut) == 0 && motorRunning)){
      motorRunning = true;
      //Fermeture
      //digitalWrite(D2, LOW);
      //digitalWrite(D3, HIGH);
      motor_cw();

      // Sécurité : si le contacteur haut est atteint (probleme!)
      // WIP - Motor still rotates a bit.
      if(digitalRead(FinCHaut) == 0) {
        motor_stop();
        break;
      }
      
    }
    
    motorRunning = false;
    //digitalWrite(D3, LOW);    //On arrete le moteur car le contact fin de course est activé
    motor_stop();
  }
  
  if (PhotoRLue < 800 && digitalRead(FinCHaut) == 1)         //Condition : Detection du jour et la porte est fermée FinCHaut = 1 signifie le boutton fin de course haut n'est pas appuyé
  {
    while(digitalRead(FinCHaut) == 1){  //Tant que la porte n'est pas ouverte, le moteur tourne
      //Fermeture
      //digitalWrite(D2, HIGH);
      //digitalWrite(D3, LOW);
      motor_ccw();
    }
    //digitalWrite(D2, LOW);
    motor_stop();
  }
  
}
