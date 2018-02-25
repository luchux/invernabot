// Inverna Bot. An Arduino based greenhouse controller for sensing and controlled environment.
// Written by Luchux

/********************************************************************/
// libraries
#include "DHT.h"
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/

/********************************************************************/
// defining some pins

#define TIPO_SENSOR_THA DHT22   // DHT22  (AM2302), AM2321
#define SENSOR_THA 12     // pin for DHT22 Temperatura-humedad Aire
#define SENSOR_TIERRA 2   // pin for ground sensor
#define SENSOR_LUZ 0      // fotovoltaic sensor

#define SONAR_DISPARADOR 8 //sonar (measuring plants growing height)
#define SONAR_RECEPTOR 9
/********************************************************************/

/********************************************************************/
// Initializing DHT sensor
DHT dht(SENSOR_THA, TIPO_SENSOR_THA);

// Intializing ground sensor
OneWire oneWire(SENSOR_TIERRA); 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 

float distancia;
float humedad;
float temperatura;
int luz;
long tiempo;

class LuzSensor {
  int pin;
  int minRead; 
  int maxRead; 
};

void setup() {
  Serial.begin(9600);
  
  //pinModes for sonar
  pinMode(SONAR_DISPARADOR, OUTPUT);
  pinMode(SONAR_RECEPTOR, INPUT);
  
  // start dht sensor
  dht.begin();
  
  // start ground sensor
  sensors.begin(); 
}

void loop() {
  // showing in serial 
  Serial.println("=========================================");
  Serial.println("Nueva medición de datos...");
  delay(1000);
  
  //reading humidity
  humedad = dht.readHumidity();
  
  //reading temperature
  temperatura = dht.readTemperature();
   
  //showing values
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %\t");
    
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" *C ");

  //Reading underground temperature
  sensors.requestTemperatures(); // leyendo la temperatura del sensor
  Serial.print("Temperatura Suelo: "); 
  Serial.println(sensors.getTempCByIndex(0));
  Serial.println();

  //reading light
  int nivelDeLuz;
  int nivelDeLuzAjustado;

  nivelDeLuz=analogRead(SENSOR_LUZ);
  nivelDeLuzAjustado = map(nivelDeLuz, 990, 1030, 0, 100); //el sensor con esta resistencia va entre 990, y 1030.
  
  //showing values
  Serial.print("Luz: ");
  Serial.print(nivelDeLuzAjustado);
  Serial.println(" [0-100]");
  
  /********************************************************************/
  // Sonar activity
  
  // sending pulse de 10useg
  digitalWrite(SONAR_DISPARADOR, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_DISPARADOR, LOW);
  
  // reading pulse (divided by 2, distance goes + back)
  tiempo = (pulseIn(SONAR_RECEPTOR, HIGH)/2); 

  //transforming time into distance by sound speed
  distancia = float(tiempo * 0.0343);
  
  //showing
  Serial.print("Distancia: ");
  Serial.println(distancia);
  Serial.println();
  delay(1000);
}
