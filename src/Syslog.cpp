
#include "Syslog.h"

// Public Methods //////////////////////////////////////////////////////////////
Syslog::Syslog(const char* deviceHostname , const char* appName, uint16_t priDefault) {
#ifdef _SYSLOG_USE_NETWORK
  this->_server = NULL;
  this->_port = 0;
  this->use_protocol=false;
#endif /* _SYSLOG_USE_NETWORK */
  this->_deviceHostname = (deviceHostname == NULL) ? SYSLOG_NILVALUE : deviceHostname;
  this->_appName = (appName == NULL) ? SYSLOG_NILVALUE : appName;
  this->_priDefault = priDefault;

#ifdef _SYSLOG_USE_FILE
#ifdef _SYSLOG_USE_SD_FAT
  this->use_file=-1;
#else /* _SYSLOG_USE_SD_FAT */
  this->use_file=false;
#endif /* _SYSLOG_USE_SD_FAT */
#endif /* _SYSLOG_USE_FILE */

#ifdef USE_SERIAL
  this->use_serial=0;
#endif /* USE_SERIAL */

#ifdef _SYSLOG_USE_RTC
  this->rtc=NULL;
#endif /* _SYSLOG_USE_RTC */

#ifdef _SYSLOG_USE_NTP
  this->ntpClient=NULL;
#endif /* _SYSLOG_USE_NTP */
}

void Syslog::SetLogInfo(const char* deviceHostname, const char* appName, uint16_t priDefault) {
  this->_deviceHostname = (deviceHostname == NULL) ? SYSLOG_NILVALUE : deviceHostname;
  this->_appName = (appName == NULL) ? SYSLOG_NILVALUE : appName;
  this->_priDefault = priDefault;
}

#ifdef _SYSLOG_USE_FILE
#ifdef _SYSLOG_USE_SD_FAT
bool Syslog::SetFile(File *file) {
  bool flag=file->seek(file->size());
  if (!flag) {return false;}
  this->fatFile=file;
  this->use_file=0;
  return true;
}

bool Syslog::SetFile(File32 *file) {
  bool flag=file->seek(file->size());
  if (!flag) {return false;}
  this->fat32File=file;
  this->use_file=1;
  return true;
}

bool Syslog::SetFile(ExFile *file) {
  bool flag=file->seek(file->size());
  if (!flag) {return false;}
  this->exFatFile=file;
  this->use_file=2;
  return true;
}

/*
bool Syslog::SetFile(FsFile *file) {
  bool flag=file->seek(file->size());
  if (!flag) {return false;}
  this->sdFsFile=file;
  this->use_file=0;
  return true;
}
*/

void Syslog::UnsetFile(void) {
  this->use_file=-1;
}
#else /* _SYSLOG_USE_SD_FAT */
bool Syslog::SetFile(File *file) {
  bool flag=file->seek(file->size());
  if (!flag) {return false;}
  this->logFile=file;
  this->use_file=true;
  return true;
}

void Syslog::UnsetFile(void) {
  this->use_file=false;
}
#endif /* _SYSLOG_USE_SD_FAT */
#endif /* _SYSLOG_USE_FILE */

#ifdef _SYSLOG_USE_HARDWARE_SERIAL

#if HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_MKR
void Syslog::SetSerial(HardwareSerial *serial) {
  this->channel.hSerial=serial;
  this->use_serial=TYPE_HARDWARE_SERIAL;
}
#endif /* HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_MKR */

#if HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_NORMAL && HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_USBCDC
void Syslog::SetSerial(Serial_ *serial) {
  this->channel._Serial=serial;
  this->use_serial=TYPE_USB_SERIAL;
}
#endif /* HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_NORMAL && HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_USBCDC */

#if HARDWARE_SERIAL_TYPE==SERIAL_TYPE_MKR
void Syslog::SetSerial(Uart *serial) {
  this->channel.uSerial=serial;
  this->use_serial=TYPE_UART_SERIAL;
}
#endif /* HARDWARE_SERIAL_TYPE==SERIAL_TYPE_MKR */

#if HARDWARE_SERIAL_TYPE==SERIAL_TYPE_USBCDC
void Syslog::SetSerial(USBCDC *serial) {
  this->channel.usbSerial=serial;
  this->use_serial=TYPE_UART_SERIAL;
}
#endif /* HARDWARE_SERIAL_TYPE==SERIAL_TYPE_USBCDC */

#endif /* _SYSLOG_USE_HARDWARE_SERIAL */

#ifdef _SYSLOG_USE_SOFTWARE_SERIAL
void Syslog::SetSerial(SoftwareSerial *serial) {
  this->channel.sSerial=serial;
  this->use_serial=TYPE_SOFTWARE_SERIAL;
}
#endif /* _SYSLOG_USE_SOFTWARE_SERIAL */

#ifdef USE_SERIAL
void Syslog::UnsetSerial(void) {
  this->use_serial=0;
}
#endif /* USE_SERIAL */

#ifdef _SYSLOG_USE_NETWORK
void Syslog::SetProtocol(UDP &client, uint8_t protocol) {
  this->_client = &client;
  this->_protocol = protocol;
  this->use_protocol=true;
}

void Syslog::SetProtocol(UDP &client, const char* server, uint16_t port, uint8_t protocol) {
  this->_client = &client;
  this->_protocol = protocol;
  this->_server = server;
  this->_port = port;
  this->use_protocol=true;
}

void Syslog::SetProtocol(UDP &client, IPAddress ip, uint16_t port, uint8_t protocol) {
  this->_client = &client;
  this->_protocol = protocol;
  this->_ip = ip;
  this->_server = NULL;
  this->_port = port;
  this->use_protocol=true;
}

void Syslog::UnsetProtocol(void) {
  this->use_protocol=false;
}

Syslog &Syslog::server(const char* server, uint16_t port) {
  this->_server = server;
  this->_port = port;
  return *this;
}

Syslog &Syslog::server(IPAddress ip, uint16_t port) {
  this->_ip = ip;
  this->_server = NULL;
  this->_port = port;
  return *this;
}
#endif /* _SYSLOG_USE_NETWORK */

Syslog &Syslog::deviceHostname(const char* deviceHostname) {
  this->_deviceHostname = (deviceHostname == NULL) ? SYSLOG_NILVALUE : deviceHostname;
  return *this;
}

Syslog &Syslog::appName(const char* appName) {
  this->_appName = (appName == NULL) ? SYSLOG_NILVALUE : appName;
  return *this;
}

Syslog &Syslog::defaultPriority(uint16_t pri) {
  this->_priDefault = pri;
  return *this;
}

Syslog &Syslog::logMask(uint8_t priMask) {
  this->_priMask = priMask;
  return *this;
}

bool Syslog::log(uint16_t pri, const __FlashStringHelper *message) {
  return this->_sendLog(pri, message);
}

bool Syslog::log(uint16_t pri, const String &message) {
  return this->_sendLog(pri, message.c_str());
}

bool Syslog::log(uint16_t pri, const char *message) {
  return this->_sendLog(pri, message);
}

bool Syslog::vlogf(uint16_t pri, const char *fmt, va_list args) {
  char *message;
  size_t initialLen;
  size_t len;
  bool result;

  initialLen = strlen(fmt);

  message = new char[initialLen + 1];

  len = vsnprintf(message, initialLen + 1, fmt, args);
  if (len > initialLen) {
    delete[] message;
    message = new char[len + 1];

    vsnprintf(message, len + 1, fmt, args);
  }

  result = this->_sendLog(pri, message);

  delete[] message;
  return result;
}

bool Syslog::vlogf_P(uint16_t pri, PGM_P fmt_P, va_list args) {
  char *message;
  size_t initialLen;
  size_t len;
  bool result;

  initialLen = strlen_P(fmt_P);

  message = new char[initialLen + 1];

  len = vsnprintf_P(message, initialLen + 1, fmt_P, args);
  if (len > initialLen) {
    delete[] message;
    message = new char[len + 1];

    vsnprintf(message, len + 1, fmt_P, args);
  }

  result = this->_sendLog(pri, message);

  delete[] message;
  return result;
}


bool Syslog::logf(uint16_t pri, const char *fmt, ...) {
  va_list args;
  bool result;

  va_start(args, fmt);
  result = this->vlogf(pri, fmt, args);
  va_end(args);
  return result;
}

bool Syslog::logf(const char *fmt, ...) {
  va_list args;
  bool result;

  va_start(args, fmt);
  result = this->vlogf(this->_priDefault, fmt, args);
  va_end(args);
  return result;
}

bool Syslog::logf_P(uint16_t pri, PGM_P fmt_P, ...) {
  va_list args;
  bool result;

  va_start(args, fmt_P);
  result = this->vlogf_P(pri, fmt_P, args);
  va_end(args);
  return result;
}

bool Syslog::logf_P(PGM_P fmt_P, ...) {
  va_list args;
  bool result;

  va_start(args, fmt_P);
  result = this->vlogf_P(this->_priDefault, fmt_P, args);
  va_end(args);
  return result;
}

bool Syslog::log(const __FlashStringHelper *message) {
  return this->_sendLog(this->_priDefault, message);
}

bool Syslog::log(const String &message) {
  return this->_sendLog(this->_priDefault, message.c_str());
}

bool Syslog::log(const char *message) {
  return this->_sendLog(this->_priDefault, message);
}

#ifdef _SYSLOG_USE_RTC
bool Syslog::SetRtc(RTC_Unified *rtc, uint8_t format) {
  if ((0!=format)&&(1!=format)) return false;
  this->rtc=rtc;
  this->time_format=format;
  return true;
}
#endif /* _SYSLOG_USE_RTC */

#ifdef _SYSLOG_USE_NTP
bool Syslog::SetNTP(NTPClient *client, uint8_t format) {
  if ((0!=format)&&(1!=format)) return false;
  this->ntpClient=client;
  this->time_format=format;
  return true;
}
#endif /* _SYSLOG_USE_NTP */

// Private Methods /////////////////////////////////////////////////////////////
#ifdef _SYSLOG_USE_RTC
String Syslog::dateString(void) {
  date_t date;
  this->rtc->getTime(&date);
  String result;
  if (this->time_format==DATE_TIME) {
    result=String(date.year)+"/"+String(date.month)+"/"+String(date.mday)+" ";
    switch(date.wday) {
      case SUN : result+="SUN";break;
      case MON : result+="MON";break;
      case TUE : result+="TUE";break;
      case WED : result+="WED";break;
      case THU : result+="THU";break;
      case FRI : result+="FRI";break;
      case SAT : result+="SAT";break;
    }
    result=result+" "+String(date.hour)+":"+String(date.minute)+":"+String(date.second);
  } else {
    result=String(this->rtc->convertDateToEpoch(date));
  }
  return result;
}
#endif /* _SYSLOG_USE_RTC */

#ifdef _SYSLOG_USE_NTP
String Syslog::dateNtpString(void) {
  this->ntpClient->update();
  String result;
  if (this->time_format==DATE_TIME) {
    result=dateString(this->ntpClient->getEpochTime());
  } else {
    result=String(this->ntpClient->getEpochTime());
  }
  return result;
}
#endif /* _SYSLOG_USE_NTP */

#if defined(_SYSLOG_USE_FILE) || defined(USE_SERIAL)
String Syslog::priorityString(uint16_t pri) {
  String result="";
  switch(LOG_PRIMASK&pri) {
    case LOG_EMERG  :result=result+"Emergency,";break;
    case LOG_ALERT  :result=result+"Alert,";break;
    case LOG_CRIT   :result=result+"Critical,";break;
    case LOG_ERR    :result=result+"Error,";break;
    case LOG_WARNING:result=result+"Warning,";break;
    case LOG_NOTICE :result=result+"Notice,";break;
    case LOG_INFO   :result=result+"Info,";break;
    case LOG_DEBUG  :result=result+"Debug,";break;
    default:result=result+"Unknown,";
  }
  switch(LOG_FACMASK&pri) {
    case LOG_KERN    :result=result+"Kernel";break;
    case LOG_USER    :result=result+"User";break;
    case LOG_MAIL    :result=result+"Mail";break;
    case LOG_DAEMON  :result=result+"Daemon";break;
    case LOG_AUTH    :result=result+"Auth";break;
    case LOG_SYSLOG  :result=result+"Syslog";break;
    case LOG_LPR     :result=result+"LPR";break;
    case LOG_NEWS    :result=result+"News";break;
    case LOG_UUCP    :result=result+"UUCP";break;
    case LOG_CRON    :result=result+"Cron";break;
    case LOG_AUTHPRIV:result=result+"AuthPriv";break;
    case LOG_FTP     :result=result+"FTP";break;
    case LOG_LOCAL0  :result=result+"Local0";break;
    case LOG_LOCAL1  :result=result+"Local1";break;
    case LOG_LOCAL2  :result=result+"Local2";break;
    case LOG_LOCAL3  :result=result+"Local3";break;
    case LOG_LOCAL4  :result=result+"Local4";break;
    case LOG_LOCAL5  :result=result+"Local5";break;
    case LOG_LOCAL6  :result=result+"Local6";break;
    case LOG_LOCAL7  :result=result+"Local7";break;
    default:result=result+"Unknown";
  }
  return result;
}
#endif /* _SYSLOG_USE_FILE || USE_SERIAL */

#ifdef _SYSLOG_USE_NETWORK
bool Syslog::_sendProtocol(uint16_t pri, const char *message) {
  int result;
  if (this->_server != NULL) {
    result = this->_client->beginPacket(this->_server, this->_port);
  } else {
    result = this->_client->beginPacket(this->_ip, this->_port);
  }

  if (result != 1)
    return false;

  // IETF Doc: https://tools.ietf.org/html/rfc5424
  // BSD Doc: https://tools.ietf.org/html/rfc3164
  this->_client->print('<');
  this->_client->print(pri);
  if (this->_protocol == SYSLOG_PROTO_IETF) {
    this->_client->print(F(">1 - "));
  } else {
    this->_client->print(F(">"));
  }
  this->_client->print(this->_deviceHostname);
  this->_client->print(' ');
  this->_client->print(this->_appName);
  if (this->_protocol == SYSLOG_PROTO_IETF) {
    this->_client->print(F(" - - - \xEF\xBB\xBF"));
  } else {
    this->_client->print(F("[0]: "));
  }
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->_client->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->_client->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->_client->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->_client->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->_client->print(message);
  this->_client->endPacket();

  return true;
}
#endif /* _SYSLOG_USE_NETWORK */

#ifdef _SYSLOG_USE_FILE
#ifdef _SYSLOG_USE_SD_FAT
void Syslog::_sendFile(uint16_t pri, const char *message) {
  if (this->use_file==0) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
    this->fatFile->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
    this->fatFile->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
    this->fatFile->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
    this->fatFile->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
    this->fatFile->print(priorityString(pri));
    this->fatFile->print(' ');
    this->fatFile->print(this->_deviceHostname);
    this->fatFile->print(' ');
    this->fatFile->print(this->_appName);
    this->fatFile->print(' ');
    this->fatFile->println(message);
    this->fatFile->flush();
  }
  if (this->use_file==1) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
    this->fat32File->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
    this->fat32File->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
    this->fat32File->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
    this->fat32File->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
    this->fat32File->print(priorityString(pri));
    this->fat32File->print(' ');
    this->fat32File->print(this->_deviceHostname);
    this->fat32File->print(' ');
    this->fat32File->print(this->_appName);
    this->fat32File->print(' ');
    this->fat32File->println(message);
    this->fat32File->flush();
  }
  if (this->use_file==2) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
    this->exFatFile->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
    this->exFatFile->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
    this->exFatFile->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
    this->exFatFile->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
    this->exFatFile->print(priorityString(pri));
    this->exFatFile->print(' ');
    this->exFatFile->print(this->_deviceHostname);
    this->exFatFile->print(' ');
    this->exFatFile->print(this->_appName);
    this->exFatFile->print(' ');
    this->exFatFile->println(message);
    this->exFatFile->flush();
  }
//  if (this->use_file==3) {
//#ifdef _SYSLOG_OUTPUT_TIME
//#ifdef _SYSLOG_USE_RTC
//    this->sdFsFile->print(this->dateString());
//#endif /* _SYSLOG_USE_RTC */
//#ifdef _SYSLOG_USE_NTP
//    this->sdFsFile->print(this->dateNtpString());
//#endif /* _SYSLOG_USE_NTP */
//#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
//    this->sdFsFile->print(millis());
//#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
//    this->sdFsFile->print(' ');
//#endif /* _SYSLOG_OUTPUT_TIME */
//    this->sdFsFile->print(priorityString(pri));
//    this->sdFsFile->print(' ');
//    this->sdFsFile->print(this->_deviceHostname);
//    this->sdFsFile->print(' ');
//    this->sdFsFile->print(this->_appName);
//    this->sdFsFile->print(' ');
//    this->sdFsFile->println(message);
//    this->sdFsFile->flush();
//  }
}
#else /* _SYSLOG_USE_SD_FAT */
void Syslog::_sendFile(uint16_t pri, const char *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->logFile->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->logFile->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->logFile->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->logFile->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->logFile->print(priorityString(pri));
  this->logFile->print(' ');
  this->logFile->print(this->_deviceHostname);
  this->logFile->print(' ');
  this->logFile->print(this->_appName);
  this->logFile->print(' ');
  this->logFile->println(message);
  this->logFile->flush();
}
#endif /* _SYSLOG_USE_SD_FAT */
#endif /* _SYSLOG_USE_FILE */

#ifdef _SYSLOG_USE_HARDWARE_SERIAL
#if HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_MKR
void Syslog::_sendHardSerial(uint16_t pri, const char *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->channel.hSerial->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->channel.hSerial->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->channel.hSerial->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->channel.hSerial->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->channel.hSerial->print(priorityString(pri));
  this->channel.hSerial->print(' ');
  this->channel.hSerial->print(this->_deviceHostname);
  this->channel.hSerial->print(' ');
  this->channel.hSerial->print(this->_appName);
  this->channel.hSerial->print(' ');
  this->channel.hSerial->println(message);
}
#endif /* HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_MKR */

#if HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_NORMAL && HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_USBCDC
void Syslog::_sendUsbSerial(uint16_t pri, const char *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->channel._Serial->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->channel._Serial->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->channel._Serial->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->channel._Serial->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->channel._Serial->print(priorityString(pri));
  this->channel._Serial->print(' ');
  this->channel._Serial->print(this->_deviceHostname);
  this->channel._Serial->print(' ');
  this->channel._Serial->print(this->_appName);
  this->channel._Serial->print(' ');
  this->channel._Serial->println(message);
}
#endif /* HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_NORMAL && HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_USBCDC */

#if HARDWARE_SERIAL_TYPE==SERIAL_TYPE_MKR
void Syslog::_sendUartSerial(uint16_t pri, const char *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->channel.uSerial->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->channel.uSerial->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->channel.uSerial->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->channel.uSerial->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->channel.uSerial->print(priorityString(pri));
  this->channel.uSerial->print(' ');
  this->channel.uSerial->print(this->_deviceHostname);
  this->channel.uSerial->print(' ');
  this->channel.uSerial->print(this->_appName);
  this->channel.uSerial->print(' ');
  this->channel.uSerial->println(message);
}
#endif /* HARDWARE_SERIAL_TYPE==SERIAL_TYPE_MKR */
#if HARDWARE_SERIAL_TYPE==SERIAL_TYPE_USBCDC
void Syslog::_sendUartSerial(uint16_t pri, const char *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->channel.usbSerial->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->channel.usbSerial->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->channel.usbSerial->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->channel.usbSerial->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->channel.usbSerial->print(priorityString(pri));
  this->channel.usbSerial->print(' ');
  this->channel.usbSerial->print(this->_deviceHostname);
  this->channel.usbSerial->print(' ');
  this->channel.usbSerial->print(this->_appName);
  this->channel.usbSerial->print(' ');
  this->channel.usbSerial->println(message);
}
#endif /* HARDWARE_SERIAL_TYPE==SERIAL_TYPE_USBCDC */
#endif /* _SYSLOG_USE_HARDWARE_SERIAL */

#ifdef _SYSLOG_USE_SOFTWARE_SERIAL
void Syslog::_sendSoftSerial(uint16_t pri, const char *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->channel.sSerial->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->channel.sSerial->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->channel.sSerial->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->channel.sSerial->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->channel.sSerial->print(priorityString(pri));
  this->channel.sSerial->print(' ');
  this->channel.sSerial->print(this->_deviceHostname);
  this->channel.sSerial->print(' ');
  this->channel.sSerial->print(this->_appName);
  this->channel.sSerial->print(' ');
  this->channel.sSerial->println(message);
}
#endif /* _SYSLOG_USE_SOFTWARE_SERIAL */

inline bool Syslog::_sendLog(uint16_t pri, const char *message) {
  bool result=true;

  // Check priority against priMask values.
  if ((LOG_MASK(LOG_PRI(pri)) & this->_priMask) == 0)
    return true;

  // Set default facility if none specified.
  if ((pri & LOG_FACMASK) == 0)
    pri = LOG_MAKEPRI(LOG_FAC(this->_priDefault), pri);

#ifdef _SYSLOG_USE_NETWORK
  if (this->use_protocol) {
    if ((this->_server == NULL && this->_ip == INADDR_NONE) || this->_port == 0) {
      result=false;
    } else {
      if (!(this->_sendProtocol(pri, message))) result=false;
    }
  }
#endif /* _SYSLOG_USE_NETWORK */

#ifdef _SYSLOG_USE_FILE
#ifdef _SYSLOG_USE_SD_FAT
  if (this->use_file!=-1) {
    this->_sendFile(pri, message);
  }
#else /* _SYSLOG_USE_SD_FAT */
  if (this->use_file) {
    this->_sendFile(pri, message);
  }
#endif /* _SYSLOG_USE_SD_FAT */
#endif /* _SYSLOG_USE_FILE */

#ifdef _SYSLOG_USE_HARDWARE_SERIAL

#if HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_MKR
  if (TYPE_HARDWARE_SERIAL==this->use_serial) {
    this->_sendHardSerial(pri, message);
  }
#endif /* HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_MKR */

#if HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_NORMAL  && HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_USBCDC
  if (TYPE_USB_SERIAL==this->use_serial) {
    this->_sendUsbSerial(pri, message);
  }
#endif /* HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_NORMAL   && HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_USBCDC */

#if HARDWARE_SERIAL_TYPE==SERIAL_TYPE_MKR
  if (TYPE_UART_SERIAL==this->use_serial) {
    this->_sendUartSerial(pri, message);
  }
#endif /* HARDWARE_SERIAL_TYPE==SERIAL_TYPE_MKR */

#if HARDWARE_SERIAL_TYPE==SERIAL_TYPE_USBCDC
  if (TYPE_UART_SERIAL==this->use_serial) {
    this->_sendUartSerial(pri, message);
  }
#endif /* HARDWARE_SERIAL_TYPE==SERIAL_TYPE_USBCDC */

#endif /* _SYSLOG_USE_HARDWARE_SERIAL */

#ifdef _SYSLOG_USE_SOFTWARE_SERIAL
  if (TYPE_SOFTWARE_SERIAL==this->use_serial) {
    this->_sendSoftSerial(pri, message);
  }
#endif /* _SYSLOG_USE_SOFTWARE_SERIAL */

  return result;
}

#ifdef _SYSLOG_USE_NETWORK
bool Syslog::_sendProtocol(uint16_t pri, const __FlashStringHelper *message) {
  int result;
  if (this->_server != NULL) {
    result = this->_client->beginPacket(this->_server, this->_port);
  } else {
    result = this->_client->beginPacket(this->_ip, this->_port);
  }

  if (result != 1)
    return false;

  // IETF Doc: https://tools.ietf.org/html/rfc5424
  // BSD Doc: https://tools.ietf.org/html/rfc3164
  this->_client->print('<');
  this->_client->print(pri);
  if (this->_protocol == SYSLOG_PROTO_IETF) {
    this->_client->print(F(">1 - "));
  } else {
    this->_client->print(F(">"));
  }
  this->_client->print(this->_deviceHostname);
  this->_client->print(' ');
  this->_client->print(this->_appName);
  if (this->_protocol == SYSLOG_PROTO_IETF) {
    this->_client->print(F(" - - - \xEF\xBB\xBF"));
  } else {
    this->_client->print(F("[0]: "));
  }
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->_client->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->_client->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->_client->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->_client->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */

  this->_client->print(message);
  this->_client->endPacket();

  return true;
}
#endif /* _SYSLOG_USE_NETWORK */

#ifdef _SYSLOG_USE_FILE
#ifdef _SYSLOG_USE_SD_FAT
void Syslog::_sendFile(uint16_t pri, const __FlashStringHelper *message) {
  if (this->use_file==0) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
    this->fatFile->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
    this->fatFile->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
    this->fatFile->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
    this->fatFile->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
    this->fatFile->print(priorityString(pri));
    this->fatFile->print(' ');
    this->fatFile->print(this->_deviceHostname);
    this->fatFile->print(' ');
    this->fatFile->print(this->_appName);
    this->fatFile->print(' ');
    this->fatFile->println(message);
    this->fatFile->flush();
  }
  if (this->use_file==1) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
    this->fat32File->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
    this->fat32File->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
    this->fat32File->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
    this->fat32File->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
    this->fat32File->print(priorityString(pri));
    this->fat32File->print(' ');
    this->fat32File->print(this->_deviceHostname);
    this->fat32File->print(' ');
    this->fat32File->print(this->_appName);
    this->fat32File->print(' ');
    this->fat32File->println(message);
    this->fat32File->flush();
  }
  if (this->use_file==2) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
    this->exFatFile->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
    this->exFatFile->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
    this->exFatFile->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
    this->exFatFile->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
    this->exFatFile->print(priorityString(pri));
    this->exFatFile->print(' ');
    this->exFatFile->print(this->_deviceHostname);
    this->exFatFile->print(' ');
    this->exFatFile->print(this->_appName);
    this->exFatFile->print(' ');
    this->exFatFile->println(message);
    this->exFatFile->flush();
  }
//  if (this->use_file==3) {
//#ifdef _SYSLOG_OUTPUT_TIME
//#ifdef _SYSLOG_USE_RTC
//    this->sdFsFile->print(this->dateString());
//#endif /* _SYSLOG_USE_RTC */
//#ifdef _SYSLOG_USE_NTP
//    this->sdFsFile->print(this->dateNtpString());
//#endif /* _SYSLOG_USE_NTP */
//#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
//    this->sdFsFile->print(millis());
//#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
//    this->sdFsFile->print(' ');
//#endif /* _SYSLOG_OUTPUT_TIME */
//    this->sdFsFile->print(priorityString(pri));
//    this->sdFsFile->print(' ');
//    this->sdFsFile->print(this->_deviceHostname);
//    this->sdFsFile->print(' ');
//    this->sdFsFile->print(this->_appName);
//    this->sdFsFile->print(' ');
//    this->sdFsFile->println(message);
//    this->sdFsFile->flush();
//  }
}
#else /* _SYSLOG_USE_SD_FAT */
void Syslog::_sendFile(uint16_t pri, const __FlashStringHelper *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->logFile->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->logFile->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->logFile->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->logFile->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->logFile->print(priorityString(pri));
  this->logFile->print(' ');
  this->logFile->print(this->_deviceHostname);
  this->logFile->print(' ');
  this->logFile->print(this->_appName);
  this->logFile->print(' ');
  this->logFile->println(message);
  this->logFile->flush();
}
#endif /* _SYSLOG_USE_SD_FAT */
#endif /* _SYSLOG_USE_FILE */

#ifdef _SYSLOG_USE_HARDWARE_SERIAL
#if HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_MKR
void Syslog::_sendHardSerial(uint16_t pri, const __FlashStringHelper *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->channel.hSerial->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->channel.hSerial->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->channel.hSerial->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->channel.hSerial->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->channel.hSerial->print(priorityString(pri));
  this->channel.hSerial->print(' ');
  this->channel.hSerial->print(this->_deviceHostname);
  this->channel.hSerial->print(' ');
  this->channel.hSerial->print(this->_appName);
  this->channel.hSerial->print(' ');
  this->channel.hSerial->println(message);
}
#endif /* HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_MKR */

#if HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_NORMAL && HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_USBCDC
void Syslog::_sendUsbSerial(uint16_t pri, const __FlashStringHelper *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->channel._Serial->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->channel._Serial->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->channel._Serial->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->channel._Serial->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->channel._Serial->print(priorityString(pri));
  this->channel._Serial->print(' ');
  this->channel._Serial->print(this->_deviceHostname);
  this->channel._Serial->print(' ');
  this->channel._Serial->print(this->_appName);
  this->channel._Serial->print(' ');
  this->channel._Serial->println(message);
}
#endif /* HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_NORMAL && HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_USBCDC */

#if HARDWARE_SERIAL_TYPE==SERIAL_TYPE_MKR
void Syslog::_sendUartSerial(uint16_t pri, const __FlashStringHelper *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->channel._Serial->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->channel.uSerial->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->channel.uSerial->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->channel._Serial->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->channel.uSerial->print(priorityString(pri));
  this->channel.uSerial->print(' ');
  this->channel.uSerial->print(this->_deviceHostname);
  this->channel.uSerial->print(' ');
  this->channel.uSerial->print(this->_appName);
  this->channel.uSerial->print(' ');
  this->channel.uSerial->println(message);
}
#endif /* HARDWARE_SERIAL_TYPE==SERIAL_TYPE_MKR */

#if HARDWARE_SERIAL_TYPE==SERIAL_TYPE_USBCDC
void Syslog::_sendUartSerial(uint16_t pri, const __FlashStringHelper *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->channel.usbSerial->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->channel.usbSerial->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->channel.usbSerial->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->channel.usbSerial->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->channel.usbSerial->print(priorityString(pri));
  this->channel.usbSerial->print(' ');
  this->channel.usbSerial->print(this->_deviceHostname);
  this->channel.usbSerial->print(' ');
  this->channel.usbSerial->print(this->_appName);
  this->channel.usbSerial->print(' ');
  this->channel.usbSerial->println(message);
}
#endif /* HARDWARE_SERIAL_TYPE==SERIAL_TYPE_USBCDC */

#endif /* _SYSLOG_USE_HARDWARE_SERIAL */

#ifdef _SYSLOG_USE_SOFTWARE_SERIAL
void Syslog::_sendSoftSerial(uint16_t pri, const __FlashStringHelper *message) {
#ifdef _SYSLOG_OUTPUT_TIME
#ifdef _SYSLOG_USE_RTC
  this->channel.sSerial->print(this->dateString());
#endif /* _SYSLOG_USE_RTC */
#ifdef _SYSLOG_USE_NTP
  this->channel.sSerial->print(this->dateNtpString());
#endif /* _SYSLOG_USE_NTP */
#if !defined(_SYSLOG_USE_RTC) && !defined(_SYSLOG_USE_NTP)
  this->channel.sSerial->print(millis());
#endif /* not _SYSLOG_USE_NTP && not _SYSLOG_USE_RTC */
  this->channel.sSerial->print(' ');
#endif /* _SYSLOG_OUTPUT_TIME */
  this->channel.sSerial->print(priorityString(pri));
  this->channel.sSerial->print(' ');
  this->channel.sSerial->print(this->_deviceHostname);
  this->channel.sSerial->print(' ');
  this->channel.sSerial->print(this->_appName);
  this->channel.sSerial->print(' ');
  this->channel.sSerial->println(message);
}
#endif /* _SYSLOG_USE_SOFTWARE_SERIAL */

inline bool Syslog::_sendLog(uint16_t pri, const __FlashStringHelper *message) {
  bool result=true;

  // Check priority against priMask values.
  if ((LOG_MASK(LOG_PRI(pri)) & this->_priMask) == 0)
    return true;

  // Set default facility if none specified.
  if ((pri & LOG_FACMASK) == 0)
    pri = LOG_MAKEPRI(LOG_FAC(this->_priDefault), pri);

#ifdef _SYSLOG_USE_NETWORK
  if (this->use_protocol) {
    if ((this->_server == NULL && this->_ip == INADDR_NONE) || this->_port == 0) {
      result=false;
    } else {
      if (!(this->_sendProtocol(pri, message))) result=false;
    }
  }
#endif /* _SYSLOG_USE_NETWORK */

#ifdef _SYSLOG_USE_FILE
#ifdef _SYSLOG_USE_SD_FAT
  if (this->use_file!=-1) {
    this->_sendFile(pri, message);
  }
#else /* _SYSLOG_USE_SD_FAT */
  if (this->use_file) {
    this->_sendFile(pri, message);
  }
#endif /* _SYSLOG_USE_SD_FAT */
#endif /* _SYSLOG_USE_FILE */

#ifdef _SYSLOG_USE_HARDWARE_SERIAL

#if HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_MKR
  if (TYPE_HARDWARE_SERIAL==this->use_serial) {
    this->_sendHardSerial(pri, message);
  }
#endif /* HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_MKR */

#if HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_NORMAL  && HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_USBCDC
  if (TYPE_USB_SERIAL==this->use_serial) {
    this->_sendUsbSerial(pri, message);
  }
#endif /* HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_NORMAL  && HARDWARE_SERIAL_TYPE!=SERIAL_TYPE_USBCDC */

#if HARDWARE_SERIAL_TYPE==SERIAL_TYPE_MKR
  if (TYPE_UART_SERIAL==this->use_serial) {
    this->_sendUartSerial(pri, message);
  }
#endif /* HARDWARE_SERIAL_TYPE==SERIAL_TYPE_MKR */

#if HARDWARE_SERIAL_TYPE==SERIAL_TYPE_USBCDC
  if (TYPE_UART_SERIAL==this->use_serial) {
    this->_sendUartSerial(pri, message);
  }
#endif /* HARDWARE_SERIAL_TYPE==SERIAL_TYPE_USBCDC */
#endif /* _SYSLOG_USE_HARDWARE_SERIAL */

#ifdef _SYSLOG_USE_SOFTWARE_SERIAL
  if (TYPE_SOFTWARE_SERIAL==this->use_serial) {
    this->_sendSoftSerial(pri, message);
  }
#endif /* _SYSLOG_USE_SOFTWARE_SERIAL */

  return result;
}

