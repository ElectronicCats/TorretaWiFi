/* 
 * Electronic Cats 
 * Dec 2017 */
#include <ESP8266WiFi.h>  
 
//CONFIGURACIÓN PARA ACCES POINT
  char*       TKDssid;          // SERVER WIFI NAME 
  char*       TKDpassword;      // SERVER PASSWORD 
#define  LED0 14 
#define  LED1 16 
#define MAXSC 6                 // Número máximo de clientes que se pueden conectar 
 
WiFiServer TKDServer(80);//Puerto de comunicación 
WiFiClient TKDClient[MAXSC];//(Dispositivos conectados al server)  

void setup(void){ 
  Serial.begin(9600); 
  //--------RECEPTOR---Mensaje de conexión-------// 
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  Serial.println(); 
  Serial.println("I/O Pins Modes Set....Done"); 
  //Configuración para conectar a Access Piont 
  SetWifi("RECEPTOR", "12345678");
  digitalWrite(LED1, HIGH); 
} 
 
void loop(void){ 
  //Funcion para clientes disponibles 
   AvailableClients(); 
  //Ver datos de clientes 
   AvailableMessage();  
   delay(1000);
} 
 
void SetWifi(char* Name, char* Password) 
  {  
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
