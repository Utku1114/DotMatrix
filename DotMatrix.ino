#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <MD_MAXPanel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW // Matrixde aynalama olmaması için ICSTATION_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

#define MAX_DEVICES 4 // Matrix Sayısı
#define CLK_PIN   14  // or SCK // D5 
#define DATA_PIN  13  // or MOSI // D7
#define CS_PIN    2  // or SS // D4
MD_Parola Display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES); //Matrixin pinlerini tanımlıyoruz

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.domainhizmetleri.com", 28800, 60000); // NTP Yani Network Time Protocol Sunucusunu tanımlıyoruz

/*        WiFi Ağ Adı Ve Şifresi        */
const char* ssid     = "******";
const char* password = "*******";

// Saat tanımlamaları
String Time, hour, minute;
String Formatted_date;
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;

void setup() {
  Serial.begin(115200);
  
  /*        WiFi Bağlantısı Kuruluyor       */
  WiFi.begin(ssid, password);
  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  // NTP Clientini tanımladık ve utc ayarını 3600 x 3 olarak ayarladık (UTC 3) 
  timeClient.begin();
  timeClient.setTimeOffset(10800);
  
  // Matrix ekranını oluşturcuk
  Display.begin();
  Display.setIntensity(0);
  Display.displayClear();

}

void loop()
 {
   // Saat Döngüsü
   Dongu();
}

void Dongu() {
	// Güncelleme
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }

  currentMillis = millis();

  if (currentMillis - previousMillis > interval)  {
previousMillis = millis();

Formatted_date = timeClient.getFormattedDate();
Serial.println(Formatted_date);

hour = Formatted_date.substring(11, 13);
minute = Formatted_date.substring(14, 16);

// Saati ekrana(matrixe) yazdırma
Time = hour + ":" + minute;
Serial.println(Time);
Display.setTextAlignment(PA_CENTER);
Display.print(Time);

}
}                                                     