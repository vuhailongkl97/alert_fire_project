
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
 
WiFiClient client;

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
 
  if (client.connect(server, 80)) { 

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

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

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
  client.stop();

  Serial.println("Waiting…");
  // Đợi 10s rồi gửi tiếp dữ liệu
  delay(5000);
}
