
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DHTPIN D1
#define DHTTYPE DHT11
#define LM35 A0
#define MQ2 D5
#define SPEAKER D6
#define LED D3

DHT dht(DHTPIN,DHTTYPE);

String apiKey = "UTEVDG8PBX5I6M6F";
const char* ssid = "AndroidAP";
const char* password = "110119961";

const char* server = "api.thingspeak.com";
const char* mqtt_server = "192.168.43.49";

WiFiClient espclient;
PubSubClient client(espclient);  
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client","long","1")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  pinMode(LM35,INPUT);
  pinMode(MQ2,INPUT);
  pinMode(SPEAKER,OUTPUT);
  pinMode(LED,OUTPUT);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); //Báo kết nối thành công
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  float DHT_hum = dht.readHumidity();
  float DHT_tem = dht.readTemperature();
  float f = dht.readTemperature(true);

  if(isnan(DHT_hum) || isnan(DHT_tem) || isnan(f)){
     Serial.println("Failed to read from DHT sensor!");
     delay(1000);
     return;
  }
  float LM35_tem = (analogRead(LM35)*333.0/1024.0);
  Serial.print("LM35: ");
  Serial.println(LM35_tem);

  float MQ2_value = digitalRead(MQ2);
  Serial.print("MQ2: ");
  Serial.println(MQ2_value);
  Serial.print("\tDo am: ");
  Serial.print(DHT_hum);
  Serial.print(" %\t");
  Serial.print("Nhiet do: ");
  Serial.print(DHT_tem);
  Serial.println(" *C ");

  if( MQ2_value == 0 )
  {
    digitalWrite(SPEAKER,LOW);
    delay(1000);
  }
  else
  {
    digitalWrite(SPEAKER,HIGH);
    delay(1000);
  }
 
    Serial.print("Temperature: ");
    Serial.println(DHT_tem);
    Serial.print(" oC Humidity: ");
    Serial.println(DHT_hum);
    Serial.print(" % LM35: ");
    Serial.println(LM35_tem);
    Serial.print(" oC MQ2: ");
    Serial.println(MQ2_value);
    Serial.println("% send to Thingspeak");
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  char temparr[8];
 
  String room = "1";
  
  sprintf(temparr, "%2.2f",LM35_tem);
  Serial.print("Temperature: ");
   String tempString(temparr);
   Serial.print(tempString);
//  client.publish("esp8266/temperature", tempString);
//  
//  char humString[8];
//  sprintf(humString, "%2.2f", DHT_hum);
//  Serial.print("Humidity: ");
//  Serial.println(humString);
//  client.publish("esp8266/floors", humString);


  String totalLine = "{\"room\":\"" + room + "\", \"temperature\":" + tempString + "}";
  Serial.println(totalLine.c_str());
  client.publish("esp8266/room", totalLine.c_str(), true);

  Serial.println("Waiting…");

  
  delay(5000);
}
