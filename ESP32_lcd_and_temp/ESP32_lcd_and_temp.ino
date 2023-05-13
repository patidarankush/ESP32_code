/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

// #include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFunHTU21D.h>

//soil moisture sensor variable declaration
// #define AOUT_PIN 14 // // ESP32 pin GIOP36 (ADC0) that connects to AOUT pin of moisture sensor
// # define sensorPin 36
int sensor_pin = 36;
int Moisturedata;

//humidity and temprature value calculation
//Create an instance of the object
HTU21D myHumidity = HTU21D();

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  Wire.begin();
  delay(10);

  //humidity and temprature setup
  myHumidity.begin();

  //trial code humidity and temprature setup
  //  bool status = myHumidity.begin();
  // if (!status) {
  //   Serial.println("Could not find a valid HUmidity sensor sensor, check wiring!");
  //   while (1);
  // }

  //lcd display setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever  //TODO  Take care here
  }
}

int value = 0;

//function for reading temprature and humidity
void Tempcal() {
  //  Wire.beginTransmission(0x40);
  float humd = myHumidity.readHumidity();
  float temp = myHumidity.readTemperature();

  Serial.print("Time:");
  Serial.print(millis());
  Serial.print(" Temperature:");
  Serial.print(temp, 1);
  Serial.print("C");
  Serial.print(" Humidity:");
  Serial.print(humd, 1);
  Serial.print("%");

  Serial.println();
  Wire.endTransmission();
}

void lcddisplay() {

  display.clearDisplay();

  display.setTextSize(1);       // Normal 1:1 pixel scale
  display.setTextColor(WHITE);  // Draw white text
  display.setCursor(0, 0);      // Start at top-left corner
  display.println(F("Ankush Here"));
  // display.println(F(myString));
  // display.println(F(data));
  display.display();
  delay(2000);
}

void Moisture() {
  int value = analogRead(sensor_pin);  // read the analog value from sensor

  Serial.print("The soil Moisture is : ");
  Serial.println(value);
  Serial.println(" ");
  delay(1000);

  // Moisturedata = analogRead(sensor_pin);
  // Moisturedata = map(Moisturedata, 550, 0, 0, 100);
  // Serial.print("Moisture : ");
  // Serial.print(Moisturedata);
  // Serial.println("%");
  // delay(1000);
}

// I2c Scan Function
void Itwoc() {
  int nDevices = 0;

  Serial.println("Scanning...");

  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of
    // the Wire.endTransmission to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      ++nDevices;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  delay(5000);  // Wait 5 seconds for next scan
}


void loop() {

  Itwoc();
  Tempcal();
  Moisture();
  lcddisplay();
}
