#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 172, 16, 13, 177 };
byte gateway[] = { 172, 16, 13, 254 };
EthernetServer server(80);
// Definimos constantes para el primer led RGB
#define ledRojo 3
#define ledVerde 5
#define ledAzul 6

String estado="OFF"; //Estado del Led inicialmente "OFF"

void setup(){ // Inicio setup()
  Ethernet.begin(mac, ip, dns, gateway);
  server.begin();
  
  Serial.begin(9600);
  delay(1000);
  // Estableciendo puertos como salida para el primer led RGB
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
}// Fin setup()



void loop(){ // Inicio loop()

  EthernetClient client = server.available(); //Creamos un cliente Web
  if (client) {
    boolean currentLineIsBlank = true; //Una petición HTTP acaba con una línea en blanco
    String cadena=""; //Creamos una cadena de caracteres vacía  
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();//Leemos la petición HTTP carácter por carácter
        Serial.write(c);//Visualizamos la petición HTTP por el Monitor Serial
        cadena.concat(c);//Unimos el String 'cadena' con la petición HTTP (c). De esta manera convertimos la petición HTTP a un String
 
        //Ya que hemos convertido la petición HTTP a una cadena de caracteres, ahora podremos buscar partes del texto.
        int posicion=cadena.indexOf("LED="); //Guardamos la posición de la instancia "LED=" a la variable 'posicion'
 
        if(cadena.substring(posicion)=="LED=ON"){//Si a la posición 'posicion' hay "LED=ON"
          verOpcionArduino(255,0,0);
          estado="ON";
        }
        if(cadena.substring(posicion)=="LED=OFF"){//Si a la posición 'posicion' hay "LED=OFF"
          verOpcionArduino(0,255,0);
          estado="OFF";
        }
        //Cuando reciba una línea en blanco, quiere decir que la petición HTTP ha acabado y el servidor Web está listo para enviar una respuesta
        if (c == '\n' && currentLineIsBlank) {
 
            // Enviamos al cliente una respuesta HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
 
            //Página web en formato HTML
            client.println("<html>");
            client.println("<head>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1 align='center'>DIYMakers</h1><h3 align='center'>LED controlado por Servidor Web con Arduino</h3>");
            //Creamos los botones. Para enviar parametres a través de HTML se utiliza el metodo URL encode. Los parámetros se envian a través del símbolo '?'
            client.println("<div style='text-align:center;'>");
            client.println("<button onClick=location.href='./?LED=ON\' style='margin:auto;background-color: #84B1FF;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:65px;'>");
            client.println("ON");
            client.println("</button>");
            client.println("<button onClick=location.href='./?LED=OFF\' style='margin:auto;background-color: #84B1FF;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:65px;'>");
            client.println("OFF");
            client.println("</button>");
            client.println("<br /><br />");
            client.println("<b>LED = ");
            client.print(estado);
            client.println("</b><br />");
            client.println("</b></body>");
            client.println("</html>");
            break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }  
      }
    }
    //Dar tiempo al navegador para recibir los datos
    delay(1);
    client.stop();// Cierra la conexión
  }
} // Fin loop()

void verOpcionArduino(int Rojo, int Verde, int Azul){

analogWrite(ledRojo, Rojo);
analogWrite(ledVerde, Verde);
analogWrite(ledAzul, Azul);

}
