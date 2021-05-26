#include <ESP8266WiFi.h>

// wifi config
const char *ssid = "wifi_name";
const char *password = "wifi_password";

// ip config
IPAddress localIP(192, 168, 1, 185);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

// host config
const char *hostIP = "192.168.1.184";
const int hostPin = 5;

void setup() {
    Serial.begin(115200);
    delay(10);

    // Configures static IP address
    if (!WiFi.config(localIP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("STA Failed to configure");
    }

    // Explicitly set the ESP8266 to be a WiFi-client
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Wifi try to connect
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    // Turn on pin 5
    remoteCall(hostIP, hostPin, true);
    delay(3000);

    // Turn off pin 5
    remoteCall(hostIP, hostPin, false);
    delay(3000);
}

void remoteCall(const char *host, int pin, bool status) {
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