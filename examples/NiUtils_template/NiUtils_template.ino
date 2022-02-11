
#include "config.h"
#include "NiUtils.h"

#ifdef USE_ETHERNET
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xF0, 0x0D };
#endif /* USE_ETHERNET */

#define PORT_NUMBER 9999

#ifdef USE_UDP
UDP_CLASS udpClient;
#endif /* USE_UDP */
SERVER_CLASS server(PORT_NUMBER);
CLIENT_CLASS client;

void setup() {
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {
    ;
  }
  Serial.println("");
  Serial.println("setup start.");

  if (!CheckNif()) {
    Serial.println("No network interface shield.");
    while(true) {};
  }
#ifdef USE_WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASS);
#endif /* USE_WIFI */
#ifdef USE_ETHERNET
  Ethernet.begin(mac);
#endif /* USE_ETHERNET */
  uint8_t retVal=NifInitStatus();
  switch(retVal) {
    case 0: break;
    case 1: Serial.println("Can not connect to WiFi network.");while(true) {};
    case 2: Serial.println("Ethernet link is off.");while(true) {};
  }
  Serial.println(NetworkStatus());
  Serial.println("all setup done.");

}

void loop() {

}
