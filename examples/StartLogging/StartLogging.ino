
#include "config.h"
#include <Syslog.h>
#include "NiUtils.h"
#include "RTC_Utils.h"

#if defined(_SYSLOG_USE_SOFTWARE_SERIAL) || defined(_SYSLOG_USE_HARDWARE_SERIAL)
#define USE_SERIAL
#endif

#ifdef _SYSLOG_USE_NETWORK
#ifdef USE_ETHERNET
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xF0, 0x0D };
#endif /* USE_ETHERNET */

UDP_CLASS udpClient;
#endif /* _SYSLOG_USE_NETWORK */

#ifdef _SYSLOG_USE_NTP
UDP_CLASS ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, TZ_OFFSET, 60000);
#endif /* _SYSLOG_USE_NTP */

/*
 * Serial Mega
 * Serial1 MKR
 */
#ifdef _SYSLOG_USE_SOFTWARE_SERIAL
SoftwareSerial mySerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX); // RX, TX
#endif /* _SYSLOG_USE_SOFTWARE_SERIAL */

Syslog syslog(DEVICE_HOSTNAME, APP_NAME, LOG_KERN);

#ifdef _SYSLOG_USE_FILE
File logfile;
#endif /* _SYSLOG_USE_FILE */

#ifdef _SYSLOG_USE_RTC
#include <Wire.h>
RTC_CLASS rtc = RTC_CLASS(&Wire);
#endif /* _SYSLOG_USE_RTC */

int iteration = 1;

void setup() {
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {
    ;
  }
  Serial.println("");
  Serial.println("setup start.");
#ifdef USE_SERIAL
#ifdef _SYSLOG_USE_HARDWARE_SERIAL
#if HARD_SERIAL!=Serial
  HARD_SERIAL.begin(SERIAL_SPEED);
#endif
  if (HARD_SERIAL) {
    syslog.SetSerial(&HARD_SERIAL);
  } else {
    Serial.println("setup serial failure.");
  }
#endif /* _SYSLOG_USE_HARDWARE_SERIAL */
#ifdef _SYSLOG_USE_SOFTWARE_SERIAL
  mySerial.begin(SERIAL_SPEED);
  if (mySerial) {
    syslog.SetSerial(&mySerial);
  } else {
    Serial.println("setup serial failure.");
  }
#endif /* _SYSLOG_USE_SOFTWARE_SERIAL */
  syslog.log(LOG_INFO, "setup serial syslog channel...done.");
#endif /* USE_SERIAL */


#ifdef _SYSLOG_USE_FILE
  SD.begin(SD_CS_PIN);
  logfile=SD.open(LOGFILE_NAME,FILE_WRITE);
  if (logfile) {
    syslog.SetFile(&logfile);
  } else {
    syslog.log(LOG_INFO, "open logfile failure.");
  }
#endif /* _SYSLOG_USE_FILE */

#ifdef _SYSLOG_USE_NETWORK
  if (!CheckNif()) {
    syslog.log(LOG_INFO, "No network interface shield.");while(true) {};
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
    case 1: syslog.log(LOG_INFO, "Can not connect to WiFi network.");while(true) {};
    case 2: syslog.log(LOG_INFO, "Ethernet link is off.");while(true) {};
  }
  syslog.log(LOG_INFO, NetworkStatus());
  syslog.SetProtocol(udpClient, SYSLOG_SERVER, SYSLOG_PORT);
  syslog.log(LOG_INFO, "setup network syslog channel...done.");
#endif /* _SYSLOG_USE_NETWORK */

#ifdef _SYSLOG_USE_RTC
  if (rtc.begin()) {
    syslog.log(LOG_INFO, "Successful initialization of the RTC"); // 初期化成功
  } else {
    syslog.log(LOG_INFO, "Failed initialization of the RTC");  // 初期化失敗
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
    syslog.log(LOG_INFO, "set time to RTC fail.") ;     // 初期化失敗
  } else {
    syslog.log(LOG_INFO, "set time to RTC success."); // 初期化成功
  }
  syslog.SetRtc(&rtc,DATE_TIME);
#endif /* _SYSLOG_USE_RTC */

#ifdef _SYSLOG_USE_NTP
  timeClient.begin();
  syslog.SetNTP(&timeClient, DATE_TIME);
#endif /* _SYSLOG_USE_NTP */

  syslog.log(LOG_INFO, "all setup done.");
}

void loop() {

}
