/*************************************************************************************************
 * This Example sends  harcoded data to Ubidots and serves as example for users which 
 * desire build a HTTP request to the Ubidots API with any device. Please reference to the link below 
 * to find more information about it - Ubidots REST API Reference - https://ubidots.com/docs/api/
 *
 * You will find a help guide for this code on the link below: 
 * http://help.ubidots.com/connect-your-devices/connect-your-intel-edison-to-ubidots-using-arduino-over-http
 * 
 * This example is given AS IT IS without any warranty.
 *  
 * Made by María Carlina Hernández.
 *************************************************************************************************/

/********************************
 * Libraries included
 *******************************/
#include <Ubidots.h>
#include <SPI.h>
#include "DHT.h"
/********************************
 * Constants and objects
 *******************************/
#define LED  15

#define pin1 13
DHT dht1(pin1,DHT11);

/* Assigns the network parameters */
char* WIFI_SSID = "IoT"; // your network WIFI_SSID (name)
char* WIFI_PASSWORD = "1t3s0IoT18"; // your network password

/* Assigns the Ubidots parameters */
char const * TOKEN = "BBFF-O9rfEZsGHo2Pjt6uuzEvHPXkMA82CL"; // Assign your Ubidots TOKEN
char const * DEVICE_LABEL = "esp32"; // Assign the unique device label 
char const * VARIABLE_LABEL_1 = "temperatura"; // Assign the unique variable label to publish data to Ubidots (1) 
char const * VARIABLE_LABEL_2 = "humedad"; // Assign the unique variable label to publish data to Ubidots (2)
char const * VARIABLE_LABEL_3 = "Led"; // Assign the unique variable label to publish data to Ubidots (2)

/* Parameters needed for the requests */
char const * VERSION = "1.0";
char const * SERVER = "https://industrial.api.ubidots.com/api/v2.0/devices/esp32/variables/";
int PORT = 80;
char topic[700];
char payload[300];
String estado = "";
bool led = 0;
/* initialize the library instance */
WiFiClient client;
Ubidots ubidots(TOKEN,UBI_HTTP);

/********************************
 * Auxiliar Functions
 *******************************/

/********************************
 * Main Functions
 *******************************/
 
void setup() {
  Serial.begin(115200);
   dht1.begin();
   pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID); 
  /* Connects to AP */
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASSWORD);
 

}

void loop() {
  /* Reads sensors values */
 
  float t1 = dht1.readTemperature();
  float h1 = dht1.readHumidity();
   while (isnan(t1) || isnan(h1)) {
   Serial.println("Lectura fallida en el sensor DHT11, repitiendo lectura...");
   delay(3000); // esperar 3 seg
   t1 = dht1.readTemperature();
   h1 = dht1.readHumidity();
   }
  Serial.print("Temperatura antes del led: ");
  Serial.println(t1);
 

  ubidots.add("humedad",h1);
  ubidots.add("temperatura",t1);
  
   if(t1 >= 28){
    digitalWrite(LED, HIGH); 
    estado = "Encendido";
    led = 1;
    
  }
   if(t1 <= 27){
    digitalWrite(LED, LOW); 
    estado = "Apagado";
    led = 0;
 
  }
  ubidots.add("led",led);
  bool enviado = false;
  enviado = ubidots.send();
  /* Calls the Ubidots Function POST */

  /* Prints the data posted on the Serial Monitor */
  Serial.println("Posting data to Ubidots");
  Serial.print("Temperature: ");
  Serial.println(t1);
  Serial.print("Humidity: ");
  Serial.println(h1);

  delay(5000); 
}
