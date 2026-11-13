#include <pgmspace.h>
 
#define SECRET
 
const char WIFI_SSID[] = "xxxx";               
const char WIFI_PASSWORD[] = "xxxxx";           
 
#define THINGNAME "notifyReachedOfficeDevicce"
 
int8_t TIME_ZONE = -5; //NYC(USA): -5 UTC
 
const char MQTT_HOST[] = "xxxx";
 
 
static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
xxxx
-----END CERTIFICATE-----
)EOF";
 
 
 
// Copy contents from XXXXXXXX-certificate.pem.crt here ▼
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
xxxx
-----END CERTIFICATE-----
)KEY";
 
 
// Copy contents from  XXXXXXXX-private.pem.key here ▼
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
xxxx
-----END RSA PRIVATE KEY-----
)KEY";