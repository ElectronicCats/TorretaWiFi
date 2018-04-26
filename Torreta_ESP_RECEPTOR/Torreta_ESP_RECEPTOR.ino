/* 
 * Electronic Cats 
 * Dec 2017 
 */ 
#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h> 
#include <ESP8266mDNS.h> 
 
/////////////RTC////////////// 
 
 
/////////////WEB SERVER ////////////// 
ESP8266WebServer server(80); 
 
//Pass para conectarme al servidor  
//const char WiFiAPPSK[] = "12345678"; 
 
//////////// CONFIGURACIÓN PARA ACCES POINT////// 
 // Authentication Variables 
  char*       TKDssid;              // SERVER WIFI NAME 
  char*       TKDpassword;          // SERVER PASSWORD 
#define  LED0 2 
#define MAXSC 6 // Número máximo de clientes que se pueden conectar 
 
WiFiServer TKDServer(80);//Puerto de comunicación 
WiFiClient TKDClient[MAXSC];//(Dispositivos conectados al server) 
 
///////////////////////////////////////////////// 


/*FUNCIONES*/ 
void handleRoot(); 
void handleNotFound(); 
 
void setup(void){ 
 /* //Entradas y Salidas 
  pinMode(Amarillo, INPUT); 
  pinMode(Verde, INPUT); */          
  Serial.begin(9600); 
  //--------RECEPTOR---Mensaje de conexión-------// 
  Serial.println(); 
  Serial.println("I/O Pins Modes Set....Done"); 
 
  //Configuración para conectar a Access Piont 
  //SetWifi("RECEPTOR", "12345678"); 
   
    //HTML 
  server.on("/", handleRoot); 
  server.on("/inline", []() 
    { 
      server.send(200, "text/plain", "this works as well"); 
    }); 
  server.onNotFound(handleNotFound); 
   SetWifi; 
  server.begin(); 
  Serial.println(F("HTTP server started")); 
     
  //ESP8266 
  if (MDNS.begin("esp8266"))  
    { 
      Serial.println(F("MDNS responder started")); 
    } 
  
} 
 
void loop(void){ 
  Serial.println("CLEARDATA"); //CLEAR DATA EN EXCEL 
  Serial.println("LABEL,Color,Fecha,Contador,ON,OFF,Duracion,Acumulado"); //Etiquetas en el documento excell 
 
  server.handleClient(); 
  //Calculo de tiempocon RTC 
 
  //Funcion para clientes disponibles 
   AvailableClients(); 
  //Ver datos de clientes 
   AvailableMessage();  
} 

/*void setupWiFi() 
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
}*/ 
//HTML 
void handleRoot() { 
  //Serial.println("Conectado a html"); 
  char temp[2500]; 
    snprintf (temp, 2500, 
          "<html>\ 
            <head>\ 
            <meta http-equiv='refresh' content='1'/>\ 
            <title>\Torreta ESP</title>\ 
            <STYLE type='text/css'>\ 
            H1 { text-align: center}\ 
              </STYLE>\ 
            <style>\ 
            body { background-color: #000000; font-family: Arial, Helvetica, Sans-Serif; Color: #FFFFFF; }\ 
            </head>\ 
            <body>\ 
              <h1>TORRETA</h1>\ 
              <style>\ 
              div   { text-align: center; }\ 
              table { margin: auto; }\ 
              </style>\ 
              <table>\ 
              </tr>\ 
              <th>\Memoria utilizada:</th>\ 
              <td>\%02d%</td>\ 
              </tr>\ 
              </table>\ 
              </div>\ 
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
            </body>\ 
            </html>" 
/*         ,porce 
         ,ActualAmllo, datedia, datemes, dateyear, contadorA1, TimeONamarillohor, TimeONamarillomin, TimeONamarilloseg,TimeOFFamarillohor, TimeOFFamarillomin,TimeOFFamarilloseg, TimeAcumAmlloONhor, TimeAcumAmlloONmin, TimeAcumAmlloONseg 
         ,ActualVerde, datedia, datemes, dateyear, contadorV1, TimeONverdehor, TimeONverdemin, TimeONverdeseg, TimeOFFverdehor, TimeOFFverdemin, TimeOFFverdeseg, TimeAcumVerdeONhor, TimeAcumVerdeONmin, TimeAcumVerdeONseg 
       */ ); 
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
 
  void SetWifi() 
  { 
    char* Name; 
    char* Password; 
 
    Name="RECEPTOR"; 
    Password="12345678"; 
     
    // Stop Any Previous WIFI 
    WiFi.disconnect(); 
       
    // Setting The Wifi Mode 
    WiFi.mode(WIFI_STA); 
    Serial.println("WIFI Mode : AccessPoint Station"); 
     
    // Setting The AccessPoint Name & Password 
    TKDssid      = Name; 
    TKDpassword  = Password; 
     
    // Starting The Access Point 
    WiFi.softAP(TKDssid, TKDpassword); 
      Serial.println("WIFI < " + String(TKDssid) + " > ... Started"); 
     
    // Wait For Few Seconds 
    delay(1000); 
     
    // Getting Server IP 
    IPAddress IP = WiFi.softAPIP(); 
     
    // Printing The Server IP Address 
    Serial.print("AccessPoint IP : "); 
    Serial.println(IP); 
 
    // Printing MAC Address 
    Serial.print("AccessPoint MC : "); 
    Serial.println(String(WiFi.softAPmacAddress())); 
 
    // Starting Server 
    TKDServer.begin(); 
    TKDServer.setNoDelay(true); 
    Serial.println("Server Started"); 
  } 

  void AvailableClients() 
  {    
    if (TKDServer.hasClient()) 
    { 
      // Read LED0 Switch To Low If High. 
      if(digitalRead(LED0) == HIGH) digitalWrite(LED0, LOW); 
       
      // Light Up LED1 
     //digitalWrite(LED1, HIGH); 
       
      for(uint8_t i = 0; i < MAXSC; i++) 
      { 
        //find free/disconnected spot 
        if (!TKDClient[i] || !TKDClient[i].connected()) 
        { 
          // Checks If Previously The Client Is Taken 
          if(TKDClient[i]) 
          { 
            TKDClient[i].stop(); 
          } 
 
          // Checks If Clients Connected To The Server 
          if(TKDClient[i] = TKDServer.available()) 
          { 
            Serial.println("New Client: " + String(i)); 
          } 
 
          // Continue Scanning 
          continue; 
        } 
      } 

       //no free/disconnected spot so reject 
      WiFiClient TKDClient = TKDServer.available(); 
      TKDClient.stop(); 
    } 
    else 
    { 
      // This LED Blinks If No Clients Where Available 
      digitalWrite(LED0, HIGH); 
      delay(250); 
      digitalWrite(LED0, LOW); 
      delay(250); 
    } 
  } 
 
//==================================================================================== 
 
  void AvailableMessage() 
  { 
    //check clients for data 
    for(uint8_t i = 0; i < MAXSC; i++) 
    { 
      if (TKDClient[i] && TKDClient[i].connected() && TKDClient[i].available()) 
      { 
          while(TKDClient[i].available()) 
          { 
            String Message = TKDClient[i].readStringUntil('\r'); 
               Serial.print("DATA,"); 
            Serial.println(Message); 
            TKDClient[i].flush(); 
            Serial.println("Client No " + String(i) + " - " + Message); 
          } 
      } 
    } 
  } 
