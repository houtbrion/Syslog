#define DEBUG

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

/*
 * ネットワーク関係の定義
 */

//#define USE_ETHERNET_W5XXX
//#define USE_ETHERNET
//#define USE_WIFI_NINA
//#define USE_WIFI_NORMAL
//#define USE_WIFI
/*
 * WiFi関係
 */
#define WIFI_SSID "foo"
#define WIFI_PASS "bar"

/*
 * syslogフォーマット
 */
#define DEVICE_HOSTNAME "my-device" // ホスト名
#define APP_NAME "my-app"           // アプリ名

/*
 * syslogプロトコル関係
 */
#define SYSLOG_SERVER "syslog-server" // syslogサーバ名
#define SYSLOG_PORT 514               // ポート番号

/*
 * ファイル関係
 */
#define SD_CS_PIN 4                // SDのチップセレクトピン番号
#define LOGFILE_NAME "/syslog.txt"  // logファイル名

/*
 * NTP関係
 */
#define NTP_SERVER "europe.pool.ntp.org" // NTPサーバ
#define TZ_OFFSET 32400                  // UTCとの差(秒)

/*
 * RTC関係
 */


////#define USE_RTC_4543
////#define USE_DS3234
//#define USE_RTC8564NB
//#define USE_DS1307
//#define USE_DS3231
//#define USE_PCF8523
//#define USE_RV8803
#define USE_RX8025
//#define USE_RX8900
