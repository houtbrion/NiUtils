#ifndef _WIFI_UTILS_H_
#define _WIFI_UTILS_H_
#include <Arduino.h>
#include <detectArduinoHardware.h>
#include "config.h"
#include <NiUtils.h>

#ifdef USE_WIFI_NORMAL
#include <WiFi.h>
#endif /* USE_WIFI_NORMAL */

#ifdef USE_WIFI_NINA
#include <WiFiNINA.h>
#endif /* USE_WIFI_NINA */

#ifdef USE_UDP
#include <Udp.h>
#endif /* USE_UDP */

#define WIFI_RETRY_MAX 30
#define WIFI_TRY_INTERVAL 1000

uint8_t NifInitStatus(void);
void PrintNetworkStatus(void);
bool CheckNif(void);
String NetworkStatus(void);

#endif /* _WIFI_UTILS_H_ */
