
//http://www.elechouse.com/elechouse/images/product/13.56MHZ_RFID_Module/RFID%20with%20Arduino.pdf
//http://www.elechouse.com/elechouse/images/product/13.56MHZ_RFID_Module/13.56MHZ_RFID_Manual.pdf


#include <SoftwareSerial.h> 

const int ledPin =  13;
int cont=0;

SoftwareSerial mySerial(2, 3); //pin2 Rx, pin3 Tx 

int CMD[64];
int comlen =0;
int out_flag =0;

void setup() 
{ 
  Serial.begin(9600);
  mySerial.listen();
  Serial.println("Serial number will be displayed here if a card is detected by the module:\n"); 
  // set the data rate for the SoftwareSerial port 
  mySerial.begin(9600); 
  delay(10);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  mySerial.write(0x02); //Send the command to RFID, please refer to RFID manual 
  delay(20);
} 
void loop() // run over and over 
{ 

      while (Serial.available()) 
      {
          int a = SerialReadHexDigit();
          //Serial.println(a);
          if(a>=0){
          CMD[comlen] = a;
          comlen++;
          }
          delay(10);
      }
      
      for(int i=0; i<comlen; i+=2){
          int c = mySerial.write( CMD[i]*16 + CMD[i+1]);
        }
      comlen =0;
      
      while (mySerial.available()) {
      byte C = mySerial.read();
      if (C<16) Serial.print("0");
      Serial.print(C ,HEX); //Display the Serial Number in HEX 
      Serial.print(" ");
      
      out_flag =1;
      }
     if (out_flag >0) {
       Serial.println();
       toogleLED();
       delay(20);
       out_flag = 0;
     }
     
     
}



int SerialReadHexDigit()
{
    byte c = (byte) Serial.read();
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else {
        return -1;   // getting here is bad: it means the character was invalid
    }
}

void toogleLED(){
  if (cont==0){
    digitalWrite(ledPin, HIGH);
    cont=1;
  } else {
    digitalWrite(ledPin, LOW);
    cont=0;
  }
}
