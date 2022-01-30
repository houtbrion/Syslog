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
 * WiFi関係
 */
#define USE_WIFI  // WiFiを使う場合
#define WIFI_SSID "houtbrion"
#define WIFI_PASS "houtbrionhome"

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
