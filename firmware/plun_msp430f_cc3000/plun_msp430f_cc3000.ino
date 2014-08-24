
/*
* PLUN Satellite for MSP430
 */
/*
Development environment setup
 (1) paste ArduinoJson Parser into the hardware/lm4f/libraries directory
     - cloned from https://github.com/bblanchon/ArduinoJson
 (2) paste MQTT into the hardware/lm4f/libraries directory
 */


#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <TCPclient.h>
#include <JsonGenerator.h>
#include <JsonParser.h>
#include <MQTTClient.h>

#define LED1  P1_0
#define LED2  P4_7

using namespace ArduinoJson::Parser;


/*
* Device Profile (user can edit below)
 */
char* device_id = "noname MSP430 device";
char* device_sub_topic = "mqtt_in";
char* device_pub_topic = "mqtt_out";


/*
* global variables
 */
char ssid[] = "nsynapse";
char pass[] = "ghkdqudgns";
IPAddress group_ip(239,0,0,1);
IPAddress host_ip(0,0,0,0);
unsigned int group_port = 8009;
unsigned int udp_local_port = 2807;
long previousMillis = 0;
long interval = 1000;
#define UDP_BUFFER_LEN  128

WiFiUDP Udp;
MQTTClient Mqttclient;

enum State {
  IDLE=0, GADGET_READY=10, WIFI_READY=20, MQTT_READY=30, MQTT_PUB=40, MQTT_SUB=50, MQTT_CLOSE=60 };
int system_state = IDLE;

const char* msg_knock = "@plunhost/knock";
char udp_msg_buffer[UDP_BUFFER_LEN];
char mqtt_buffer[1024];
JsonParser<128> parser;

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
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  String in_msg;
  int n = 0;
  for(n=0;n<length;n++)
    in_msg += (char)payload[n];

  api_process(&in_msg);  
  digitalWrite(BLUE_LED, LOW);
}

void connect_retry()
{
  digitalWrite(LED1, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  
  system_state = WIFI_READY;
}

/*
* setup
 */
void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(PF_4, INPUT_PULLUP);
  
  attachInterrupt(PF_4, connect_retry, FALLING);

  digitalWrite(RED_LED, HIGH);


  Serial.begin(115200);

  //cc3000 setup
  WiFi.setCSpin(18);  // 18: P2_2 @ F5529, PE_0 @ LM4F/TM4C
  WiFi.setENpin(2);   //  2: P6_5 @ F5529, PB_5 @ LM4F/TM4C
  WiFi.setIRQpin(19); // 19: P2_0 @ F5529, PB_2 @ LM4F/TM4C

  WiFi.begin(ssid, pass);

  while(WiFi.status()!=WL_CONNECTED || WiFi.localIP()[0]==0)
    delay(10);
    
  Udp.begin(udp_local_port);

  digitalWrite(RED_LED, LOW);    
  
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
      Serial.println("{Command:0}");
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
      Udp.beginPacket(group_ip, group_port);
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
      Mqttclient.connect(device_id);
      Mqttclient.subscribe(device_sub_topic);
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
      Mqttclient.publish(device_pub_topic, mqtt_msg);
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






















