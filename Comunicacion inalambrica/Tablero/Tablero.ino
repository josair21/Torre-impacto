#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#define CE_PIN 9
#define CSN_PIN 10
bool transmitir_senal = 0;
byte direccion_escritura[5] ={'w','r','i','t','e'};
byte direccion_lectura[5] ={'r','e','a','d'};
RF24 radio(CE_PIN, CSN_PIN);
unsigned long comando = 6088, verificacion = 1997, last_time = millis(), last_time_echo = millis();
void setup(){
  Serial.begin(9600);
  printf_begin();
  radio.begin();
 
  pinMode(2, INPUT_PULLUP);
  pinMode(7, OUTPUT);
  digitalWrite(7, 0);
  transmitir_senal = 0;
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setPayloadSize(8);
   radio.printDetails();
  radio.openWritingPipe(direccion_escritura);
  radio.openReadingPipe(1, direccion_lectura);
  radio.startListening();
  
}
 
void loop(){
  unsigned long datos = 999;
  if(millis() - last_time > 800 & digitalRead(2) == 0){
    transmitir_senal = 1;
    last_time = millis();
    last_time_echo = millis();
  }
  if(transmitir_senal == 1){
    radio.stopListening();
    radio.write(&comando, sizeof(comando));
    Serial.print("ENVIADO: ");
    Serial.println(comando);
    transmitir_senal = 0;
    radio.startListening();
    //delay(5);
  }
  else{
    if (radio.available()){
      radio.read(&datos,sizeof(datos));
      Serial.print("Salida del electroiman en: ");
      Serial.println(datos);
      if(datos == 1) digitalWrite(7, 1);
      else if(datos == 0) digitalWrite(7, 0);
      //delay(5);
    }
  }
}
