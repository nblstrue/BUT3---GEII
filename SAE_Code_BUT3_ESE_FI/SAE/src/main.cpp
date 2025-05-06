#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define OK_BUTTON           D2
#define LED                 D3
#define ANL_BUTTON          D4
#define dirPin              D5
#define stepPin             D6
#define UP_BUTTON           D7
#define DOWN_BUTTON         D8
#define FREQUENCE_PIN       D12
#define BATTERY_PIN         D13
#define SCREEN_WIDTH        128  // display display width, in pixels
#define SCREEN_HEIGHT       128 // display display height, in pixels
#define SCREEN_RESET        -1   // can set an display reset pin if desired
#define SCREEN_ADRESS       0x3C
#define stepsPerRevolution  200

Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET, 1000000, 100000);
void  GME128128_begin(uint8_t adress);
void  GME128128_menu();
void  Frequence();
void  Auto();
void  Exit();
void  Battery();
void  Manuel();
void  RappelNotes();
void  pwm_gestion();
float freq = 0;
const int  frequency_notes[] = {82, 110.00, 147, 196.00, 247, 330};
int freq_mem = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(DOWN_BUTTON, INPUT);
  pinMode(UP_BUTTON, INPUT);
  pinMode(ANL_BUTTON, INPUT);
  pinMode(OK_BUTTON, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(FREQUENCE_PIN, INPUT);
  GME128128_begin(SCREEN_ADRESS);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  while(!digitalRead(DOWN_BUTTON))
    Frequence();
  while(!digitalRead(UP_BUTTON))
    Auto();
  Battery(); delay(5000);
  Manuel(); delay(3000);
  RappelNotes(); delay(3000);
}

void loop()
{
   Auto();
}

void pwm_gestion()
{
  for(int i = 0; i < 256; i++)
  {
    analogWrite(LED, i);
    delay(10);
  }
  for(int i = 255; i > 0; i--)
  {
    analogWrite(LED, i);
    delay(10);
  }
}

void GME128128_begin(uint8_t adress)
{
    display.clearDisplay();
    display.begin(adress, true);
    display.display();
    delay(2000);
    display.clearDisplay();
    display.drawPixel(10, 10, SH110X_WHITE);
    display.display();
    delay(2000);
    display.clearDisplay();

    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(1, 1); display.print("BUT GE2I - PROJET SAE\n");
    display.setCursor(1, 8); display.print("3E ANNEE\n\n");
    display.print("Lathro-Seri Nathan pour la programmation\n\nIdmond Benjy pour la conception\n\nRamirez Victor pour\nl'alimentation\n\nGuitare Electro-\nAcoustique");
    display.display();
    delay(3000);
    display.clearDisplay();
}

void GME128128_menu()
{
    static int menu_direction = 0;

    if(digitalRead(OK_BUTTON) && digitalRead(ANL_BUTTON))
    {
        display.clearDisplay();
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(1);
        display.setCursor(1, 1);
        display.print("ERROR: both buttons have high state");
        display.display();
    }
    else
    {
        display.clearDisplay();
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(1);
        display.setCursor(55, 1);
        display.print("-->MENU<--\n\n");
        display.setCursor(1, 2);
    
        if(!digitalRead(DOWN_BUTTON) && digitalRead(UP_BUTTON))
            menu_direction++;
        else
        {
            if(digitalRead(DOWN_BUTTON) && !digitalRead(UP_BUTTON))
                menu_direction--;
        }
        if(menu_direction >= 6)
            menu_direction = 0;
        else
        {
            if(menu_direction <= -1)
                menu_direction = 5;
        }

        switch (menu_direction)
            {
            case 0:
                display.print("\n\n-> Frequencemetre\n\nMode Manuel\n\nMode Automatique\n\nBatterie\n\nRappels Notes\n\nExit");
                    if(digitalRead(OK_BUTTON) && !digitalRead(ANL_BUTTON))
                        for(int i = 0; i < 10; i++)
                            Frequence(); 
                break;
            case 1:
                display.print("\n\nFrequencemetre\n\n-> .\n\nMode Automatique\n\nBatterie\n\nRappels Notes\n\nExit");
                    if(digitalRead(OK_BUTTON) && !digitalRead(ANL_BUTTON))
                        break;
                break;
            case 2:
                display.print("\n\nFrequencemetre\n\nMode Manuel\n\n-> Mode Automatique\n\nBatterie\n\nRappels Notes\n\nExit");
                    if(digitalRead(OK_BUTTON) && !digitalRead(ANL_BUTTON))
                        for(int i = 0; i < 10; i++)
                            Auto();
                break;
            case 3:
                display.print("\n\nFrequencemetre\n\nMode Manuel\n\nMode Automatique\n\n-> Batterie\n\nRappels Notes\n\nExit");
                    if(digitalRead(OK_BUTTON) && !digitalRead(ANL_BUTTON))
                        for(int i = 0; i < 10; i++)
                            Battery();
                break;
            case 4:
                display.print("\n\nFrequencemetre\n\nMode Manuel\n\nMode Automatique\n\nBatterie\n\n-> Rappels Notes\n\nExit");
                    if(digitalRead(OK_BUTTON) && !digitalRead(ANL_BUTTON))
                        for(int i = 0; i < 10; i++)
                            RappelNotes();
                break;
            default:
                display.print("\n\nFrequencemetre\n\nMode Manuel\n\nMode Automatique\n\nBatterie\n\nRappels Notes\n\n-> Exit");
                    if(!digitalRead(OK_BUTTON) && digitalRead(ANL_BUTTON))
                        Exit();
                break;
        }
        display.display();
    }
}

void Frequence()
{
    const unsigned char music_note [] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf8, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xf8, 0x00, 
        0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf8, 0x00, 
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xfe, 0x78, 0x00, 
        0x00, 0x00, 0x00, 0xff, 0xff, 0xf0, 0x78, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x78, 0x00, 
        0x00, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x78, 0x00, 
        0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x78, 0x00, 
        0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x78, 0x00, 
        0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x78, 0x00, 
        0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x78, 0x00, 
        0x00, 0x00, 0x00, 0xf0, 0x00, 0x3e, 0x78, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0xff, 0xf8, 0x00, 
        0x00, 0x00, 0x00, 0xf0, 0x01, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0xff, 0xf8, 0x00, 
        0x00, 0x00, 0x00, 0xf0, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0xff, 0xf8, 0x00, 
        0x00, 0x00, 0x00, 0xf0, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x38, 0xf0, 0x0f, 0xff, 0xf8, 0x00, 
        0x00, 0x01, 0xff, 0xf0, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xff, 0xf0, 0x0f, 0xff, 0xf8, 0x00, 
        0x00, 0x07, 0xff, 0xf0, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x07, 0xff, 0xf0, 0x00, 
        0x00, 0x0f, 0xff, 0xf0, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x03, 0xff, 0xe0, 0x00, 
        0x00, 0x1f, 0xff, 0xf0, 0x01, 0xff, 0xc0, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x00, 0xff, 0x80, 0x00, 
        0x00, 0x1f, 0xff, 0xf0, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    const char      frequency_names[] = {'E2', 'A2', 'D3', 'G3', 'B3', 'E4'};
    unsigned long   period            = 0;
    unsigned long   sumhigh           = 0;
    unsigned long   sumlow            = 0;
    int             f_ch_indice       = 0;
    unsigned long   a                 = 0;

    for(int i = 0; i < 5; i++)
    {
        a = pulseIn(FREQUENCE_PIN, HIGH);
        sumhigh+=a;
    }
    for(int i = 0; i < 5; i++)
    {
        a = pulseIn(FREQUENCE_PIN, LOW);
        sumlow+=a;
    }

    period = (sumhigh + sumlow)/5;
    freq = 1000000.0/period;
    
    /*display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.drawBitmap(1, 1, music_note, 64, 64, SH110X_WHITE);
    display.setTextSize(2);
    display.setCursor(1, 70);
    display.println("FREQUENCE");
    display.setTextSize(1);
    display.setCursor(1, 90);
    display.print("Frequence = "); display.print(freq); display.print(" Hz");

    if(freq == 4294967295)
        freq = 0;

    int horschamp = 0;

    for(int i = 0; i < sizeof(frequency_notes); i++)
    {
        if((freq/frequency_notes[i]) > 0.05)
        {
            f_ch_indice = i;
            display.setCursor(1, 100);
            display.setTextSize(1); display.print("Note = "); display.print(frequency_notes[i]);
            freq_mem = i;       
        }
        else
            horschamp++;
    }

    display.display();*/
}

void Auto()
{
    Frequence();

    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(1, 1);
    display.println("Accordage en cours...");
    display.display();
    
    if(freq > frequency_notes[freq_mem]*1.05)
    {
        digitalWrite(dirPin, HIGH);
        for(int i = 0; i < 1000; i++)
        {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(4300);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(4300);
            if(freq == frequency_notes[freq_mem])
                break;
        }
        delay(1000);
    }
    else{
        if(freq < frequency_notes[freq_mem]*0.95)
        {
            digitalWrite(dirPin, LOW);
            for(int i = 0; i < 1000; i++)
            {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(4300);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(4300);
                if(freq == frequency_notes[freq_mem])
                    break;
            }
            delay(1000);
        }
    }
}

void Manuel()
{
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(1, 1);
    display.println("Montrez leur votre talent !");
    display.display();

    //fonction de mise en veille du systeme
}

void Battery()
{
    int battery_lvl = 4;
    int battery_prc = (1 - ((6-battery_lvl)/battery_lvl)) * 100;

    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(1, 10);
    display.print("Niveau de la Batterie (exemple) = "); display.print(battery_prc); display.print(" %\n");
    display.setCursor(1, 40);
    display.print("Tension de Batterie (exemple) = "); display.print(battery_lvl); 
    display.display(); 
}

void Exit()
{
    display.clearDisplay();
            display.setTextColor(SH110X_WHITE);
            display.setTextSize(1);
            display.setCursor(1, 1);
            display.print("Stoping...");
            display.display();
            delay(3000);
            display.clearDisplay();
            display.setTextColor(SH110X_WHITE);
            display.setTextSize(1);
            display.setCursor(1, 1);
            display.print("Reset the Arduino for launching back the code");
            display.display();
            delay(500);
            
            exit(0);
}

void RappelNotes()
{
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(1, 1);
    display.println("Rappel:\n");
    display.print("Nom - Frequence\n");
    display.print("Mi aigu - 329.63Hz\n");
    display.print("Si - 246.94 Hz\n");
    display.print("Sol-G3-196.00 Hz\n");
    display.print("Ré-D3-146.83 Hz\n");
    display.print("La - 110.00 Hz\n");
    display.print("Mi grave - 82.41\n");
}
