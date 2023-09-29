//include libraries
#include <Wire.h> // Library for I2C communication
#include <Adafruit_GFX.h> //OLED
#include <Adafruit_SSD1306.h> //OLED
#include <Adafruit_MLX90614.h>//senzor temperatura
#include <virtuabotixRTC.h> //ceas Real Time
#include <SPI.h>
#include <SD.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

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


int melody2[] = {

  NOTE_D4,4, NOTE_A4,4, NOTE_A4,4,
  REST,8, NOTE_E4,8, NOTE_B4,2,
  NOTE_F4,4, NOTE_C5,4, NOTE_C5,4,
  REST,8, NOTE_E4,8, NOTE_B4,2,
  NOTE_D4,8, NOTE_F4,8, NOTE_D5,2,
  
  NOTE_D4,8, NOTE_F4,8, NOTE_D5,2,
  NOTE_E5,-4, NOTE_F5,8, NOTE_E5,8, NOTE_E5,8,
  NOTE_E5,8, NOTE_C5,8, NOTE_A4,2,
  NOTE_A4,4, NOTE_D4,4, NOTE_F4,8, NOTE_G4,8,
  NOTE_A4,-2,
  NOTE_A4,4, NOTE_D4,4, NOTE_F4,8, NOTE_G4,8,
  NOTE_E4,-2,
  NOTE_D4,8, NOTE_F4,8, NOTE_D5,2,
  NOTE_D4,8, NOTE_F4,8, NOTE_D5,2,

  NOTE_E5,-4, NOTE_F5,8, NOTE_E5,8, NOTE_E5,8,
  NOTE_E5,8, NOTE_C5,8, NOTE_A4,2,
  NOTE_A4,4, NOTE_D4,4, NOTE_F4,8, NOTE_G4,8,
  NOTE_A4,2, NOTE_A4,4,
  NOTE_D4,1, 
};
int melody1[] = {
  
  NOTE_D5,-4, NOTE_A5,8, NOTE_FS5,8, NOTE_D5,8,
  NOTE_E5,-4, NOTE_FS5,8, NOTE_G5,4,
  NOTE_FS5,-4, NOTE_E5,8, NOTE_FS5,4,
  NOTE_D5,-2,
  NOTE_D5,-4, NOTE_A5,8, NOTE_FS5,8, NOTE_D5,8,
  NOTE_E5,-4, NOTE_FS5,8, NOTE_G5,4,
  
  NOTE_FS5,-4, NOTE_E5,8, NOTE_FS5,4,
  NOTE_D5,-2,
  NOTE_D5,-4, NOTE_A5,8, NOTE_FS5,8, NOTE_D5,8,
  NOTE_E5,-4, NOTE_FS5,8, NOTE_G5,4,
  NOTE_FS5,-1,

};

// change this to make the song slower or faster
int tempo = 200;

//  pinii Trig și Echo pentru senzorul IR de distanta
const int triggerPin = 23;
const int echoPin = 22;
long duration;
int distance;

const int sensorPin = 2; //pin senzor obstacol

const int speakerPin = 3; //pin aplificator R

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
virtuabotixRTC myRTC(42, 44, 46);//(DATA, CLK, RST)

File myFile;

double Ambient=0;
double Object=0;

const int LedR=6;
const int LedV=7;

int notes1 = sizeof(melody1) / sizeof(melody1[0]) / 2;
int notes2 = sizeof(melody2) / sizeof(melody2[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

int SDpin = 53;

// push-buton
const int buttonPin = 24;
int buttonState = 0;
int previousButtonState = 0;
int buttonPressCount = 0;


void setup() 
{
  Serial.begin(9600);
  
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LedR, OUTPUT);
  pinMode(LedV, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  
  // Set the current date, and time in the following format:
  // seconds, minutes, hours, day of the week, day of the month, month, year
  //myRTC.setDS1302Time(00, 46, 13, 3, 5, 7, 2023);
  
   // Inițializarea ecranului OLED
   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
   {
      Serial.println(F("Eroare la inițializarea ecranului OLED"));
      while (true);
   }
   display.clearDisplay();// Curățarea ecranului
    
   pinMode(sensorPin, INPUT);//senzor obstacol
   
  //amplificator Audio
   pinMode(speakerPin, OUTPUT);
   
   //Card SD  
   Serial.println("Initializing SD card...");
   if (!SD.begin(SDpin)) 
   {
      Serial.println("initialization failed!");
     while (1);
   }
   Serial.println("initialization done.");
    
  //Senzor temperatura
   if (!mlx.begin()) 
   {
      Serial.println("Error connecting to MLX sensor. Check wiring.");
      while (1);
   };
   mlx.readEmissivity();
 
}



void loop() 
{
  // Citirea valorii de la senzor obstacol
  int IRState = digitalRead(sensorPin);
  buttonState = digitalRead(buttonPin);
  digitalWrite(LedV, LOW);
  digitalWrite(LedR, LOW);
  //Actualizare timp
  myRTC.updateTime();
  
   Serial.print("Current Date / Time: ");
   Serial.print(myRTC.dayofmonth);
   Serial.print("/");
   Serial.print(myRTC.month);
   Serial.print("/");
   Serial.print(myRTC.year);
   Serial.print("  ");
   Serial.print(myRTC.hours);
   Serial.print(":");
   Serial.print(myRTC.minutes);
   Serial.print(":");
   Serial.println(myRTC.seconds);
   
  //Declarare timp
  int Day = myRTC.dayofmonth;
  int Month = myRTC.month;
  int Year = myRTC.year;
  int Secs = myRTC.seconds;
  int Hours = myRTC.hours;
  int Minutes = myRTC.minutes;

  
  //Citire temperatura  ambientala  
  Ambient = mlx.readAmbientTempC();
  Object = 1.0 + mlx.readObjectTempC() ;
    if (buttonState != previousButtonState) {
      if (buttonState == LOW) {
        buttonPressCount++;}}
  if (IRState == 1) 
  {
    Serial.println("Senzor IR inactiv");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    display.setCursor(0, 0); // Set the cursor on the first column and first row.
    display.print("Date: ");
    display.print(Day); display.print("."); display.print(Month); display.print("."); display.println(Year);
     
    display.setCursor(0, 25); //Set the cursor on the third column and the second row (counting starts at 0!).
    display.print("Time: ");
    display.print(Hours); display.print(":"); display.print(Minutes); display.print(":"); display.print(Secs);
    
    display.setCursor(0, 50); 
    display.print("Ambient : "); 
    display.println(Ambient); 
    
    display.display();
  } 
  else 
  {//senzor IR activ
        if (buttonPressCount % 2 == 0) 
        {//om
           Serial.println("S-a selectat temperatura pentru om");
             display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 5); 
            display.println("Se masoara tempertura omului "); 
           
             display.println(" "); 
            display.println("Apasa butonul pentru a schimba "); 
            display.println(""); 
            display.display();
          delay(2000);
            // Transmiterea semnalului ultrasunete
            digitalWrite(triggerPin, LOW);
            delayMicroseconds(2);
            digitalWrite(triggerPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(triggerPin, LOW);
            // Măsurarea duratei de timp pentru semnalul de ecou
            duration = pulseIn(echoPin, HIGH);
            // Calcularea distanței în centimetri
            distance = duration * 0.034 / 2;

            delay(1000);
            
            if (distance>=3 && distance<=4)  Object = 2.3 + mlx.readObjectTempC() ;
            if (distance>=5 && distance<=7)  Object = 3.0 + mlx.readObjectTempC() ;
            if (distance>7 && distance<9)  Object = 4.0 + mlx.readObjectTempC() ;
            if (distance>=9)  Object = 4.5 + mlx.readObjectTempC() ;
            if (distance>12)  {
              display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);

            display.setCursor(0, 30); 
            display.print("EROARE CITIRE "); 
            display.display();
           digitalWrite(LedR, HIGH);
           
            }
            else{
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            
            display.setCursor(0, 15); 
            display.print("Ambiental: "); 
            display.println(Ambient);  
            
            display.setCursor(0, 40); 
            display.print("Human : ");
            //Object=40;
            display.println(Object);
            display.display();
            
            delay(1000);
            
        Serial.println(distance);
        Serial.println(Object);
        
        
            //Scriere pe card SD
            myFile = SD.open("file.txt", FILE_WRITE);
            if (myFile) {
              Serial.print("Writing to file.txt...");
              myFile.print("Date: ");
              myFile.println(Day); myFile.print("."); myFile.print(Month); myFile.print("."); myFile.println(Year);
              myFile.print("Time: ");
              myFile.println(Hours); myFile.print(":"); myFile.print(Minutes); myFile.print(":"); myFile.println(Secs);
              myFile.print("Temperatura omului: ");
              myFile.println(Object);
              // close the file:
              myFile.close();
              Serial.println("done.");
            } else {
              // if the file didn't open, print an error:
              Serial.println("error opening test.txt");
            }
            
          if (Object<38 & Object>33) //temperatura normala pentru om
          {
             digitalWrite(LedV, HIGH); // Porniti becul
            for (int thisNote = 0; thisNote < notes1 * 2; thisNote = thisNote + 2) {
                // calculates the duration of each note
                divider = melody1[thisNote + 1];
                if (divider > 0) {
                  // regular note, just proceed
                  noteDuration = (wholenote) / divider;
                } else if (divider < 0) {
                  // dotted notes are represented with negative durations!!
                  noteDuration = (wholenote) / abs(divider);
                  noteDuration *= 1.5; // increases the duration in half for dotted notes
                }
                // we only play the note for 90% of the duration, leaving 10% as a pause
                tone(speakerPin, melody1[thisNote], noteDuration * 0.9);
               
                
                // Wait for the specief duration before playing the next note.
                delay(noteDuration);
            
                // stop the waveform generation before the next note.
                noTone(speakerPin);
                
              }
            delay(2000);
          }
          else if (Object<45 & Object>38) // Febra
          {
            digitalWrite(LedR, HIGH);
            for (int thisNote = 0; thisNote < notes2 * 2; thisNote = thisNote + 2) {
            
                // calculates the duration of each note
                divider = melody2[thisNote + 1];
                if (divider > 0) {
                  // regular note, just proceed
                  noteDuration = (wholenote) / divider;
                } else if (divider < 0) {
                  // dotted notes are represented with negative durations!!
                  noteDuration = (wholenote) / abs(divider);
                  noteDuration *= 1.5; // increases the duration in half for dotted notes
                }
            
                // we only play the note for 90% of the duration, leaving 10% as a pause
                tone(speakerPin, melody2[thisNote], noteDuration * 0.9);
            
                // Wait for the specief duration before playing the next note.
                delay(noteDuration);
            
                // stop the waveform generation before the next note.
                noTone(speakerPin);
              }
            delay(2000);
          }else if (Object > 45)
          {
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);

            display.setCursor(0, 30); 
            display.print("EROARE CITIRE "); 
            display.display();
           digitalWrite(LedR, HIGH);
          }
          }
          
        }
        else 
        {//obiect
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 5); 
            display.println("Se masoara tempertura unui obiect "); 
             display.println(" "); 
            display.println("Apasa butonul pentru a schimba "); 
            display.println(""); 
            display.display();
          delay(2000);
          
            Serial.println("S-a selectat obiect");
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            
            display.setCursor(0, 15); 
            display.print("Ambiental: "); 
            display.println(Ambient);  
            
            display.setCursor(0, 40); 
            display.print("Object : 40");
            //display.println(Object);
            display.display();
            
            myFile = SD.open("file.txt", FILE_WRITE);
            if (myFile) {
              Serial.print("Writing to file.txt...");
              myFile.print("Date: ");
              myFile.println(Day); myFile.print("."); myFile.print(Month); myFile.print("."); myFile.println(Year);
              myFile.print("Time: ");
              myFile.println(Hours); myFile.print(":"); myFile.print(Minutes); myFile.print(":"); myFile.println(Secs);
              myFile.print("Temperatura obiectului: ");
              myFile.println(Object);
              // close the file:
              myFile.close();
              Serial.println("done.");
            } else {
              // if the file didn't open, print an error:
              Serial.println("error opening test.txt");
                    }
            delay(2000);
           }
        }
      
previousButtonState = buttonState; 
delay(1000);
}
