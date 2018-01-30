/*
 * Electromnic Cats
 * Dec 2017
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "RTClib.h"
#include "FS.h"


/////////////RTC//////////////

RTC_DS3231 rtc;

/////////////WEB SERVER //////////////
ESP8266WebServer server(80);

const char WiFiAPPSK[] = "123456789";

///VARIABLES DE ESTADOS///

int Amarillo = D0;
int AntesAmllo;
int ActualAmllo;
int contadorA1 = 0;

int Verde = D5;
int AntesVerde;
int ActualVerde;
int contadorV1 = 0;

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
int datedia;
int datemes;
int dateyear;
int HorDif;  
int MinDif;
int SegDif; 
int tam_file;
int porce;

/*FUNCIONES*/
void handleRoot();
void handleNotFound();

void setup(void){
  //Entradas y Salidas
  pinMode(Amarillo, INPUT);
  pinMode(Verde, INPUT);          
  Serial.begin(115200);

  //SPIFFS
    Serial.print("Iniciando SPIFFS card...");
    SPIFFS.begin();
    
  //HTML
  server.on("/", handleRoot);
  server.on("/down", handleDownload);
  server.on("/form", formatear);
  server.on("/inline", []()
    {
      server.send(200, "text/plain", "this works as well");
    });
  server.onNotFound(handleNotFound);
  setupWiFi();
  server.begin();
  Serial.println(F("HTTP server started"));
  
  
  //RCT

  rtc.begin(); //Inicializamos el RTC
  Serial.println(F("Estableciendo Hora y fecha..."));
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println(F("DS3231 actualizada con la hora:"));
  Serial.print(F("Fecha = "));
  Serial.print(__DATE__);
  Serial.print(F("  Hora = "));
  Serial.println(__TIME__);
  
  TimeAntesVerde=TimeAntesAmllo=rtc.now();
   datedia=TimeAntesAmllo.day();
   datemes=TimeAntesAmllo.month();
   dateyear=TimeAntesAmllo.year();
  Serial.print(F("Hora de encendido: "));
  Serial.print(TimeAntesAmllo.hour(), DEC);
  Serial.print(':');
  Serial.print(TimeAntesAmllo.minute(), DEC);
  Serial.print(':');
  Serial.print(TimeAntesAmllo.second(), DEC);
  Serial.println("");
  Serial.print(TimeAntesAmllo.day(), DEC);
  Serial.print('/');
  Serial.print(TimeAntesAmllo.month(), DEC);
  Serial.print('/');
  Serial.print(TimeAntesAmllo.year(), DEC);
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

      Serial.println(F("AMARILLO ON "));
      DiferenciaTiempos(TimeActualAmllo, TimeAntesAmllo, TimeAcumAmlloOFFhor, TimeAcumAmlloOFFmin, TimeAcumAmlloOFFseg);
      TimeAcumAmlloOFFseg=GlobalTimeAcumSeg;
      TimeAcumAmlloOFFmin=GlobalTimeAcumMin;
      TimeAcumAmlloOFFhor=GlobalTimeAcumHor;
      Serial.print(F("Time Actual: "));
      ImpresionDeTiempos (TimeActualAmllo);
      Serial.println("");
      Serial.print(F("Time Antes: "));
      ImpresionDeTiempos (TimeAntesAmllo);
      Serial.println("");;
      contadorA1++;
    }

    else //actualA==0
      {
        TimeActualAmllo = rtc.now();
        TimeOFFamarillohor = TimeActualAmllo.hour();  //Para poder restarle  
        TimeOFFamarillomin = TimeActualAmllo.minute();  //Para poder restarle
        TimeOFFamarilloseg = TimeActualAmllo.second();  //Tiempo de Cambios
        Serial.print(F("AMARILLO OFF "));
        DiferenciaTiempos(TimeActualAmllo, TimeAntesAmllo, TimeAcumAmlloONhor, TimeAcumAmlloONmin, TimeAcumAmlloONseg);
        TimeAcumAmlloONseg=GlobalTimeAcumSeg;
        TimeAcumAmlloONmin=GlobalTimeAcumMin;
        TimeAcumAmlloONhor=GlobalTimeAcumHor;
        Serial.print(F("Time Actual: "));
        ImpresionDeTiempos (TimeActualAmllo);
        Serial.println(F(""));
        Serial.print(F("Time Antes: "));
        ImpresionDeTiempos (TimeAntesAmllo);
        Serial.println("");
        sdcard(contadorA1, TimeONamarillohor, TimeONamarillomin, TimeONamarilloseg, TimeOFFamarillohor, TimeOFFamarillomin, TimeOFFamarilloseg, TimeAcumAmlloONhor, TimeAcumAmlloONmin, TimeAcumAmlloONseg);
      }
      TimeAntesAmllo = TimeActualAmllo;
      AntesAmllo = ActualAmllo;
      porce=(tam_file/3600);
      Serial.println(tam_file);
      Serial.println(porce);
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
      Serial.print(F("Time Actual: "));
      ImpresionDeTiempos (TimeActualVerde);
      Serial.println(F(""));
      Serial.print(F("Time Antes: "));
      ImpresionDeTiempos (TimeAntesVerde);
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
        Serial.print(F("Time Actual: "));
        ImpresionDeTiempos (TimeActualVerde);
        Serial.println(F(""));
        Serial.print(F("Time Antes: "));
        ImpresionDeTiempos (TimeAntesVerde);
        Serial.println("");
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
//HTML
void handleRoot() {
  //Serial.println("Conectado a html");
  char temp[1500];
    snprintf (temp, 1500,
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
              <table>\
              </tr>\
              <th>\Memoria utilizada:</th>\
              <td>\%02d%</td>\
              </tr>\
              </table>\
              <table>\
                <tr>\
                  <th>\Color de torreta</th>\
                  <th>\Estado</th>\
                  <th>\Fecha</th>\
                  <th>\Veces Encendido</th>\
                  <th>\Hora encendido</th>\
                  <th>\Hora apagado</th>\
                  <th>\Tiempo Encendido</th>\
                </tr>\
                <tr>\
                  <td>\Amarillo</td>\
                  <td>\%01d</td>\
                  <td>\%02d/%02d/%02d</td>\
                  <td>\%01d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                </tr>\
                <tr>\
                  <td>\Verde</td>\
                  <td>\%01d</td>\
                  <td>\%02d/%02d/%02d</td>\
                  <td>\%01d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                </tr>\
              </table>\
              <a href='/down'>Ir a archivo</a>\
              <a href='/form'>Nuevo historial</a>\
            </body>\
            </html>"
         ,porce
         ,ActualAmllo, datedia, datemes, dateyear, contadorA1, TimeONamarillohor, TimeONamarillomin, TimeONamarilloseg,TimeOFFamarillohor, TimeOFFamarillomin,TimeOFFamarilloseg, TimeAcumAmlloONhor, TimeAcumAmlloONmin, TimeAcumAmlloONseg
         ,ActualVerde, datedia, datemes, dateyear, contadorV1, TimeONverdehor, TimeONverdemin, TimeONverdeseg, TimeOFFverdehor, TimeOFFverdemin, TimeOFFverdeseg, TimeAcumVerdeONhor, TimeAcumVerdeONmin, TimeAcumVerdeONseg
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
   SegDif = (TimeActual.second()-TimeAntes.second());  //Tiempo de Cambios
  if(SegDif<0)
  {
    SegDif = 60 + SegDif;
    Min = Min - 1;  //TimeActual.minute() - 1
  }
  TimeAcumSeg = SegDif + TimeAcumSeg; //Segundos totales
  MinDif = Min - TimeAntes.minute();
  if(MinDif<0)
  {
    MinDif = 60 + MinDif;
    Hor = Hor - 1;  //TimeActual.hour() - 1
  }
  TimeAcumMin = MinDif + TimeAcumMin; //Minutos totales
   HorDif = Hor - TimeAntes.hour();
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
 
  Serial.print(F("Acumulado:"));
  Serial.print(HorDif);
  Serial.print(':');
  Serial.print(MinDif);
  Serial.print(':');
  Serial.print(SegDif);
  Serial.println(F(""));
}

void ImpresionDeTiempos(DateTime TimeGlobal) 
{
  Serial.print(TimeGlobal.hour());
  Serial.print(':');
  Serial.print(TimeGlobal.minute());
  Serial.print(':');
  Serial.print(TimeGlobal.second());
}

void sdcard(int count, int horON, int minON, int segON, int OFFhor, int OFFmin, int OFFseg, int acuhr, int acumin, int acumseg)
{
  int tam;
  String dataString="";
  dataString += String(datedia);//valor que va en el contador
  dataString +="/";
  dataString += String(datemes);//valor que va en el contador
  dataString +="/";
  dataString += String(dateyear);//valor que va en el contador
  dataString +=","; //la coma deja divir en columnas
  dataString += String(count);//valor que va en el contador
  dataString +=","; //la coma deja divir en columnas
  dataString += String(horON);//valor que va en el contador
  dataString +=":";
  dataString += String(minON);//valor que va en el contador
  dataString +=":";
  dataString += String(segON);//valor que va en el contador
  dataString +=","; //la coma deja divir en columnas
  dataString += String(OFFhor);//valor que va en el contador
  dataString +=":";
  dataString += String(OFFmin);//valor que va en el contador
  dataString +=":";
  dataString += String(OFFseg);//valor que va en el contador
  dataString +=","; //la coma deja divir en columnas
  dataString += String(HorDif);//valor que va en el contador
  dataString +=":";
  dataString += String(MinDif);//valor que va en el contador
  dataString +=":";
  dataString += String(SegDif);//valor que va en el contador
  dataString +=","; //la coma deja divir en columnas
  dataString += String(acuhr);//valor que va en el contador
  dataString +=":";
  dataString += String(acumin);//valor que va en el contador
  dataString +=":";
  dataString += String(acumseg);//valor que va en el contador
  dataString +=","; //la coma deja divir en columnas

  // open file for writing
  File f = SPIFFS.open("/f.txt", "a+");
  if (!f) {
      Serial.println("file open failed");
  }
  Serial.println("====== Writing to SPIFFS file =========");
  // write strings to file
  f.println(dataString);
  Serial.println(dataString);
      int size=f.size();
     if (size>360000)
        {
         Serial.print("formatear");
         Serial.print("the memory will finish");
         //Next lines have to be done ONLY ONCE!!!!!When SPIFFS is formatted ONCE you can comment these lines out!!
         Serial.println("Please wait 30 secs for SPIFFS to be formatted");
         SPIFFS.format();
         Serial.println("Spiffs formatted");
         Serial.print(size);
        }
  tam_file=size;
  f.close();
  }
  void handleDownload() 
  {
    int32_t time = millis();
    // open file for reading
    File dataFile = SPIFFS.open("/f.txt", "a+");//guardar mas datos
    int fsizeDisk = dataFile.size();
    Serial.print("fsizeDisk: "); Serial.println(fsizeDisk);

    String WebString = "";
    WebString += "HTTP/1.1 200 OK\r\n";
    WebString += "Content-Type: text/plain\r\n";
    WebString += "Content-Disposition: attachment; filename=\"datalog.csv\"\r\n";
    WebString += "Content-Length: " + String(fsizeDisk) + "\r\n";
    WebString += "\r\n";
    server.sendContent(WebString);

    char buf[1024];
    int siz = dataFile.size();
    while(siz > 0) {
        size_t len = std::min((int)(sizeof(buf) - 1), siz);
        dataFile.read((uint8_t *)buf, len);
        server.client().write((const char*)buf, len);
        siz -= len;
    }
    Serial.print(siz);
    Serial.println(" Bytes left!");

    dataFile.close();
    time = millis() - time;
    Serial.print(time); Serial.println(" ms elapsed");
}
void formatear()
{
         Serial.print("formatear");
         Serial.print("the memory will finish");
         //Next lines have to be done ONLY ONCE!!!!!When SPIFFS is formatted ONCE you can comment these lines out!!
         Serial.println("Please wait 30 secs for SPIFFS to be formatted");
         SPIFFS.format();
         Serial.println("Spiffs formatted");
         Serial.print(tam_file);
  }
