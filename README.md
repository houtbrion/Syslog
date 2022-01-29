# Syslog


https://github.com/arcao/Syslog

#define USE_NETWORK
//#define USE_FILE
#define USE_HARDWARE_SERIAL
//#define USE_SOFTWARE_SERIAL

#define OUTPUT_TIME
//#define USE_RTC
#define USE_NTP


#include <NTPClient.h>
#include <dateUtils.h>
#include <RTC_U.h>
#include <SoftwareSerial.h>

    Syslog(const char* deviceHostname = SYSLOG_NILVALUE, const char* appName = SYSLOG_NILVALUE, uint16_t priDefault = LOG_KERN);
    void SetLogInfo(const char* deviceHostname = SYSLOG_NILVALUE, const char* appName = SYSLOG_NILVALUE, uint16_t priDefault = LOG_KERN);
#ifdef USE_NETWORK
    void SetProtocol(UDP &client, uint8_t protocol = SYSLOG_PROTO_IETF);
    void SetProtocol(UDP &client, const char* server, uint16_t port, uint8_t protocol = SYSLOG_PROTO_IETF);
    void SetProtocol(UDP &client, IPAddress ip, uint16_t port, uint8_t protocol = SYSLOG_PROTO_IETF);
    void UnsetProtocol(void);
#endif /* USE_NETWORK */
#ifdef USE_FILE
    bool SetFile(File *file);
    void UnsetFile(void);
#endif /* USE_FILE */
#ifdef USE_HARDWARE_SERIAL
    void SetSerial(HardwareSerial *serial);
#endif /* USE_HARDWARE_SERIAL */
#ifdef USE_SOFTWARE_SERIAL
    void SetSerial(SoftwareSerial *serial);
#endif /* USE_SOFTWARE_SERIAL */
#ifdef USE_SERIAL
    void UnsetSerial(void);
#endif /* USE_SERIAL */

#ifdef USE_NETWORK
    Syslog &server(const char* server, uint16_t port);
    Syslog &server(IPAddress ip, uint16_t port);
#endif /* USE_NETWORK */
    Syslog &deviceHostname(const char* deviceHostname);
    Syslog &appName(const char* appName);
    Syslog &defaultPriority(uint16_t pri = LOG_KERN);

    Syslog &logMask(uint8_t priMask);

    bool log(uint16_t pri, const __FlashStringHelper *message);
    bool log(uint16_t pri, const String &message);
    bool log(uint16_t pri, const char *message);

    bool vlogf(uint16_t pri, const char *fmt, va_list args) __attribute__((format(printf, 3, 0)));
    bool vlogf_P(uint16_t pri, PGM_P fmt_P, va_list args) __attribute__((format(printf, 3, 0)));
    
    bool logf(uint16_t pri, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
    bool logf(const char *fmt, ...) __attribute__((format(printf, 2, 3)));

    bool logf_P(uint16_t pri, PGM_P fmt_P, ...) __attribute__((format(printf, 3, 4)));
    bool logf_P(PGM_P fmt_P, ...) __attribute__((format(printf, 2, 3)));

    bool log(const __FlashStringHelper *message);
    bool log(const String &message);
    bool log(const char *message);
#ifdef USE_RTC
    bool SetRtc(RTC_Unified *rtc, uint8_t format);
#endif /* USE_RTC */
#ifdef USE_NTP
    bool SetNTP(NTPClient *client, uint8_t format);
    String dateNtpString(void);
#endif /* USE_NTP */







An Arduino library for logging to Syslog server via `UDP` protocol in 
[IETF (RFC 5424)] and [BSD (RFC 3164)] message format

[![Build Status](https://travis-ci.org/arcao/Syslog.svg?branch=master)](https://travis-ci.org/arcao/Syslog) [![Join the chat at https://gitter.im/arcao/Syslog](https://badges.gitter.im/arcao/Syslog.svg)](https://gitter.im/arcao/Syslog?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

How to use, see [examples].

## Features
 - Supports original Syslog severity level and facility constants
 - Supports both Syslog messge formats: [IETF (RFC 5424)] and [BSD (RFC 3164)]
 - Supports `printf`-like formatting via `logf` methods (use `vsnprintf` method
   inside)
 - Supports fluent interface, see [AdvancedLogging] example
 - Allows to ignore sending specified severity levels with `logMask` function, 
   see [AdvancedLogging] example
 - Independent on underlying network hardware. The network hardware library has
   to implement methods of `UDP` astract class only.

## Compatible Hardware
The library uses the Arduino UDP Network API (`UDP` class) for interacting with 
the underlying network hardware. This means it Just Works with a growing number
of boards and shields, including:

 - ESP8266 / ESP32
 - Arduino Ethernet
 - Arduino Ethernet Shield
 - Arduino YUN – use the included `BridgeUDP` in place of `EthernetUDP`, and
   be sure to call a `Bridge.begin()` first
 - Arduino WiFi Shield
 - Intel Galileo/Edison
 - Arduino/Genuino MKR1000
 - [Arduino module RTL00(RTL8710AF), F11AMIM13 (RTL8711AM)][RTLDUINO]
 - ... you tell me!

## Syslog message formats
This library supports both Syslog message formats [IETF (RFC 5424)] and 
[BSD (RFC 3164)]. The newer **IETF** format is used by default. If you want to use
older "obsolete" **BSD** format, just specify it with `SYSLOG_PROTO_BSD` constant 
in a last constructor parameter.

```c
Syslog syslog(udpClient, host, port, device_hostname, app_name, default_priority, SYSLOG_PROTO_BSD);
// or
Syslog syslog(udpClient, ip, port, device_hostname, app_name, default_priority, SYSLOG_PROTO_BSD);
// or
Syslog syslog(udpClient, SYSLOG_PROTO_BSD);
```

## Limitations
 - This library is sending empty timestamp in the syslog messages. For IETF 
   format it is `NILVALUE` (char `-`) in `TIMESTAMP` field, for BSD format the 
   `TIMESTAMP` field is completely ommited. Syslog server should use a time
   of receiving message in this case. It is OK in most cases. This issue will be
   fixed in some of the next releases.
   

[IETF (RFC 5424)]: https://tools.ietf.org/html/rfc5424
[BSD (RFC 3164)]: https://tools.ietf.org/html/rfc3164
[examples]: https://github.com/arcao/Syslog/tree/master/examples
[AdvancedLogging]: https://github.com/arcao/Syslog/blob/master/examples/AdvancedLogging/AdvancedLogging.ino
[RTLDUINO]: https://github.com/pvvx/RtlDuino
