/* Hidrate-se IoT - ESP32 + HiveMQ Cloud*/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define BUTTON_PIN 5
#define LED_PIN 2

const char* ssid = "Wokwi-GUEST";
const char* wifi_pass = "";

const char* mqtt_host = "3890aa849cd145fa81195df8211610de.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "nome_usuário"; 
const char* mqtt_pass = "password"; 

WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastDrink = 0;
unsigned long interval = 20000; // valor inicial: 20s
unsigned long lastDhtTime = 0;
bool lembreteEnviado = false;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");
  for (unsigned int i = 0; i < length; i++) Serial.print((char)payload[i]);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();

  // conexão Wi-Fi
  WiFi.begin(ssid, wifi_pass);
  Serial.print("Conectando WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado!");

  // conexão segura (modo teste)
  wifiClient.setInsecure();

  mqttClient.setServer(mqtt_host, mqtt_port);
  mqttClient.setCallback(callback);

  lastDrink = millis();
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando MQTT...");
    if (mqttClient.connect("HidrateseESP32", mqtt_user, mqtt_pass)) {
      Serial.println("conectado!");
      mqttClient.subscribe("hidratese/controle");
    } else {
      Serial.print("falha, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" tentando novamente...");
      delay(2000);
    }
  }
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  // leitura do botão
  if (digitalRead(BUTTON_PIN) == LOW) {
    lastDrink = millis();
    lembreteEnviado = false;
    mqttClient.publish("hidratese/consumo", "usuario_bebeu");
    Serial.println("💧 Usuário bebeu água!");
    delay(500);
  }

  // leitura do sensor DHT a cada 5 segundos
  if (millis() - lastDhtTime > 5000) {
    float t = dht.readTemperature();
    if (!isnan(t)) {
      char buf[64];
      snprintf(buf, sizeof(buf), "Temperatura: %.1fC", t);
      mqttClient.publish("hidratese/ambiente", buf);
      Serial.println(buf);

      // Ajusta o tempo entre lembretes conforme temperatura (teste)
      if (t >= 30.0) {
        interval = 10000; // calor → 10s
        Serial.println("🌡️ Calor detectado → Lembrete a cada 10s");
      } else if (t >= 24.0) {
        interval = 15000; // morno → 15s
        Serial.println("🌤️ Temperatura morna → Lembrete a cada 15s");
      } else {
        interval = 20000; // fresco → 20s
        Serial.println("❄️ Temperatura fresca → Lembrete a cada 20s");
      }
    }
    lastDhtTime = millis();
  }

  // alerta de lembrete
  if (millis() - lastDrink > interval && !lembreteEnviado) {
    digitalWrite(LED_PIN, HIGH);
    mqttClient.publish("hidratese/alerta", "⏰ Lembrete: beba água!");
    Serial.println("⚠️ LED ACESO → Hora de beber água!");
    lembreteEnviado = true;
  } else if (millis() - lastDrink <= interval) {
    digitalWrite(LED_PIN, LOW);
  }
}
