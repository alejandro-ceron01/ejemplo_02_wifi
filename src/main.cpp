
#include<Arduino.h>
#include <WiFi.h>

const char * ssid = "Claro_5G_004259";
const char * pass = "C7M3S2Q54K6";
const char * host = "dweet.io";
const int port = 80;
/**
 * Funcon que inicializa el dispositivo
 * puerto serial(velocidad)
 * direccion de piner entrada y salida
 * se puede crear objetos que representan sensores, dispositivos wifi y objetos de librerias externas
*/

void setup() {
  Serial.begin(115200);//configuramos a 115200 bits por segundo
  WiFi.begin(ssid,pass);//inicializamos el modulo de wifi y que se vincula a la red indicada en el ssid
  Serial.print("Intentando coneccion!");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Conectado! ");
  Serial.println("Direccion IP: ");
  Serial.print(WiFi.localIP());

  pinMode(2,OUTPUT);//configuramos el pin 2 como salida
  digitalWrite(2,HIGH);//Encendemos el LED
}

void loop() {

  WiFiClient cliente;


  if(!cliente.connect(host,port)){
    Serial.println("conexion fallida al servidor");
    delay(1000);
    return;
    //si pasa el if es porque si hubo conexion cone el server
    //ahora tenemos un flujo de transmision y otro de recepcion

  }

  int humedad = random(100);
  int temperatura = random(100);

  String datoDwet = "temperatura=" + String(temperatura) + "&" + "humedad=" + String(humedad);

  String dweetURl = "/dweet/for/Alejandro-ceron?" + datoDwet;
  

  String request =String("GET ") + dweetURl + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";

  Serial.print(request);
  

  //Enviamos la peticion en protocolo http al server 
  cliente.print(request);

  unsigned long inicio = millis();
  while (cliente.available()==0)
  {
    if (millis() - inicio > 5000)
    {
      Serial.println("Tiempo de esper del servidor agotado");
      cliente.stop();
      return;
    }
    
  }


  //si se llega a este punto es porque se recibieron datos del server
  //(cliente.available()!=0)
  
while (cliente.available())
{
  String linea = cliente.readStringUntil('\r'); //lea una linea hasta que se encuentre el caracter de enter
  Serial.println(linea);
}

Serial.println("fin de conexion al servidor");
cliente.stop();
delay(5000);//para evitar de que hagan muchas peticiones al servidor y nos metan en un blacklist


  delay(500);//retardo de 500ms
  digitalWrite(2,LOW);//Apagamos el LED
  delay(500);
  digitalWrite(2,HIGH);//Encender el LED
  
}
