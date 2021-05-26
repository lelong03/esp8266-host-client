#include <ESP8266WiFi.h>

const char *ssid = "wifi_name";
const char *password = "wifi_password";

void setup() {
    Serial.begin(115200);
    delay(10);

    // Explicitly set the ESP8266 to be a WiFi-client
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void loop() {
    // Turn on pin 5
    remoteCall("192.168.1.184", 5, true);

    delay(5000);

    // Turn off pin 5
    remoteCall("192.168.1.184", 5, false);
}

void remoteCall(char *host, int pin, bool status) {
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;

    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed: " + *host);
        return;
    }

    // We now create a URI for the request. Something like /turn_on/?pin=5
    String url;
    if (status) {
        url = "/turn_on/";
    } else {
        url = "/turn_off/";
    }

    url += "?pin=";
    url += pin;

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
        }
    }
}