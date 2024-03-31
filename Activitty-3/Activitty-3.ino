#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char* ssid = "Test";
const char* password = "12345678";

const long utcOffsetInSeconds = 5*3600+1800;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

WiFiServer server(12345);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected...");
  Serial.println(WiFi.localIP());
  timeClient.begin();

  server.begin();
}

void loop() {
  timeClient.update();
  String currentTime = timeClient.getFormattedTime();

  WiFiClient client = server.available();
  if (client) {
    Serial.print(currentTime);
    Serial.print(", ");
    
    // Read a message from the client
    String message = client.readStringUntil('\n');
    Serial.println("Client: " + message);

    // Send a response back to the client
    client.println("Server: Received your message - " + message);
    client.stop();
  }
  delay(1000);
}