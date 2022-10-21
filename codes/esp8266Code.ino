#include <Bounce2.h> // Used for "debouncing" the pushbutton
#include <ESP8266WiFi.h> // Enables the ESP8266 to connect to the local network (via WiFi)
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker
#define RelePin 4
#define SensorPin A0

//MUDAR PARA O IP ESTATICO DA RASP
const char* mqtt_server = "IP do broker da forma 192.168.x.x";

//MUDAR ESSAS PARA A REDE
const char* ssid = "nome da rede";
const char* wifi_password = "senha da rede";

//MUDAR PARA QUAL RELE QUER LIGAR
const char* mqtt_topic = "rele/primeiro";
//MUDAR O NUMERO DE ACORDO COM O RELE EX PRIMEIRO = 1 SEGUNDO = 2 ETC
const char* clientID = "Client ID 1";

const char* mqtt_username = "username do broker";
const char* mqtt_password = "senha do broker";

float initialTime = millis();

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (client.connect(clientID, mqtt_username, mqtt_password)) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {

  // Begin Serial on 115200
  // Remember to choose the correct Baudrate on the Serial monitor!
  // This is just for debugging purposes
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password (Setup Earlier in the Instructable)
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }

  client.subscribe(mqtt_topic);
  client.setCallback(callback);
  
  pinMode(RelePin, OUTPUT); //rele 
  
}

void callback(char* topic, byte* message, unsigned int length) {

  char messageTemp[15];
  
  for (int i = 0; i < length; i++) {
    messageTemp[i] = (char)message[i];
  }
  messageTemp[length] = '\0';
  
  if(strcmp(messageTemp,"1") == 0){
    digitalWrite(RelePin, HIGH);   // turn the LED on or off, the opposite of the current state
    
    
  } else{
    digitalWrite(RelePin, LOW);
  
  } 

}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");
  
  float currentTime = millis();
  
  if( (currentTime - initialTime)>= 5000){
    
    initialTime = millis();
  }

}
