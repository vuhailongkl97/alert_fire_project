import pymysql as pl
import json
import ast

sql_create_table = """CREATE TABLE sensors(
		id INT(10) PRIMARY KEY  auto_increment , 
		Sensor_ID varchar(10) not null, 
		Date_and_Time datetime not null, 
		Temperature int(3) not null,
		Humidity int(3) not null
		)
	  """

sql_insert = """insert into sensors(Sensor_ID,Date_and_Time, Temperature, Humidity) VALUES ('%s', '%s', %d, %d) """

def create_table():

	db = pl.connect("localhost", "root", "1" , "nodelogin")
	cursor = db.cursor()
	cursor.execute("DROP TABLE IF EXISTS sensors" )
	cursor.execute(sql_create_table)

	db.close()
def insert_table(json_data):
	db = pl.connect("localhost", "root", "1" , "nodelogin")
	cursor = db.cursor()

	json_Dict = ast.literal_eval(json_data.decode('utf-8'))
	print (json_Dict )

	SensorID = json_Dict['Sensor_ID']
	Date_and_Time = json_Dict['Date']
	Temperature = json_Dict['Temperature']
	Humidity = json_Dict['Humidity']

	val = (SensorID, Date_and_Time, Temperature, Humidity)

	cursor.execute(sql_insert % val);
	db.commit()
	db.close()
def insert_table_with_raw_data(sensor_id, date_and_time, temperature, humidity):
	db = pl.connect("localhost", "root", "1" , "nodelogin")
	cursor = db.cursor()
	val = (sensor_id, date_and_time, temperature, humidity)

	cursor.execute(sql_insert % val);
	db.commit()
	db.close()
	
