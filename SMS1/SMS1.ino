#include <SoftwareSerial.h>  //Esta libreria se requiere para poder emular otro puerto serial por software desde Arduino, 
                             //ya que Arduino solo incluye un solo puerto serial por hardware.
 
SoftwareSerial GPRS(7, 8);  // Se indica que pines se utilizaran para el puerto serial por software.
char ch1=13;
char ch2=26;
String str;
int num=0;
unsigned char buffer[254]; // Buffer donde se almacenara el mensaje recibido
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
  GPRS.begin(19200);               // Velocidad de comunicacion Serial del GPRS Shield
  Serial.begin(19200);             // Velocidad de comunicacion Serial del puerto de Arduino
  
  //Inicilizacion del GPRS Shield
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  delay(1000);
  //Fin de la rutina de inicializacion del GPRS Shield

  GPRS.write("AT+CMGF=1");         // Inicializa el modo lectura.
  GPRS.write(char(13));            // Se envia un ENTER para que el comando sea procesado por GPRS Shield
  delay(1000); //Un retardo de un segundo para esperar a que el shield GPRS procese el comando.
  num=0;
 
}
 
void loop() //Ciclo infinito
{
  
  paso++;
  if (paso>35) { 
    n1++; 
    n2=n1;
    switch (n1) {
     case 1:
       resetear=0;
       GPRS.write("AT+CMGR=1"); //Comando para Leer el mensaje 1 del GPRS Shield
       break;
     case 2:
       GPRS.write("AT+CMGR=2"); //Comando para Leer el mensaje 2 del GPRS Shield
       break;
     case 3:
       GPRS.write("AT+CMGR=3"); //Comando para Leer el mensaje 3 del GPRS Shield
       break;
     case 4:
       GPRS.write("AT+CMGR=4"); //Comando para Leer el mensaje 4 del GPRS Shield
       break;
     case 5:
       GPRS.write("AT+CMGR=5"); //Comando para Leer el mensaje 5 del GPRS Shield
       break;
     case 6:
       GPRS.write("AT+CMGR=6"); //Comando para Leer el mensaje 6 del GPRS Shield
       break;
     case 7:
       GPRS.write("AT+CMGR=7"); //Comando para Leer el mensaje 7 del GPRS Shield
       break;
     case 8:
       GPRS.write("AT+CMGR=8"); //Comando para Leer el mensaje 8 del GPRS Shield
       break;
     case 9:
       GPRS.write("AT+CMGR=9"); //Comando para Leer el mensaje 9 del GPRS Shield
       break;
     case 10:
       GPRS.write("AT+CMGR=10"); //Comando para Leer el mensaje 10 del GPRS Shield
       n1=0;
       if (resetear=0){softReset();}
       break;

    } 
    GPRS.write(char(13));
    delay(500);
    paso=0;
    
  }
   else
    {
       //No hacer nada
    }
  if (GPRS.available())              // ¿Hay datos de entrada en el puerto softwareserial?, 
  {                                  // Entoces hay que leer los datos provenientes del GPRS shield
    resetear=1;
    paso=0; 
    while(GPRS.available())          // Lee los datos de Softserial 
     {
      
      buffer[count++]=GPRS.read();     // Se escriben los datos al bufer
     
     }
        
    //Para Monitorear lo que llega al buffer, quitar el comentario de las siguientes lineas.
    /*
    Serial.write(char(13));
    Serial.write("Buffer=->");
    for (int i=0; i<count;i++)
      {
        Serial.write(buffer[i]);
      }
    Serial.write("<-");
    Serial.write(char(13));
    Serial.write("Buffer[27]=");
    Serial.write(buffer[27]);
    Serial.write(char(13));
    Serial.write("Buffer[26]=");
    Serial.write(buffer[26]);
    Serial.write(char(13));
   */
   //Fin del monitoreo de lo que llega al buffer
   
    if ((buffer[3]=='C') || (buffer[26]=='D') || (buffer[27]=='D'))  //¿Se encuentra en buffer el texto REC READ?
    {
      j=0;
      for (int i=31; i<42;i++)
      { 
        telefono[j]=buffer[i]; //Esta linea obtiene el numero de telefono que envio el SMS
        j++;
      }      
      k=67;
      for (int i=82; i<count;i++)
      { 
        if (buffer[i]=='"') {k=i;} //Esta linea es para determinar la posicion de donde inicia el mensaje de texto recibido.
      }
      j=0;
      k=k+3;
      for (int i=k; i<=(k+10);i++)
      { 
        imensaje[j]=buffer[i]; //Se obtiene el texto (ON/OFF) recibido.
        j++;
      }
      if (true)  //Este es para validar el numero de telefono origen, en este caso valida que el segundo numero que conforma
                               //el numero de telefono que envia el mensaje sea el 4 (X4XXXXXXXX)
      {      
      Serial.write("Telefono:="); //Se muestra el numero de telefono desde donde se realizo el mensaje
      Serial.write(telefono,10);
      Serial.write(", Mensaje:="); //Se muestra el mensaje recibido
      Serial.write(imensaje,3);
      Serial.write(char(10));  //  Salto de linea
      Serial.write(char(13));      
      if ((imensaje[0]=='O') && (imensaje[1]=='N')) { 
        digitalWrite(led, HIGH);  //Enciende LED
         }
      if ((imensaje[0]=='O') && (imensaje[1]=='F') && (imensaje[2]=='F')) { 
        digitalWrite(led, LOW);  //Apaga LED
         }

      switch (n2) {
       case 1:
         GPRS.write("AT+CMGD=1"); //Comando para Eliminar el mensaje 1 del GPRS Shield
         break;
       case 2:
         GPRS.write("AT+CMGD=2"); //Comando para Eliminar el mensaje 2 del GPRS Shield
         break;
       case 3:
         GPRS.write("AT+CMGD=3"); //Comando para Eliminar el mensaje 3 del GPRS Shield
         break;
       case 4:
         GPRS.write("AT+CMGD=4"); //Comando para Eliminar el mensaje 4 del GPRS Shield
         break;
       case 5:
         GPRS.write("AT+CMGD=5"); //Comando para Eliminar el mensaje 5 del GPRS Shield
         break;
       case 6:
         GPRS.write("AT+CMGD=6"); //Comando para Eliminar el mensaje 6 del GPRS Shield
         break;
       case 7:
         GPRS.write("AT+CMGD=7"); //Comando para Eliminar el mensaje 7 del GPRS Shield
         break;
       case 8:
         GPRS.write("AT+CMGD=8"); //Comando para Eliminar el mensaje 8 del GPRS Shield
         break;
       case 9:
         GPRS.write("AT+CMGD=9"); //Comando para Eliminar el mensaje 9 del GPRS Shield
         break;
       case 10:
         GPRS.write("AT+CMGD=10"); //Comando para Eliminar el mensaje 10 del GPRS Shield
         break;
      }
      GPRS.write(char(13));

      
      paso=0;
      delay(150);
      /* //Esta subrutina es para enviar una respuesta desde el GPRS Shield al Telefono Celular desde donde se envio el mensaje         
         //Se tiene en comentarios para gastar menos tiempo aire ya que cada mensaje enviado desde el GPRS Shield 
         //Causa un costo con nuestra compañia de telefono.
      GPRS.write("AT+CMGS=");
      GPRS.write(char(34));
      GPRS.write(telefono,10);
      GPRS.write(char(34));
      GPRS.write(char(13));
      GPRS.write("Hola, el comando <");
      GPRS.write(imensaje,6);
      GPRS.write("> se proceso OK");
      GPRS.write(char(26));
      GPRS.write(char(13));
      */
      telefono[0]='0'; //Se limpia el numero de telefono
      telefono[1]='0';
      paso=0;
      }
       else
         {
           Serial.write(buffer,count);  //Se envia datos recibidos al Monitor Serial de la PC para observar lo que esta pasando.
         }
      
    }
    
    
    clearBufferArray();              // Se llama la subrutina para limpiar el buffer "clearBufferArray"
    count = 0;                       // se coloca el contador de bytes a cero
    paso=0;
    
  }
  if (Serial.available())     // si hay datos en el puerto serial del Arduino, entonces hay datos provenientos de la PC
    {
      paso=0; 
      GPRS.write(Serial.read());       // Se escriben los datos provenientes de la PC al  GPRS shield 
    }  
}

void clearBufferArray()              // Funcion para limpiar el buffer
{
  for (int i=0; i<254;i++)
    { buffer[i]=NULL;}                  // Se limpia el buffer
}
