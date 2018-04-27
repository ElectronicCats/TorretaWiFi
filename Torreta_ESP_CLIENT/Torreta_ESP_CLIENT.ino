/* Electromnic Cats Dec 2017 */
#include <ESP8266WiFi.h>
#include "RTClib.h"
 
/*----RTC----*/
RTC_DS3231 rtc;

//CLIENT VARIABLES
 char*         TKDssid;            // Wifi Name
 char*         TKDpassword;        // Wifi Password
 
 int             TKDServerPort  = 80;
 IPAddress       TKDServer(192,168,4,1);//IP de estación
 WiFiClient      TKDClient;
 
/*VARIABLES DE TORRETA*/
int Amarillo = 16;
int AntesAmllo;
int ActualAmllo;
int contadorA1 = 0;

int Verde = 14;
int AntesVerde;
int ActualVerde;
int contadorV1 = 0;
 
/*VARIABLES DE TIEMPO*/
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
 
void TKDRequest();
 
void setup(void){
  //Entradas y Salidas
  pinMode(Amarillo, INPUT);//Variable Torreta
  pinMode(Verde, INPUT);//Variable Torreta       
  Serial.begin(115200);
  // Print Message Of I/O Setting Progress
  Serial.println("\nI/O Pins Modes Set .... Done");
 
    // Starting To Connect
    if(WiFi.status() == WL_CONNECTED)
    {
      WiFi.disconnect();
      WiFi.mode(WIFI_OFF);
      delay(50);
    }
 
    WiFi.mode(WIFI_STA);            // To Avoid Broadcasting An SSID
    WiFi.begin("RECEPTOR", "12345678");      // The SSID That We Want To Connect To

    // Printing Message For User That Connetion Is On Process
    Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");
 
    // WiFi Connectivity
    CheckWiFiConnectivity();        // Checking For Connection
 

    Serial.println("!-- Client Device Connected --!");
 
    // Printing IP Address
    Serial.println("Connected To      : " + String(WiFi.SSID()));
    Serial.println("Signal Strenght   : " + String(WiFi.RSSI()) + " dBm");
    Serial.print  ("Server IP Address : ");
    Serial.println(TKDServer);
    Serial.print  ("Server Port Num   : ");
    Serial.println(TKDServerPort);
    // Printing MAC Address
    Serial.print  ("Device MC Address : ");
    Serial.println(String(WiFi.macAddress()));
    // Printing IP Address
    Serial.print  ("Device IP Address : ");
    Serial.println(WiFi.localIP());
    
    // Conecting The Device As A Client
    TKDRequest();
 
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
  //Calculo de tiempocon RTC
  ActualAmllo= digitalRead(Amarillo); 
  ActualVerde = digitalRead(Verde);
  if (AntesAmllo != ActualAmllo) 
  {
    if (ActualAmllo==1)
    {
  TimeActualAmllo = rtc.now();
  TimeONamarillohor = TimeActualAmllo.hour();    
  TimeONamarillomin = TimeActualAmllo.minute();  
  TimeONamarilloseg = TimeActualAmllo.second();  
  TimeOFFamarillohor = 0;    
  TimeOFFamarillomin = 0;  
  TimeOFFamarilloseg = 0; 

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
  dataString += String("TORRETA No.1");
  dataString +=",";//la coma deja divir en columnas
  dataString += String(datedia);//Fecha actual
  dataString +="/";
  dataString += String(datemes);
  dataString +="/";
  dataString += String(dateyear);
  dataString +=","; 
  dataString += String("AMARILLO");//Color activado
  dataString +=",";
  dataString += String(count);//Valor de veces encendidas
  dataString +=","; 
  dataString += String(horON);//Hora de encendido
  dataString +=":";
  dataString += String(minON);
  dataString +=":";
  dataString += String(segON);
  dataString +=","; 
  dataString += String(OFFhor);//Hora de apagado
  dataString +=":";
  dataString += String(OFFmin);
  dataString +=":";
  dataString += String(OFFseg);
  dataString +=","; 
  dataString += String(HorDif);//Duracion de ultimo encendido
  dataString +=":";
  dataString += String(MinDif);
  dataString +=":";
  dataString += String(SegDif);
  dataString +=","; 
  dataString += String(acuhr);//Suma de los tiempos encendido
  dataString +=":";
  dataString += String(acumin);
  dataString +=":";
  dataString += String(acumseg);
  dataString +=",";
  Serial.println(dataString);
  TKDClient.println(dataString);
  TKDClient.flush();
  }
  
  void CheckWiFiConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      for(int i=0; i < 10; i++)
      {
        delay(250);
        Serial.print(".");
      }
      Serial.println("");
    }
  }
 
 void TKDRequest()
  {
    // First Make Sure You Got Disconnected
    TKDClient.stop();
 
    // If Sucessfully Connected Send Connection Message
    if(TKDClient.connect(TKDServer, TKDServerPort))
    {
      Serial.println("TORRETA 1 CONNECTED");
    }
  }
