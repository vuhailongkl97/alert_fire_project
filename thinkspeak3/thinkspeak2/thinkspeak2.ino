
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DHTPIN D1
#define DHTTYPE DHT11
#define LM35 A0
#define MQ2 D5
#define SPEAKER D6
#define LED D3

/*lamnguyenk52a31@gmail.com
Bkav@2019
*/

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
  Serial.println("WiFi connected"); 
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
 
  if (espclient.connect(server, 80)) { 

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(DHT_tem);
    postStr += "&field2=";
    postStr += String(DHT_hum);
    
    //Lam edit
    postStr += "&field3=";
    postStr += String(LM35_tem);
    postStr += "&field4=";
    postStr += String(MQ2_value);
    postStr += "\r\n\r\n"; 

    espclient.print("POST /update HTTP/1.1\n");
    espclient.print("Host: api.thingspeak.com\n");
    espclient.print("Connection: close\n");
    espclient.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    espclient.print("Content-Type: application/x-www-form-urlencoded\n");
    espclient.print("Content-Length: ");
    espclient.print(postStr.length());
    espclient.print("\n\n");
    espclient.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(DHT_tem);
    Serial.print(" oC Humidity: ");
    Serial.print(DHT_hum);
    Serial.print(" % LM35: ");
    Serial.print(LM35_tem);
    Serial.print(" oC MQ2: ");
    Serial.print(MQ2_value);
    Serial.println("% send to Thingspeak");
  }
  espclient.stop();

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
  // Đợi 10s rồi gửi tiếp dữ liệu
  delay(5000);
}
