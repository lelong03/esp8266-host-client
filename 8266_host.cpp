#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "wifi_name";
const char *password = "wifi_password";


ESP8266WebServer server(80);
IPAddress local_IP(192, 168, 1, 184);

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

    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();

    // when the server receives a request with /turn_on/ in the string then run the handleOn function
    server.on("/turn_on/", HTTP_GET, handleOn); 

    // when the server receives a request with /turn_off/ in the string then run the handleOff function
    server.on("/turn_off/", HTTP_GET, handleOff); 

    server.begin();
}

void loop() {
  server.handleClient();
}