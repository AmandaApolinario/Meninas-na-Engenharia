/*
 * ESP8266 (Adafruit HUZZAH) Mosquitto MQTT Publish Example
 * Thomas Varnish (https://github.com/tvarnish), (https://www.instructables.com/member/Tango172)
 * Made as part of my MQTT Instructable - "How to use MQTT with the Raspberry Pi and ESP8266"
 */
//http://arduino.esp8266.com/stable/package_esp8266com_index.json
//https://dl.espressif.com/dl/package_esp32_index.json
#include <Bounce2.h> // Used for "debouncing" the pushbutton
#include <WiFi.h> // Enables the ESP8266 to connect to the local network (via WiFi)
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker
#define RelePin1 23 //D22
#define RelePin2 21 //D21
#define RelePin3 19 //D19
#define SensorPin1 36
#define SensorPin2 32
#define SensorPin3 39
#define SensorPin4 25
#define SensorPin5 27
#define SensorPin6 12


//MUDAR PARA O IP ESTATICO DA RASP
const char* mqtt_server = "IP do broker da forma 192.168.x.x";

//MUDAR ESSAS PARA A REDE
const char* ssid = "nome da rede";
const char* wifi_password = "senha da rede";

//MUDAR AS NUMERACOES DOS ESPS
const char* mqtt_topic1 = "rele/quarto";
const char* mqtt_topic2 = "rele/quinto";
const char* mqtt_topic3 = "rele/sexto";
//MUDAR O NUMERO DE ACORDO COM O RELE EX PRIMEIRO = 1 SEGUNDO = 2 ETC
const char* clientID = "Client ID 1";

const char* mqtt_username = "username do broker";
const char* mqtt_password = "senha do broker";

float initialTime1 = millis();
float initialTime2 = millis();
float initialTime3 = millis();

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
      client.subscribe(mqtt_topic1);
      client.subscribe(mqtt_topic2);
      client.subscribe(mqtt_topic3);
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

  client.subscribe(mqtt_topic1);
  client.subscribe(mqtt_topic2);
  client.subscribe(mqtt_topic3);
  client.setCallback(callback);
  
  pinMode(RelePin1, OUTPUT); //rele 1
  pinMode(RelePin2, OUTPUT); //rele 2 
  pinMode(RelePin3, OUTPUT); //rele 3  
  
}

void callback(char* topic, byte* message, unsigned int length) {

  char messageTemp[15];
  
  for (int i = 0; i < length; i++) {
    messageTemp[i] = (char)message[i];
  }
  messageTemp[length] = '\0';
  
  if( strcmp(topic,"rele/quarto")==0){
    int adc = analogRead(SensorPin4);
    if (analogRead(SensorPin4) >= 0){
      if(strcmp(messageTemp,"1") == 0){
        digitalWrite(RelePin1, HIGH);   // turn the LED on or off, the opposite of the current state
        printf("high 1\n");
    
  } else{
    digitalWrite(RelePin1, LOW);
    printf("low 1\n");
    }
    }
  }


  if(strcmp(topic,"rele/quinto")==0){

    if(analogRead(SensorPin5) >= 0){
      if(strcmp(messageTemp,"1") == 0){
      digitalWrite(RelePin2, HIGH);   // turn the LED on or off, the opposite of the current state
      printf("high 2\n");
    
  } else{
    digitalWrite(RelePin2, LOW);
    printf("low 2\n");
    }
    }
  }


  if(strcmp(topic,"rele/sexto")==0){

    if(analogRead(SensorPin6) >= 0){
          if(strcmp(messageTemp,"1") == 0){
    digitalWrite(RelePin3, HIGH);   // turn the LED on or off, the opposite of the current state
    printf("high 3\n");
    
  } else{
    digitalWrite(RelePin3, LOW);
    printf("low 3\n");
    }
  }
    }

}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP32Client");
    
  float currentTime1 = millis();
  float currentTime2 = millis();
  float currentTime3 = millis();
  
   if( (currentTime1 - initialTime1)>= 10000){
    int sampling = 400;                        //Numero de amostras 
    float adc_rms = 0;                        //ADC rms
    float V_rms = 0;                         //Tensao rms
    float A_rms = 0;                        //Corrente rms
    float P = 0;                           //Potencia
    float conv = 0.00080586;            //Valor de conversao adc para tensao
    int aux[sampling];
    for(int i=0; i < sampling; i++){
      aux[i] = 0;
    }
    for(int i = 0; i < sampling; i++){
      aux[i] = analogRead(SensorPin1);
    }
    for(int i = 0; i < sampling; i++){
      adc_rms = adc_rms + ((aux[i]*conv)*(aux[i]*conv))/sampling;
    }
    if(5.2 < adc_rms < 5.28){
      V_rms = (adc_rms*24.33) - 5.22;
    }
    else if(5.28 < adc_rms < 5.36){
      V_rms = (adc_rms*23.97) - 5.3;
    }
    else {
      V_rms = (adc_rms*21.17) - 6.1; 
    }
    A_rms = adc_rms/66;
    P = (V_rms)*(A_rms);

    char buffer[65];
    buffer[0] = '4';
    buffer[1] = ' ';
    for (int i = 2; i < 8 ; i++) {
      buffer[i] = (char)String(P)[i-2];
    }
    buffer[8]= '\0';
    Serial.println(buffer);
    client.publish("rele/Status", buffer);

    initialTime1 = millis();
  }

  if( (currentTime2 - initialTime2) >= 10000){
    int sampling = 400;                        //Numero de amostras 
    float adc_rms = 0;                        //ADC rms
    float V_rms = 0;                         //Tensao rms
    float A_rms = 0;                        //Corrente rms
    float P = 0;                           //Potencia
    float conv = 0.00080586;            //Valor de conversao adc para tensao
    int aux[sampling];
    for(int i=0; i < sampling; i++){
      aux[i] = 0;
    }
    for(int i = 0; i < sampling; i++){
      aux[i] = analogRead(SensorPin2);
    }
    for(int i = 0; i < sampling; i++){
      adc_rms = adc_rms + ((aux[i]*conv)*(aux[i]*conv))/sampling;
    }
    if(5.2 < adc_rms < 5.28){
      V_rms = (adc_rms*24.33) - 5.22;
    }
    else if(5.28 < adc_rms < 5.36){
      V_rms = (adc_rms*23.97) - 5.3;
    }
    else {
      V_rms = (adc_rms*21.17) - 6.1; 
    }
    A_rms = adc_rms/66;
    P = (V_rms)*(A_rms);

    char buffer[65];
    buffer[0] = '5';
    buffer[1] = ' ';
    for (int i = 2; i < 8 ; i++) {
      buffer[i] = (char)String(P)[i-2];
    }
    buffer[8]= '\0';
    Serial.println(buffer);
    client.publish("rele/Status", buffer);

    initialTime2 = millis();
  }

  if( (currentTime3 - initialTime3)>= 10000){
    int sampling = 400;                        //Numero de amostras 
    float adc_rms = 0;                        //ADC rms
    float V_rms = 0;                         //Tensao rms
    float A_rms = 0;                        //Corrente rms
    float P = 0;                           //Potencia
    float conv = 0.00080586;            //Valor de conversao adc para tensao
    int aux[sampling];
    for(int i=0; i < sampling; i++){
      aux[i] = 0;
    }
    for(int i = 0; i < sampling; i++){
      aux[i] = analogRead(SensorPin3);
    }
    for(int i = 0; i < sampling; i++){
      adc_rms = adc_rms + ((aux[i]*conv)*(aux[i]*conv))/sampling;
    }
    if(3.2 < adc_rms < 3.28){
      V_rms = (adc_rms*38.957) - 3.22;
    }
    else if(3.28 < adc_rms < 3.36){
      V_rms = (adc_rms*37.798) - 3.3;
    }
    else {
      V_rms = (adc_rms*30.602) - 4.1; 
    }
    A_rms = adc_rms/66;
    P = (V_rms)*(A_rms);
    Serial.println(adc_rms);
    Serial.println(P);

    char buffer[65];
    buffer[0] = '6';
    buffer[1] = ' ';
    for (int i = 2; i < 8 ; i++) {
      buffer[i] = (char)String(P)[i-2];
    }
    buffer[8]= '\0';
    Serial.println(buffer);
    client.publish("rele/Status", buffer);

    initialTime3 = millis();
  }
  
}
