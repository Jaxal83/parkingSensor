/* Modified from:
 * ESP32 AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "PubSubClient.h"

#include "index.h"  //Web page header file

//Enter your SSID and PASSWORD
const char* ssid = "#########";
const char* password = "#########";

// MQTT broker
const char* mqtt_server = "#########";
const char* mqtt_topic = "#########";
const char* mqtt_username = "#########";
const char* mqtt_password = "#########";
const char* clientID = "#########";

char latestMqttMessage[20];

WebServer server(80);
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleADC() { 
 server.send(200, "text/plane", String(latestMqttMessage)); 
}
//===============================================================
// MQTT Callback function to handle incoming messages
//===============================================================
void callback(char* topic, byte* message, unsigned int length){
//  Serial.print("m: ");

  for (int i=0; i<length; i++){
//    Serial.print((char)message[i]);
    latestMqttMessage[i] = (char)message[i];
  }
//  Serial.println();
}

//===============================================================
// Setup
//===============================================================

void setup(void){
//  Serial.begin(115200);
//  Serial.println();
//  Serial.println(WiFi.macAddress());
//  Serial.println("Booting Sketch...");

//ESP32 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);

//  Serial.println("Connecting to ");
//  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
//      Serial.print(".");
      delay(10);
    }
    
//If connection successful show IP address in serial monitor
//  Serial.println("");
//  Serial.print("Connected to ");
//  Serial.println(ssid);
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  client.setCallback(callback);
//----------------------------------------------------------------


  server.on("/", handleRoot);      //This is display page
  server.on("/readADC", handleADC);//To get update of ADC Value only
 
  server.begin();                  //Start server
//  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();

  if (!client.connected()){
//    Serial.print("Reconnecting to MQTT.");
    while(!client.connect(clientID, mqtt_username, mqtt_password)){
//      Serial.print(".");
      delay(10);
    }
//    Serial.println("reconnected.");
    client.subscribe(mqtt_topic);
  }
  client.loop();
}
