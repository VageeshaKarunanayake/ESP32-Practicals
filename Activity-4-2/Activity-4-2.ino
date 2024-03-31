#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

const char* ssid = "Test";
const char* password = "12345678";

const char* http_username = "admin";
const char* http_password = "password";

AsyncWebServer server(80);

// Define the pin numbers for the 8 LEDs
int ledPins[] = {2, 4, 5, 18, 19, 21, 22, 23};

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected...");
  Serial.println(WiFi.localIP());

  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    
    String html = "<html><body>";
    html += "<h1>8-Bit Counter</h1>";
    html += "<form action='/update' method='post'>";
    html += "Enter a decimal value (0-255): <input type='number' name='value' min='0' max='255'><br>";
    html += "<input type='submit' value='Submit'>";
    html += "</form>";
    html += "</body></html>";
    
    request->send(200, "text/html", html);
  });

  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    
    if (request->hasArg("value")) {
      int decimalValue = request->arg("value").toInt();
      updateLEDs(decimalValue);
    }
    
    request->send(200, "text/plain", "LEDs updated!");
  });

  server.begin();
}

void loop() {
  // Your main loop code here
}

// Convert decimal to 8-bit binary and update LEDs
void updateLEDs(int decimalValue) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(ledPins[i], (decimalValue & (1 << i)) ? HIGH : LOW);
  }
}