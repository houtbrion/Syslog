# Syslog

Arduino用のsyslogプロトコルライブラリ[https://github.com/arcao/Syslog](https://github.com/arcao/Syslog)を拡張し，シリアル出力やファイルへの保存に対応したライブラリです．
また，[オリジナルのライブラリ](https://github.com/arcao/Syslog)は時刻情報に対応していないため，RTCやNTPと連携する機能も追加しています．

syslogプロトコルに関する詳細は[オリジナルのライブラリ](https://github.com/arcao/Syslog)を参照してください．
## インストール

### 依存するライブラリのインストール
以下のライブラリをインストールしてください．
- [NTPClient.h](https://github.com/arduino-libraries/NTPClient)
- [dateUtils.h](https://github.com/houtbrion/dateUtils)
- [RTC_U.h](https://github.com/houtbrion/RTC_U)
- [SoftwareSerial.h](https://www.arduino.cc/en/Reference/softwareSerial)

### 設定
アプリをコンパイルする際に，このライブラリの「config.h」を編集して，必要な機能だけを生かしてください．
```
#define USE_NETWORK           // syslogプロトコルでサーバに送る機能
#define USE_FILE              // ファイルにログを保存する機能
#define USE_HARDWARE_SERIAL   // ログをハードウェアシリアルに出力する機能
#define USE_SOFTWARE_SERIAL   // ログをソフトシリアルに出力する機能
#define OUTPUT_TIME           // 時刻情報を出力する機能(以下の両方をコメントアウトした場合はmillis()の出力を利用)
#define USE_RTC               // 時刻情報をRTCから取得
#define USE_NTP               // 時刻情報をNTPで取得
```

使わない機能の定義をコメントアウトすることで，本ライブラリの不要な機能を削るだけでなく，上記のRTCやNTPなどのライブラリもリンクされないので，メモリやバイナリサイズの圧縮に役立ちます．

# APIマニュアル

## Syslogオブジェクトの生成
syslogオブジェクトの生成は，引数としてデバイス名，アプリ名，syslogのプライオリティ(種別情報とfacilityの論理OR))を用いて行います．
```
Syslog(const char* deviceHostname = SYSLOG_NILVALUE, const char* appName = SYSLOG_NILVALUE, uint16_t priDefault = LOG_KERN)
```
ログの種別情報は以下の8個．
```
#define LOG_EMERG   0 /* system is unusable               */
#define LOG_ALERT   1 /* action must be taken immediately */
#define LOG_CRIT    2 /* critical conditions              */
#define LOG_ERR     3 /* error conditions                 */
#define LOG_WARNING 4 /* warning conditions               */
#define LOG_NOTICE  5 /* normal but significant condition */
#define LOG_INFO    6 /* informational                    */
#define LOG_DEBUG   7 /* debug-level messages             */
```

ファシリティは以下の通り．
```
#define LOG_KERN     (0<<3)  /* kernel messages                           */
#define LOG_USER     (1<<3)  /* random user-level messages                */
#define LOG_MAIL     (2<<3)  /* mail system                               */
#define LOG_DAEMON   (3<<3)  /* system daemons                            */
#define LOG_AUTH     (4<<3)  /* security/authorization messages           */
#define LOG_SYSLOG   (5<<3)  /* messages generated internally by syslogd  */
#define LOG_LPR      (6<<3)  /* line printer subsystem                    */
#define LOG_NEWS     (7<<3)  /* network news subsystem                    */
#define LOG_UUCP     (8<<3)  /* UUCP subsystem                            */
#define LOG_CRON     (9<<3)  /* clock daemon                              */
#define LOG_AUTHPRIV (10<<3) /* security/authorization messages (private) */
#define LOG_FTP      (11<<3) /* ftp daemon                                */
#define LOG_LOCAL0   (16<<3) /* reserved for local use                    */
#define LOG_LOCAL1   (17<<3) /* reserved for local use                    */
#define LOG_LOCAL2   (18<<3) /* reserved for local use                    */
#define LOG_LOCAL3   (19<<3) /* reserved for local use                    */
#define LOG_LOCAL4   (20<<3) /* reserved for local use                    */
#define LOG_LOCAL5   (21<<3) /* reserved for local use                    */
#define LOG_LOCAL6   (22<<3) /* reserved for local use                    */
#define LOG_LOCAL7   (23<<3) /* reserved for local use                    */
```

## syslogプロトコルを使うためのAPI関数
デバイス名，アプリ名，デフォルトのプライオリティを再設定するためのAPI．
```
void SetLogInfo(const char* deviceHostname = SYSLOG_NILVALUE, const char* appName = SYSLOG_NILVALUE, uint16_t priDefault = LOG_KERN)
```

個別の設定のみを書き換えるAPIは以下の3種類．
```
Syslog &deviceHostname(const char* deviceHostname)
Syslog &appName(const char* appName)
Syslog &defaultPriority(uint16_t pri = LOG_KERN)
```

プライオリティが低いログを無視する(サーバへ送信したりファイルに保存しない)ためのマスク値を設定するためのAPI．
```
Syslog &logMask(uint8_t priMask)
```
## syslogプロトコルを使うためのAPI関数
syslogプロトコルでの送信を行うための設定を行うAPI．第3引数はプロトコルの種別を「SYSLOG_PROTO_IETF(RFC 5424)」と「SYSLOG_PROTO_BSD(RFC 3164)」から選択可能ですが，この引数を省略した場合は「SYSLOG_PROTO_IETF(RFC 5424)」になります．またサーバは文字列もしくはIPアドレスで指定できます．
```
void SetProtocol(UDP &client, uint8_t protocol = SYSLOG_PROTO_IETF);
void SetProtocol(UDP &client, const char* server_name, uint16_t port, uint8_t protocol = SYSLOG_PROTO_IETF);
void SetProtocol(UDP &client, IPAddress ip, uint16_t port, uint8_t protocol = SYSLOG_PROTO_IETF);
```
サーバの情報だけを設定するAPI．
```
Syslog &server(const char* server, uint16_t port);
Syslog &server(IPAddress ip, uint16_t port);
```

syslogプロトコルでの送信を止めるAPI．
```
void UnsetProtocol(void);
```

## ファイルへの保存機能を使うためのAPI
ログをファイルに保存する機能を有効化するためのAPIで，アプリケーションでログファイルをオープンし，引数として与えてください．なお，このAPIを呼び出すと，引数で指定したファイルの最後までファイルポインタを進めます．そのため，既存のファイルを引数として与えると追記する動作となります．
```
bool SetFile(File *file);
```
下のAPI関数はログをファイルに書き込む機能を停止するAPIです．
```
void UnsetFile(void);
```

## シリアルにログを出力する機能を利用するためのAPI
ハードウェアシリアルにログを出力するための機能を有効にするAPIです．第1引数にログを出力するハードウェアシリアルへのポインタを用いてください．
```
void SetSerial(HardwareSerial *serial);
```

LeonardoやMKR系ボードでは，``Serial``や``Serial1``が``HardwareSerial``ではなく，``Serial_``や``Uart``型の場合があるため，Leonardoなどで``Serial``にログを出力場合に利用してください．なお，どの機種が対応するかは，この[資料](https://www.arduino.cc/reference/en/language/functions/communication/serial/)を参照してください．

```
void SetSerial(Serial_ *serial);
void SetSerial(Uart *serial);
```

ソフトシリアルにログを出力するための設定を行うAPIは以下の関数となります．
```
void SetSerial(SoftwareSerial *serial);
```

シリアルにログを出力する機能を無効化するためのAPIになります．
```
void UnsetSerial(void);
```

シリアルにログを出力する場合，利用できるシリアルはハードウェアシリアル，ソフトシリアルのうち1つに限られますので，ご注意ください．

## ログに時刻情報を設定するためのAPI

ログに埋める時刻情報をRTCから取得するための設定用API．

第2引数は以下の値のどちらかを与えます．
```
#define EPOCH_TIME 0  /* unix time */
#define DATE_TIME  1  /* date format */
```
```
bool SetRtc(RTC_Unified *rtc, uint8_t format)
```

時刻情報をNTPで取得する場合の設定API．NTPクライアントの設定はアプリケーション側で行い，クライアントオブジェクトへのポインタを第1引数として与えます．
```
bool SetNTP(NTPClient *client, uint8_t format)
```

## ログを出力するAPI
本ライブラリでは，以下のAPIを用いてログメッセージを出力する場合，上記の設定APIで選択したすべての出力先にログメッセージが出力されます．
なお，各APIについての引数の意味などの詳細は，[オリジナルのライブラリ](https://github.com/arcao/Syslog)を参照してください．
```
bool log(const __FlashStringHelper *message)
bool log(const String &message)
bool log(const char *message)
```

```
bool log(uint16_t pri, const __FlashStringHelper *message)
bool log(uint16_t pri, const String &message)
bool log(uint16_t pri, const char *message)
```

```
bool vlogf(uint16_t pri, const char *fmt, va_list args)
bool vlogf_P(uint16_t pri, PGM_P fmt_P, va_list args)
```

``` 
bool logf(uint16_t pri, const char *fmt, ...)
bool logf(const char *fmt, ...)
```

```
bool logf_P(uint16_t pri, PGM_P fmt_P, ...)
bool logf_P(PGM_P fmt_P, ...)
```

[IETF (RFC 5424)]: https://tools.ietf.org/html/rfc5424
[BSD (RFC 3164)]: https://tools.ietf.org/html/rfc3164
[examples]: https://github.com/arcao/Syslog/tree/master/examples
[AdvancedLogging]: https://github.com/arcao/Syslog/blob/master/examples/AdvancedLogging/AdvancedLogging.ino
[RTLDUINO]: https://github.com/pvvx/RtlDuino
