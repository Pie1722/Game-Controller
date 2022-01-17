//GAMEPAD LIBRARIES
#include <BleGamepad.h>
#include <FastLED.h>

//SENSOR LIBRARIES  -------------------------------
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

CRGB leds[6];
BleGamepad bleGamepad("GAMEPAD V1", "147258", 100);

// TRIGGER BUTTONS
#define LT 2
#define RT 23

// LEFT-JOYSTICK
#define LH 34
#define LV 35
#define LS 15

// RIGHT-JOYSTICK
#define RH 39
#define RV 36
#define RS 19

// D-PAD
#define L1 32
#define L2 33
#define L3 27
#define L4 14

#define R1 18
#define R2 5
#define R3 17
#define R4 16

// BATTERY VOLTAGE
#define ADC 4

// NEOPIXEL
#define DATA_PIN 13

// PERIFERALS
#define MIC 25
#define SPEAKER 26

// bmp image size
#define bitmap_height   109
#define bitmap_width    55

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const unsigned char bitmap [] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x80, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x60, 0x00,
  0x00, 0x00, 0x00, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x80, 0x00, 0x00, 0x39, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0xF1, 0xE3, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0xC0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x60, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x18, 0x3E, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x77, 0x05, 0x2D, 0x1C,
  0xCF, 0x1B, 0x93, 0xE1, 0x34, 0x0E, 0xB9, 0xC4, 0xF2, 0xAD, 0x4D, 0xAA, 0x46, 0x7E, 0x02, 0x90,
  0xCB, 0x46, 0xF6, 0xFE, 0x15, 0xE8, 0x0B, 0xCE, 0x82, 0x9A, 0x32, 0xEF, 0x90,
};

int buttons[8] = {32, 33, 27, 14, 18, 5, 17, 16};

int period = 1000;
unsigned long time_now = 0;

const int numberOfPotSamples = 5;     // Number of pot samples to take (to smooth the values)
const int delayBetweenSamples = 2;    // Delay in milliseconds between pot samples
const int delayBetweenHIDReports = 5; // Additional delay in milliseconds between HID reports
const int debounceDelay = 10;        // Delay in milliseconds between button press

int previousButton1State = HIGH;
int previousButton2State = HIGH;
int previousButton3State = HIGH;
int previousButton4State = HIGH;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  //  initialize display

void setup() {

  //Starting Serial Communication
  Serial.begin(115200);               

  //Starting I2C
  Wire.begin;
  SPI.begin;

  //Starting Gamepad
  bleGamepad.begin();
  Serial.println("Starting BLE work!");

   //Starting RTC
   if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  rtc.adjust(DateTime(__DATE__, __TIME__));                             // adjust date and time if powerloss

  //This block of code initializes the OLED display.
  //SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {                        // Address 0x3C for 128x32 & 0x3D for 128x64 display
    Serial.println(F("SSD1306 allocation failed"));
  }

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
  display.clearDisplay();               // clear the buffer

  ----------------------------------------------------------------------------------
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, 6);
  FastLED.setBrightness(64);
  for (int i = 0; i < 8; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }
  pinMode(LT, INPUT_PULLUP);
  pinMode(RT, INPUT_PULLUP);
  pinMode(LS, INPUT_PULLUP);
  pinMode(RS, INPUT_PULLUP);
  
  leds[0] = CRGB::Blue;
  leds[5] = CRGB::Blue;
  FastLED.show();
  delay(200);
  leds[1] = CRGB::Blue;
  leds[4] = CRGB::Blue;
  FastLED.show();
  delay(300);
  leds[2] = CRGB::Blue;
  leds[3] = CRGB::Blue;
  FastLED.show();
  delay(1000);
  FastLED.clear();
  FastLED.show();

  showBitmap();                         // print image in oled display

}

void loop() {
  
  if (bleGamepad.isConnected()) {
    for (int i = 0; i < 6; i++) {
      leds[i] = CHSV( HUE_GREEN, 255, 64);
    }
    FastLED.show();
    while (millis() > time_now + period) {
      Serial.println("Checking Battery Level");
      batteryLevel();
      time_now = millis();
    }
    int currentButton1State = digitalRead(R1);
    int currentButton2State = digitalRead(R2);
    int currentButton3State = digitalRead(R3);
    int currentButton4State = digitalRead(R4);

    if (currentButton1State != previousButton1State) {
      if (currentButton1State == LOW)
        bleGamepad.press(BUTTON_5);
      else
        bleGamepad.release(BUTTON_5);
    }
    previousButton1State = currentButton1State;

    if (currentButton2State != previousButton2State) {
      if (currentButton2State == LOW)
        bleGamepad.press(BUTTON_2);
      else
        bleGamepad.release(BUTTON_2);
    }
    previousButton2State = currentButton2State;

    if (currentButton3State != previousButton3State) {
      if (currentButton3State == LOW)
        bleGamepad.press(BUTTON_1);
      else
        bleGamepad.release(BUTTON_1);
    }
    previousButton3State = currentButton3State;

    if (currentButton4State != previousButton4State) {
      if (currentButton4State == LOW)
        bleGamepad.press(BUTTON_4);
      else
        bleGamepad.release(BUTTON_4);
    }
    previousButton4State = currentButton4State;

    if (digitalRead(LS) == LOW) {
      Serial.println("Left Joystick");
      bleGamepad.press(BUTTON_6);
      delay(debounceDelay);
      bleGamepad.release(BUTTON_6);
    }
    if (digitalRead(RS) == LOW) {
      Serial.println("Right Joystick");
      bleGamepad.press(BUTTON_3);
      delay(debounceDelay);
      bleGamepad.release(BUTTON_3);
    }

    if (digitalRead(LT) == LOW) {
      Serial.println("Left trigger");
      bleGamepad.press(BUTTON_7);
      delay(debounceDelay);
      bleGamepad.release(BUTTON_7);
    }
    if (digitalRead(RT) == LOW) {
      Serial.println("Right trigger");
      bleGamepad.press(BUTTON_8);
      delay(debounceDelay);
      bleGamepad.release(BUTTON_8);
    }

    int potValues[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues[i] = analogRead(LH);
      delay(delayBetweenSamples);
    }
    int potValue = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue += potValues[i];
    }
    potValue = potValue / numberOfPotSamples;
    int adjustedValue = map(potValue, 0, 4095, 127, -127);


    int potValues2[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues2[i] = analogRead(LV);
      delay(delayBetweenSamples);
    }
    int potValue2 = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue2 += potValues2[i];
    }
    potValue2 = potValue2 / numberOfPotSamples;
    int adjustedValue2 = map(potValue2, 0, 4095, 127, -127);


    int potValues3[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues3[i] = analogRead(RH);
      delay(delayBetweenSamples);
    }
    int potValue3 = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue3 += potValues3[i];
    }
    potValue3 = potValue3 / numberOfPotSamples;
    int adjustedValue3 = map(potValue3, 0, 4095, 255, 0);


    int potValues4[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues4[i] = analogRead(RV);
      delay(delayBetweenSamples);
    }
    int potValue4 = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue4 += potValues4[i];
    }
    potValue4 = potValue4 / numberOfPotSamples;
    int adjustedValue4 = map(potValue4, 0, 4095, 255, 0);
    
      Serial.print(adjustedValue);
      Serial.print(" || ");
      Serial.print(adjustedValue2);
      Serial.print(" || ");
      Serial.print(adjustedValue3);
      Serial.print(" || ");
      Serial.println(adjustedValue4);
    
    bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, DPAD_CENTERED);
    delay(delayBetweenHIDReports);

    if (digitalRead(L1) == LOW)
      bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, DPAD_UP);
    if (digitalRead(L2) == LOW)
      bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, DPAD_LEFT);
    if (digitalRead(L3) == LOW)
      bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, DPAD_DOWN);
    if (digitalRead(L4) == LOW)
      bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, DPAD_RIGHT);
  }
   rtc(); 
}

void rtc() {
  
  DateTime now = rtc.now();

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(75, 0);
  display.println(now.second(), DEC);

  display.setTextSize(2);
  display.setCursor(25, 0);
  display.println(":");

  display.setTextSize(2);
  display.setCursor(65, 0);
  display.println(":");

  display.setTextSize(2);
  display.setCursor(40, 0);
  display.println(now.minute(), DEC);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(now.hour(), DEC);

  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(now.day(), DEC);

  display.setTextSize(1);
  display.setCursor(25, 20);
  display.println("-");

  display.setTextSize(1);
  display.setCursor(40, 20);
  display.println(now.month(), DEC);

  display.setTextSize(1);
  display.setCursor(55, 20);
  display.println("-");

  display.setTextSize(1);
  display.setCursor(70, 20);
  display.println(now.year(), DEC);

  display.setTextSize(2);
  display.setCursor(0, 40);
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);
  
}

void batteryLevel() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.size(1);
  int sensorValue = analogRead(ADC);
  float voltage = sensorValue * (5.12 / 4095.0);
  Serial.print(voltage);
  Serial.print("V ||");
  int percentage = (voltage / 4.2) * 100;
  display.print(percentage);
  display.println("%");
  bleGamepad.setBatteryLevel(percentage);
  if (percentage < 60) {
    Serial.println("LOW battery");
    for (uint8_t i = 30; i < 220; i++) {
      for (int j = 0; j < 6; j++) {
        leds[j] = CHSV( HUE_RED, 255, i);
      }
      FastLED.show();
      delay(15);
    }
    for (uint8_t i = 220; i > 30; i--) {
      for (int j = 0; j < 6; j++) {
        leds[j] = CHSV( HUE_RED, 255, i);
      }
      FastLED.show();
      delay(15);
    }
  }
}

void showBitmap(void) {
  
  display.clearDisplay();
  display.drawBitmap(0, 0, bitmap, bitmap_height, bitmap_width, WHITE);
  display.display();
  delay(2500);
  
  display.invertDisplay(true);                  //inverse the colour
  
  display.clearDisplay();
  display.drawBitmap(0, 0, bitmap, bitmap_height, bitmap_width, WHITE);
  display.display();
  delay(2500);
}
