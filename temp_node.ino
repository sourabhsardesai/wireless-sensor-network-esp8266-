

#include <ESP8266WiFi.h>     // wifi library to access the wifi apis for esp
#include <PubSubClient.h>        // mqtt client library 


const char* ssid = "DesiBoys";    // ssid for wifi netwrok
const char* password = "indopakdesiboys";
byte mqtt_server[] = {35,166,15,82};// my mqtt broker ip addresss, 1883 is a default port for broker



WiFiClient espClient;          //  declaring wifi client
PubSubClient client(espClient);                //   passing the wifii client for the pubsub client


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Global varaibles 

char buf[30];


char* outTopic1     = "4321/temp";


char* inTopic1      = "4321/t1";

char* clientId     = "temp_node";

char* payloadOn1  = "true";
char* payloadOn2  = "false";
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This is the function which executes to accomplish the wifi connection 
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
//WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {      // this loops till the esp connects to the internet

    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
   Serial.println(WiFi.macAddress());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void callback(char* topic, byte* payload, unsigned int length) {    // this function is callback function which receives the data from the mqtt server, it is event based.
    char* json;
  json = (char*) malloc(length + 1);        // json receives the data from the payload byte varaible

  memcpy(json, payload, length);                   // memory has to be allocated hence memcpy functions is used to allocated dedicated memory to staore the data from payload varaible
  
  json[length] = '\0';
Serial.println(topic);
Serial.println(json);
  if (String(payloadOn2) == String(json))  // The payloadOn2 varaible is used store the expected value coming from the received and json has the current value of the data received which is compared and if the data matches the respective command is executed.
 
    {
    digitalWrite(2,HIGH);
    digitalWrite(D2,LOW);
  }

  if (String(payloadOn1) == String(json))  // The payloadOn1 varaible is used store the expected value coming from the received and json has the current value of the data received which is compared and if the data matches the respective command is executed.

    {
    digitalWrite(2,LOW);
    digitalWrite(D2,HIGH);
  }


 
  free(json);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reconnect function is used to keep esp connected with the server


  
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {      //   this loop makes sure that the connections is  alive to the broker
 
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("temp_node")) {    //  this loop connects it with mqtt broker
   
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      client.subscribe("4321/t1");           // subscribes to the topic in the inTopic1 varaible
   
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This function i executed at the begining and only once all the basic setup is stated in this function


void setup() {

  int sensorPin = A0; 
  
  Serial.begin(115200);
  
  setup_wifi();                   //  Make connection to the wifi
  client.setServer(mqtt_server, 1883);    // pass mqtt broker ip address and port number

  client.setCallback(callback);     // set callback function by passing callback function

  Serial.println("start");
  

    
   pinMode(sensorPin, INPUT);     //  configure the A0 pin as input
      pinMode(2, OUTPUT);        //   configure the led pin as output
       pinMode(D2, OUTPUT);                    //   configure the D2 pin as output
//   pinMode(9, INPUT);//ir6
digitalWrite(2,HIGH);   // turn the led pin  high
digitalWrite(D2,LOW);
  }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This loop keeps going continuosly in which main logic is written
 

void loop() {


 if (!client.connected()) {       //   checks for the continuos connection
    reconnect();
  }
  
//Serial.println("in loop");   
 char buf[30];    
  float sen1, sen2;
     
  sen1=analogRead(A0);     // Reads the analog data from A0
float  val = (float(sen1)/float(1024)) * float(3300);      // Divides raw value with 1024 (3v) maximum
 
Serial.println ( val);

float lit = val/float(10); // Gives the value in degree celcius 
Serial.println ( lit);
 String k=String(lit);
 k.toCharArray(buf,6);
 client.publish(outTopic1,buf);
 Serial.println (sen1);



  

 
    client.loop();
  delay(2000);
  
}
