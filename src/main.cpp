#include <WiFi.h>

// connect to phone hotspot
const char* ssid = "";
const char* password = "";

// set server port
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// to check for timeout
unsigned long current_time = millis();
unsigned long previous_time = 0;
const long timeout_ms = 2000;

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
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
    WiFiClient client = server.available();
    String current_line = "";
    char c;

    if (client) {
        current_time = millis();
        previous_time = current_time;
        Serial.println("New Client.");
        while (client.connected() && current_time - previous_time <= timeout_ms) {
            current_time = millis();
            if (client.available()) {
                c = client.read();
                Serial.write(c);
                header += c;
                if (c == '\n') {
                    if (current_line.length() == 0) {
                        // http header
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        // http page content
                        client.println(
                                "<!DOCTYPE html>"
                                "<html lang='en'>"
                                "   <body>"
                                "       <h1>Hello</h1>"
                                "   </body>"
                                "</html>"
                                );
                        client.println();

                        break;
                    } else {
                        current_line = "";
                    }
                } else if (c != '\r') {
                    current_line += c;
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

