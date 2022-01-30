
#include "config.h"

#include <Syslog.h>

#ifdef USE_RTC
#include "RTC_8564NB_U.h"
#endif /* USE_RTC */

#if HARDWARE_TYPE==ARDUINO_UNO_WIFI_DEV_ED
#define USE_WIFI_NINA
#endif /* ARDUINO_UNO_WIFI_DEV_ED */
#if defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_MKRVIDOR4000)
#define USE_WIFI_NINA
#endif /* ARDUINO_SAMD_NANO_33_IOT || ARDUINO_SAMD_MKRWIFI1010 || ARDUINO_SAMD_MKRVIDOR4000 */

#ifdef USE_NETWORK
#ifdef USE_WIFI
#ifdef USE_WIFI_NINA
#include <WiFiNINA.h>
#else /* USE_WIFI_NINA */
#include <WiFi.h>
#endif /* USE_WIFI_NINA */
#include <WiFiUdp.h>
#else /* USE_WIFI */
#include <Ethernet.h>
#include <EthernetUdp.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xF0, 0x0D };
#endif /* USE_WIFI */
#endif /* USE_NETWORK */

#ifdef USE_NTP
#ifdef USE_WIFI
WiFiUDP ntpUDP;
#else /* USE_WIFI */
EthernetUDP ntpUDP;
#endif /* USE_WIFI */

NTPClient timeClient(ntpUDP, NTP_SERVER, TZ_OFFSET, 60000);
#endif /* USE_NTP */

/*
 * Serial Mega
 * Serial1 MKR
 */
#ifdef USE_SOFTWARE_SERIAL
SoftwareSerial mySerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX); // RX, TX
#endif /* USE_SOFTWARE_SERIAL */

#ifdef USE_NETWORK
IPAddress serverAddr(192, 168, 1, 2);
#endif /* USE_NETWORK */

#ifdef USE_NETWORK
#ifdef USE_WIFI
int status = WL_IDLE_STATUS;
#endif /* USE_WIFI */

// A UDP instance to let us send and receive packets over UDP
#ifdef USE_WIFI
WiFiUDP udpClient;
#else /* USE_WIFI */
EthernetUDP udpClient;
#endif /* USE_WIFI */
#endif /* USE_NETWORK */

// Create a new syslog instance with LOG_KERN facility
Syslog syslog(DEVICE_HOSTNAME, APP_NAME, LOG_KERN);

#ifdef USE_FILE
File logfile;
#endif /* USE_FILE */

#ifdef __RTC_EPSON_8564NB_U_H__
RTC_8564NB_U rtc = RTC_8564NB_U(&Wire);
#endif /* __RTC_EPSON_8564NB_U_H__ */

int iteration = 1;

void setup() {
#ifdef DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("setup start.");
#endif /* DEBUG */

#ifdef USE_NETWORK
#ifdef USE_WIFI
#if CPU_TYPE!=TYPE_ESP32
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }
#endif /* CPU != ESP32 */
  // attempt to connect to Wifi network:
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    delay(10000);
  }
#else /* USE_WIFI */
  Ethernet.begin(mac);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    while (true) {
      delay(1);
    }
  }
#endif /* USE_WIFI */
  printNetworkStatus();

  syslog.SetProtocol(udpClient, SYSLOG_SERVER, SYSLOG_PORT);
#endif /* USE_NETWORK */

#ifdef USE_RTC
  if (rtc.begin()) {
    Serial.println("Successful initialization of the RTC"); // 初期化成功
  } else {
    Serial.print("Failed initialization of the RTC");  // 初期化失敗
  }

  /* 時刻データをRTCに登録するための変数定義 */
  date_t date;
  date.year=2022;
  date.month=1;
  date.mday=23;
  date.wday=SUN;
  date.hour=21;
  date.minute=42;
  date.second=0;
  date.millisecond=0;

  /* RTCに現在時刻を設定 */
  if (!rtc.setTime(&date)) {
    Serial.print("set time to RTC fail.") ;     // 初期化失敗
  } else {
    Serial.println("set time to RTC success."); // 初期化成功
  }
  syslog.SetRtc(&rtc,DATE_TIME);
#endif /* USE_RTC */

#ifdef USE_NTP
  timeClient.begin();
  syslog.SetNTP(&timeClient, DATE_TIME);
#endif /* USE_NTP */

#ifdef USE_FILE
  SD.begin(SD_CS_PIN);
  logfile=SD.open(LOGFILE_NAME,FILE_WRITE);
  if (logfile) {
    syslog.SetFile(&logfile);
  } else {
    Serial.println("open logfile failure.");
  }
#endif /* USE_FILE */

#ifdef USE_SERIAL
#ifdef USE_HARDWARE_SERIAL
#if HARD_SERIAL!=Serial
  HARD_SERIAL.begin(SERIAL_SPEED);
#endif
  if (HARD_SERIAL) {
    syslog.SetSerial(&HARD_SERIAL);
  } else {
    Serial.println("setup serial failure.");
  }
#endif /* USE_HARDWARE_SERIAL */
#ifdef USE_SOFTWARE_SERIAL
  mySerial.begin(SERIAL_SPEED);
  if (mySerial) {
    syslog.SetSerial(&mySerial);
  } else {
    Serial.println("setup serial failure.");
  }
#endif /* USE_SOFTWARE_SERIAL */
#endif /* USE_SERIAL */

#ifdef DEBUG
  Serial.println("setup done.");
#endif /* DEBUG */
}

void loop() {
  // Severity levels can be found in Syslog.h. They are same like in Linux 
  // syslog.
  syslog.log(LOG_INFO, "Begin loop");

  // Log message can be formated like with printf function.
  syslog.logf(LOG_ERR,  "This is error message no. %d", iteration);
  syslog.logf(LOG_INFO, "This is info message no. %d", iteration);

  // You can force set facility in pri parameter for this log message. More 
  // facilities in syslog.h or in Linux syslog documentation.
  syslog.logf(LOG_DAEMON | LOG_INFO, "This is daemon info message no. %d", 
    iteration);

  // F() macro is supported too
  syslog.log(LOG_INFO, F("End loop"));
  iteration++;
  
  // wait ten seconds before sending log message again
  delay(10000);
}

#ifdef USE_NETWORK
void printNetworkStatus() {
#ifdef USE_WIFI
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
#else /* USE_WIFI */
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
#endif /* USE_WIFI */
}
#endif /* USE_NETWORK */
