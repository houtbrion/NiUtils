#ifndef _ETHERNET_UTILS_H_
#include <Arduino.h>
#include <detectArduinoHardware.h>
#include "config.h"
#include <NiUtils.h>

#ifdef ETHERNET_W5XXX
#include <Ethernet.h>
#endif /* ETHERNET_W5XXX */

#ifdef USE_UDP
#include <Udp.h>
#endif /* USE_UDP */

bool CheckNif(void);
uint8_t NifInitStatus(void);
void PrintNetworkStatus(void);
String NetworkStatus(void);

#endif /* _ETHERNET_UTILS_H_ */
