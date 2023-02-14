    # client class which provides all of the functions to publish messages and subscribe to topics.
import paho.mqtt.client as paho # To use the client class you need to import it
import time
import sys
import datetime
#import time
broker="192.168.1.20"  #host name
topic="string" #topic name
#topic1="test1"
        
def on_message(client, userdata, message):
 # print("received data is :")  
 # print(str(message.payload.decode("utf-8")) ) #printing Received message
 received_data = str(message.payload.decode("utf-8"));
 #print(received_data);
 clock_frequency, mode,peripheral, current, power, temperature = received_data.split("||")
 print(clock_frequency);
 print(mode);
 print(peripheral);
 print(current);
 print(power);
 print(temperature);
 
 
 # print("")    
client= paho.Client("user") #create client object 
client.on_message=on_message
#client1= paho.Client("user") #create client object 
#client1.on_message=on_message
    
print("connecting to broker host",broker)
client.connect(broker)
#client1.connect(broker)#connection establishment with broker
print("subscribing begins here")    
#client.subscribe("topic")
client.subscribe(topic)#subscribe topic test
#client.subscribe("topic")
client.loop_forever()
#client1.loop_forever()