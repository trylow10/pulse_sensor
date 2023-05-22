#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

#define sensor A0
#define Highpulse 540
#define UpperThreshold 518
#define LowerThreshold 509

int sX = 0;
int sY = 50;  // Adjusted position of the pulse bar
int x = 0;
int Svalue;
int value;
unsigned long Stime = 0;
unsigned long Ltime = 0;
int count = 0;
float BPM = 0.0;
bool isTiming = false;

void setup() {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

void loop() {
  Svalue = analogRead(sensor);
  Serial.println(Svalue);
  value = map(Svalue, 0, 1024, 0, 45);

  int y = 50 - value;  // Adjusted position of the pulse bar

  if (x > 127) {
    x = 0;
    sX = 0;
    display.clearDisplay();
  }

  display.drawLine(sX, sY, x, y, WHITE);
  sX = x;
  sY = y;
  x++;

  BPMCalculation();

  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("BPM :");
  display.display();
}

void BPMCalculation() {
  if (Svalue > Highpulse) {
    if (!isTiming) {
      Ltime = micros();
      isTiming = true;
    }
  } else {
    if (isTiming) {
      Stime = micros() - Ltime;
      isTiming = false;

      if (Stime >= 60000000UL) {  // 60 seconds in microseconds
        BPM = (count / (Stime / 60000000.0)) * 60.0;
        count = 0;
        Ltime = micros();
        display.setCursor(60, 0);
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.print(BPM, 0);  // Print BPM value without decimal places
        display.print("   ");
        display.display();
        tone(8, 1000, 250);
      }
    }
  }

  if (isTiming) {
    count++;
  }

  display.writeFillRect(0, 50, 128, 16, BLACK);
  display.setCursor(0, 50);
  display.print(BPM, 1);  // Print BPM value with 1 decimal place
  display.print(" BPM");
  display.display();
}
