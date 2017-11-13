#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "RTClib.h"

/////////////RTC//////////////
#if defined(ARDUINO_ARCH_SAMD)
   #define Serial SerialUSB
#endif
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/////////////WEB SEREVR //////////////
ESP8266WebServer server(80);
IPAddress timeServerIP; // time.nist.gov NTP server address
WiFiUDP udp;
const char* ssid = "TIH-Alpha2.4";
const char* password = "/*inventor*/";
const char WiFiAPPSK[] = "12345678";
unsigned int localPort = 2390;
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; 
byte packetBuffer[ NTP_PACKET_SIZE];

///VARIABLES DE ESTADOS///
int amarillo = D0;
int anteriorA;
int actualA = digitalRead(amarillo);
int contadorA1 = 0;
boolean valorA = digitalRead(amarillo);
int verde = D5;
int anteriorV;
int actualV = digitalRead(verde);
int contadorV1 = 0; 
boolean valorV = digitalRead(verde);

///VARIABLES DE TIEMPO EPOCH///
/*unsigned long epoch = 0;
int tiempoA1=0;
int tiempoA0=0;
int epochA1=0;
int epochA0=0;
int horaA1=0;
int minuA1=0;
int segA1=0;
int tiempoV1=0;
int tiempoV0=0;
int epochV1=0;
int epochV0=0;
int horaV1=0;
int minuV1=0;
int segV1=0;*/

///VARIABLES DE TIEMPO RTC///
DateTime tiempoA01;
DateTime tiempoA10;
int horaA0=0;
int minA0=0;
int segA0=0;
int horaA1=0;
int minA1=0;
int segA1=0;
int resta=1;
  
/*LIBRERIAS*/
///HTML///
void handleRoot();
void handleNotFound();


void setup(void){
  pinMode(amarillo, INPUT);
  pinMode(verde, INPUT);          
  Serial.begin(115200);

  // Wait for connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //ESP8266
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
    Serial.println("Starting UDP");
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());
  }

  //HTML
  /*server.on("/", handleRoot);
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  setupWiFi();
  server.begin();
  Serial.println("HTTP server started");*/

  //RCT
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (amarillo==0)
  {
    tiempoA01=rtc.now();
    Serial.print(tiempoA01.hour(), DEC);
    Serial.print(';');
    Serial.print(tiempoA01.minute(), DEC);
    Serial.print(';');
    Serial.print(tiempoA01.second(), DEC);
    Serial.println("");
    }
  else{
    tiempoA10=rtc.now();
    Serial.print(tiempoA10.hour(), DEC);
    Serial.print(':');
    Serial.print(tiempoA10.minute(), DEC);
    Serial.print(':');
    Serial.print(tiempoA10.second(), DEC);
    Serial.println("");
    }  
}


void loop(void){
  server.handleClient();
  
  /*WiFi.hostByName(ntpServerName, timeServerIP);
  sendNTPpacket(timeServerIP);
  int cb = udp.parsePacket();
  if (!cb) {}
  else {
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    epoch = secsSince1900 - seventyYears - 18000;
  }*/
  
  /*actualA = digitalRead(amarillo);
  valorA = !actualA; 
  Serial.print("amarillo: ");
  Serial.println(!digitalRead(amarillo));  
  if (anteriorA != actualA) // ha habido un cambio de estado
  {
    if (valorA==1)
    {
      contadorA1++; // cuenta los cambios de estado
      epochA1=epoch+epochA1;
      tiempoA0=epochA1-epochA0;
      }
      else
      {
        epochA0=epoch+epochA0;
        tiempoA1=epochA0-epochA1;
        }
    
    anteriorA = actualA;
  }

    
    actualV = digitalRead(verde);
    valorV = !actualV;
    Serial.print("verde: ");
    Serial.println(!digitalRead(verde));
  if (anteriorV != actualV) // ha habido un cambio de estado
  {
       if (valorV==1)
    {
      contadorV1++; // cuenta los cambios de estado
      epochV1=epoch+epochV1;
      tiempoV0=epochV1-epochV0;
      }
      else
      {
        epochV0=epoch+epochV0;
        tiempoV1=epochV0-epochV1;
        }
    anteriorV = actualV;
  }
*/

  ////con rct////
  actualA = digitalRead(amarillo);
  valorA = actualA; 
  actualV = digitalRead(verde);
  valorV = actualV;
  
  if (anteriorA != actualA) // ha habido un cambio de estado
  {
    if (valorA==1)
    {
      contadorA1++; // cuenta cuantas veces se encendio
      tiempoA01 = rtc.now();
      
      Serial.println("");
      Serial.print("hora de cambio a prendido:");
      Serial.print(tiempoA01.hour());
      Serial.print(':');
      Serial.print(tiempoA01.minute());
      Serial.print(':');
      Serial.print(tiempoA01.second());
      Serial.println("");

      segA0=(tiempoA01.second()-tiempoA10.second())+segA0;
      if (segA0<0)
      {
        tiempoA01.minute()=tiempoA01.minute()-resta;
        segA0=60+segA0;
        }
      minA0=(tiempoA01.minute()-tiempoA10.minute())+minA0;
      if (minA0<0)
      {
        tiempoA01.hour()=tiempoA01.hour()-resta;
        minA0=60+minA0;
        }
      horaA0=(tiempoA01.hour()-tiempoA10.hour())+horaA0
      Serial.print("horas apagado:");
      Serial.print(horaA0);
      Serial.print(':');
      Serial.print(minA0);
      Serial.print(':');
      Serial.print(segA0);
      Serial.println("");
    }
    else //valorA==0
      {
      tiempoA10 = rtc.now();
      Serial.println("");
      Serial.print("hora de cambio a apagado:");
      Serial.print(tiempoA10.hour());
      Serial.print(':');
      Serial.print(tiempoA10.minute());
      Serial.print(':');
      Serial.print(tiempoA10.second());
      Serial.println("");
      
      segA1=(tiempoA10.second()-tiempoA01.second())+segA1;
      if (segA1<0)
      {
        tiempoA10.minute()=tiempoA10.minute()-resta;
        segA1=60+segA1;
        }
      minA1=(tiempoA10.minute()-tiempoA01.minute())+minA1;
      if (minA1<0)
      {
        tiempoA10.hour()=tiempoA10.hour()-resta;
        minA1=60+minA1;
        }
      horaA1=(tiempoA10.hour()-tiempoA01.hour())+horaA1;
      
      Serial.print("horas prendido:");
      Serial.print(horaA1);
      Serial.print(':');
      Serial.print(minA1);
      Serial.print(':');
      Serial.print(segA1);
      Serial.println("");
      }
      
      anteriorA = actualA;
  }
  Serial.print("Amarillo: ");
  Serial.println(digitalRead(amarillo));
  Serial.print("Verde: ");
  Serial.println(digitalRead(verde));

  delay (10000);
}


void setupWiFi()
{
  WiFi.mode(WIFI_AP_STA);
  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "Torreta" + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
  AP_NameChar[i] = AP_NameString.charAt(i);
 
  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

unsigned long sendNTPpacket(IPAddress& address)
{
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}


/////HTML/////
/*void handleRoot() {
   horaA1=((tiempoA1  % 86400L) / 3600); 
    if ( ((tiempoA1 % 3600) / 60) < 10 ) {
      minuA1=0;
    }
    minuA1=(tiempoA1  % 3600) / 60;
    if ( (tiempoA1 % 60) < 10 ) {
      segA1=0;
    }
    segA1=tiempoA1 % 60;

      horaV1=((tiempoV1  % 86400L) / 3600); 
    if ( ((tiempoV1 % 3600) / 60) < 10 ) {
      minuV1=0;
    }
    minuV1=(tiempoV1  % 3600) / 60;
    if ( (tiempoV1 % 60) < 10 ) {
      segV1=0;
    }
    segV1=tiempoV1 % 60;

      Serial.println("Conectado a html");
  char temp[1000];
    snprintf ( temp, 1000,
          "<html>\
            <head>\
            <meta http-equiv='refresh' content='1'/>\
            <title>\Torreta ESP</title>\
            <style>\
            body { background-color: #17202A; font-family: Arial, Helvetica, Sans-Serif; Color: #FF5733; }\
            </style>\
            </head>\
            <body>\
              <h1>TORRETA</h1>\
              <table border=4>\
              <caption>\ </caption>\
                <tr>\
                  <th>\Color de torreta</th>\
                  <th>\Estado</th>\
                  <th>\Veces Encendido</th>\
                  <th>\Tiempo Encendido</th>\
                </tr>\
                <tr>\
                  <td>\Amarillo</td>\
                  <td>\%01d</td>\
                  <td>\%01d</td>\
                  <td>\%02d:%02d:%02d</td>\
                </tr>\
                <tr>\
                  <td>\Verde</td>\
                  <td>\%01d</td>\
                  <td>\%01d</td>\
                  <td>\%02d:%02d:%02d</td>\
                </tr>\
              </table>\
            </body>\
            </html>",
  
        valorA, contadorA1, horaA1, minuA1, segA1,
        valorV, contadorV1, horaV1, minuV1, segV1
           );
    server.send ( 200, "text/html", temp );
}*/

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
