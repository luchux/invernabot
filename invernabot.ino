// Ejemplo de muestra de sensores de humedad y temperatura, luz y servos. 
// Written by Luchux

/********************************************************************/
// Incluímos las librerías 
#include "DHT.h"
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/

/********************************************************************/
// Definimos los pines del Arduino para cada sensor/emisor

#define TIPO_SENSOR_THA DHT22   // DHT 22  (AM2302), AM2321
#define SENSOR_THA 12     // Definimos el pin para el sensor DHT22 Temperatura-humedad Aire
#define SENSOR_TIERRA 2
#define SENSOR_LUZ 0

#define SONAR_DISPARADOR 8
#define SONAR_RECEPTOR 9
/********************************************************************/

/********************************************************************/
// Inizializamos el sensor de Temperatura y Humedad  Aire.
DHT dht(SENSOR_THA, TIPO_SENSOR_THA);

// Inizializamos el sensor de tierra
OneWire oneWire(SENSOR_TIERRA); 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 

float distancia;
float humedad;
float temperatura;
int luz;
long tiempo;

void setup() {
  Serial.begin(9600);
  
  //Definimos los pines de tipo entrada o salida para el Sonar
  pinMode(SONAR_DISPARADOR, OUTPUT);
  pinMode(SONAR_RECEPTOR, INPUT);
  
  // Iniciamos el Sensor DHT22 de humedad y temperatura del aire
  dht.begin();
  
  // Iniciamos el Sensor de temperatura subterraneo
  sensors.begin(); 
}

void loop() {
  // Esperamos un poquito antes de la mediciones
  Serial.println("=========================================");
  Serial.println("Nueva medición de datos...");
  delay(5000);
  

  //Leemos la humedad
  humedad = dht.readHumidity();
  
  //leemos la temperatura
  temperatura = dht.readTemperature();
  
  //leemos la luz
  luz=analogRead(SENSOR_LUZ);
  
  //Mostramos valores en la consola
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %\t");
    
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" *C ");

  //Temperatura subterranea
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  Serial.print("Temperatura Suelo: "); 
  Serial.println(sensors.getTempCByIndex(0));
  Serial.println();

  int nivelDeLuz;
  int nivelDeLuzAjustado;
  nivelDeLuz = analogRead(0);
  nivelDeLuzAjustado = map(nivelDeLuz, 990, 1030, 0, 100); //el sensor con esta resistencia va entre 990, y 1030.

  Serial.print("Luz: ");
  Serial.print(nivelDeLuzAjustado);
  Serial.println(" [0-100]");
  
  /********************************************************************/



  // SENSOR de DISTANCIA
  // lanzamos un pequeño pulso para activar el sensor
  digitalWrite(SONAR_DISPARADOR, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_DISPARADOR, LOW);
  
  // medimos el pulso de respuesta
  tiempo = (pulseIn(SONAR_RECEPTOR, HIGH)/2); // dividido por 2 por que es el 
                                       // tiempo que el sonido tarda
                                       // en ir y en volver
  // ahora calcularemos la distancia en cm
  // sabiendo que el espacio es igual a la velocidad por el tiempo
  // y que la velocidad del sonido es de 343m/s y que el tiempo lo 
  // tenemos en millonesimas de segundo
  distancia = float(tiempo * 0.0343);
  
  // y lo mostramos por el puerto serie una vez por segundo
  Serial.print("Distancia: ");
  Serial.println(distancia);
  Serial.println();
  delay(1000);
}
