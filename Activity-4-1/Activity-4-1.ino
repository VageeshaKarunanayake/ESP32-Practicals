#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

const char* ssid = "Test";
const char* password = "12345678";

const char* http_username = "admin";
const char* http_password = "password";

AsyncWebServer server(80);

int ledPin1 = 16;
int ledPin2 = 14;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected...");
  Serial.println(WiFi.localIP());

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(200, "text/html", "Control LEDs: <a href='/on'>Turn On</a> <a href='/off'>Turn Off</a>");
  });

  // Route to turn the LED on
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    request->redirect("/");
  });

  // Route to turn the LED off
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    request->redirect("/");
  });

  // Route to change LED intensity
  server.on("/intensity", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    String value = request->arg("value");
    int intensity = value.toInt();
    Serial.println(intensity);
    analogWrite(ledPin1, intensity);
    analogWrite(ledPin2, intensity);
    request->send(200, "text/plain", "Intensity set to " + value);
  });

  server.begin();
}

void loop() {
  // Your main loop code here
}