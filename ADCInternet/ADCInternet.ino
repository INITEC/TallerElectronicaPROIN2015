//--------------------------------------------WAPOSAT----------------------------------------------------
#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>         //Include the software serial library 
#include <stdio.h>
#include <math.h>

int analogPin0=0;   // entrada del voltaje de sensor
int analogPin1=1;   // entrada del voltaje de sensor
int analogPin2=2;   // entrada del voltaje de sensor

float v_out0;             //voltage output from temp sensor 
float v_out1;             //voltage output from temp sensor 
float v_out2;             //voltage output from temp sensor

//----------------------------configuracion shell ethernet----------------------------------------------------------
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 172, 16, 13, 179 };
byte gateway[] = { 172, 16, 13, 254 };
byte server[] = { 45, 55, 150, 245 }; // IP Publico del servidor WAPOSAT
//byte server[] = { 172, 16, 13, 1 }; // IPServidor LAN


boolean lastConnected = false;                 // state of the connection last time through the main loop
EthernetClient client;
//----------------------------------------------------------------------------------------------------


void setup()
{
//------------------------entrada analogica y alimentacion 5v--------------------------------------------------------------
  pinMode(analogPin0, INPUT);    // configurando al puesto A0 como entrada analogica
  pinMode(analogPin1, INPUT);    // configurando al puesto A0 como entrada analogica
  pinMode(analogPin2, INPUT);    // configurando al puesto A0 como entrada analogica

//-----------------configuracion de serial pc---------------------------------------------------------  
  Ethernet.begin(mac, ip, dns, gateway); // inicializa ethernet shield
  Serial.begin(9600);
//-----------------configuracion de serial sensor-----------------------------------------------------
  delay(1000); // espera 1 segundo despues de inicializar
}

void loop()
{
        
//-----------------------conectando al servidor--------------------------------------------------------    
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  if(!client.connected()) {  
    httpRequest();
  }
  
  lastConnected = client.connected();

}

//---------------funcion de sensor de temperatura-------------------------------------------------------------------
void read_port(){
v_out0 = analogRead(0);   //read the input pin
delay(10);
v_out1 = analogRead(1);   //read the input pin
delay(10);
v_out2 = analogRead(2);   //read the input pin
delay(10);
}

//---------------------------------------------------------------------------------------------------------------------

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    read_port();
    // send the HTTP PUT request:
    client.print("GET /index/Template/InsertData3.php?equipo=4&sensor1=1&sensor2=2&sensor3=3&valor1="); // Envia los datos utilizando GET
    client.print(v_out0);
    client.print("&valor2=");
    client.print(v_out1);
    client.print("&valor3=");
    client.print(v_out2);
    client.println(" HTTP/1.0");
    client.println();
        
    delay(1000);
  } 
  else {

    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}

