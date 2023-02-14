# client class which provides all of the functions to publish messages and subscribe to topics.
import paho.mqtt.client as paho # To use the client class you need to import it
import psycopg2                 # API to connect with postgresql
import time                     # To known current time (timestamp)
import sys
from datetime import datetime, timezone


broker="192.168.1.20"              #To connect to mosquitto broker (inside local machine), host name 
topic="string"                     # Topic name to connect with MCU client


# Connection to Postgresql Database, user is postgres

# datetime object containing current date and time
now = datetime.now()

conn = psycopg2.connect(database = "parameters", user = "postgres", password = "postgresql", host = "127.0.0.1", port = "5432")
print ("Opened database successfully")


 #Obtained message from the mcu client       
def on_message(client, userdata, message):
    #conn = psycopg2.connect(database = "parameters", user = "postgres", password = "postgresql", host = "127.0.0.1", port = "5432")
    cur = conn.cursor()
    dt = datetime.now(timezone.utc)
    global received_data,CLOCKFREQUENCY, MODE,PERIPHERALS, CURRENT,POWER, TEMPERATURE
    print("received data is :")  
    received_data = str(message.payload.decode("utf-8"));  # storing message into new variable received_data
    print(received_data);
    CLOCKFREQUENCY, MODE,SPI,I2C, CURRENT,POWER, TEMPERATURE = received_data.split("||")
    print(CLOCKFREQUENCY,MODE,SPI,I2C,CURRENT,POWER,TEMPERATURE)
    #Adding parameters reading into columns of mcu_parameters Table
    cur.execute('INSERT INTO Mcu_parameters(EXACT_TIME) VALUES (%s)', (dt,))
    cur.execute('INSERT INTO Mcu_parameters(CLOCKFREQUENCY) VALUES (%s)', (CLOCKFREQUENCY,))
    cur.execute('INSERT INTO Mcu_parameters(SPI) VALUES (%s)', (SPI,))
    cur.execute('INSERT INTO Mcu_parameters(I2C) VALUES (%s)', (I2C,))  
    cur.execute('INSERT INTO Mcu_parameters(CURRENT) VALUES (%s)', (CURRENT,))
    cur.execute('INSERT INTO Mcu_parameters(POWER) VALUES (%s)', (POWER,))
    cur.execute('INSERT INTO Mcu_parameters(TEMPERATURE) VALUES (%s)', (TEMPERATURE,)) 
    print ("Records created successfully")

    conn.commit()

    #conn.close()   

client= paho.Client("user") #create client object 
client.on_message=on_message 

 # Split the string into new vairables, when there is '||'.'
'''
print(CLOCKFREQUENCY);
print(MODE);
print(PERIPHERALS);
print(CURRENT);
print(POWER);
print(TEMPERATURE);

'''

print("connecting to broker host",broker)
client.connect(broker)
#client1.connect(broker)#connection establishment with broker
print("subscribing begins here")    
#client.subscribe("topic")
client.subscribe(topic)#subscribe topic test
client.loop_forever()
 

 





