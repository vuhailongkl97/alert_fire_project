import paho.mqtt.client as mqtt
import my_lib_sql as ml 
import ast
from datetime import datetime
from gtts import gTTS
import os

MQTT_Broker = "localhost"
MQTT_Port   = 1883
Keep_Alive_Interval = 45
MQTT_Topic2 = "esp8266/room"


def on_message(client, userdata, msg):
	print ("MQTT Topic :  " +msg.topic)
	print ("saving .....")
	json_data =  msg.payload
	json_Dict = ast.literal_eval(json_data.decode('utf-8'))
	print(json_Dict)
	room = json_Dict['room']
	Temperature = json_Dict['temperature']

	#print("Room %s : Temperature %f" % (room, Temperature))

	text = "Room %s : Temperature %2.1f" % (room, Temperature)
	print(text)
	tts = gTTS(text=text,lang='en')
	tts.save("good.mp3")
	os.system("mplayer good.mp3")

def on_connect(client, userdata, flags, rc):
	if rc != 0: #ket noi thanh cong khi rc = 0
		pass
		print("Unable to connect to MQTT Broker...")
	else:
		print("Connected with MQTT Broker: " + str(MQTT_Broker))
	client.subscribe(MQTT_Topic2,0)

ml.create_table()
client = mqtt.Client()
client.username_pw_set(username="long",password="1")
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_Broker, MQTT_Port, Keep_Alive_Interval)
client.loop_forever()

