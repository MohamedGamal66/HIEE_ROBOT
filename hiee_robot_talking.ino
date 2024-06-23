/* Including libraries  */
#include <Arduino.h>
#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"
#include "hiee-audio.h"
#include "expressionbitmap1.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
WiFiClient client;
ESP8266WebServer server(80); /* defining server port */

/* AudioGeneratorWAV *wav;
  AudioFileSourcePROGMEM *file;
  AudioOutputI2SNoDAC *out; */

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/* PWM constants for servo control */
#define min_pulse_width 102   /* Defining minimum pulsewidth */
#define max_pulse_width 512   /* Defining maximum pulsewidth */
#define frequency 50   /* Defining frequency as 50 Hz */

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
/* OLED display dimensions */
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "hiee"; /* Your SSID */
const char* password = "12345678"; /* Your Password */

String  data = "";
Servo headservo;

void setup()
{
  Serial.begin(115200);
  Serial.println("GPIO test!");

    /* Begin display setup */
  if (!display.begin(0x3C)) { // Initialize with I2C address 0x3C
    Serial.println(F("OLED init failed"));
    while (1); // Loop forever if display initialization fails
  }
  display.display(); // Show initial buffer
  display.clearDisplay(); // Clear the buffer

  pwm.begin();
  pwm.setPWMFreq(frequency);
  delay(5000);
  headservo.attach(0);/* Pin D3 */
  initial_position();
  initial_exp();

  /* Connecting to WiFi */
  connectToWiFi();

  server.on("/", handleRequest);
  server.onNotFound(handleRequest);

  /* Starting Server */
  server.begin();
  Serial.println("Server started");

}

/* The programed instraction can robot do */
void loop() {
  server.handleClient();
  data = server.arg("data");

  if (data != "") {

    if (data.equalsIgnoreCase("hi") || data.equalsIgnoreCase("hello")) {
      say_hi(); 
    }
    if (data.equalsIgnoreCase("what is your name") || data.equalsIgnoreCase("your name")) {
      my_name_hiee();
    }
    
    if (data.equalsIgnoreCase("What are the institute specializations") || data.equalsIgnoreCase("institute categories")) {
     institute_categories();
    }
    if (data.equalsIgnoreCase("Who founded the institute") || data.equalsIgnoreCase("when the institute founded")) {
      talk_exp();
      institute_founder();
    }
    if (data.equalsIgnoreCase("Do you know Dean") || data.equalsIgnoreCase("Who is the dean of the institute")) {
      the_dean();
    }
  }
  data = "";
}

/* Reuest and resonse handling */
void handleRequest() {
  if ( server.hasArg("data") ) {
    Serial.println(server.arg("Key"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}

/* WiFi handling */
void connectToWiFi() {
  delay(3000);
  WiFi.disconnect();
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(500);
    Serial.print("...");
  }
  Serial.println("Connected");
  Serial.println("Local IP is : ");
  Serial.print((WiFi.localIP().toString()));
  Serial.println("\n");
  delay(5000);
}

/* Servo handling */
void setServo(int servo, int angle) {
  int pulselength;
  pulselength  = map(angle, 0, 180, min_pulse_width, max_pulse_width); /* Converting degree of rotation to pulse length */
  pwm.setPWM(servo, 0, pulselength); /* Passing pulse length */
}

/******************** initial position ********************/
void initial_position() {
  headservo.write(90);
  setServo(8, 0);
  setServo(9, 0);
  setServo(10, 20);
  setServo(11, 90);
  setServo(12, 150);
  setServo(13, 30);
  setServo(14, 30);
  setServo(15, 90);

  setServo(0, 180);
  setServo(1, 180);
  setServo(2, 160);
  setServo(3, 90);
  setServo(4, 30);
  setServo(5, 150);
  setServo(6, 150);
  setServo(7, 90);
}
/******************** say hi ********************/
void say_hi() {
  talk_exp();
  AudioGeneratorWAV *wav;
  AudioFileSourcePROGMEM *file;
  AudioOutputI2SNoDAC *out;

  audioLogger = &Serial;
  file = new AudioFileSourcePROGMEM( hi, sizeof(hi) );
  out = new AudioOutputI2SNoDAC();
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);

  for (int i = 0; i <= 180; i++) {
    setServo(8, 0 + i);
    if (i <= 90) {
      setServo(9, 0 + i);
    }
    if (i <= 40) {
      setServo(10, 20 + i);
    }
    delay(10);
  }
  for (int i = 1; i <= 1; i++) {
    for (int i = 0; i <= 60; i++) {
      setServo(10, 60 + i);
      if (!wav->loop()) wav->stop();
      delay(10);
    }
    for (int i = 0; i <= 60; i++) {
      setServo(10, 120 - i);
      if (!wav->loop()) wav->stop();
      delay(10);
    }
  }
  for (int i = 0; i <= 180; i++) {
    setServo(8, 180 - i);
    if (i <= 90) {
      setServo(9, 90 - i);
    }
    if (i <= 40) {
      setServo(10, 60 - i);
    }
    if (!wav->loop()) wav->stop();
    delay(10);
  }
  initial_exp();
  delay(3000);
}

/******************** my name is hiee ********************/
void my_name_hiee() {
  talk_exp();
  AudioGeneratorWAV *wav;
  AudioFileSourcePROGMEM *file;
  AudioOutputI2SNoDAC *out;

  audioLogger = &Serial;
  file = new AudioFileSourcePROGMEM( myname, sizeof(myname) );
  out = new AudioOutputI2SNoDAC();
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);

  for (int i = 0; i <= 90; i++) {
    setServo(8, 0 + i);
    if (i <= 20) {
      setServo(9, 0 + i);
    }
    delay(12);
  }
  for (int i = 0; i <= 70; i++) {
    setServo(10, 20 + i);
    if (!wav->loop()) wav->stop();
    delay(12);
  }
  for (int i = 0; i <= 70; i++) {
    setServo(10, 90 - i);
    if (!wav->loop()) wav->stop();
    delay(12);
  }
  for (int i = 0; i <= 90; i++) {
    setServo(8, 90 - i);
    if (i <= 20) {
      setServo(9, 20 - i);
    }
    if (!wav->loop()) wav->stop();
    delay(12);
  }
  initial_exp();
  delay(3000);
}
/******************** institute categories ********************/
void institute_categories() {
  talk_exp();
  AudioGeneratorWAV *wav;
  AudioFileSourcePROGMEM *file;
  AudioOutputI2SNoDAC *out;

  audioLogger = &Serial;
  file = new AudioFileSourcePROGMEM( comandcom, sizeof(comandcom) );
  out = new AudioOutputI2SNoDAC();
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);

  for (int i = 0; i <= 90; i++) {
    setServo(8, 0 + i);
    if (i <= 20) {
      //setServo(9, 0 + i);
      setServo(1, 180 - i);
      setServo(2, 160 - i);
    }
    if (!wav->loop()) wav->stop();
    delay(22);
  }
  for (int i = 0; i <= 90; i++) {
    setServo(8, 90 - i);
    if (i <= 20) {
      //setServo(9, 20 - i);
      setServo(1, 160 + i);
      setServo(2, 140 + i);
    }
    if (!wav->loop()) wav->stop();
    delay(22);
  }
  initial_exp();
  delay(3000);
}

/******************** institute founder********************/
void institute_founder() {
  talk_exp();
  AudioGeneratorWAV *wav;
  AudioFileSourcePROGMEM *file;
  AudioOutputI2SNoDAC *out;

  audioLogger = &Serial;
  file = new AudioFileSourcePROGMEM( founded, sizeof(founded) );
  out = new AudioOutputI2SNoDAC();
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);

  for (int i = 0; i <= 90; i++) {
    setServo(8, 0 + i);
    if (i <= 20) {
      setServo(9, 0 + i);
      setServo(1, 180 - i);
      setServo(2, 160 - i * 2);
    }
    if (!wav->loop()) wav->stop();
    delay(20);
  }
  for (int i = 0; i <= 70; i++) {
    setServo(10, 20 + i);
    if (!wav->loop()) wav->stop();
    delay(20);
  }
  for (int i = 0; i <= 70; i++) {
    setServo(10, 90 - i);
    if (!wav->loop()) wav->stop();
    delay(20);
  }
  for (int i = 0; i <= 90; i++) {
    setServo(8, 90 - i);
    if (i <= 20) {
      setServo(9, 20 - i);
      setServo(1, 160 + i);
      setServo(2, 120 + i * 2);
    }
    if (!wav->loop()) wav->stop();
    delay(20);
  }
  initial_exp();
  delay(3000);
}

/******************** The Dean ********************/
void thank_you() {
  the_dean();
  AudioGeneratorWAV *wav;
  AudioFileSourcePROGMEM *file;
  AudioOutputI2SNoDAC *out;

  audioLogger = &Serial;
  file = new AudioFileSourcePROGMEM( drahmed, sizeof(drahmed) );
  out = new AudioOutputI2SNoDAC();
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);

  for (int i = 0; i <= 60; i++) {
    setServo(8, 0 + i);
    if (i <= 20) {
      setServo(1, 180 - i);
    }
    if (!wav->loop()) wav->stop();
    delay(22);
  }
  for (int i = 0; i <= 60; i++) {
    setServo(8, 60 - i);
    if (i <= 20) {
      setServo(1, 160 + i);
    }
    if (!wav->loop()) wav->stop();
    delay(22);
  }
  initial_exp();
  delay(3000);
}

/* robot oled exp*/
void initial_exp() {
  display.clearDisplay();
  display.drawBitmap(0, 0, myBitmap6, 128, 64, 1);
  display.display();
}
void talk_exp() {
  display.clearDisplay();
  display.drawBitmap(0, 0, myBitmap12, 128, 64, 1);
  display.display();
}
