// Definimos constantes para el primer led RGB
#define led1Rojo 3
#define led1Verde 5
#define led1Azul 6

// Almacena el valor de nuestra elección
char MiOpcion;

// Almacena el valor de la elección de Arduino
int respuestaArduino;

void setup(){ // Inicio setup()

// Estableciendo puertos como salida para el primer led RGB
pinMode(led1Rojo, OUTPUT);
pinMode(led1Verde, OUTPUT);
pinMode(led1Azul, OUTPUT);


// Inicia la conexión serial a una velocidad de 9600 bits por segundo
Serial.begin(9600);
//Imprime en pantalla los siguientes mensajes
Serial.println("Arduino te da la bienvenida listo para jugar, empecemos!!");
Serial.println("Introduce 0 para elegir piedra, 1 para papel o 2 para tijera");

}// Fin setup()

void loop(){ // Inicio loop()

verOpcionArduino(100,100,100);
delay(1000);
verOpcionArduino(0,255,0);
delay(1000);
verOpcionArduino(0,0,255);
delay(1000);


} // Fin loop()

void verOpcionArduino(int Rojo, int Verde, int Azul){

analogWrite(led1Rojo, Rojo);
analogWrite(led1Verde, Verde);
analogWrite(led1Azul, Azul);

}
