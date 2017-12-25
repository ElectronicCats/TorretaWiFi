/*
 * Electromnic Cats
 * Dec 2017
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h> //Revisar si es necesaria esta biblioteca??
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "RTClib.h"\

/////////////RTC//////////////

RTC_DS1307 rtc;

/////////////WEB SEREVR //////////////
ESP8266WebServer server(80);

IPAddress timeServerIP; // time.nist.gov NTP server address
WiFiUDP udp;
const char* ssid = "Familia Rodriguez";
const char* password = "rodriguez2020";
const char WiFiAPPSK[] = "12345678";
unsigned int localPort = 2390;
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; 
byte packetBuffer[ NTP_PACKET_SIZE];

///VARIABLES DE ESTADOS///
int Amarillo = D6;
int AntesAmllo;
int ActualAmllo;
int contadorA1 = 0;
int contadorA0 = 0;

int Verde = D7;
int AntesVerde;
int ActualVerde;
int contadorV1 = 0;
int contadorV0 = 0;

///VARIABLES DE TIEMPO RTC///
DateTime TimeGlobal;
int GlobalTimeAcumSeg;
int GlobalTimeAcumMin;
int GlobalTimeAcumHor;
DateTime TimeAntesAmllo;
DateTime TimeActualAmllo;
int TimeONamarilloseg;
int TimeONamarillomin;
int TimeONamarillohor;
int TimeOFFamarilloseg;
int TimeOFFamarillomin;
int TimeOFFamarillohor;
int TimeAcumAmlloOFFseg;
int TimeAcumAmlloOFFmin;
int TimeAcumAmlloOFFhor;
int TimeAcumAmlloONseg;
int TimeAcumAmlloONmin;
int TimeAcumAmlloONhor;
DateTime TimeAntesVerde;
DateTime TimeActualVerde;
int TimeONverdeseg;
int TimeONverdemin;
int TimeONverdehor;
int TimeOFFverdeseg;
int TimeOFFverdemin;
int TimeOFFverdehor;
int TimeAcumVerdeOFFseg;
int TimeAcumVerdeOFFmin;
int TimeAcumVerdeOFFhor;
int TimeAcumVerdeONseg;
int TimeAcumVerdeONmin;
int TimeAcumVerdeONhor;

/*FUNCIONES*/
void handleRoot();
void handleNotFound();

void setup(void){
  //Entradas y Salidas
  pinMode(Amarillo, INPUT);
  pinMode(Verde, INPUT);          
  Serial.begin(115200);
 
  //Wait for connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  //ESP8266
  if (MDNS.begin("esp8266")) 
    {
      Serial.println("MDNS responder started");
      Serial.println("Starting UDP");
      udp.begin(localPort);
      Serial.print("Local port: ");
      Serial.println(udp.localPort());
    }
 

  //HTML
  server.on("/", handleRoot);
  server.on("/inline", []()
    {
      server.send(200, "text/plain", "this works as well");
    });
  server.onNotFound(handleNotFound);
  setupWiFi();
  server.begin();
  Serial.println("HTTP server started");
  
  
  //RCT
  rtc.begin(); //Inicializamos el RTC
  Serial.println("Estableciendo Hora y fecha...");
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("DS1307 actualizado con la hora y fecha que se compilo este programa:");
  Serial.print("Fecha = ");
  Serial.print(__DATE__);
  Serial.print("  Hora = ");
  Serial.println(__TIME__);
  
  TimeAntesVerde=TimeAntesAmllo=rtc.now();
  Serial.print("Hora de encendido: ");
  Serial.print(TimeAntesAmllo.hour(), DEC);
  Serial.print(':');
  Serial.print(TimeAntesAmllo.minute(), DEC);
  Serial.print(':');
  Serial.print(TimeAntesAmllo.second(), DEC);
  Serial.println("");
}

void loop(void){
  server.handleClient();
  //Calculo de tiempocon RTC
  ActualAmllo= digitalRead(Amarillo); 
  ActualVerde = digitalRead(Verde);
  if (AntesAmllo != ActualAmllo) //ha habido un cambio de estado
  {
    if (ActualAmllo==1)
    {
      TimeActualAmllo = rtc.now();
  TimeONamarillohor = TimeActualAmllo.hour();  //Para poder restarle  
  TimeONamarillomin = TimeActualAmllo.minute();  //Para poder restarle
  TimeONamarilloseg = TimeActualAmllo.second();  //Tiempo de Cambios
  TimeOFFamarillohor = 0;  //Para poder restarle  
  TimeOFFamarillomin = 0;  //Para poder restarle
  TimeOFFamarilloseg = 0;  //Tiempo de Cambios

      Serial.print("AMARILLO ON ");
      DiferenciaTiempos(TimeActualAmllo, TimeAntesAmllo, TimeAcumAmlloOFFhor, TimeAcumAmlloOFFmin, TimeAcumAmlloOFFseg);
      TimeAcumAmlloOFFseg=GlobalTimeAcumSeg;
      TimeAcumAmlloOFFmin=GlobalTimeAcumMin;
      TimeAcumAmlloOFFhor=GlobalTimeAcumHor;
      Serial.print("Time Actual: ");
      ImpresionDeTiempos (TimeActualAmllo);
      Serial.println("");
      Serial.print("Time Antes: ");
      ImpresionDeTiempos (TimeAntesAmllo);
      Serial.println("");
      Serial.print("Horas Apagado: ");
      ImpresionDeTiempos (TimeAcumAmlloOFFhor);
      Serial.println("");
      contadorA1++;
    }

    else //actualA==0
      {
        TimeActualAmllo = rtc.now();
        TimeOFFamarillohor = TimeActualAmllo.hour();  //Para poder restarle  
        TimeOFFamarillomin = TimeActualAmllo.minute();  //Para poder restarle
        TimeOFFamarilloseg = TimeActualAmllo.second();  //Tiempo de Cambios
        Serial.print("AMARILLO OFF ");
        DiferenciaTiempos(TimeActualAmllo, TimeAntesAmllo, TimeAcumAmlloONhor, TimeAcumAmlloONmin, TimeAcumAmlloONseg);
        TimeAcumAmlloONseg=GlobalTimeAcumSeg;
        TimeAcumAmlloONmin=GlobalTimeAcumMin;
        TimeAcumAmlloONhor=GlobalTimeAcumHor;
        Serial.print("Time Actual: ");
        ImpresionDeTiempos (TimeActualAmllo);
        Serial.println("");
        Serial.print("Time Antes: ");
        ImpresionDeTiempos (TimeAntesAmllo);
        Serial.println("");
        Serial.print("Horas Apagado: ");
        ImpresionDeTiempos (TimeAcumAmlloONhor);
        Serial.println("");
        contadorA0++;
      }
      TimeAntesAmllo = TimeActualAmllo;
      AntesAmllo = ActualAmllo;
  }
     if (AntesVerde != ActualVerde) //ha habido un cambio de estado
  {
    if (ActualVerde==1)
    {
      TimeActualVerde = rtc.now();
     TimeONverdehor = TimeActualVerde.hour();  //Para poder restarle  
     TimeONverdemin = TimeActualVerde.minute();  //Para poder restarle
     TimeONverdeseg = TimeActualVerde.second();  //Tiempo de Cambios
     TimeOFFverdehor = 0;  //Para poder restarle  
     TimeOFFverdemin = 0;  //Para poder restarle
     TimeOFFverdeseg = 0;  //Tiempo de Cambios
      Serial.print("VERDE ON");
      DiferenciaTiempos(TimeActualVerde, TimeAntesVerde, TimeAcumVerdeOFFhor, TimeAcumVerdeOFFmin, TimeAcumVerdeOFFseg);
      TimeAcumVerdeOFFseg=GlobalTimeAcumSeg;
      TimeAcumVerdeOFFmin=GlobalTimeAcumMin;
      TimeAcumVerdeOFFhor=GlobalTimeAcumHor;
      Serial.print("Time Actual: ");
      ImpresionDeTiempos (TimeActualVerde);
      Serial.println("");
      Serial.print("Time Antes: ");
      ImpresionDeTiempos (TimeAntesVerde);
      Serial.println("");
      Serial.print("Horas Apagado: ");
      ImpresionDeTiempos (TimeAcumVerdeOFFhor);
      Serial.println("");
      contadorV1++;
    }
    else //actualA==0
      {
        TimeActualVerde = rtc.now();
     TimeOFFverdehor = TimeActualVerde.hour();  //Para poder restarle  
     TimeOFFverdemin = TimeActualVerde.minute();  //Para poder restarle
     TimeOFFverdeseg = TimeActualVerde.second();  //Tiempo de Cambios
        Serial.print("VERDE OFF");
        DiferenciaTiempos(TimeActualVerde, TimeAntesVerde, TimeAcumVerdeONhor, TimeAcumVerdeONmin, TimeAcumVerdeONseg);
        TimeAcumVerdeONseg=GlobalTimeAcumSeg;
        TimeAcumVerdeONmin=GlobalTimeAcumMin;
        TimeAcumVerdeONhor=GlobalTimeAcumHor;
        Serial.print("Time Actual: ");
        ImpresionDeTiempos (TimeActualVerde);
        Serial.println("");
        Serial.print("Time Antes: ");
        ImpresionDeTiempos (TimeAntesVerde);
        Serial.println("");
        Serial.print("Horas Apagado: ");
        ImpresionDeTiempos (TimeAcumVerdeONhor);
        Serial.println("");
        contadorV0++;
      }
      TimeAntesVerde = TimeActualVerde;
      AntesVerde = ActualVerde;
  }
  delay (10);
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

//HTML
void handleRoot() {
  //Serial.println("Conectado a html");
  char temp[1000];
    snprintf (temp, 1000,
          "<html>\
            <head>\
            <meta http-equiv='refresh' content='1'/>\
            <title>\Torreta ESP</title>\
            </head>\
            <body>\
            <script>\
               function usrpas(){\
               if (document.form1.txt.value=='admin' && document.form1.num.value=='1234'){window.location='home.htm'}\
                else {alert('Error en Usuario o Contraseña. Intenta de nuevo.')}\
                    }\
             document.oncontextmenu=new Function('return false');\
            </script>\
            <form>\
            <input type='text' name='txt'> Usuario (admin)<br>\
            <input type='text' name='num' maxlength='4'> Contraseña (1234)<br>\
           <input type='button' value='ir a..' onclick='usrpas()'>\
            </form>\
              <h1>TORRETA</h1>\
              <table>\
                <tr>\
                  <th>\Color de torreta</th>\
                  <th>\Estado</th>\
                  <th>\Veces Encendido</th>\
                  <th>\Hora encendido</th>\
                  <th>\Hora apagado</th>\
                  <th>\Tiempo Encendido</th>\
                </tr>\
                <tr>\
                  <td>\Amarillo</td>\
                  <td>\%01d</td>\
                  <td>\%01d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                </tr>\
                <tr>\
                  <td>\Verde</td>\
                  <td>\%01d</td>\
                  <td>\%01d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                </tr>\
              </table>\
            </body>\
            </html>"
         ,ActualAmllo, contadorA1, TimeONamarillohor, TimeONamarillomin, TimeONamarilloseg,TimeOFFamarillohor, TimeOFFamarillomin,TimeOFFamarilloseg, TimeAcumAmlloONhor, TimeAcumAmlloONmin, TimeAcumAmlloONseg
         ,ActualVerde, contadorV1, TimeONverdehor, TimeONverdemin, TimeONverdeseg, TimeOFFverdehor, TimeOFFverdemin, TimeOFFverdeseg, TimeAcumVerdeONhor, TimeAcumVerdeONmin, TimeAcumVerdeONseg
        );
   server.send ( 200, "text/html", temp );
}

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


void DiferenciaTiempos(DateTime TimeActual, DateTime TimeAntes, int TimeAcumHor, int TimeAcumMin, int TimeAcumSeg)
{
  GlobalTimeAcumSeg=0;
  GlobalTimeAcumMin=0;
  GlobalTimeAcumHor=0;
  int Hor = TimeActual.hour();  //Para poder restarle  
  int Min = TimeActual.minute();  //Para poder restarle
  int SegDif = (TimeActual.second()-TimeAntes.second());  //Tiempo de Cambios
  if(SegDif<0)
  {
    SegDif = 60 + SegDif;
    Min = Min - 1;  //TimeActual.minute() - 1
  }
  TimeAcumSeg = SegDif + TimeAcumSeg; //Segundos totales
  int MinDif = Min - TimeAntes.minute();
  if(MinDif<0)
  {
    MinDif = 60 + MinDif;
    Hor = Hor - 1;  //TimeActual.hour() - 1
  }
  TimeAcumMin = MinDif + TimeAcumMin; //Minutos totales
  int HorDif = Hor - TimeAntes.hour();
  TimeAcumHor = HorDif + TimeAcumHor; //Horas totales

  int mindiv=TimeAcumSeg/60;
  if(0<mindiv)
  {
    TimeAcumMin=TimeAcumMin+mindiv;
    TimeAcumSeg=TimeAcumSeg-(mindiv*60);
  }
  GlobalTimeAcumSeg=TimeAcumSeg;
  int hordiv=TimeAcumSeg/60;
  if(0<mindiv)
  {
    TimeAcumHor=TimeAcumHor+hordiv;
    TimeAcumMin=TimeAcumMin-(hordiv*60);
  }
  GlobalTimeAcumMin=TimeAcumMin;
  GlobalTimeAcumHor=TimeAcumHor;
 
  Serial.print("Acumulado:");
  Serial.print(HorDif);
  Serial.print(':');
  Serial.print(MinDif);
  Serial.print(':');
  Serial.print(SegDif);
  Serial.println("");
}

void ImpresionDeTiempos(DateTime TimeGlobal) 
{
  Serial.print(TimeGlobal.hour());
  Serial.print(':');
  Serial.print(TimeGlobal.minute());
  Serial.print(':');
  Serial.print(TimeGlobal.second());
}

