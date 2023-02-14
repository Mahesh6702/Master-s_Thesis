# client class which provides all of the functions to publish messages and subscribe to topics.
import paho.mqtt.client as paho # To use the client class you need to import it
import psycopg2                 # API to connect with postgresql
import time                     # To known current time (timestamp)
import sys
from datetime import datetime, timezone


broker="192.168.1.20"              #To connect to mosquitto broker (inside local machine), host name 
topic="string"                     # Topic name to connect with MCU client


# Connection to Postgresql Database, user is postgres
conn = psycopg2.connect(database = "parameters", user = "postgres", password = "postgresql", host = "127.0.0.1", port = "5432")
print ("Opened database successfully")
cur = conn.cursor()
# datetime object containing current date and time
now = datetime.now()
dt = datetime.now(timezone.utc)


 #Obtained message from the mcu client       
def on_message(client, userdata, message):
 global received_data
 print("received data is :")  
 received_data = str(message.payload.decode("utf-8"));  # storing message into new variable received_data
 client= paho.Client("user") #create client object 
 client.on_message=on_message 

 # Split the string into new vairables, when there is '||'.
 print(received_data);
 CLOCKFREQUENCY, MODE,PERIPHERALS, CURRENT,POWER, TEMPERATURE = received_data.split("||")
 print(CLOCKFREQUENCY);
 print(MODE);
 print(PERIPHERALS);
 print(CURRENT);
 print(POWER);
 print(TEMPERATURE);
 

 #Adding parameters reading into columns of mcu_parameters Table
 cur.execute('INSERT INTO Mcu_parameters(EXACT_TIME) VALUES (%s)', (dt,))
 cur.execute('INSERT INTO Mcu_parameters(CLOCKFREQUENCY) VALUES (%s)', (CLOCKFREQUENCY,))
 cur.execute('INSERT INTO Mcu_parameters(PERIPHERALS) VALUES (%s)', (PERIPHERALS,))
 cur.execute('INSERT INTO Mcu_parameters(CURRENT) VALUES (%s)', (CURRENT,))
 cur.execute('INSERT INTO Mcu_parameters(POWER) VALUES (%s)', (POWER,))
 cur.execute('INSERT INTO Mcu_parameters(TEMPERATURE) VALUES (%s)', (TEMPERATURE,)) 

 conn.commit()
 print ("Records created successfully")
 conn.close()   
 
 print("connecting to broker host",broker)
 client.connect(broker)
 #client1.connect(broker)#connection establishment with broker
 print("subscribing begins here")    
 #client.subscribe("topic")
 client.subscribe(topic)#subscribe topic test
 #client.subscribe("topic")
 client.loop_forever()
 #client1.loop_forever()




