
#include "config.h"
#include <Syslog.h>
#include "NiUtils.h"
#include "RTC_Utils.h"

#ifdef USE_NETWORK
#ifdef USE_ETHERNET
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xF0, 0x0D };
#endif /* USE_ETHERNET */

UDP_CLASS udpClient;
#endif /* USE_NETWORK */

#ifdef USE_NTP
UDP_CLASS ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, TZ_OFFSET, 60000);
#endif /* USE_NTP */

/*
 * Serial Mega
 * Serial1 MKR
 */
#ifdef USE_SOFTWARE_SERIAL
SoftwareSerial mySerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX); // RX, TX
#endif /* USE_SOFTWARE_SERIAL */

Syslog syslog(DEVICE_HOSTNAME, APP_NAME, LOG_KERN);

#ifdef USE_FILE
File logfile;
#endif /* USE_FILE */

#ifdef USE_RTC
RTC_CLASS rtc = RTC_CLASS(&Wire);
#endif /* USE_RTC */

int iteration = 1;

void setup() {
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {
    ;
  }
  Serial.println("setup start.");

#ifdef USE_NETWORK
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

  Serial.println("setup done.");
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
