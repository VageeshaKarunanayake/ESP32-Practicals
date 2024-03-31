#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Test";
const char* password = "12345678";
const char* mqtt_server = "io.adafruit.com";
const int mqtt_port = 1883;
const char* mqtt_user = "Vageesha";
const char* mqtt_password = "secret_key";

const char* topic = "Vageesha/feeds/temp";  // MQTT topic to publish temperature data

// Analog pin where the temperature sensor is connected
const int sensorPin = 34;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("MQTT Connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
  float sensorValue = analogRead(sensorPin);
  float voltage = (sensorValue / 4095.0) * 3.3; // Assuming a 12-bit ADC
  Serial.print(voltage);
  float temperatureCelsius = map(voltage, 0.00, 3.30, 293.15, 493.15);

  char message[10];
  dtostrf(temperatureCelsius, 3, 2, message); // Convert to string with 2 decimal places

  if (client.connected()) {
    client.publish(topic, message);
    Serial.print("Published to MQTT: ");
    Serial.println(message);
  } else {
    Serial.println("MQTT not connected, reconnecting...");
    if (reconnect()) {
      client.publish(topic, message);
      Serial.print("Published to MQTT: ");
      Serial.println(message);
    }
  }

  client.loop();
  delay(10000); // Publish every 10 seconds
}

boolean reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Reconnected to MQTT");
      return true;
    } else {
      Serial.print("MQTT Connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
  return false;
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle MQTT subscription messages if needed
}
