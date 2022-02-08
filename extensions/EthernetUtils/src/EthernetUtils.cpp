#include <EthernetUtils.h>

/*
 * bool CheckNif(void)
 * 
 */
bool CheckNif(void) {
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    return false;
  }
  return true;
}

/*
 * uint8_t NifInitStatus(void)
 * 返り値
 * 0 : 成功
 * 2 : LINK OFF
 */
uint8_t NifInitStatus(void) {
  if (Ethernet.linkStatus() == LinkOFF) {
    return 2;
  }
  return 0;
}

void PrintNetworkStatus(void) {
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

String NetworkStatus(void) {
  IPAddress address = Ethernet.localIP();
  String result="IPaddress: "+ String(address[0]) + "." + 
        String(address[1]) + "." + 
        String(address[2]) + "." + 
        String(address[3]);
  return result;
}