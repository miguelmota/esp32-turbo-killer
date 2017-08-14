#include <Adafruit_NeoPixel.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define SPEAKER_PIN 18
#define PIXELS_PIN 5
#define NUM_PIXELS 5

// For the ESP-WROVER_KIT, these are the default.
// TFT Display Pins
#define TFT_CS    19
#define TFT_DC    22
#define TFT_MOSI  23
#define TFT_CLK   26
#define TFT_RST   21
#define TFT_MISO  25
#define TFT_LED   5  // GPIO not managed by library

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIXELS_PIN, NEO_GRB);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

int colors[3][3] = { {132, 0, 114}, {0, 81, 132}, {97, 0, 201} };
int colorIndex = 0;

char notes[] = "ebaeba egabDg ega"; // a space represents a rest
float beats[] = { 1,0.5,1,1,1,2,0.5,0.5,1,1,1,1,2,0.5,0.5,0.5,2 };
int tempo = 300;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(SPEAKER_PIN, HIGH);
    delayMicroseconds(tone);
    digitalWrite(SPEAKER_PIN, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 852 };

  // play the tone corresponding to the note name
  for (int i = 0; i < strlen(names); i++) {
    if (names[i] == note) {

      Serial.print(colorIndex);
      int r = colors[colorIndex][0];
      int g = colors[colorIndex][1];
      int b = colors[colorIndex][2];
      
      int j = NUM_PIXELS;
      while (j--) {
        pixels.setPixelColor(j, r, g, b);
        pixels.show();
      }

      colorIndex++;

      if (colorIndex == 3) {
        colorIndex = 0;
      }
      
      playTone(tones[i], duration);
      
      j = NUM_PIXELS;
      while (j--) {
        pixels.setPixelColor(j, pixels.Color(0,0,0,0));
        pixels.show();
      }
    }
  }
}

void start() {
  for (int i = 0; i < strlen(notes); i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }

    // pause between notes
    delay(tempo / 2); 
  }
}

void paintScreen() {
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_PINK);
  tft.setTextSize(4);
  tft.println("");
  tft.println(" Turbo killer");
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.println("");
  tft.println(" Carpenter Brut");
  start();
}

void setup() {
  Serial.begin(9600);
  pinMode(SPEAKER_PIN, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(255);

  tft.begin();
  pinMode(TFT_LED, OUTPUT);

  paintScreen();
  start();
}

void loop() {
  
}
