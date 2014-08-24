
#include <WiFi.h>
#include <Energia.h>
#include <IPAddress.h>

#define MQTT_MAX_PACKET_SIZE  1024  //
#define MQTT_KEEPALIVE  15000  //maximum 255000

#define MQTTPROTOCOLVERSION  3
#define MQTTCONNECT     1 << 4  // Client request to connect to Server
#define MQTTCONNACK     2 << 4  // Connect Acknowledgment
#define MQTTPUBLISH     3 << 4  // Publish message
#define MQTTPUBACK      4 << 4  // Publish Acknowledgment
#define MQTTPUBREC      5 << 4  // Publish Received (assured delivery part 1)
#define MQTTPUBREL      6 << 4  // Publish Release (assured delivery part 2)
#define MQTTPUBCOMP     7 << 4  // Publish Complete (assured delivery part 3)
#define MQTTSUBSCRIBE   8 << 4  // Client Subscribe request
#define MQTTSUBACK      9 << 4  // Subscribe Acknowledgment
#define MQTTUNSUBSCRIBE 10 << 4 // Client Unsubscribe request
#define MQTTUNSUBACK    11 << 4 // Unsubscribe Acknowledgment
#define MQTTPINGREQ     12 << 4 // PING Request
#define MQTTPINGRESP    13 << 4 // PING Response
#define MQTTDISCONNECT  14 << 4 // Client is Disconnecting
#define MQTTReserved    15 << 4 // Reserved

#define MQTTQOS0  (0<<1)
#define MQTTQOS1  (1<<1)
#define MQTTQOS2  (2<<1)

class MQTTClient
{
private:
  WiFiClient _client;
  IPAddress host_addr;
  uint16_t host_port;
  char  buffer[MQTT_MAX_PACKET_SIZE];
  uint8_t nextMsgId;
  long lastActivity;
  void (*mqtt_callback)(char*,uint8_t*,int);
  uint8_t readPacket();
  uint8_t readByte();
  int write(uint8_t header, uint8_t* buf, uint8_t length);
  uint8_t writeString(char* str, uint8_t* buf, uint8_t pos);

public:
  MQTTClient();
  MQTTClient(IPAddress ip, uint16_t port, void(*mqtt_callback)(char*,uint8_t*,int));
  MQTTClient(IPAddress ip, uint16_t port);

  void setConnection(IPAddress ip, uint16_t port);
  void setConnection(IPAddress ip, uint16_t port, void(*mqtt_callback)(char*,uint8_t*,int));
  int connect(char* id);
  int connect(char* id, char* willTopic, uint8_t willQoS, uint8_t willRetain, char* willMessage);
  void disconnect();
  int publish(char* topic, char* payload);
  int publish(char* topic, uint8_t* payload, uint8_t plength);
  int publish(char* topic, uint8_t* payload, uint8_t plength, uint8_t retained);
  void subscribe(char* topic);
  int connected();
  int loop();
};
