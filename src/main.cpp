#include <Arduino.h> 
#include <WiFi.h> //Inclusão Biblioteca ESP32
#include <DHT.h> //Inclusão Biblioteca Sensor DHT22
#include <DHT_U.h> //Inclusão Biblioteca Sensor DHT22

#define DHTPIN 4 //Define o correspodente ao D4
#define DHTTYPE DHT22 //Define o tipo de DHT
#define MSG_BUFFER_SIZE (50) //Define o tamanho do Buffer de Recebimento
char msg[MSG_BUFFER_SIZE]; //Mensagem de tamanho definido pelo Buffer
DHT_Unified dht(DHTPIN, DHTTYPE); //Valor do Pino e o tipo de DHT
uint32_t delayMS; // Váriavel de atraso
unsigned long lastMsg = 0; //Atribui o valor zero à variável "LastMsg"
int value = 0; //Atribui o valor zero à variável "Value"

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println(F("Sensor DHT22"));
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("--------------------------------------------------"));
  Serial.println(F("Temperatura"));
  Serial.print(F("Sensor:"));
  Serial.println(sensor.name);
  Serial.print("Valor máximo: ");
  Serial.print(sensor.max_value);
  Serial.println(F("°C"));
  Serial.print("Valor mínimo: ");
  Serial.print(sensor.min_value);
  Serial.println(F("°C"));
  Serial.println(F("--------------------------------------------------"));

  dht.humidity().getSensor(&sensor);
  Serial.println(F("--------------------------------------------------"));
  Serial.println(F("Umidade"));
  Serial.print(F("Sensor:"));
  Serial.println(sensor.name);
  Serial.print("Valor máximo: ");
  Serial.print(sensor.max_value);
  Serial.println(F("%"));
  Serial.print("Valor mínimo: ");
  Serial.print(sensor.min_value);
  Serial.println(F("%"));
  Serial.println(F("--------------------------------------------------"));
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  delay(delayMS);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if(isnan(event.temperature))
  {
    Serial.println(F("Erro na leitura da temperatura!"));
  }
  else
  {
    Serial.print(F("Temperatura: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    sprintf(msg, "%f", event.temperature);
  }
  dht.humidity().getEvent(&event);
  if(isnan(event.relative_humidity))
  {
    Serial.println(F("Erro na leitura da umidade!"));
  }
}