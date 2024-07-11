#include <WiFiNINA.h>
#include <SPI.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;            // dati personali, passati al programma con l’include del file
char pass[] = SECRET_PASS;         // arduino_secrets.h
int status = WL_IDLE_STATUS;        // status del WiFi

char servername[]= "server";

int ThermistorPin = A0;           // valori utili alla conversione del valore restituito dal sensore di 
int Vo;                                       //  temperatura analogico nel valore in gradi celsius
float R1 = 1700;                       // valore di R1 sulla scheda 
float logR2, R2, T;
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;      // coefficienti steinhart-hart per il 
                                                                                                                             // termistore

int C = 0;       // flag per sapere se termostato è acceso o spento, parte spento

const int port = 8181;       //  port per l’interlacciamento Client-Server

WiFiClient client;

void setup() {

  Serial.begin(9600);
  Serial.println("Sto tentando di connettermi al network WPA...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) {
    Serial.println("Non sono riuscito a collegarmi al WiFi");
    while(true);          // non fare nient’ altro
  }
  else {
    Serial.println("Connesso al WiFi");
    Serial.println("\Connessione in corso...");               
    if (client.connect(servername, port)) {                 // se la connessione va a buon fine…
      Serial.println("Connesso");
    }
  }

}

void loop() {

  uint8_t buf;

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);          // calcola resistenza sul termistore
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));                   // temperatura in Kelvin
  T = T - 273.15;                       //conversione da Kelvin in Celsius
 

  Serial.print("Temperatura: "); 
  Serial.print(T);
  Serial.println(" C"); 

  delay(500);



  if (T < 20 && C == 0) {
        buf = 1;
        client.write(buf);          // manda buffer di 1 byte per segnalare di far partire termostato
        C = 1;               // ora termostato è acceso
  }
  if (T > 20 && C == 1){
        buf = 0;
        client.write(buf);                // manda buffer di 1 byte per segnalare di spegnere termostato
        C = 0;            // ora termostato è spento
  } 

}
