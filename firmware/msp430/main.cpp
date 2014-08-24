
/*
MSP430f5529 Firmware for uOSF Service
Author : Byunghun Hwang <bhhwang@nsynapse.com>
 */


#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <mqtt.h>

#define HOST_BROADCAST_PORT  8888
#define MQTT_ID  "msp430dev03"
#define PUB_LED  P2_5
#define SUB_LED  P2_4

char ssid[] = "nsynapse";
char pass[] = "ghkdqudgns";

WiFiUDP Udp;
WiFiClient client;
MQTTClient mqtt(&client);

IPAddress BroadcastIP(0,0,0,0);
IPAddress DeviceIP(0,0,0,0);
IPAddress SubnetMask(0,0,0,0);
IPAddress HostIP(0,0,0,0);

enum {IDLE = 0, INIT=10, FINDHOST=20, MQTT_INIT=30, MQTT_START=40, MQTT_STOP=50 };
int state;

boolean host_connected = false;

//defined function

void turnon(int pin){  digitalWrite(pin, HIGH);}
void turnoff(int pin){  digitalWrite(pin, LOW);}

void enable_indication()
{
  TA0CCTL0 = CCIE;
  TA0CCR0 = 32768/6;  //(32.768KHz)
  TA0CTL = TASSEL_1+MC_1+TACLR+ID_0;  //CLK Source(ACLK)+MC1(up mode)+TACLR(clear time)+ID1
}
void disable_indication(){  TA0CTL = 0;  TA0CCTL0 &= ~CCIE; }

void enable_subscribe()
{
  TA1CCTL0 = CCIE;
  TA1CCR0 = 32768/3;
  TA1CTL = TASSEL_1+MC_1+TACLR+ID_0;
}
void disable_subscribe(){  TA1CTL = 0;  TA1CCTL0 &= ~CCIE; }

void enable_publish()
{
  TA2CCTL0 = CCIE;
  TA2CCR0 = 32768/10;
  TA2CTL = TASSEL_1+MC_1+TACLR+ID_0;
}
void disable_publish(){  TA2CTL = 0;  TA2CCTL0 &= ~CCIE; }

void disable_WDT(){ WDTCTL = WDTPW + WDTHOLD; }

int indicator_state;
int prev_indicator_state;
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{

  switch(indicator_state)
  {
    case IDLE:
    {
      turnoff(RED_LED); turnoff(GREEN_LED);
    }
    break;
    case INIT:
    {
      if(prev_indicator_state!=indicator_state) { turnon(RED_LED); turnoff(GREEN_LED); prev_indicator_state=indicator_state; }
      else { turnoff(RED_LED); turnoff(GREEN_LED); prev_indicator_state=-1; }
    }
    break;
    case FINDHOST:
    {
      if(prev_indicator_state!=indicator_state) { turnon(GREEN_LED); turnoff(RED_LED); prev_indicator_state=indicator_state; }
      else { turnoff(GREEN_LED); turnoff(RED_LED); prev_indicator_state=-1; }
    }
    break;
    case MQTT_INIT:
    {
      turnon(GREEN_LED);
    }
    break;
    case MQTT_START:
    {
      if(prev_indicator_state!=indicator_state) { turnon(RED_LED); prev_indicator_state=indicator_state; }
      else { turnoff(RED_LED); prev_indicator_state=-1; }
    }
    break;
    case MQTT_STOP:
    break;
  }
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{

}

int prev_pub = 0;
int prev_sub = 0;
bool flag = false;
unsigned int value = 0;
#pragma vector = TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR(void)
{
  if(flag==true)
    return;

  flag = true;
  if(mqtt.connected())
  {
    mqtt.publish("comp", "test");

    /*if(mqtt.loop())
    {
      if(prev_sub)
      {
        turnon(SUB_LED);
        prev_sub = 0;
      }
      else
      {
        turnoff(SUB_LED);
        prev_sub = 1;
      }
    }*/

    if(prev_pub)
    {
      turnon(PUB_LED);
      prev_pub = 0;
    }
    else
    {
      turnoff(PUB_LED);
      prev_pub = 1;
    }
  }
  else
    turnon(PUB_LED);

  flag = false;
}

int test = 0;
void callback(char* topic, byte* payload, unsigned int length)
{
  if(test)
  {
    turnon(SUB_LED);
    test = 0;
  }
  else
  {
    turnoff(SUB_LED);
    test = 1;
  }
  /*byte* p = (byte*)malloc(length);
  memcpy(p,payload,length);

  free(p);*/
  //turnon(SUB_LED);
}


bool findhost()
{
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    HostIP = Udp.remoteIP();
    return true;
  }
  return false;
}

bool mqtt_init()
{
  mqtt.setConnection(HostIP, 1883);
  mqtt.setCallback(callback);
  if(mqtt.connect(MQTT_ID))
  {
    mqtt.subscribe("comp2");
    return true;
  }
  return false;
}

/*bool mqtt2_init()
{
  mqtt2.setConnection(HostIP, 1883);
  if(mqtt2.connect("wa"))
    if(mqtt2.subscribe("comp2"))
      return true;
    return false;
}*/


void push_stop_mqtt()
{
  state = MQTT_STOP;
}
void push_start_mqtt()
{
  state = MQTT_START;
}

void setup() {

  Serial.begin(115200);

  // Set communication pins for CC3000
  WiFi.setCSpin(18);  // 18: P2_2 @ F5529, PE_0 @ LM4F/TM4C
  WiFi.setENpin(2);   //  2: P6_5 @ F5529, PB_5 @ LM4F/TM4C
  WiFi.setIRQpin(19); // 19: P2_0 @ F5529, PB_2 @ LM4F/TM4C

  //i/o setting
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(PUB_LED, OUTPUT);
  pinMode(SUB_LED, OUTPUT);
  pinMode(PUSH1, INPUT_PULLUP); // released = HIGH, pressed = LOW
  attachInterrupt(PUSH1, push_start_mqtt, FALLING);
  pinMode(PUSH2, INPUT_PULLUP);
  attachInterrupt(PUSH2, push_stop_mqtt, FALLING);

  //default
  turnoff(RED_LED);
  turnoff(GREEN_LED);
  turnoff(PUB_LED);
  turnoff(SUB_LED);

  state = INIT;

  enable_indication();
  //disable_WDT();

  //__enable_interrupt();
  //__bis_SR_register(LPM3_bits+GIE); // LPM0 with interrupts enabled

}

void loop()
{
  Serial.println(state);
  switch(state)
  {
    case IDLE:  break;
    case INIT:
    {
      indicator_state = INIT;

      WiFi.begin(ssid, pass);

      while(WiFi.localIP()[0]==0)
        delay(10);

      //getting ip address
      DeviceIP = WiFi.localIP();
      SubnetMask = WiFi.subnetMask();
      BroadcastIP[0] = (DeviceIP[0]|~SubnetMask[0]);
      BroadcastIP[1] = (DeviceIP[1]|~SubnetMask[1]);
      BroadcastIP[2] = (DeviceIP[2]|~SubnetMask[2]);
      BroadcastIP[3] = (DeviceIP[3]|~SubnetMask[3]);

      state = FINDHOST;
    }
    break;
    case FINDHOST:
    {
      indicator_state = FINDHOST;

      Udp.flush();
      Udp.begin(HOST_BROADCAST_PORT);

      state = FINDHOST+1;
    }
    break;
    case FINDHOST+1:
    {
      if(findhost())
      {
        Udp.stop();  //!!!must stop udp for mqtt client
        state = MQTT_INIT;
      }
    }
    break;
    case MQTT_INIT:
    {
      indicator_state = MQTT_INIT;
      if(mqtt_init())
      {
        state = IDLE;
      }
    }
    break;
    case MQTT_START:
    {
      indicator_state = MQTT_START;
      if(mqtt.connected())
      {
        mqtt.publish("comp", "{uid:00000000000000000000000000000001,value:10,components:[\"compc\",\"compd\"]}");
        delay(500);
      }
      /*enable_publish();
      disable_WDT();  //disable watchdog timer
      delay(200);
      state = IDLE;*/
    }
    break;
    case MQTT_STOP:
    {
      disable_publish();
      //disable_subscribe();
      delay(200);
      state = IDLE;
      turnoff(PUB_LED);
      turnoff(SUB_LED);
    }
    break;
  }

}
