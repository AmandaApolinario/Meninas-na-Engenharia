"""
Python MQTT Subscription client
Thomas Varnish (https://github.com/tvarnish), (https://www.instructables.com/member/Tango172)
Written for my Instructable - "How to use MQTT with the Raspberry Pi and ESP8266"
"""
import paho.mqtt.client as mqtt

# Don't forget to change the variables for the MQTT broker!
mqtt_username = "nome do broker"
mqtt_password = "senha do broker"
mqtt_broker_ip = "IP do broker na forma 192.168.x.x"

client = mqtt.Client()
# Set the username and password for the MQTT client
client.username_pw_set(mqtt_username, mqtt_password)

# These functions handle what happens when the MQTT client connects
# to the broker, and what happens then the topic receives a message
def on_connect(client, userdata, flags, rc):
    # rc is the error code returned when connecting to the broker
    print ("Connected!", str(rc))
    
    # Once the client has connected to the broker, subscribe to the topic
    client.subscribe("rele/Control/1")
    client.subscribe("rele/Control/0")
    client.subscribe("rele/Status")
    
    
def on_message(client, userdata, msg):
    # This function is called everytime the topic is published to.
    # If you want to check each message, and do something depending on
    # the content, the code to do this should be run in this function
    
    print ("Topic: ", msg.topic + "\nMessage:\n" + str(msg.payload.decode("utf-8")))
    
    if( msg.topic == "rele/Control/1"):
        if(str(msg.payload.decode("utf-8")) == "1"):
            client.publish("rele/primeiro","1")
        
        elif(str(msg.payload.decode("utf-8")) == "2"):
            client.publish("rele/segundo","1")
        
        elif(str(msg.payload.decode("utf-8")) == "3"):
            client.publish("rele/terceiro","1")

        elif(str(msg.payload.decode("utf-8")) == "4"):
            client.publish("rele/quarto","1")
            
        elif(str(msg.payload.decode("utf-8")) == "5"):
            client.publish("rele/quinto","1")
            
        elif(str(msg.payload.decode("utf-8")) == "6"):
            client.publish("rele/sexto","1")

    if( msg.topic == "rele/Control/0"):
        if(str(msg.payload.decode("utf-8")) == "1"):
            client.publish("rele/primeiro","0")
        
        elif(str(msg.payload.decode("utf-8")) == "2"):
            client.publish("rele/segundo","0")
        
        elif(str(msg.payload.decode("utf-8")) == "3"):
            client.publish("rele/terceiro","0")

        elif(str(msg.payload.decode("utf-8")) == "4"):
            client.publish("rele/quarto","0")
        
        elif(str(msg.payload.decode("utf-8")) == "5"):
            client.publish("rele/quinto","0")
            
        elif(str(msg.payload.decode("utf-8")) == "6"):
            client.publish("rele/sexto","0") 
            
    elif(msg.topic ==  "rele/Status"):
        f = open("/home/saotorquarto/Documentos/fila.txt","a")
        print(str(msg.payload.decode("utf-8")) + "\n")
        f.write(str(msg.payload.decode("utf-8")) + "\n")
        f.close()
    
    # The message itself is stored in the msg variable
    # and details about who sent it are stored in userdata

# Here, we are telling the client which functions are to be run
# on connecting, and on receiving a message
client.on_connect = on_connect
client.on_message = on_message

# Once everything has been set up, we can (finally) connect to the broker
# 1883 is the listener port that the MQTT broker is using
client.connect(mqtt_broker_ip, 1883)

# Once we have told the client to connect, let the client object run itself
client.loop_forever()
client.disconnect()

