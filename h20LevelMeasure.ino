#define TRIG_PIN 2            // D4
#define ECHO_PIN 0            // D3
#define SPEED_OF_SOUND  0.034 // 340 m/s or 0.034 cm/µs

long duration;
int distance;

void setup(){
  pinMode(TRIG_PIN, OUTPUT); // Sets the TRIG_PIN as an Output
  pinMode(ECHO_PIN, INPUT); // Sets the ECHO_PIN as an Input
  Serial.begin(115200);
}

void loop() {

  char shadow[150];


   // Clears the TRIG_PIN
   digitalWrite(TRIG_PIN, LOW);
   delayMicroseconds(2);

   // Set the trigger pin HIGH state for 10 micro seconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance (end to end time, therefore divide by 2)
  distance = (duration*SPEED_OF_SOUND)/2;
  
  // Prints the distance on the Serial Monitor
          
  sprintf(shadow, 
        "\n{\"state\":{\"reported\":{\"Device\":\"HCSR04\",\"DeviceID\":\"H2OSensor21632\",\"Lat\":\"44.438615\",\"Long\":\"8.889838\",\"distance_in_CM\":\"%d\"}}}", 
          distance );

  Serial.print(shadow);
  delay(5000);
 
}
