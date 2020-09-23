/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

int pos = 0; 


const char *ssid = "********";
const char *password = "**********";

ESP8266WebServer server(80);


void handleRoot() {
  char temp[600];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 600,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='20'/>\
    <title>KIKI's Gateway</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from KIKI!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <button type='button' onclick=\"window.location.replace('./lock')\">Lock!!</button><br>\
    <button type='button' onclick=\"window.location.replace('./unlock')\">Unlock!!</button><br><br>\
    <button type='button' onclick=\"window.location.replace('./instant')\">Instant open!!</button><br><br>\
  </body>\
</html>",

           hr, min % 60, sec % 60
          );
  server.send(200, "text/html", temp);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN,  HIGH); // turn it off
  pinMode(4, OUTPUT); 
  digitalWrite(4, HIGH);           // Turn on D2  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.hostname());
  WiFi.hostname("KiKiGateway");

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/lock", lock);
  server.on("/unlock", unlock);
  server.on("/instant", instantOpen);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

void lock(){

  char temp[400];
   snprintf(temp, 400,"<html>\
  <head>\
    <meta http-equiv=\"refresh\" content=\"0;URL='./'\" />\  
    <title>Kiki's gateway has been locked!!</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>The gateway is LOCKED!</h1>\
  </body>\
</html>");
  
  server.send(200, "text/html", temp);
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on 
  digitalWrite(4, LOW);             // turn off pin D2
  delay(150);
}

void unlock(){

  char temp[400];
   snprintf(temp, 400,"<html>\
  <head>\
    <meta http-equiv=\"refresh\" content=\"0;URL='./'\" />\  
    <title>Kiki's gateway is OPEN!!</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>The gateway is OPEN!</h1>\
  </body>\
</html>");
  
  server.send(200, "text/html", temp);
  digitalWrite(LED_BUILTIN,  HIGH);   // Turn the LED on 
  digitalWrite(4, HIGH);             // turn on pin D2
  delay(150);
}

void instantOpen(){

  char temp[400];
   snprintf(temp, 400,"<html>\
  <head>\
    <meta http-equiv=\"refresh\" content=\"0;URL='./'\" />\  
    <title>Kiki's gateway is OPEN!!</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>The gateway is always OPEN!</h1>\
  </body>\
</html>");
  
  server.send(200, "text/html", temp);
  digitalWrite(4, LOW);             // turn off pin D2
  delay(1000);
  digitalWrite(LED_BUILTIN,  LOW);   // Turn the LED on 
  digitalWrite(4, HIGH);             // turn on pin D2
  delay(3500);
  digitalWrite(4, LOW);             // turn off pin D2
  digitalWrite(LED_BUILTIN,  HIGH);   // Turn the LED off 
}
