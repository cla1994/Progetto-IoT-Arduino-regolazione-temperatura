#include <SPI.h>
#include <WiFiNINA.h>
#include “arduino_secrets.h”             // il file è lo stesso di quello usato con Client

char ssid[] = "tuo network";
char pass[] = "tua password";
int status = WL_IDLE_STATUS;
const int port = 8181;

WiFiServer server(port);

void setup() {

  pinMode(13, OUTPUT);              // pin dove collego circuito ausiliario/led/lampadina ecc.
   
  Serial.begin(9600);
  Serial.println(“Tentativo di connessione al network...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) {
    Serial.println("Connessione non riuscita");
    while(true);
  }
  else {
    server.begin();
  }
}

void loop() {
  
  WiFiClient client = server.available();               // si mette in ascolto per altri Client in arrivo
  
  while(client.connected()) {
    if (client.available()) {
       uint8_t buf;               // informazione in un byte
       size_t length = 1;
       client.read(&buf, length);           // legge l’informazione mandata dall’altro Arduino
       azionaAttuatore(buf);                  // funzione che prende buf come argomento, agisce sul PIN 13
    }                                                          // riportata qua sotto
  }
}

char *azionaAttuatore(uint8_t cmd) {
  Serial.print(cmd);

  switch(cmd) {
    case 49:                          //valore ASCII di 1, accende luce/termostato
      digitalWrite(13, HIGH);
      return "> acceso \n";
    case 48:                         //valore ASCII per 0, spegne luce/termostato
      digitalWrite(13, LOW);
      return "> spento \n";

    default:
      return "> con 1 è acceso, con 0 è spento \n";  
  }
}
