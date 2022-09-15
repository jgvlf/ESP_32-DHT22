#include <Arduino.h> 
#include <PubSubClient.h>
#include <WiFi.h> //Inclusão Biblioteca ESP32
#include <DHT.h> //Inclusão Biblioteca Sensor DHT22
#include <DHT_U.h> //Inclusão Biblioteca Sensor DHT22

const char* ssid = "<your wifi name>"; //O Nome da sua rede WIFI
const char* password = "<your wifi password>"; // A Senha da rede WIFI
const char* mqtt_server = "broker.mqtt-dashboard.com"; //Endereço do broker em núvem disponível

WiFiClient espClient; //Cliente ESP WIFI
PubSubClient client(espClient); //Cliente ESP Publicador e Subscrito

#define DHTPIN 4 //Define o correspodente ao D4
#define DHTTYPE DHT22 //Define o tipo de DHT
#define MSG_BUFFER_SIZE (50) //Define o tamanho do Buffer de Recebimento
char msg[MSG_BUFFER_SIZE]; //Mensagem de tamanho definido pelo Buffer
DHT_Unified dht(DHTPIN, DHTTYPE); //Valor do Pino e o tipo de DHT
uint32_t delayMS; // Váriavel de atraso
unsigned long lastMsg = 0; //Atribui o valor zero à variável "LastMsg"
int value = 0; //Atribui o valor zero à variável "Value"

void setup_wifi(){
  delay(10);
  Serial.println("");
  Serial.print("Conectando com");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect(){
  while(!client.connected()){
    Serial.print("Aguardando conexão MQTT...");
    String clientId = "ESP32Client";
    clientId += String(random(0xffff), HEX);
    if(client.connect(clientId.c_str())){
      Serial.println("Conectado");
      client.subscribe("TLG/Server");
    }
    else{
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println("tente novamente em 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
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
  else
  {
    Serial.print(F("Umidade: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    sprintf(msg, "%f", event.relative_humidity);
  }


}