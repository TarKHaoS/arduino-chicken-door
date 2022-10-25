// Déclaration des variables et des constantes
int PhotoR = A0;  // entrée analogique de la photorésistance
int FinCHaut = 9; // entrée du fin de course HAUT. Une broche sur la masse l'autre sur l'arduino
int FinCBas = 12; // entrée du fin de course BAS. Une broche sur la masse l'autre sur l'arduino
int D2 = 2;       // montée
int D3 = 3;       // descente

//variable pour stocker la valeur lue après conversion
int PhotoRLue;

void setup()
{
// démarrage la liaison série entre entrée analogique et ordi
Serial.begin(115200);
//Déclaration des contacts fin de course en entrée avec utilisation de la fonction PULLUP interne
pinMode(FinCHaut, INPUT_PULLUP); //Les Pull Up sont des résistances internes à l'arduino.
//Donc de base lorsque le boutton n'est pas appuyé on lit un état haut (5V = niveau logique 1)
pinMode(FinCBas, INPUT_PULLUP);
}

void loop()
{
    PhotoRLue = analogRead(PhotoR);
    //Valeur comprise entre 0 et 1024
    //on affiche la valeur lue sur la liaison série
    Serial.print("PhotoRLue = ");
    Serial.print(PhotoRLue);
    //on saute une ligne entre deux affichages
    Serial.println();

    if (PhotoRLue > 800 & digitalRead(FinCBas) == 1)         //Condition : Detection de la nuit et la porte est ouverte
    //Alors on ferme la porte
    {
      while(digitalRead(FinCBas) == 1){  //Tant que la porte n'est pas fermé, le moteur tourne   
          //Fermeture
        digitalWrite(D2, LOW);
        digitalWrite(D3, HIGH);
      
        // Sécurité : si le contacteur haut est atteint (probleme!)
        if(digitalRead(FinCHaut) == 1) {
          break;
        }
      }
      digitalWrite(D3, LOW);    //On arrete le moteur car le contact fin de course est activé
    }   
    if (PhotoRLue < 800 && digitalRead(FinCHaut) == 1)         //Condition : Detection du jour et la porte est fermée FinCHaut = 1 signifie le boutton fin de course haut n'est pas appuyé
    {
            while(digitalRead(FinCHaut) == 1){  //Tant que la porte n'est pas ouverte, le moteur tourne   
          //Fermeture
        digitalWrite(D2, HIGH);
        digitalWrite(D3, LOW);
      }
    digitalWrite(D2, LOW);
    }
}
