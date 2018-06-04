/*
 * Electromnic Cats
 * Dec 2017
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>  //Ticker Library 
#include "RTClib.h"
#include "FS.h"

/////////////RTC//////////////

RTC_DS3231 rtc;

Ticker blinker;

/////////////WEB SERVER //////////////
ESP8266WebServer server(80);

const char WiFiAPPSK[] = "12345678";

///VARIABLES DE ESTADOS///

int Amarillo = 16;//16
int AntesAmllo;
int ActualAmllo;
int contadorA1 = 0;

int Verde = 14;//14
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
DateTime TimeAntesmanual; 
DateTime TimeActualmanual; 
int TimeONmanualseg; 
int TimeONmanualmin; 
int TimeONmanualhor; 
int TimeOFFmanualseg; 
int TimeOFFmanualmin; 
int TimeOFFmanualhor; 
int TimeAcummanualOFFseg; 
int TimeAcummanualOFFmin; 
int TimeAcummanualOFFhor; 
int TimeAcummanualONseg; 
int TimeAcummanualONmin; 
int TimeAcummanualONhor;
DateTime TimeActualdow; 
int Timedowhor;  //Para poder restarle   
int Timedowmin;  //Para poder restarle 
int Timedowseg; 
int timedowload; 
int TimeAcumdowONseg; 
int TimeAcumdowONmin; 
int TimeAcumdowONhor; 
int TimeAcumdowONsegmanual; 
int TimeAcumdowONminmanual; 
int TimeAcumdowONhormanual; 
int TimeAcumdowONsegverde; 
int TimeAcumdowONminverde; 
int TimeAcumdowONhorverde; 
int datedia;
int datemes;
int dateyear;
int HorDif;  
int MinDif;
int SegDif; 
int tam_file;
int porce;
int color=0; 

int actualflagmanual=0; 
int antesflagmanual=0;   
int flagtest=0;  
DateTime Timetest=0;   
bool flagAmllo=0; 
bool flagVerde=0; 

//Manual mode//
int Antesmanual; 
int Actualmanual; 
int contadorT1 = 0; 
int contadorM1 = 0;
int timemanual; 

//Parpadeo// 
int countOFF=0; 
int Amarilloint=0;

/*FUNCIONES*/ 
void setupWiFi(); 
void handleRoot(); 
void handleNotFound(); 
void DiferenciaTiempos(DateTime TimeActual, DateTime TimeAntes, int TimeAcumHor, int TimeAcumMin, int TimeAcumSeg); 
void ImpresionDeTiempos(DateTime TimeGlobal); 
void sdcard(int color, int count, int horON, int minON, int segON, int OFFhor, int OFFmin, int OFFseg, int acuhr, int acumin, int acumseg); 
void handleDownload(); 
void formatear();
void changeState();

void setup(void){
  //Entradas y Salidas
  pinMode(Amarillo, INPUT);
  pinMode(Verde, INPUT);          
  Serial.begin(115200);

  /*parpadeo*/ 
  //Initialize Ticker every 0.5s 
  blinker.attach(.5, changeState); //Use <strong>attach_ms</strong> if you need time in ms 
 
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

  //SPIFFS
    Serial.print("Iniciando SPIFFS card...");
    SPIFFS.begin();
    
  //RCT
  rtc.begin(); //Inicializamos el RTC
  Serial.println(F("Estableciendo Hora y fecha..."));
 // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println(F("DS3231 actualizada con la hora:"));
  Serial.print(F("Fecha = "));
  Serial.print(__DATE__);
  Serial.print(F("  Hora = "));
  Serial.println(__TIME__);
  
  Timetest=TimeAntesVerde=TimeAntesAmllo=rtc.now();
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

       if((digitalRead(Verde)==1) && (digitalRead(Amarillo)==1)) 
       {
        if(flagtest==0)//Detecs change test (yellow and green =1) 
         { 
          contadorT1++; 
          Serial.println("Test ON"); 
          Serial.print(F("Time Actual: ")); 
          ImpresionDeTiempos (Timetest); 
          Serial.print(F(" Contador test: ")); 
          Serial.println(contadorT1); 
          flagtest=1;
         }
       } 
    else if(!ActualAmllo && !ActualVerde)//Detecs manual mode (yellow and green =0) 
     { 
       actualflagmanual=1;//Activation manual mode 
     } 
   else//Detecs green or yellow are ON 
     { 
       if(actualflagmanual) 
        { 
        actualflagmanual=0; 
        } 
     } 

  if((actualflagmanual!= antesflagmanual)&&(flagtest==0)) 
{ 
    if(actualflagmanual) 
    { 
  TimeActualmanual = rtc.now(); 
  TimeONmanualhor = TimeActualmanual.hour();  //Para poder restarle   
  TimeONmanualmin = TimeActualmanual.minute();  //Para poder restarle 
  TimeONmanualseg = TimeActualmanual.second();  //Tiempo de Cambios 
   Serial.println(F("MANUAL ON ")); 
      Serial.print(F("Time Actual: ")); 
      ImpresionDeTiempos (TimeActualmanual); 
      Serial.println(""); 
      contadorM1++;
      timemanual=0;
        color=3; 
        sdcard(color, contadorM1, TimeONmanualhor, TimeONmanualmin, TimeONmanualseg, timemanual,timemanual, timemanual, timemanual, timemanual, timemanual);  
      } 
    else //actual manual off 
      { 
        Serial.print(F("MANUAL OFF "));   
      } 
      antesflagmanual=actualflagmanual;    
   }

   
  if(flagtest==0) 
  { 
    if((ActualAmllo==1) && (flagAmllo==0))
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
       TimeAntesAmllo=TimeActualAmllo;
      flagAmllo=1;
    }

    if((ActualAmllo==0) && (flagAmllo==1)) //actualA==0
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
        color=1; 
        sdcard(color, contadorA1, TimeONamarillohor, TimeONamarillomin, TimeONamarilloseg, TimeOFFamarillohor, TimeOFFamarillomin, TimeOFFamarilloseg, TimeAcumAmlloONhor, TimeAcumAmlloONmin, TimeAcumAmlloONseg);
     TimeAntesAmllo = TimeActualAmllo;
     AntesAmllo = ActualAmllo;
     flagAmllo=0; 
    } 
   if((ActualVerde==1) && (flagVerde==0))
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
      TimeAntesVerde=TimeActualVerde;
      flagVerde=1;
    }
    if((ActualVerde==0) && (flagVerde==1)) // //actualV==0
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
        color=2; 
        sdcard(color, contadorV1, TimeONverdehor, TimeONverdemin, TimeONverdeseg, TimeOFFverdehor, TimeOFFverdemin, TimeOFFverdeseg, TimeAcumVerdeONhor, TimeAcumVerdeONmin, TimeAcumVerdeONseg);          
        flagVerde=0;
      TimeAntesVerde = TimeActualVerde;
      AntesVerde = ActualVerde;
      } 
  }
 delay (1000);  
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);
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
  char temp[2500];
    snprintf (temp, 2500,
          "<html>\
            <head>\
            <meta http-equiv='refresh' content='1'/>\
            <style>\
            body { background-color: #000000; font-family: Arial, Helvetica, Sans-Serif; Color: #FFFFFF; }\
            </style>\
            <script type='text/javascript'>\
            function ConfirmDemo() {\
            var mensaje = confirm('Al iniciar un nuevo historial se borraran los datos anteriores Desea continual?');\
            if (mensaje) {document.historial.submit();}\
            else {alert('Seguimiento historial')}\
            }\
            </script>\
            </head>\
            <body>\
            <style>\ 
              div   { text-align: center; }\ 
              table { margin: auto; }\ 
              </style>\
              <table>\
              </tr>\
              <th>\Memory doc:</th>\
              <td>\%02d%</td>\
              <th>\Test:</th>\
              <td>\%02d</td>\
              </tr>\
              </table>\
              </div>\
              <table>\
                <tr>\
                  <th>\State</th>\
                  <th>\   </th>\
                  <th>\Date</th>\
                  <th>\Times ON</th>\
                  <th>\Activation Time</th>\
                  <th>\Deactivation Time</th>\
                  <th>\Accumulaate Time ON</th>\
                </tr>\
                <tr>\
                  <td>\WORKS</td>\
                  <td>\%01d</td>\
                  <td>\%02d/%02d/%02d</td>\
                  <td>\%01d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                </tr>\
                <tr>\
                  <td>\PREVENTIVE</td>\
                  <td>\%01d</td>\
                  <td>\%02d/%02d/%02d</td>\
                  <td>\%01d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                </tr>\
              </table>\
              <div>\
              <style type='text/css'>\
              <!-- .centrar { text-align:center; }-->\
              </style>\
              <a href='/down'>Descargar historial</a>\
              <form name=historial action='/form'>\
              <input type='button' onclick='ConfirmDemo()' value='Nuevo historial' />\
              </form>\
            </body>\
            </html>"
         ,porce
         ,contadorT1
         ,ActualVerde, datedia, datemes, dateyear, contadorV1, TimeONverdehor, TimeONverdemin, TimeONverdeseg, TimeOFFverdehor, TimeOFFverdemin, TimeOFFverdeseg, TimeAcumVerdeONhor, TimeAcumVerdeONmin, TimeAcumVerdeONseg
         ,ActualAmllo, datedia, datemes, dateyear, contadorA1, TimeONamarillohor, TimeONamarillomin, TimeONamarilloseg,TimeOFFamarillohor, TimeOFFamarillomin,TimeOFFamarilloseg, TimeAcumAmlloONhor, TimeAcumAmlloONmin, TimeAcumAmlloONseg
         );
   server.send ( 200, "text/html", temp );
}

void handleNotFound()
{
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

void sdcard(int color, int count, int horON, int minON, int segON, int OFFhor, int OFFmin, int OFFseg, int acuhr, int acumin, int acumseg) 
{ 
  int tam; 
  String colors; 
  if (color==1) 
  { 
     colors="PREVENTIVE"; 
    } 
   if (color==2) 
   { 
     colors="WORKS"; 
    } 
     if (color==3) 
   { 
     colors="MANUAL MODE"; 
     HorDif=0;
     MinDif=0;
     SegDif=0;
    }  
  String dataString=""; 
  dataString += String(colors);//valor que va en el contador 
  dataString +=","; //la coma deja divir en columnas 
  dataString += String(datedia);
  dataString +="/";
  dataString += String(datemes);
  dataString +="/";
  dataString += String(dateyear);
  dataString +=",";
  dataString += String(count);
  dataString +=","; 
  dataString += String(horON);
  dataString +=":";
  dataString += String(minON);
  dataString +=":";
  dataString += String(segON);
  dataString +=",";
  dataString += String(OFFhor);
  dataString +=":";
  dataString += String(OFFmin);
  dataString +=":";
  dataString += String(OFFseg);
  dataString +=","; 
  dataString += String(HorDif);
  dataString +=":";
  dataString += String(MinDif);
  dataString +=":";
  dataString += String(SegDif);
  dataString +=","; 
  dataString += String(acuhr);
  dataString +=":";
  dataString += String(acumin);
  dataString +=":";
  dataString += String(acumseg);
  dataString +=","; 

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
     TimeActualdow = rtc.now(); 
         Timedowhor = TimeActualdow.hour();  //Para poder restarle   
         Timedowmin = TimeActualdow.minute();  //Para poder restarle 
         Timedowseg = TimeActualdow.second();  //Tiempo de Cambios 
         Serial.print(F("DOWLOAD ")); 
         Serial.print(F("Time Actual: ")); 
         ImpresionDeTiempos (TimeActualdow); 
         Serial.println(F("")); 
         if(ActualAmllo) 
         { 
         DiferenciaTiempos(TimeActualdow, TimeAntesAmllo, TimeAcumAmlloONhor, TimeAcumAmlloONmin, TimeAcumAmlloONseg); 
         Serial.print(F("Time Antes: ")); 
         ImpresionDeTiempos (TimeAntesAmllo); 
         Serial.println(""); 
         } 
         if(ActualVerde) 
         { 
         DiferenciaTiempos(TimeActualdow, TimeAntesVerde, TimeAcumVerdeONhor, TimeAcumVerdeONmin, TimeAcumVerdeONseg); 
         Serial.print(F("Time Antes: ")); 
         ImpresionDeTiempos (TimeAntesVerde); 
         Serial.println(""); 
         } 
          if(actualflagmanual) 
         { 
         DiferenciaTiempos(TimeActualdow, TimeAntesmanual, TimeAcummanualONhor, TimeAcummanualONmin, TimeAcummanualONseg); 
         Serial.print(F("Time Antes: ")); 
         ImpresionDeTiempos (TimeAntesmanual); 
         Serial.println(""); 
         } 
         TimeAcumdowONseg=GlobalTimeAcumSeg; 
         TimeAcumdowONmin=GlobalTimeAcumMin; 
         TimeAcumdowONhor=GlobalTimeAcumHor; 
         TimeAcumdowONsegmanual=GlobalTimeAcumSeg; 
         TimeAcumdowONminmanual=GlobalTimeAcumMin; 
         TimeAcumdowONhormanual=GlobalTimeAcumHor; 
         TimeAcumdowONsegverde=GlobalTimeAcumSeg; 
         TimeAcumdowONminverde=GlobalTimeAcumMin; 
         TimeAcumdowONhorverde=GlobalTimeAcumHor; 
         int timedowload=0;  
         if(ActualAmllo) 
         { 
         color=1; 
         sdcard(color, contadorA1, TimeONamarillohor, TimeONamarillomin, TimeONamarilloseg, timedowload, timedowload, timedowload, TimeAcumdowONhor, TimeAcumdowONmin, TimeAcumdowONseg);       
         } 
         if(ActualVerde)
         { 
        color=2; 
        sdcard(color, contadorV1, TimeONverdehor, TimeONverdemin, TimeONverdeseg, timedowload, timedowload, timedowload, TimeAcumdowONhorverde, TimeAcumdowONminverde, TimeAcumdowONsegverde);          
         } 
         if(actualflagmanual) 
         { 
         color=3; 
         sdcard(color, contadorM1, TimeONmanualhor, TimeONmanualmin, TimeONmanualseg, timedowload, timedowload, timedowload, TimeAcumdowONhormanual, TimeAcumdowONminmanual, TimeAcumdowONsegmanual);       
         }  
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
         contadorT1=0; 
         contadorM1=0; 
         contadorA1=0; 
         contadorV1=0; 
         TimeAcumVerdeONhor=0; 
         TimeAcumVerdeONmin=0; 
         TimeAcumVerdeONseg=0; 
         TimeAcumAmlloONhor=0; 
         TimeAcumAmlloONmin=0; 
         TimeAcumAmlloONseg=0; 
         TimeAcummanualONhor=0; 
         TimeAcummanualONmin=0; 
         TimeAcummanualONseg=0;
  }

  void changeState() 
{ 
  ActualVerde = digitalRead(Verde);
  Amarilloint= digitalRead(Amarillo); 
  
  
  if(Amarilloint) 
  { 
     ActualAmllo=1; 
     countOFF=0;
  } 
  else  
    { 
    countOFF++; 
    if(countOFF>5) 
      { 
        ActualAmllo=0;  
        countOFF=0;
        flagtest=0; 
      } 
    } 
  }
 
