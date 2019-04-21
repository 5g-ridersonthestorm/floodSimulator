/*************************************************************
  
  Water pump controller, uses NodeMCU ESP8266 board to relay-control
  two water pumps.
  
 *************************************************************/


#include <ESP8266WiFi.h>

// Variable to store the HTTP request
String header;

// Assign output variables to GPIO pins
const int relayPin1 = 16;  // D0
const int relayPin2 = 5;   // D1

// Auxiliar variables to store the current output state
String pump1State = "off";
String pump2State = "off";

// Set web server port number to 80
WiFiServer server(80);


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "James Redmi Note5";
char pass[] = "********";
/*
char ssid[] = "h20-PumpServer";
char pass[] = "f-lowh20";
*/

// Assign output variables to GPIO pins
// const int relayPin1 = 5;
// const int relayPin2 = 4;


void setup() {
  Serial.begin(115200);

  pinMode(relayPin1, OUTPUT);
  digitalWrite(relayPin1, LOW);  //Set the pin to LOW (0V)
  delay(1500);                   //Delay 5 seconds
  
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin2, LOW);  //Set the pin to LOW (0V)
  delay(1500);                   //Delay 5 seconds
 
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}




void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /pump/in") >= 0) {
              Serial.println("Pump water in");
              pump1State = "on";
              digitalWrite(relayPin1, HIGH);
              pump2State = "off";
              digitalWrite(relayPin2, LOW);
            } else if (header.indexOf("GET /pump/out") >= 0) {
              Serial.println("Pump water out");
              pump2State = "on";
              digitalWrite(relayPin2, HIGH);
              pump1State = "off";
              digitalWrite(relayPin1, LOW);
            } else if (header.indexOf("GET /pump/off") >= 0) {
              Serial.println("All off");
              pump2State = "off";
              digitalWrite(relayPin2, LOW);
              pump1State = "off"; 
              digitalWrite(relayPin1, LOW);
            } 
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: monospace; color:#ffffff;  display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button1 { background-color: #0BFE33; border-radius: 8px; width:75%;  color: white; padding: 16px 40px; text-decoration: none; font-size: 40px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #ff9999; border-radius: 8px; width:75%;color: white; padding: 16px 40px; text-decoration: none; font-size: 40px; margin: 2px; cursor: pointer;}");
            client.println(".button3 {background-color: #FE0B0B; border-radius: 8px;width:75%;color: white; padding: 16px 40px; text-decoration: none; font-size: 40px; margin: 2px; cursor: pointer;}</style></head>");    
         
            // Web Page Heading
            client.println("<body bgcolor=\"#1a1a1a\"><h1>Water Pump Controller</h1>");
            client.println("<hr>");
            client.println("<hr>");
            
            // Display current state, and ON/OFF buttons for GPIO 16  
            client.println("<p>Left pump " + pump1State + "</p>");
            client.println("<p>Right pump " + pump2State + "</p>");
            
            // If the pump1State is off, it displays the 'Water in' button       
            if (pump1State=="off") {
              client.println("<p><a href=\"/pump/in\"><button class=\"button1\">Water in</button></a></p>");
            } else {
              client.println("<p><a href=\"/pump/out\"><button class=\"button2\">Water out</button></a></p>");
            } 
            
            client.println("<p><a href=\"/pump/off\"><button class=\"button3\">Off</button></a></p>");
               
            client.println("<hr>");
            client.println("<hr>");
            
            client.println("<center><small>&copy; 5G Riders on the Storm</small></center>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

 


