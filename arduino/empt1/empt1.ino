
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

YunServer server;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define NUMPIXELS      1

int sensorPin = A0;
int sensorValue = 0;

const int motorPin =  A1;//12;
const int ledPIN = A2;//13;

bool badBoy = false;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, A2, NEO_GRB + NEO_KHZ800);
int delayval = 1000; // delay for half a second

void setup() {
  // Bridge startup
  pinMode(A0, INPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
//  pinMode(13, OUTPUT);
  pinMode(12,OUTPUT);
  pixels.begin();
  
  Bridge.begin();


  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
 
  pixels.setPixelColor(0, pixels.Color(0, 150, 0)); // Moderately bright green color.
  pixels.show();
}

void loop() {
  YunClient client = server.accept();
  if (client) {
    process(client);
    client.stop();
  }
  if(badBoy){
      pixels.setPixelColor(0, pixels.Color(150,0, 0)); // Moderately bright green color.
      pixels.show();
  }else{
    pixels.setPixelColor(0, pixels.Color(0,150, 0)); // Moderately bright green color.
    pixels.show();
  }

  delay(50); // Poll every 50ms
}

void process(YunClient client) {

  String command = client.readStringUntil('/');

  if (command == "digital") {
    digitalCommand(client);
  }

  if (command == "analog") {
    analogCommand(client);
  }

  if (command == "mode") {
    modeCommand(client);
  }
}

void digitalCommand(YunClient client) {
  int pin, value;
  
  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  if (client.read() == '/') {
    value = client.parseInt();  
    digitalWrite(pin, value);
    if(value == 1){
      badBoy = true;  
    }else{
      badBoy = false;
    }
    
    
  }
  else {
    value = digitalRead(pin);
  }

  // Send feedback to client
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.println(value);

  // Update datastore key with the current pin value
  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}

void analogCommand(YunClient client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/analog/5/120"
  if (client.read() == '/') {
    // Read value and execute command
    value = client.parseInt();
    analogWrite(pin, value);
    if(value == 255){
      badBoy = true;  
    }else{
      badBoy = false;
    }
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  }
  else {
    // Read analog pin
    value = analogRead(pin);

    // Send feedback to client
    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);

    // Update datastore key with the current pin value
    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}

void modeCommand(YunClient client) {
  int pin;

  // Read pin number
  pin = client.parseInt();

  // If the next character is not a '/' we have a malformed URL
  if (client.read() != '/') {
    client.println(F("error"));
    return;
  }

  String mode = client.readStringUntil('\r');

  if (mode == "input") {
    pinMode(pin, INPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as INPUT!"));
    return;
  }

  if (mode == "output") {
    pinMode(pin, OUTPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as OUTPUT!"));
    return;
  }

  client.print(F("error: invalid mode "));
  client.print(mode);
}
//IPADDRESS/ARDUINO/ANALOG/PIN --> READ ANALOG
////IPADDRESS/ARDUINO/DIGITAL/PIN/1 --> WRITE DIGITAL


