  # client class which provides all of the functions to publish messages and subscribe to topics.
import paho.mqtt.client as paho # To use the client class you need to import it
import time
import sys
import datetime
#import time
broker="192.168.1.20"  #host name

#broker="127.0.0.1"
#topic="string" #topic name
topic="test" #topic name
        
def on_message(client, userdata, message):
  print("received data is :")  
  print(str(message.payload.decode("utf-8")) ) #printing Received message
  print("")
    
client= paho.Client("user") #create client object 
client.on_message=on_message
   
print("connecting to broker host",broker)
client.connect(broker)#connection establishment with broker
print("subscribing begins here")    
client.subscribe(topic)#subscribe topic test
client.loop_forever()