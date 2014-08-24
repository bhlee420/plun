/*
MQTT 3.1 for MSP430F5529LP
*/

#ifndef mqtt_h
#define mqtt_h
#include "utility/SimplelinkWifi.h"
#include "TCPclient.h"
#include "Energia.h"

#define MQTT_MAX_PACKET_SIZE	256
#define MQTT_KEEPALIVE	14400 	///keepalive interval in seconds

#define MQTTPROTOCOLVERSION 3
#define MQTTCONNECT 		1 << 4 // Client request to connect to Server
#define MQTTCONNACK 		2 << 4 // Connect Acknowledgment
#define MQTTPUBLISH 		3 << 4 // Publish message
#define MQTTPUBACK 			4 << 4 // Publish Acknowledgment
#define MQTTPUBREC 			5 << 4 // Publish Received (assured delivery part 1)
#define MQTTPUBREL 			6 << 4 // Publish Release (assured delivery part 2)
#define MQTTPUBCOMP 		7 << 4 // Publish Complete (assured delivery part 3)
#define MQTTSUBSCRIBE 		8 << 4 // Client Subscribe request
#define MQTTSUBACK 			9 << 4 // Subscribe Acknowledgment
#define MQTTUNSUBSCRIBE 	10 << 4 // Client Unsubscribe request
#define MQTTUNSUBACK 		11 << 4 // Unsubscribe Acknowledgment
#define MQTTPINGREQ 		12 << 4 // PING Request
#define MQTTPINGRESP 		13 << 4 // PING Response
#define MQTTDISCONNECT 		14 << 4 // Client is Disconnecting
#define MQTTRESERVED 		15 << 4 // Reserved

#define MQTTQOS0 (0 << 1)
#define MQTTQOS1 (1 << 1)
#define MQTTQOS2 (2 << 1)


class MQTTClient {
private:
	WiFiClient* _client;
	uint8_t buffer[MQTT_MAX_PACKET_SIZE];
	uint16_t nextMsgId;
	unsigned long lastOutActivity;
	unsigned long lastInActivity;
	bool pingOutstanding;
	void (*callback)(char*,uint8_t*, unsigned int);
	uint16_t readPacket();
	uint8_t readByte();
	bool write(uint8_t header, uint8_t* buf, uint16_t length);
	uint16_t writeString(char* src, uint8_t* dest, uint16_t pos);
	IPAddress hostip;
	uint16_t port;

public:
   MQTTClient(WiFiClient* client);
   void setConnection(IPAddress ip, uint16_t port);
   void setCallback(void (*callback)(char*,uint8_t*,unsigned int));
   bool connect(char *id);
   bool connect(char *id, char *user, char *pass);
   bool connect(char *id, char* willTopic, uint8_t willQos, uint8_t willRetain, char* willMessage);
   bool connect(char *id, char *user, char *pass, char* willTopic, uint8_t willQos, uint8_t willRetain, char* willMessage);
   void disconnect();
   bool publish(char* topic, char* payload);
   bool publish(char* topic, char* payload, uint16_t plength);
   bool publish(char* topic, char* payload, uint16_t plength, bool retained);
   bool subscribe(char *topic);
   bool loop();
   bool connected();
};

#endif
