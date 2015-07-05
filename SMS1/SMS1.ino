#include <SoftwareSerial.h>  //Esta libreria se requiere para poder emular otro puerto serial por software desde Arduino, 
                             //ya que Arduino solo incluye un solo puerto serial por hardware.
 
SoftwareSerial GPRS1(7, 8);  // Se indica que pines se utilizaran para el puerto serial por software.
char ch1=13;
char ch2=26;
String str;
int num=0;
unsigned char buffer[500]; // Buffer donde se almacenara el mensaje recibido
unsigned char telefono[10]; //Numero telefonico de origen
unsigned char imensaje[10]; //mensaje de entrada
unsigned char omensaje1[200];
int paso=0;
int puedo_enviar, resetear;
int count=0;     // Contador para saber la cantidad de bytes del buffer
int n1,n2,n3;
int j=0;
int k=0;
int led = 13;
char ch01,ch02,ch03;
String inputString = "";

void softReset()  //Reseteo de Arduino y GPRS Shield por Software
                  //Esto es util para cuando el shield del GPRS no responde a los comandos AT que envia Arduino
{
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  Serial.write("Reseteando Arduino");Serial.write(char(10));Serial.write(char(13));
  delay(2000);
  asm volatile ("  jmp 0");  //Reset
}


void setup()
{
  count=0;
  n1=0;
  n2=0;
  n3=0;
  pinMode(9, OUTPUT); 
  pinMode(led, OUTPUT); 
  //Se utilizaran dos puertos Seriales, GPRS es para comunicar con el GPRS Shield
  //Serial es para podernos comunicar con Arduino desde el Monitor Serial de la PC
  GPRS1.begin(19200);               // Velocidad de comunicacion Serial del GPRS Shield
  Serial.begin(19200);             // Velocidad de comunicacion Serial del puerto de Arduino
  
  //Inicilizacion del GPRS Shield
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  delay(1000);
  //Fin de la rutina de inicializacion del GPRS Shield

  GPRS1.println("AT+CMGF=1");         // Inicializa el modo lectura.
  delay(100); //Un retardo de un segundo para esperar a que el shield GPRS procese el comando.
  num=0;
  n1=0; 
}
 
void loop() //Ciclo infinito
{

    switch (n1) {
     case 1:
       resetear=0;
       GPRS1.println("AT+CMGR=1"); //Comando para Leer el mensaje 1 del GPRS Shield
       break;
     case 2:
       GPRS1.println("AT+CMGR=2"); //Comando para Leer el mensaje 2 del GPRS Shield
       break;
     case 3:
       GPRS1.println("AT+CMGR=3"); //Comando para Leer el mensaje 3 del GPRS Shield
       break;
     case 4:
       GPRS1.println("AT+CMGR=4"); //Comando para Leer el mensaje 4 del GPRS Shield
       break;
     case 5:
       GPRS1.println("AT+CMGR=5"); //Comando para Leer el mensaje 5 del GPRS Shield
       break;
     case 6:
       GPRS1.println("AT+CMGR=6"); //Comando para Leer el mensaje 6 del GPRS Shield
       break;
     case 7:
       GPRS1.println("AT+CMGR=7"); //Comando para Leer el mensaje 7 del GPRS Shield
       break;
     case 8:
       GPRS1.println("AT+CMGR=8"); //Comando para Leer el mensaje 8 del GPRS Shield
       break;
     case 9:
       GPRS1.println("AT+CMGR=9"); //Comando para Leer el mensaje 9 del GPRS Shield
       break;
     case 10:
       GPRS1.println("AT+CMGR=10"); //Comando para Leer el mensaje 10 del GPRS Shield
       n1=0;
       break;
  }
  delay(100);  
  n1++;
   
  if (GPRS1.available())              // ¿Hay datos de entrada en el puerto softwareserial?, 
  {                                  // Entoces hay que leer los datos provenientes del GPRS shield
    resetear=1;
    paso=0; 
    while(GPRS1.available())          // Lee los datos de Softserial 
     {
      
      buffer[count++]=GPRS1.read();     // Se escriben los datos al bufer
     
     }
    Serial.write(buffer,count);  //Se envia datos recibidos al Monitor Serial de la PC para observar lo que esta pasando.
    
    clearBufferArray();              // Se llama la subrutina para limpiar el buffer "clearBufferArray"
    count = 0;                       // se coloca el contador de bytes a cero
    paso=0;
    
  }
  if (Serial.available())     // si hay datos en el puerto serial del Arduino, entonces hay datos provenientos de la PC
    {
      paso=0; 
      GPRS1.write(Serial.read());       // Se escriben los datos provenientes de la PC al  GPRS shield 
    }
  
  delay(5000);  
}

void clearBufferArray()              // Funcion para limpiar el buffer
{
  for (int i=0; i<254;i++)
    { buffer[i]=NULL;}                  // Se limpia el buffer
}
