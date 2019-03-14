/*************************************************************
  Use Blynk on Cloud to turn on and off relays for water pumps
  
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "6292c6d452fd4dec9a41a29ea504c226";  // this configured on Android app

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";   // Add SSID to be used in Nice
char pass[] = "";   // Add passwd to be used in Nice

int relayPin1 = 16;  // D0
int relayPin2 = 5;   // D1

void setup()
{
  // Debug console
  Serial.begin(115200);

  pinMode(relayPin1, OUTPUT);
  digitalWrite(relayPin1, LOW);  //Set the pin to LOW (0V)
  delay(1500);                   //Short delay to let cheapo electronics settle down 
  
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin2, LOW);  //Set the pin to LOW (0V)
  delay(1500);                   //Short delay to let cheapo electronics settle down
  
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
