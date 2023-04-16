
#include <WiFiClientSecure.h>
#include <MQTTPubSubClient_Generic.h>       //Librería Suscripción y Publicación MQTT
#include "defines.h"                        //Definición de constantes y valores de configuración
#include "heimdall_wifi.h"                  //Librerías para conexión y estado WiFi

int status = WL_IDLE_STATUS;  // the Wifi radio's status
String lectura = "0";

WiFiClientSecure client;
MQTTPubSubClient mqttClient;

const char *PubTopic    = "prueba";            // Topic en el que publicar
const char *PubMessage  = "probando...";      // Mensaje a publicar


/**********
 * SETUP
 *********/
 
void setup()
{
  /* INICIALIZACIÓN WIFIKIT32 */
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);

  logo();
  delay(1000);
  Heltec.display->clear();
  
  /* CONEXIÓN CON WIFI*/
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  Serial.print(F("Conectando a la red: "));
  Serial.println(WIFI_SSID /*ssid*/);
  
  status = WiFi.begin(WIFI_SSID/*ssid*/,WIFI_PASS/* pass*/);
  delay(1000);
   
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED)
  {
    delay(500);
    // Connect to WPA/WPA2 network
    status = WiFi.status();
  }

  // Estado de la conexión WiFi
  printWifiStatus();

  /* CONEXIÓN CON BROKER */
  Serial.print("Conectando a Broker MQTT Cloud (HiveMQ) = "); 
  Serial.print(MQTT_SERVER); 
  Serial.print(":"); Serial.println(MQTT_PORT); 
  
  while (!client.connect(MQTT_SERVER, MQTT_PORT))
  {
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println("\nÉXITO, conectado a Broker MQTT (HiveMQ)");
  
  // initialize mqtt client
  mqttClient.begin(client);

  Serial.print("Aplicando credenciales de acceso...");

  //while (!mqttClient.connect("9af81018ca4b463eb9f5e9466caeca52.s1.eu.hivemq.cloud", "heimdall","heimdall"))
  while (!mqttClient.connect(MQTT_SERVER, MQTT_USER, MQTT_PASS/* "heimdall","heimdall"*/))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Acceso permitido");

  mqttClient.subscribe([](const String & topic, const String & payload, const size_t size)
  {
    (void) size;
    
    Serial.println("MQTT received: " + topic + " - " + payload);
  });

  // Suscripción a topic
  mqttClient.subscribe(PubTopic, [](const String & payload, const size_t size)
  {
    (void) size;
    
    Serial.print("Suscripción realizada al Topic: ");
    Serial.print(PubTopic); Serial.print(" => ");
    Serial.println(payload);
  });

  mqttClient.publish(PubTopic, PubMessage);

}

/************************/
/*       LOOP           */
/************************/
void loop()
{
  
  Serial.println();  
  mqttClient.update(); 
  mqttClient.publish(PubTopic, PubMessage);
  delay(3000);
  digitalWrite(LED,LOW);
  delay(1000);
}
