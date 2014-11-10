
/*
* PLUN Satellite for TM4C
 */
/*
Development environment setup
 (1) import ArduinoJson Parser Library (sketch > import library)
     - cloned from https://github.com/bblanchon/ArduinoJson
 (2) import MQTT library
 (3) import PLUN library
 */


#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <TCPclient.h>
#include <JsonGenerator.h>
#include <JsonParser.h>
#include <MQTTClient.h>
#include <plun.h>



using namespace std;

using namespace ArduinoJson::Parser;

/*
* Definitions & Profiles
*/
#define UDP_BUFFER_LEN  128
#define JSON_BUFFER_LEN  1024
#define DEFAULT_GROUP_PORT  8009
#define DEFAULT_UDP_PORT  2807

#define PLUN_DEVICE_ID  "TM4C_DEVICE"
#define PLUN_PUB_TOPIC  "P00000000"
#define PLUN_SUB_TOPIC  "S00000000"



/*void registerApi(const char* funcname, void(*func)(void))
{
  apiContainer[String(funcname)] = func;
}*/


/*
* Global variables
 */
char ssid[] = "nsynapse";
char pass[] = "ghkdqudgns";
IPAddress group_ip(239,0,0,1);
IPAddress host_ip(0,0,0,0);
unsigned int udp_local_port = 2807;
long previousMillis = 0;
long interval = 1000;

WiFiUDP Udp;
MQTTClient Mqttclient;

enum State {
  IDLE=0, GADGET_READY=10, WIFI_READY=20, MQTT_READY=30, MQTT_PUB=40, MQTT_SUB=50, MQTT_CLOSE=60 };
int system_state = IDLE;

const char* msg_knock = "@plunhost/knock";
char udp_msg_buffer[UDP_BUFFER_LEN];
char mqtt_buffer[JSON_BUFFER_LEN];
JsonParser<JSON_BUFFER_LEN/8> parser;

void api_process(String* msg)
{
  msg->toCharArray(mqtt_buffer, msg->length()+1);
  JsonObject root = parser.parse(mqtt_buffer);
  if(root.success())
  {
  }
  else
  {
  }
}

void mqtt_callback(char* topic, byte* payload,int length) {
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  String in_msg;
  int n = 0;
  for(n=0;n<length;n++)
    in_msg += (char)payload[n];

  api_process(&in_msg);  
  digitalWrite(BLUE_LED, LOW);
}

void connect_retry()
{
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  
  system_state = WIFI_READY;
}

/*
* setup
 */
void setup()
{
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  //pinMode(PF_4, INPUT_PULLUP);
  
  //attachInterrupt(PF_4, connect_retry, FALLING);


  Serial.begin(9600);
  Serial.println("0");
  Serial.println("1");
  Serial.println("2");  
  delay(10000);  
  
  digitalWrite(RED_LED, LOW);

  //cc3000 setup
  WiFi.setCSpin(18);  // 18: P2_2 @ F5529, PE_0 @ LM4F/TM4C
  WiFi.setENpin(2);   //  2: P6_5 @ F5529, PB_5 @ LM4F/TM4C
  WiFi.setIRQpin(19); // 19: P2_0 @ F5529, PB_2 @ LM4F/TM4C
  

  WiFi.begin(ssid, pass);

  while(WiFi.status()!=WL_CONNECTED || WiFi.localIP()[0]==0)
    delay(10);
    
  Udp.begin(DEFAULT_UDP_PORT);

  digitalWrite(RED_LED, HIGH);    
  
  system_state = WIFI_READY;    //system start point
}


void loop()
{
  switch(system_state)
  {
  case IDLE: 
    {
    }
    break;
    
  //command for getting gadget profile  
  case GADGET_READY:
    {
      //Serial.println("{Command:0}");
      system_state++;
    }
    break;
  
  case GADGET_READY+1:
    {
      system_state = WIFI_READY;
    }
    break;

  case WIFI_READY: 
    {
      digitalWrite(GREEN_LED, HIGH);
      Udp.beginPacket(group_ip, DEFAULT_GROUP_PORT);
      Udp.write((byte*)msg_knock, strlen(msg_knock));
      Udp.endPacket();
      delay(200);
      system_state++;
    }
    break;

  case WIFI_READY+1:
    {
      int size = Udp.parsePacket();
      if(size)
      {
        int rcv_len = Udp.read(udp_msg_buffer,UDP_BUFFER_LEN);
        if(rcv_len>0)
        {
          udp_msg_buffer[rcv_len] = 0;
          host_ip = Udp.remoteIP();
          system_state++;
        }
      }
      else
        system_state = WIFI_READY;
        
      digitalWrite(GREEN_LED, LOW);
      delay(200);
    }
    break;

  case WIFI_READY+2:
    {
      system_state = MQTT_READY;
    }
    break;

  case MQTT_READY:
    {
      Mqttclient.setConnection(host_ip, 1883, mqtt_callback);
      Mqttclient.connect(PLUN_DEVICE_ID);
      Mqttclient.subscribe(PLUN_SUB_TOPIC);
      system_state = MQTT_SUB;
    }
    break;
    
  case MQTT_READY+1:
    {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
      system_state = MQTT_PUB;
    }
    break;
    
  case MQTT_PUB:
    {
      char* mqtt_msg = "{this is a test data}";
      Mqttclient.publish(PLUN_PUB_TOPIC, mqtt_msg);
      system_state = MQTT_SUB;
    }
    break;
    
  case MQTT_SUB:
    {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
      Mqttclient.loop();
    }
    break;
    
  case MQTT_CLOSE:
    {
      Mqttclient.disconnect();
      system_state = IDLE;
    }
    break;
  }

  delay(1);
}






















