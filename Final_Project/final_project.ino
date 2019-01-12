/*=================
 * By: Miguel Maranha 2012138309
 * With: Luis Cruz  2011164454
* =================*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "SparkFunHTU21D.h"
// *#*#64663#*#*
// Create Sensor Instance
HTU21D myHumidity;

// Initialize ESP_Client Wifi
WiFiClient espClient; //Change This name if you use multiple ESP's
PubSubClient client(espClient);


//======== Global Variables ========//
int temp_refresh_rate = 20000 ; // HTU21D Refreshrate (Send Data to RPI every N secs)
int light_refresh_rate = 350 ; // LDR's Refreshrate (Send Data to RPI every N secs)
const int led = LED_BUILTIN; // Feedback LED
String serialdata="" ;
boolean stringComplete = false;  // whether the string is complete
char str_array[20];
int ldr_1, ldr_2, ldr_3, ldr_4;
float lux_a,lux_b,lux_c,lux_d;
char ldr_a[5],ldr_b[5],ldr_c[5],ldr_d[5]; // Acho que isto d? com 4 verificar

//  ESP Network Credencials
const char* ssid = "DESKTOP-RR394AO 1491";
const char* password = "esmr1234";

// MQQT_Broker (Raspebry pi IP)
const char* mqtt_server = "192.168.137.114";

// Aux Timers
// HTU21D 
long now_temp = millis();
long lastMeasure_temp = 0;
// LDR
long now_light = millis();
long lastMeasure_light = 0;


//======== Functions ========//
void setup_wifi(); // Connect ESP8266 to Router
void reconnect();
void callback(String topic, byte* message, unsigned int length) ;
void serialEvent(); // Receive ADC values from PIC24


void setup() {
  myHumidity.begin(); // Inicia Class do sensor de HTU21D
  
  pinMode(led, OUTPUT); // Feedback LED
  digitalWrite(led, HIGH); // Write to (HIGH IS OFF)
  
  Serial.begin(19200); // Begin Serial Comms
  setup_wifi(); // Call Start Wifi
  client.setServer(mqtt_server, 1883); // Start Client Server at @Port
  client.setCallback(callback); // ??? No idea #REVER#

  serialdata.reserve(200);
}

void loop() {
  // Trying to Connect Client to MQTT Broker
  if (!client.connected()) {
    reconnect();
  }

  // Connected With Name
  if(!client.loop())
    client.connect("ESP8266Client");

  now_light = millis();
  if (now_light - lastMeasure_light > light_refresh_rate) {
    lastMeasure_light = now_light;
    serialEvent();
    if (stringComplete) {
      //Serial.println(serialdata);
      // Convert UART rcv string to CharArray
      serialdata.toCharArray(str_array, 22);
      //Serial.print(str_array);
	  
      // Recebe String da PIC no formato <XXXX,XXXX,XXXX,XXXX> Guarda Para Endereço
      sscanf(str_array, "<%d,%d,%d,%d>", &ldr_1, &ldr_2, &ldr_3, &ldr_4);
      //Serial.printf("\nA:%d | B:%d | C:%d | D:%d |",ldr_1,ldr_2,ldr_3,ldr_4);

      //==== Sensor Calibration (Convert to Lux), acording to Excel ====//

      // Pelo Wolfram, valores lumens da lampada
      //lux_a=1000/3*log(20*ldr_1/2403); // Im dumb sorry
      //lux_b=10000/31*log(100*ldr_2/17327);
      //lux_c=10000/71*log(5000*ldr_3/14473);
      //lux_d=1250/7*log(1000*ldr_2/14587);

      // Pelo Matlab com valores da lampada
      lux_a=exp((100*ldr_1)/45543 + 1198/893);
      lux_b=exp((100*ldr_2)/71513 + 97496/71513);
      lux_c=exp((25*ldr_3)/17383 + 55805/34766);
      lux_d=exp((100*ldr_4)/72627 + 115390/72627);
      
      // Pelo Wolfram com valores do telmovel
      //lux_a=exp((100*ldr_1+69571)/31248);
      //lux_b=exp((100*ldr_2+73659)/50493);
      //lux_c=exp((10*ldr_3+16597)/5254);
      //lux_d=exp((5*(10*ldr_4+19032))/29817);
      
      // Pelo Matlab com Valores do Telemovel
//      lux_a=exp((25*ldr_1)/7812+69571/31248); 
//      lux_b=exp((100*ldr_2)/50493 + 24553/16831);
//      lux_c=exp((5*ldr_3)/2627 + 16597/5254);
//      lux_d=exp((50*ldr_4)/29817 + 31705/9939);
      // clear the string:
      serialdata = "";
      stringComplete = false;
    }

    // Conversion to const char*
    itoa(lux_a,ldr_a,10);
    itoa(lux_b,ldr_b,10);
    itoa(lux_c,ldr_c,10);
    itoa(lux_d,ldr_d,10);
    
    // Send Data by topics
    client.publish("LDR_A", ldr_a);
    client.publish("LDR_B", ldr_b);
    client.publish("LDR_C", ldr_c);
    client.publish("LDR_D", ldr_d);
    
  }
  
  now_temp = millis();
  // Publishes new temperature and humidity every 30 seconds
  if (now_temp - lastMeasure_temp > temp_refresh_rate) {
    lastMeasure_temp = now_temp;
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = myHumidity.readHumidity();
    // Read temperature as Celsius (the default)
    float t = myHumidity.readTemperature();
    
    unsigned int acq = analogRead(A0);
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Computes temperature values in Celsius
    static char temperatureTemp[7];
    dtostrf(t, 6, 2, temperatureTemp);
    
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);
    
//    Serial.print("\nHumidity: ");
//    Serial.print(h);
//    Serial.print(" %\t Temperature: ");
//    Serial.print(t);
//    Serial.print(" ºC ");
    
    // Publishes Temperature and Humidity values
    client.publish("room/temp", temperatureTemp);
    client.publish("room/humd", humidityTemp);
  }
}


//======== Functions ========//
//==== Serial Receive ====//
void serialEvent(){
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    serialdata += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

//==== Connect ESP8266 to Router ====//
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("\nMessage arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic room/led, you check if the message is either on or off. Turns the led GPIO according to the message
  if(topic=="room/led"){
      Serial.print("Changing Room led to ");
      if(messageTemp == "on"){
        digitalWrite(led, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(led, HIGH);
        Serial.print("Off");
      }
  }
  Serial.println();
}



// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("room/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
