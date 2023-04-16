#include <WiFi.h>
#include "defines.h"      // Definición de constantes
#include "heltec.h"       // Control WifiKit 32
#include "images.h"       //Imagen inicial

/* ESTADO WIFI */
void printWifiStatus()
{
  // Estado de la conexión
  
  Serial.print(F("ÉXITO, conectado a: "));
  Serial.print(WiFi.SSID());
  Serial.print(F("Dirección IP = "));
  Serial.println(WiFi.localIP());

  // Potencia de señal:
  int32_t rssi = WiFi.RSSI();
  Serial.print(F(", Potencia de señal (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
}

/* MÉTODOS QUE UTILIZAN OLED INTEGRADO */

/* IMPRIME LOGO */
void logo(){
  Heltec.display -> clear();
  Heltec.display -> drawXbm(0,5,logo_width,logo_height,(const unsigned char *)logo_bits);
  Heltec.display -> display();
}

/* ESTADO WIFI CON OLED  */
void WIFISetUp(void)
{
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  delay(100);

  byte count = 0;
  while(WiFi.status() != WL_CONNECTED && count < 10)
  {
    count ++;
    delay(500);
    Heltec.display -> drawString(0, 0, "Connecting...");
    Heltec.display -> display();
  }

  Heltec.display -> clear();
  if(WiFi.status() == WL_CONNECTED)
  {
    Heltec.display -> drawString(0, 0, "Connecting...OK.");
    Heltec.display -> display();
//    delay(500);
  }
  else
  {
    Heltec.display -> clear();
    Heltec.display -> drawString(0, 0, "Connecting...Failed");
    Heltec.display -> display();
//    while(1);
  }
  Heltec.display -> drawString(0, 10, "WIFI Setup done");
  Heltec.display -> display();
  delay(500);
}
