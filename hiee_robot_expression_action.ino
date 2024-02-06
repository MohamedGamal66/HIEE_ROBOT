/* Including libraries  */
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_PWMServoDriver.h"
#include "XT_DAC_Audio.h"
#include "Wire.h"
#include <WiFi.h>
#include "hiee-audio.h"
#include "expressionbitmap.h"

/* wifi confg */
const char *ssid = " HIEE ";
const char *password = "123456789";
WiFiServer server(80);

/* Called this way, it uses the default address 0x40 */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SCREEN_WIDTH 128 /* OLED display width, in pixels */
#define SCREEN_HEIGHT 64 /* OLED display height, in pixels */

#define min_pulse_width 125   /* This is the 'minimum' pulse length count (out of 4096) */
#define max_pulse_width 575   /* This is the 'maximum' pulse length count (out of 4096) */
#define frequency 60   /* Defining frequency as 50 Hz */

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  /* Declaration for an SSD1306 display connected to I2C (SDA, SCL pins) */
/*The (-1) parameter means that your OLED display doesn’t have a RESET pin */

XT_DAC_Audio_Class DacAudio(25, 0);   /* Create the main player class object */
/* Use GPIO 25, one of the 2 DAC pins and timer 0  */
 String data;

void setup()
{
  Serial.begin(115200);
  Serial.println("GPIO test!");
    // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  // Start the server
  server.begin();
  
  //pwm-servo-begin
  pwm.begin();
  pwm.setPWMFreq(frequency);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    /* Initialize the OLED display with the begin() method */ /*Address 0x3C for 128x64  */
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  initial_exp(); /* Initial face expression */
  initial_position(); /* Initial position */
  delay(2000);
}
/* moving method */
void setServo(int servo, int angle) {
  int pulselength;
  pulselength  = map(angle, 0, 180, min_pulse_width, max_pulse_width); /* Converting degree of rotation to pulse length */
  pwm.setPWM(servo, 0, pulselength); /* Passing pulse length */
}
/* expressions */
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
void angry_exp() {
  display.clearDisplay();
  display.drawBitmap(0, 0, myBitmap1, 128, 64, 1);
  display.display();
}
/* initial-position */
void initial_position() {

  setServo(8, 0); //right hand
  setServo(9, 0);
  setServo(10, 20);
  setServo(0, 180); //left hand
  setServo(1, 180);
  setServo(2, 160);
}

void loop(){
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    
    // Read the data from the client
    data = client.readStringUntil('\r');
    Serial.println("Received data: " + data);

    // You can process the text data here

    // Send a response to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("Data received successfully!");
    
    // Close the connection
    client.stop();
  }
  
  if (data != ""){
      if (data.equalsIgnoreCase("hi")|| data.equalsIgnoreCase("hello")) {
        void hello_humanoids();
        XT_Wav_Class ForceWithYou(hellohumanoids);
        talk_exp();
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
          DacAudio.FillBuffer();
          if (ForceWithYou.Playing == false)
            DacAudio.Play(&ForceWithYou);
          delay(10);
        }
        for (int i = 0; i <= 60; i++) {
          setServo(10, 120 - i);
          DacAudio.FillBuffer();
          if (ForceWithYou.Playing == false)
            DacAudio.Play(&ForceWithYou);
            
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
        if (i <= 120) {
          DacAudio.FillBuffer();
          if (ForceWithYou.Playing == false)
            DacAudio.Play(&ForceWithYou);
        }
        delay(10);
      }
      initial_exp();
              void my_name_is_HIEE();
              (mynameisHIEE);
               DacAudio.FillBuffer();
          if (ForceWithYou.Playing == false)
            DacAudio.Play(&ForceWithYou);
        talk_exp();
      initial_exp();
     }
  }
}
