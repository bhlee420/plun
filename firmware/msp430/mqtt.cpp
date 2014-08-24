#include "mqtt.h"


MQTTClient::MQTTClient(WiFiClient* client) {
   this->_client = client;
}


/*MQTTClient::MQTTClient(uint8_t *ip, uint16_t port, void (*callback)(char*,uint8_t*,unsigned int), WiFiClient& client) {

	this->_client = &client;
  	this->callback = callback;
   	this->ip = ip;
   	this->port = port;
}

MQTTClient::MQTTClient(char* domain, uint16_t port, void (*callback)(char*,uint8_t*,unsigned int), WiFiClient& client) {
	this->_client = &client;
	this->callback = callback;
	this->domain = domain;
	this->port = port;
}

void MQTTClient::setConnection(uint8_t *ip, uint16_t port)
{
	this->ip = ip;
	this->port = port;
}*/

void MQTTClient::setConnection(IPAddress ip, unsigned int port)
{
	this->hostip = ip;
	this->port = port;
}

void MQTTClient::setCallback(void (*callback)(char*,uint8_t*,unsigned int))
{
	this->callback = callback;
}

bool MQTTClient::connect(char *id) {
   return connect(id,NULL,NULL,0,0,0,0);
}

bool MQTTClient::connect(char *id, char *user, char *pass) {
   return connect(id,user,pass,0,0,0,0);
}

bool MQTTClient::connect(char *id, char* willTopic, uint8_t willQos, uint8_t willRetain, char* willMessage)
{
   return connect(id,NULL,NULL,willTopic,willQos,willRetain,willMessage);
}

bool MQTTClient::connect(char *id, char *user, char *pass, char* willTopic, uint8_t willQos, uint8_t willRetain, char* willMessage) {
   if (!connected())
   {
	int result = _client->connect(this->hostip, this->port);

     if (result)
     {
         nextMsgId = 1;
         char d[9] = {0x00,0x06,'M','Q','I','s','d','p',MQTTPROTOCOLVERSION};
         // Leave room in the buffer for header and variable length field
         unsigned int length = 5;

         unsigned int j;
         for (j = 0;j<9;j++) {
            buffer[length++] = d[j];
         }

         uint8_t v;
         if (willTopic) {
            v = 0x06|(willQos<<3)|(willRetain<<5);
         } else {
            v = 0x02;
         }

         if(user != NULL) {
            v = v|0x80;

            if(pass != NULL) {
               v = v|(0x80>>1);
            }
         }

         buffer[length++] = v;

         buffer[length++] = ((MQTT_KEEPALIVE) >> 8);
         buffer[length++] = ((MQTT_KEEPALIVE) & 0xFF);

         length = writeString(id,buffer,length);


         if (willTopic) {
            length = writeString(willTopic,buffer,length);
            length = writeString(willMessage,buffer,length);
         }

         if(user != NULL) {
            length = writeString(user,buffer,length);
            if(pass != NULL) {
               length = writeString(pass,buffer,length);
            }
         }

         write(MQTTCONNECT,buffer,length-5);

         lastInActivity = lastOutActivity = millis();

         while (!_client->available()) {
            unsigned long t = millis();
            if (t-lastInActivity > MQTT_KEEPALIVE*1000UL) {
               _client->stop();
               return false;
            }
         }
         uint16_t len = readPacket();

         if (len == 4 && buffer[3] == 0) {
            lastInActivity = millis();
            pingOutstanding = false;
            return true;
         }
     }
      _client->stop();
   }
   return false;
}

uint8_t MQTTClient::readByte() {
   while(!_client->available()) {}
   return _client->read();
}

uint16_t MQTTClient::readPacket() {
   uint16_t len = 0;
   buffer[len++] = readByte();
   uint8_t multiplier = 1;
   uint16_t length = 0;
   uint8_t digit = 0;
   do {
      digit = readByte();
      buffer[len++] = digit;
      length += (digit & 127) * multiplier;
      multiplier *= 128;
   } while ((digit & 128) != 0);

   for (uint16_t i = 0;i<length;i++)
   {
      if (len < MQTT_MAX_PACKET_SIZE) {
         buffer[len++] = readByte();
      } else {
         readByte();
         len = 0; // This will cause the packet to be ignored.
      }
   }

   return len;
}

bool MQTTClient::loop() {

   if(connected())
   {
      if (_client->available())
      {
         uint16_t len = readByte();
         if(len>0)
         {
            return true;
         }
      }
   }

   return false;

   /*above is for test*/

   if (connected()) {
      unsigned long t = millis();
      /*if ((t - lastInActivity > MQTT_KEEPALIVE*1000UL) || (t - lastOutActivity > MQTT_KEEPALIVE*1000UL)) {
         if (pingOutstanding) {
            _client.stop();
            return false;
         } else {
            buffer[0] = MQTTPINGREQ;
            buffer[1] = 0;
            _client.write(buffer,2);
            lastOutActivity = t;
            lastInActivity = t;
            pingOutstanding = true;
         }
      }*/

      /*if (_client->available()) {
         uint16_t len = readPacket();
         if (len > 0) {
            lastInActivity = t;
            uint8_t type = buffer[0]&0xF0;
            if (type == MQTTPUBLISH) {
               if (callback) {
                  uint16_t tl = (buffer[2]<<8)+buffer[3];
                  char topic[tl+1];
                  for (uint16_t i=0;i<tl;i++) {
                     topic[i] = buffer[4+i];
                  }
                  topic[tl] = 0;
                  // ignore msgID - only support QoS 0 subs
                  uint8_t *payload = buffer+4+tl;
                  callback(topic,payload,len-4-tl);
               }
            } else if (type == MQTTPINGREQ) {
               buffer[0] = MQTTPINGRESP;
               buffer[1] = 0;
               _client->write(buffer,2);
            } else if (type == MQTTPINGRESP) {
               pingOutstanding = false;
            }
         }
      }*/
      return true;
   }
   return false;
}

bool MQTTClient::publish(char* topic, char* payload) {
   return publish(topic, payload, strlen(payload),false);
}

bool MQTTClient::publish(char* topic, char* payload, uint16_t plength) {
   return publish(topic, payload, plength, false);
}

bool MQTTClient::publish(char* topic, char* payload, uint16_t plength, bool retained) {

	if(connected())
	{
		unsigned int length = writeString(topic, buffer, 5);

		for (uint16_t i=0;i<plength;i++) {
			buffer[length++] = payload[i];
		}

		uint8_t header = MQTTPUBLISH;

		if (retained) {
			header |= 1;
		}
		return write(header,buffer,length-5);
	}
	return false;
}


bool MQTTClient::write(uint8_t header, uint8_t* buf, uint16_t length) {

   uint8_t lenBuf[4];
   uint8_t llen = 0;
   uint8_t digit;
   uint8_t pos = 0;
   uint8_t rc;
   uint8_t len = length;
   do {
      digit = len % 128;
      len = len / 128;
      if (len > 0) {
         digit |= 0x80;
      }
      lenBuf[pos++] = digit;
      llen++;
   } while(len>0);

   buf[4-llen] = header;
   for (int i=0;i<llen;i++) {
      buf[5-llen+i] = lenBuf[i];
   }

   rc = _client->write(buf+(4-llen),length+1+llen);

   lastOutActivity = millis();
   return (rc == 1+llen+length);
}


bool MQTTClient::subscribe(char* topic) {
   if (connected()) {
      // Leave room in the buffer for header and variable length field
      uint16_t length = 7;
      nextMsgId++;
      if (nextMsgId == 0) {
         nextMsgId = 1;
      }
      buffer[0] = (nextMsgId >> 8);
      buffer[1] = (nextMsgId & 0xFF);
      length = writeString(topic, buffer,length);
      buffer[length++] = 0; // Only do QoS 0 subs
      return write(MQTTSUBSCRIBE|MQTTQOS0,buffer,length-5);
   }
   return false;
}

void MQTTClient::disconnect() {
   buffer[0] = MQTTDISCONNECT;
   buffer[1] = 0;
   _client->write((const uint8_t*)buffer,2);
   _client->stop();
   lastInActivity = lastOutActivity = millis();
}

uint16_t MQTTClient::writeString(char* src, uint8_t* dest, uint16_t pos) {

	unsigned int src_pos = pos+2;
	unsigned int src_len = strlen(src);

	unsigned int i = 0;
	for(i=0;i<src_len;i++)
		dest[src_pos++] = *(src+i);

	dest[src_pos-i-2] = (i>>8);
	dest[src_pos-i-1] = (i&0xff);

	return src_pos;


	/*char* idp = string;
	uint16_t i = 0;

	pos += 2;
	while (*idp) {
		buf[pos++] = *idp++;
		i++;
	}

	buf[pos-i-2] = (i >> 8);
	buf[pos-i-1] = (i & 0xFF);

	return pos;*/
}


bool MQTTClient::connected() {
   return _client->connected();
}
