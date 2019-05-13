import paho.mqtt.client as mqtt
import random,json
from time import sleep

MQTT_Broker = "localhost"
MQTT_Port = 1883 
Keep_Alive_Interval = 45 #thoi gian giua cac lan gui goi tin
MQTT_Topic1 = "esp8266/temperature"
MQTT_Topic2 = "esp8266/humidity"

#ham connect den host MQTT

def on_connect(client, userdata, rc):
	if rc !=0:
		pass
		print("Unable to connect to MQTT ")
	else:
		print("Connect with MQTT Broker: " + str(MQTT_Broker))
def on_publish(client, userdata, mid):
	pass
def on_disconnect(client, userdata, rc):
	if rc !=0:
		pass

mqttc = mqtt.Client()
mqttc.username_pw_set(username="long", password="1")
mqttc.on_connect = on_connect
mqttc.on_disconnect = on_disconnect
mqttc.on_publish = on_publish
mqttc.connect(MQTT_Broker, MQTT_Port, Keep_Alive_Interval)


def publish_To_Topic(topic, message):
	mqttc.publish(topic,message)
	print(("Published: " + str(message) + " " + "on MQTT Topic: " + str(topic)))
	print("")
	
while True:
	publish_To_Topic(MQTT_Topic1,"123");
	publish_To_Topic(MQTT_Topic2,"123");
	sleep(10)

