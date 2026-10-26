#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const char* topic_level = "smart_tank/level";

WiFiClient espClient;
PubSubClient client(espClient);

const int trigPin = 5;
const int echoPin = 18;
const int greenLedPin = 19;
const int redLedPin = 21;
const int TANK_HEIGHT = 100;

bool isNetworkOk = false;

long readDistance() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

void taskNetworkAndSensor(void *pvParameters) {
  WiFi.begin(ssid, password);
  client.setServer(mqtt_server, 1883);
  
  for(;;) {
    if (WiFi.status() != WL_CONNECTED) {
      isNetworkOk = false;
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      continue;
    }
    
    if (!client.connected()) {
      isNetworkOk = false;
      client.connect("ESP32_TMS_RTOS");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      continue;
    }
    
    isNetworkOk = true;
    client.loop();
    
    long distance = readDistance();
    int waterLevel = map(distance, TANK_HEIGHT, 0, 0, 100);
    waterLevel = constrain(waterLevel, 0, 100);
    
    String payload = String(waterLevel);
    client.publish(topic_level, payload.c_str());
    
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void taskLeds(void *pvParameters) {
  for(;;) {
    if (isNetworkOk) {
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(redLedPin, LOW);
    } else {
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, HIGH);
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  
  //creazione task
  xTaskCreatePinnedToCore(taskNetworkAndSensor, "SensorTask", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskLeds, "LedTask", 2048, NULL, 1, NULL, 1);
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}