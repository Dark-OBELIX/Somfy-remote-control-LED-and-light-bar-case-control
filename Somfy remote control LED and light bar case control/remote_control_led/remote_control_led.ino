#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>

// Define button pins for the relays
const int button1 = 32;
const int button2 = 13;
const int button3 = 4;

// Define relay pins
const int relay1 = 26;
const int relay2 = 27;
const int relay3 = 14;
const int relayLightbar = 12; // Relay for the Lightbar

// Define LED button pins
#define BUTTON_PIN1 25
#define BUTTON_PIN2 33

// Define LED strip pins
#define LED_PIN1 22
#define LED_PIN2 5
#define NUM_LEDS 150

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];

// LED button states
bool button1State = false;
bool button2State = false;
bool lastButton1State = HIGH;
bool lastButton2State = HIGH;

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
        #led1 {
            background-color: #f5f505;
        }
        #led2 {
            background-color: #79d2e6;
        }
        #lightbar {
            background-color: #FF4500; /* Orange color */
        }
    </style>
</head>
<body>
    <h1>SmartHome Control</h1>
    <h2>Room Control</h2>
    <button id="relay1" onclick="fetch('/relay1')">Up</button>
    <button id="relay2" onclick="fetch('/relay2')">Pause</button>
    <button id="relay3" onclick="fetch('/relay3')">Down</button>
    <button id="led1" onclick="fetch('/led1')">Office LED</button>
    <button id="led2" onclick="fetch('/led2')">Printer LED</button>
    <button id="lightbar" onclick="fetch('/lightbar')">Lightbar</button>
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

void handleLed1() {
  button1State = !button1State; // Toggle state
  Serial.print("LED Strip 1 state: ");
  Serial.println(button1State);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds1[i] = button1State ? CRGB::White : CRGB::Black;
  }
  FastLED.show();
  server.send(200, "text/plain", button1State ? "LED Strip 1 ON" : "LED Strip 1 OFF");
}

void handleLed2() {
  button2State = !button2State; // Toggle state
  Serial.print("LED Strip 2 state: ");
  Serial.println(button2State);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds2[i] = button2State ? CRGB::White : CRGB::Black;
  }
  FastLED.show();
  server.send(200, "text/plain", button2State ? "LED Strip 2 ON" : "LED Strip 2 OFF");
}

void handleLightbar() {
  Serial.println("Lightbar request");
  
  digitalWrite(relayLightbar, LOW); // Activate the relay (circuit closed)
  delay(200); // Keep the relay activated for 200 ms
  digitalWrite(relayLightbar, HIGH); // Deactivate the relay (circuit open)
  
  server.send(200, "text/plain", "Lightbar activated for 200 ms.");
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
  pinMode(relayLightbar, OUTPUT); // Set relay for Lightbar as output

  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relayLightbar, HIGH); // Deactivate the Lightbar relay by default (circuit open)

  FastLED.addLeds<WS2811, LED_PIN1, GRB>(leds1, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, LED_PIN2, GRB>(leds2, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255); // Set maximum brightness

  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);

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
  server.on("/led1", handleLed1);
  server.on("/led2", handleLed2);
  server.on("/lightbar", handleLightbar); // New route for Lightbar

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

  bool currentButton1State = digitalRead(BUTTON_PIN1);
  bool currentButton2State = digitalRead(BUTTON_PIN2);

  if (currentButton1State == LOW && lastButton1State == HIGH) {
    button1State = !button1State;
    Serial.print("Button 1 state: ");
    Serial.println(button1State);
  }

  if (currentButton2State == LOW && lastButton2State == HIGH) {
    button2State = !button2State;
    Serial.print("Button 2 state: ");
    Serial.println(button2State);
  }

  lastButton1State = currentButton1State;
  lastButton2State = currentButton2State;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds1[i] = button1State ? CRGB::White : CRGB::Black;
    leds2[i] = button2State ? CRGB::White : CRGB::Black;
  }

  FastLED.show();
  server.handleClient();
}
