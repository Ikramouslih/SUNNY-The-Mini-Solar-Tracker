#include<Servo.h>

//predefinition des notes pour le buzzer
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

// création des servo moteurs
Servo myservoH;//responsable du rotation droite et gauche
Servo myservoV;//responsable du rotation en haut et en bas

int button;//variable pour l'etat du switch
//variables pour manipulation de buzzer
int start = 0;
int end = 0;

//les angles initiales des servo moteurs
int angleH = 80;
int angleV = 80;

//les pins des LDR
const int LDRTL = 0;
const int LDRTR = 1;
const int LDRBL = 2;
const int LDRBR = 3;

//les angles maximals et minimal pour protection de dispositif
int angleV_max = 150;
int angleV_min = 30;
int angleH_max = 180;
int angleH_min = 0;

//des variables pour stocker les valeurs des LDRs
int val_LDRTL;
int val_LDRTR;
int val_LDRBL;
int val_LDRBR;

//des variables pour stocker la moyenne calculer des LDR 
//pour avoir la direction meilleur pour faire face à la lumière
int moy_TOP;
int moy_BOTTOM;
int moy_LEFT;
int moy_RIGHT;


int melody_start[] = {
  //start up sound
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,8, //1
  NOTE_G5,4, REST,4, NOTE_G4,8, REST,4, 
};

int melody_end[] = {
  //game over sound
  NOTE_C5,-4, NOTE_G4,-4, NOTE_E4,4, //45
  NOTE_A4,-8, NOTE_B4,-8, NOTE_A4,-8, NOTE_GS4,-8, NOTE_AS4,-8, NOTE_GS4,-8,
  NOTE_G4,8, NOTE_D4,8, NOTE_E4,-2,  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes_start = sizeof(melody_start) / sizeof(melody_start[0]) / 2;
int notes_end = sizeof(melody_end) / sizeof(melody_end[0]) / 2;

// change this to make the song slower or faster
int tempo = 200;

// change this to whichever pin you want to use
int buzzer = 7;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void setup()
{
  Serial.begin(9600);//debit de communication
  pinMode(6,INPUT);//Switch
  pinMode(4,OUTPUT);//LED rouge
  pinMode(5,OUTPUT);//LED vert
  pinMode(7,OUTPUT);//Buzzer
  myservoH.attach(2);//Servo moteur horizontal
  myservoV.attach(3);//Servo moteur vertical
  myservoH.write(angleH);//initialiser Servo moteur horizontal à l'angle 80
  myservoV.write(angleV);//initialiser Servo moteur vertical à l'angle 80
}

void loop()
{ 
  button = digitalRead(6);//lecture d'etat du switch
  
  if(button == 1)//si le switch est en etat on 
  {
    //allumer LED vert et etteindre LED rouge
    digitalWrite(5,HIGH);
    digitalWrite(4,LOW);

    //partie du buzzer
    start++;
    end = 0;
    if(start == 1)
    {
     // iterate over the notes of the melody.
     // Remember, the array is twice the number of notes (notes + durations)
     for (int thisNote = 0; thisNote < notes_start * 2; thisNote = thisNote + 2) 
     {  
      // calculates the duration of each note
       divider = melody_start[thisNote + 1];
       if (divider > 0) 
       {
        // regular note, just proceed
        noteDuration = (wholenote) / divider;
       } 
       else if (divider < 0) 
       {
        // dotted notes are represented with negative durations!!
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
       }
       // we only play the note for 90% of the duration, leaving 10% as a pause
       tone(buzzer, melody_start[thisNote], noteDuration * 0.9);
       // Wait for the specief duration before playing the next note.
       delay(noteDuration);
       // stop the waveform generation before the next note.
       noTone(buzzer);
     }
    }
    else
    {
      start = 2;
    }
   
    
    //lecture des angles des servo moteurs
    angleH = myservoH.read();
    angleV = myservoV.read();
    //lecture des valeurs envoyées par les LDRs
    val_LDRTL = analogRead(LDRTL);
    val_LDRTR = analogRead(LDRTR);
    val_LDRBL = analogRead(LDRBL);
    val_LDRBR = analogRead(LDRBR);


    //Affichage de ces valeurs sur le terminal
    Serial.println(val_LDRTL);
    Serial.println(val_LDRTR);
    Serial.println(val_LDRBL);
    Serial.println(val_LDRBR);

    //calcul des moyennes des capteurs LDR pour manipuler les servo moteurs
    moy_TOP = (val_LDRTL + val_LDRTR) / 2;
    moy_BOTTOM = (val_LDRBL + val_LDRBR) / 2;
    moy_LEFT = (val_LDRTL + val_LDRBL) / 2;
    moy_RIGHT = (val_LDRTR + val_LDRBR) / 2;
  
    //manipulation des servo moteur en se basant sur les moyennes calculées
    if(moy_TOP > moy_BOTTOM)
    {
      myservoV.write(angleV + 1);
      if(angleV > angleV_max)
      {
        angleV = angleV_max;
      }
        delay(10);
    }
    else if(moy_TOP < moy_BOTTOM)
    {
      myservoV.write(angleV - 1);
      if(angleV < angleV_min)
      {
        angleV = angleV_min;
      }
        delay(10);
    }
    else
    {
      myservoV.write(angleV);
    }
  
  
    if(moy_LEFT > moy_RIGHT)
    {
      myservoH.write(angleH - 1);
      if(angleH < angleH_min)
      {
        angleH = angleH_min;
      }
    
        delay(10);
    }
    else if(moy_LEFT < moy_RIGHT)
    {
      myservoH.write(angleH + 1);
      if(angleH > angleH_max)
      {
        angleH = angleH_max;
      }
        delay(10);
    }
    else
    {
      myservoH.write(angleH);
    }
  }

  
  else if(button == 0)//si le switch est en etat off
  {
    //partie LED
    digitalWrite(4,HIGH);//allumer LED rouge
    digitalWrite(5,LOW);//etteindre LED verte 
    
    //partie du buzzer
    end++;
    start = 0;
    if(end == 1)
     {
       // iterate over the notes of the melody.
       // Remember, the array is twice the number of notes (notes + durations)
       for (int thisNote = 0; thisNote < notes_end * 2; thisNote = thisNote + 2) 
       {
         // calculates the duration of each note
         divider = melody_end[thisNote + 1];
         if (divider > 0) 
        {
          // regular note, just proceed
          noteDuration = (wholenote) / divider;
         } 
         else if (divider < 0) 
         {
          // dotted notes are represented with negative durations!!
          noteDuration = (wholenote) / abs(divider);
          noteDuration *= 1.5; // increases the duration in half for dotted notes
         }
         // we only play the note for 90% of the duration, leaving 10% as a pause
         tone(buzzer, melody_end[thisNote], noteDuration * 0.9);
         // Wait for the specief duration before playing the next note.
         delay(noteDuration);
         // stop the waveform generation before the next note.
         noTone(buzzer);
        }
      }
    else
    {
      end = 2;
    }

   
  } 
}
