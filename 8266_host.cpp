#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Assign output variables to GPIO pins
const int output5 = 5;

// Wifi ssid and password
const char *ssid = "wifi_name";
const char *password = "wifi_password";

ESP8266WebServer server(80);
IPAddress localIP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

void handleOn() {
    if (server.hasArg("pin")) {
        int pin = server.arg("pin").toInt();
        digitalWrite(pin, 1);
        server.send(200, "text/html", "Turned On");
    }
}

void handleOff() {
    if (server.hasArg("pin")) {
        int pin = server.arg("pin").toInt();
        digitalWrite(pin, 0);
        server.send(200, "text/html", "Turn Off");
    }
}

void setup() {
    delay(1000);
    pinMode(output5, OUTPUT);
    digitalWrite(output5, LOW);

    // Configures static IP address
    if (!WiFi.config(localIP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("STA Failed to configure");
    }

    // Setup access point
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(myIP);

    // when the server receives a request with /turn_on/ in the string then run the handleOn function
    server.on("/turn_on/", HTTP_GET, handleOn); 

    // when the server receives a request with /turn_off/ in the string then run the handleOff function
    server.on("/turn_off/", HTTP_GET, handleOff); 

    server.begin();
}

void loop() {
  server.handleClient();
}
