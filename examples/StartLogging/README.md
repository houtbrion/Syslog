# simple logging

syslogプロトコルやファイルへのログ出力を行う場合，ボードのブート直後はネットワークやファイルが利用できないことから，それらの周辺デバイスの初期化に関するログはネットワークやファイルに出力することができません．

そのため，以下のような順番で周辺デバイスを有効にした上で，ログの出力先を順次追加していく必要があります．その実例のスケッチとなっています．


## 動作の概要
syslogライブラリ自体はログの出力先として，シリアル，ファイル，ネットワーク(syslogプロトコル)を選択できますが，全てを使うものとして説明します．
なお，このスケッチではハードウェアシリアルを緊急時のエラーメッセージの出力先として利用しています．

本スケッチは，``setup()``のみで動作しており，以下のような手順で実行されます．
- 最初にシリアルをセットアップして，成功した場合はsyslogライブラリの出力先にシリアルを登録
- SDのセットアップを試みる
    - 失敗したら，ログを出力(この時点ではシリアルのみ)
    - 成功した場合は，syslogライブラリの出力先にファイルを登録(この時点で，シリアルとファイルの両方)
- ネットワークのセットアップを試みる
    - 失敗したら，ログを出力(この時点で，シリアルとファイルの両方)
    - 成功した場合は，syslogライブラリの出力先にネットワーク(syslogプロトコルのサーバ)を登録(この時点で，3つの出力先が全て有効)
- セットアップが完了したログを出力(3つの出力先にログを出力)．

## インストール

当然ではありますが，syslogのライブラリが必要なので，それをインストールしてください．
- [syslog](https://github.com/houtbrion/Syslog)

本スケッチ用に以下のものもインストールしてください．
- [detectArduinoHardware](https://github.com/houtbrion/detectArduinoHardware)
- detectArduinoHardwareのextensionディレクトリに収納されている拡張機能ライブラリarduinoHardwareHelper
- [NiUtils](https://github.com/houtbrion/NiUtils)

RTCを利用する人は，以下のライブラリをインストールしてください．
- [RTC_U](https://github.com/houtbrion/RTC_U)
- [RTC_Uの拡張機能](https://github.com/houtbrion/RTC_U)内の各RTCチップ用のドライバと，extentionもインストールしてください．

もし，ESP8266でソフトウェアシリアルを利用する場合は，特別なライブラリが必要になるため，以下のライブラリをインストールしてください．
- [EspSoftwareSerial](https://www.arduino.cc/reference/en/libraries/espsoftwareserial/)


## カスタマイズ&ビルド


### syslogライブラリ自体のカスタマイズ
本ライブラリのトップディレクトリにあるREADME.mdを参照して利用する(可能性がある)出力先チャンネルを有効にするなどの作業を本スケッチのコンパイル前に行ってください．

本スケッチでは，シリアルとファイルとsyslogプロトコルでのサーバへの送信を行うため，3種類のチャンネル全てを有効にしてください．

### ログの設定
主に，syslogプロトコル用になりますが，ファイルやシリアルへの出力でも，ホスト名やアプリ名をログメッセージに入れるため，その定義をスケッチ同封のconfig.hの以下のところに設定してください．
```
/*
 * syslogフォーマット
 */
#define DEVICE_HOSTNAME "my-device" // ホスト名
#define APP_NAME "my-app"           // アプリ名
```

### 時刻情報
本サンプルスケッチは，ログに埋め込む時刻情報をNTPとRTCから選択することができます．RTCを利用する場合は，本スケッチ同封のconfig.hの末尾の以下の部分から利用するRTCを選んで有効化してください．
```
/*
 * RTC関係
 */
//#define USE_RTC8564NB
//#define USE_DS1307
//#define USE_DS3234
```

もし，RTC，NTP共に利用しない場合は，syslogライブラリのconfig.h(本サンプルスケッチのconfig.hではない)を以下のようにしておくと，ファイルやシリアルに出力される時刻情報はボードが起動してからのミリ秒単位の時間となります．RTCかNTPを使う場合は下2行のどちらかを有効にしてください．
```
#define OUTPUT_TIME
//#define USE_RTC
//#define USE_NTP
```

NTPを利用する場合は，本サンプルスケッチのconfig.hの以下の部分を必要に応じて変更してください．
```
/*
 * NTP関係
 */
#define NTP_SERVER "europe.pool.ntp.org" // NTPサーバ
#define TZ_OFFSET 32400                  // UTCとの差(秒)
```
``TZ_OFFSET``はUTCとの時差ですので，日本は9時間先行となります．

### シリアルの設定
ハードウェアシリアルとソフトシリアルのいずれを使うかは，syslogライブラリの設定で設定しているので，本スケッチのconfig.hでは実際に使うシリアルの名前や速度のパラメータを指定してください．
```
/*
 * シリアル関係
 */
// ソフトウェアシリアルピン番号
#define SOFT_SERIAL_RX 12
#define SOFT_SERIAL_TX 11

#define SERIAL_SPEED 9600

// ハードウェアシリアル設定(シリアルI/FとI/Fの種類)
// 通常のハードウェアシリアル
//#define HARD_SERIAL Serial
//#define SERIAL_TYPE TYPE_HARDWARE_SERIAL
// AVR系ボードの一部
#define HARD_SERIAL Serial
#define SERIAL_TYPE TYPE_USB_SERIAL
// AVR系ボードの大多数
//#define HARD_SERIAL Serial1
//#define SERIAL_TYPE TYPE_HARDWARE_SERIAL
// MKR系ボード用
//#define HARD_SERIAL Serial1
//#define SERIAL_TYPE TYPE_UART_SERIAL
```

### SD関係の設定
SDのチップセレクトピンの番号とlogを保存するファイル名を指定してください．ログファイル名はフルパスでないとESP32では動作しないため，その点はご注意ください．
```
/*
 * ファイル関係
 */
#define SD_CS_PIN 4                // SDのチップセレクトピン番号
#define LOGFILE_NAME "/syslog.txt"  // logファイル名
```

### ネットワーク関係の定義
ESP32やMKR WiFi1010，Leonardo Ethernetなどネットワークインターフェイスが内蔵の機種についてはネットワークインターフェイスの定義は不要ですが，WiFiやイーサネットのシールドを使う場合は，本スケッチ同封のconfig.hの以下の行のいずれかを有効にしてください．
```
//#define USE_ETHERNET_W5XXX
//#define USE_ETHERNET
//#define USE_WIFI_NINA
//#define USE_WIFI_NORMAL
//#define USE_WIFI
```
また，syslogサーバにデータを送るために，syslogサーバのホスト名やポート番号をconfig.hで設定します．
```
/*
 * syslogプロトコル関係
 */
#define SYSLOG_SERVER "syslog-server" // syslogサーバ名
#define SYSLOG_PORT 514               // ポート番号

```
WiFi利用の場合はconfig.hの以下の行を変更してください．
```
/*
 * WiFi関係
 */
#define WIFI_SSID "foo"
#define WIFI_PASS "bar"
```

## 動作確認済みハードウェア
<!---
|CPUアーキ|本体|ネットワークI/F|SD I/F|RTC|ソフトシリアル|
|---|---|---|---|---|---|
|AVR|[Arduino Mega 2560 R3](https://www.arduino.cc/en/Main/arduinoBoardMega2560)|[Ethernet Shield 2](https://store-usa.arduino.cc/products/arduino-ethernet-shield-2)|[Ethernet Shield 2](https://store-usa.arduino.cc/products/arduino-ethernet-shield-2)|[RTC-8564NB](https://akizukidenshi.com/catalog/g/gI-00233/)|[FT234XD](https://akizukidenshi.com/catalog/g/gK-14652/)|
|AVR|同上|同上|同上|[DS1307](https://akizukidenshi.com/catalog/g/gK-15488/)|－|
|SAMD|[Arduino MKR WiFi 1010](https://store-usa.arduino.cc/products/arduino-mkr-wifi-1010?selectedStore=us)|NINA-W102(内蔵)|[Adafruit MicroSD card breakout board+](https://www.adafruit.com/product/254)|[RTC-8564NB](https://akizukidenshi.com/catalog/g/gI-00233/)|－|
|ESP32|[ESPr Developer 32](https://www.switch-science.com/catalog/3210/)|ESP32内蔵WiFi|[Adafruit MicroSD card breakout board+](https://www.adafruit.com/product/254)|[RTC-8564NB](https://akizukidenshi.com/catalog/g/gI-00233/)|－|

## 配線
Fritzingで図面を起こしてありますが，使っているハードウェアモジュールのFrintzing用定義が回路図の作成に対応していないため，回路図はありません．

ピンの接続関係が不明な場合は同封したFritzingで図面ファイルを開いて確認してください．
### Arduino Mega
![Mega2560](./Mega_ブレッドボード.png)

### Arduino MKR WiFi 1010
![MKR_WiFi_1010](./MKR_WiFi_1010_ブレッドボード.png)

### ESP32開発用ボード
![esp32](./esp32_ブレッドボード.png)

-->


