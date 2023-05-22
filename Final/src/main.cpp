#include <LibRobus.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#define bonton_plus 43
#define bouton_Moins 41
//#define bouton_Gauche 42
#define bouton_Droit 39

#define nombreDeSousMenus 4

#define vitesse_Mobile 20

void miseAjour();
void  execute_menue();
void gauche_bercer(int index);
void retour_berceau(int index);
void droite_bercer(int index);
void rythme (int index);
void mobile_demarrer();
void mobile_arrete();
void bercement(int volume, int durer);
void musique();
//void mp3_random_play (); 
  //void mp3_single_loop (boolean state);	//set single loop

SoftwareSerial mySerial(19, 18); // RX, TX

unsigned long tempsAntiRebond = 0;  // dernière fois qu'un changement d'état de bouton a été détecté
 long delaiAntiRebond = 50;  // délai entre le dernier changement d'état de bouton et le début de l'action



int niveauVolume = 15;
int niveauMax = 30;

int minuteur   = 5;
int maximumMinuteur = 10;


int etatBoutonsPrecedent =0;
int etatBoutons =0;

int vitesse = 1500;
void bercer(int duree);
int id_servo = 0;


int etat_sousMenue = 0;
String sousMenue [4] = {"Acceuil", "Volume", "Duree", "Bercer" };

void setup() {
  // put your setup code here, to run once:
  BoardInit();
  DisplayInit();

  pinMode(bonton_plus, INPUT);
  pinMode(bouton_Moins, INPUT);
  
  pinMode(bouton_Droit, INPUT);
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  pinMode(13, OUTPUT);
  

  etat_sousMenue = 0;
 

  DISPLAY_Clear();
  DISPLAY_SetCursor(0, 0);
  //DISPLAY_Printf("15h01");
  delay(1500);
  miseAjour();
  //AX_BuzzerON();
   Serial.begin(9600);
  mySerial.begin(9600);
  mp3_set_serial (mySerial);	//set softwareSerial for DFPlayer-mini mp3 module 
 
	delay(1);  //wait 1ms for mp3 module to set volume
	mp3_set_volume (30);


}

void loop() {
  // put your main code here, to run repeatedly:
  
 // execute_menue();
 //DISPLAY_Clear();
 /*
while (!(ROBUS_IsBumper(0)||ROBUS_IsBumper(1)))
{
  /* code */
//}

execute_menue();
//mobile_demarrer();
//bercement(30,10000);
 
 
}



void execute_menue()
{
  int lectureBoutons, lectureBAS,  lectureHAUT, lectureGAUCHE, lectureDROITE;
  
while (etat_sousMenue < 3)
{
 

  // on vérifie si un des boutons est enfoncé
  lectureBAS = LOW;
  lectureHAUT = LOW;
  lectureGAUCHE = LOW;
  lectureDROITE = LOW;

  lectureBAS = digitalRead(bouton_Moins);
  lectureHAUT = digitalRead(bonton_plus);
  
  lectureDROITE = digitalRead(bouton_Droit);

  lectureBoutons = (lectureBAS ||  lectureHAUT ||  lectureGAUCHE ||  lectureDROITE) ;

   

if(lectureBoutons )
{
  
  if (lectureHAUT) 
  {  // bouton flèche vers le haut (on monte d'une ligne dans la liste)
    if(etat_sousMenue == 1)
    {
       if (niveauVolume + 1 <= niveauMax) 
       {
        niveauVolume++; 
        }
         else
         {
         AX_BuzzerON();
         delay(500);
         AX_BuzzerOFF();
         }
       
       }

     if(etat_sousMenue == 2)
      {
         if (minuteur + 1 <= maximumMinuteur) {
           minuteur++; 
         }
         else
         {
         AX_BuzzerON();
         delay(500);
         AX_BuzzerOFF();
         }
       }

       

 }

   if ( lectureBAS )
  {  // bouton flèche vers le bas: on descend d'une ligne dans la liste
         if(etat_sousMenue == 1)
         {
         if (niveauVolume - 1 > 0) 
         {
           --niveauVolume; 
         }
          else
         {
         AX_BuzzerON();
         delay(500);
         AX_BuzzerOFF();
         }
       
       }

       if(etat_sousMenue == 2)
       {
         if (minuteur - 1 > 0) {
           minuteur--; 
         }
         else
         {
         AX_BuzzerON();
         delay(500);
         AX_BuzzerOFF();
         }
       }
        
      }
    

  

 if ( lectureDROITE ) {  // bouton flèche vers la droite: on exécute le menu sélectionné
        if(etat_sousMenue<3 && etat_sousMenue>-1)
        {
            ++etat_sousMenue;

        }

        else if(etat_sousMenue == 4 )
        {
          
          // execute fonction berssage
          etat_sousMenue = 0;
        }

        else
        {
          etat_sousMenue =0;
        }
        
        
    

  }
}

miseAjour();
  delay(1000);
  etatBoutonsPrecedent = lectureBoutons;
}

void miseAjour()
{
  DISPLAY_Clear();
 
  DISPLAY_SetCursor(0, 0);
  DISPLAY_Printf(sousMenue[etat_sousMenue]);
  Serial.println(sousMenue[etat_sousMenue]);
  DISPLAY_NewLine();

  if(etat_sousMenue == 0)
  {
    Serial.println("PRESS LEFT");
    DISPLAY_Printf("Appuie Jaune" );
  } 

  else if(etat_sousMenue == 1)
  {
    char volume_Texte[5];

    itoa(niveauVolume, volume_Texte, 10);

   DISPLAY_Printf(volume_Texte);
   Serial.print(volume_Texte);

    /*
    string texteVolume;
    itoa()
    DISPLAY_Printf( "%d ",niveauVolume);
    DISPLAY_Printf( itoa(niveauVolume));
    */
  }

  else if(etat_sousMenue == 2)
  {
    char minteur_Texte[5];

    itoa(minuteur, minteur_Texte, 10);

    DISPLAY_Printf(minteur_Texte );
     Serial.print(minteur_Texte);

     DISPLAY_SetCursor(1, 6);
    DISPLAY_Printf("minutes");
    
  }

  else if(etat_sousMenue == 3)
  {
    
    DISPLAY_Printf("Bersage");
    Serial.println("CA BERCEEEEEEEEEE");
    DISPLAY_SetCursor(1, 6);
    bercement(niveauVolume, minuteur);
    etat_sousMenue = 0;
  }

  else 
  {
    
    //lcd.print("***erreur critique***");
    DISPLAY_Printf("***erreur critique***");
    delay(2000);
    etat_sousMenue = 0;
  }
  

  delay(500);

 }
}

void bercement(int volume, int durer)
{
  //AX_BuzzerON();
  mp3_set_volume(volume);
  //int CONST_augmentation;
  int NB_seconde = 60*durer;
  
  //musique();

  mobile_demarrer();
  
 double compteur =0;
 SERVO_Enable(0);
 SERVO_Enable(1);
  //for(double i =0; i< NB_seconde; i+vitesse)


  while (compteur < NB_seconde)
  {
    if(ROBUS_IsBumper(0)||ROBUS_IsBumper(1))
    {
      MOTOR_SetSpeed(0,0.25);
      MOTOR_SetSpeed(1,0.25);
      delay(500);
      MOTOR_SetSpeed(0,0);
      MOTOR_SetSpeed(1,0);

    for (int i = 0; i < 4; i++)
    {
       droite_bercer(id_servo);
      rythme(id_servo);
      gauche_bercer(id_servo);
      rythme(id_servo);
    }
    
     

      MOTOR_SetSpeed(0,-0.25);
      MOTOR_SetSpeed(1,-0.25);
      delay(500);
      MOTOR_SetSpeed(0,0);
      MOTOR_SetSpeed(1,0);
      delay(1000);
    }
    
    else{
      retour_berceau(id_servo);
      break;
    }
    
    

  }
  SERVO_Disable(id_servo);
   mobile_arrete();
   mp3_stop();
   compteur += 1500 / 1000;
   compteur = 1;
  // AX_BuzzerOFF();
}

void rythme(int index)
{
  delay(vitesse);
}

void gauche_bercer(int index)
{
  SERVO_SetAngle(index,75);

}

void droite_bercer(int index)
{
  SERVO_SetAngle(index, 95);

}
void retour_berceau(int index)
{
  SERVO_SetAngle(index, 88);
}

void mobile_demarrer(){
  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A5
  analogWrite(3, 100);
  delay(400);
  analogWrite(3, vitesse_Mobile);   //Spins the motor on Channel A at full speed
  digitalWrite(13,HIGH);
  musique();
}

void mobile_arrete(){
  digitalWrite(12, LOW); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 0);   //Spins the motor on Channel A at full speed
  digitalWrite(13,LOW);
}

void musique()
{
  //if(ROBUS_IsBumper(0))
  //{
  void mp3_random_play();
   mp3_play();
   delay(1000);


  //}

}