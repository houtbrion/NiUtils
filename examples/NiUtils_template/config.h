#define DEBUG
#define SERIAL_SPEED 9600
#define USE_UDP

/*
 * 以下のdefine文は本体にネットワーク回路が搭載されていないハードウェアを使う場合のみ有効にする
 */
/*
 * イーサネットシールドを使う場合
 */
//#define USE_ETHERNET
//#define USE_ETHERNET_W5XXX

/*
 * WiFiシールドを使う場合
 */
//#define USE_WIFI
//#define USE_WIFI_NORMAL

/*
 * WiFi NINAを使う場合
 */
//#define USE_WIFI
//#define USE_WIFI_NINA

/*
 * WiFi関係
 */
#define WIFI_SSID "foo"
#define WIFI_PASS "bar"
