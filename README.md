# NiUtils

Arduinoのプログラミングにおいて，1つのスケッチを多数の機種で対応可能にする場合，機種やネットワークコントローラチップの違いで#ifdefの山が築かれてしまい，たいへん読みにくいプログラムができあがる．

ネットワーク関係に関して，スケッチの#ifdefを削減するためのライブラリ(ヘッダファイル)です．
## インストール
本ディレクトリの他，「``extentions``」ディレストリ内の以下のライブラリをArduinoのライブラリ用フォルダにコピーしてください．
- EthernetUtils
- WiFiUtils

本ライブラリの内部で利用している以下のライブラリもインストールしてください．
- [detectArduinoHardware](https://github.com/houtbrion/detectArduinoHardware)

## 制限事項
ネットワークコントローラがオンボードの機種や標準のネットワークシールドを用いる場合は対応していますが，自分で作成した回路でネットワークコントローラチップの種類やチップの接続が標準のものと異なる場合(SPIの端子の使い方が違うなど)には対応していません．

## 本ライブラリの使い方
### 準備
スケッチと同じディレクトリにconfig.hを作成し，以下の内容をコピーしておき，必要な項目を有効化する．ただし，Arduino MKR WiFi 1010やArduino Leonardo Ethernet等，ボードにネットワーク・インターフェースを搭載している機種については，UDPの項目以外は自動で定義される．
```
//#define USE_EHTERNET       // イーサネットを使う(使うイーサネットコントローラチップの種類に関係ない項目)
//#define USE_WIFI           // WiFiを使う(WiFiのチップの種類に関係ない項目)
//#define ETHERNET_W5XXX     // Ethernetシールド等で使われているイーサネットチップを使う場合
//#define USE_WIFI_NORMAL    // WiFiシールドやESP32等で有効なWiFi用ライブラリ(WiFi.h)を使う場合
//#define USE_WIFI_NINA      // MKR系ボードやUno WiFiで利用するライブラリ(WiFiNINA.h)を使う場合
//#define USE_UDP            // スケッチでUDPを使う場合(EthernetUDP.hやWiFiUdp.h)
```
スケッチに以下の順番でconfig.hと本ライブラリをロードするように定義を追加する．
```
#include "config.h"
#include <NiUtils.h>
```

### スケッチ内での使い方

通常UDPを用いるスケッチでは，イーサネットかWiFiかで以下のような定義が必要になる．
```
#ifdef USE_ETHERNET
EthernetUDP udp // udp用オブジェクトの定義
#endif
#ifdef USE_WIFI
WiFiUDP udp // udp用オブジェクトの定義
#endif
```
本ライブラリ(実態はヘッダファイルのみ)を使うと，上の定義が下の1行にまとまる．
```
UDP_CLASS udp
```
本ヘッダファイル内部では，下のように定義がされているため，「UDP」以外にも「TCPのサーバ」と「TCPのクライアント」も#ifdefをなくすことができる．
```
#ifdef USE_ETHERNET
#define UDP_CLASS    EthernetUDP
#define SERVER_CLASS EthernetServer
#define CLIENT_CLASS EthernetClient
#include <EthernetUtils.h>
#endif /* USE_ETHERNET */

#ifdef USE_WIFI
#define UDP_CLASS    WiFiUDP
#define SERVER_CLASS WiFiServer
#define CLIENT_CLASS WiFiClient
#include <WiFiUtils.h>
#endif /* USE_WIFI */
```

そのため，最終的に#ifdefが残るのは，下のようなチップの初期化の部分だけになる．下の例はDHCPを利用する場合．
```
#ifdef USE_WIFI
    WiFi.begin(WIFI_SSID, WIFI_PASS);
#endif /* USE_WIFI */
#ifdef USE_ETHERNET
    Ethernet.begin(mac);
#endif /* USE_ETHERNET */
```

本ライブラリをスケッチ内でincludeすると，イーサネット専用ライブラリもしくはWiFi専用ライブラリが自動でロードされるため，そちらも使ってスケッチから#ifdefを削減することができる．こちらのAPIについては，下で説明します．

# APIマニュアル

## ``bool CheckNif(void)``
この関数はネットワークインターフェイスを搭載しているボードやEthernetシールドを用いている場合に，そもそもネットワークコントローラチップが存在しているか否かを判定するための関数です．

この関数は，規定のピン番号のところにコントローラチップのセレクト端子が接続されていれば``true``を返し，そうでなければ``false``を返します．現状は，規定以外のピン番号につなぐ回路には対応していません．

## ``uint8_t NifInitStatus(void)``
ネットワークへの接続ができているか否かを判定するための関数で，WiFiの場合は``begin()``を実行し，WiFiネットワークに接続できているか否か，イーサネットの場合は，``begin()``を実行してリンクがupになっているか否かを判定するものです．

成功の場合は0が返され，失敗の場合は0以上(WiFiの場合は1, イーサネットの場合は2)が返されます．

## ``void PrintNetworkStatus(void)``
IPアドレス等の情報をシリアルへの出力する関数です．

## ``String NetworkStatus(void)``
Ethernetの場合はIPアドレスのみ，WiFiの場合はIPアドレスに加えて，SSID，WiFi信号のRSSIを1行の文字列(String型)にまとめて出力する関数でログ出力に利用するためのもの．

## 使い方の実例
DHCPを用いる場合は，WiFiとイーサネット両方に対応するコードが以下のように短くまとめることができます．
```
  if (!CheckNif()) {
    Serial.println("No network interface shield.");while(true) {};
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
  PrintNetworkStatus();
```

上の例では，ネットワークコントローラを初期化(``begin()``関数)する部分以外は#ifdefなしで済ませることができています．


<!--- コメント

## 動作検証

|CPU| 機種 |ベンダ| 結果 | 備考 |
| :--- | :--- | :--- | :---: | :--- |
|AVR| [Uno R3][Uno]  |[Arduino][Arduino]|  ○    |      |
|       | [Mega2560 R3][Mega] |[Arduino][Arduino] |  ○    |      |
|       | [Leonardo Ethernet][LeonardoEth] |[Arduino][Arduino] | ○     |      |
|       | [Uno WiFi][UnoWiFi] |[Arduino][Arduino] | ○     | |
|       | [Pro mini 3.3V][ProMini] | [Sparkfun][Sparkfun] |   ×   |      |
| ARM/M0+ | [M0 Pro][M0Pro] |[Arduino][Arduino] |○||
|ESP8266|[ESPr developer][ESPrDev]| [スイッチサイエンス][SwitchScience] |||
|ESP32 | [ESPr one 32][ESPrOne32] | [スイッチサイエンス][SwitchScience] |×|　|




[Adafruit Unified Sensor Driver][AdafruitUSD]
[Groveシールド][shield]
[Arduino M0 Pro][M0Pro]
[Arduino Due][Due]
[Arduino Uno R3][Uno]
[Arduino Mega2560 R3][Mega]
[Arduino Leonardo Ethernet][LeonardoEth]
[Arduino Pro mini 328 - 3.3V/8MHz][ProMini]
[ESpr one][ESPrOne]
[ESPr one 32][ESPrOne32]
[Grove][Grove]
[Seed Studio][SeedStudio]
[Arduino][Arduino]
[Sparkfun][Sparkfun]
[スイッチサイエンス][SwitchScience]
--->
