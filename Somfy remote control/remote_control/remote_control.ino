#include <WiFi.h>
#include <WebServer.h>

// Define button pins for the relays
const int button1 = 32;
const int button2 = 13;
const int button3 = 4;

// Define relay pins
const int relay1 = 26;
const int relay2 = 27;
const int relay3 = 14;

// Replace with your Wi-Fi network credentials
const char* ssid = "your_SSID_here";
const char* password = "your_password_here";

// Create a web server on port 80
WebServer server(80);

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>SmartHome</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            background-color: #f0f0ed;
            padding: 20px;
        }
        h1 {
            font-size: 24px;
            margin-top: 20px;
        }
        h2 {
            font-size: 20px;
            color: #555;
        }
        button {
            display: block;
            width: 80%;
            margin: 20px auto;
            padding: 20px;
            font-size: 18px;
            border: none;
            color: white;
            border-radius: 5px;
            cursor: pointer;
        }
        button:hover {
            opacity: 0.9;
        }
        #relay1 {
            background-color: #4CAF50;
        }
        #relay2 {
            background-color: #555555;
        }
        #relay3 {
            background-color: #f44336;
        }
    </style>
</head>
<body>
    <h1>SmartHome Control</h1>
    <h2>Room Control</h2>
    <button id="relay1" onclick="fetch('/relay1')">Up</button>
    <button id="relay2" onclick="fetch('/relay2')">Pause</button>
    <button id="relay3" onclick="fetch('/relay3')">Down</button>
</body>
</html>
  )rawliteral";

  server.send(200, "text/html; charset=UTF-8", html);
}

void handleRelay1() {
  Serial.println("Relay 1 request");
  digitalWrite(relay1, HIGH); // Activate relay 1
  delay(500);
  digitalWrite(relay1, LOW); // Deactivate relay 1
  server.send(200, "text/plain", "Toggled Relay 1");
}

void handleRelay2() {
  Serial.println("Relay 2 request");
  digitalWrite(relay2, HIGH); // Activate relay 2
  delay(500);
  digitalWrite(relay2, LOW); // Deactivate relay 2
  server.send(200, "text/plain", "Toggled Relay 2");
}

void handleRelay3() {
  Serial.println("Relay 3 request");
  digitalWrite(relay3, HIGH); // Activate relay 3
  delay(500);
  digitalWrite(relay3, LOW); // Deactivate relay 3
  server.send(200, "text/plain", "Toggled Relay 3");
}

void reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Lost Wi-Fi connection. Attempting to reconnect...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to Wi-Fi...");
    }
    Serial.println("Reconnected to Wi-Fi");
  }
}

void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);

  Serial.begin(9600);
  Serial.println("ESP32 ready");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  server.on("/", handleRoot);
  server.on("/relay1", handleRelay1);
  server.on("/relay2", handleRelay2);
  server.on("/relay3", handleRelay3);

  server.begin();
}

void loop() {
  reconnectWiFi();

  int state1 = digitalRead(button1);
  int state2 = digitalRead(button2);
  int state3 = digitalRead(button3);

  if (state1 == LOW) {
    Serial.println("Button 1 is pressed");
    digitalWrite(relay1, HIGH); // Activate relay 1
  } else {
    digitalWrite(relay1, LOW); // Deactivate relay 1
  }

  if (state2 == LOW) {
    Serial.println("Button 2 is pressed");
    digitalWrite(relay2, HIGH); // Activate relay 2
  } else {
    digitalWrite(relay2, LOW); // Deactivate relay 2
  }

  if (state3 == LOW) {
    Serial.println("Button 3 is pressed");
    digitalWrite(relay3, HIGH); // Activate relay 3
  } else {
    digitalWrite(relay3, LOW); // Deactivate relay 3
  }

  server.handleClient();
}
