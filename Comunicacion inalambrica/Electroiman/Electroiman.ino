#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#define CE_PIN 9
#define CSN_PIN 10
bool transmitir_senal = 0;
byte direccion_escritura[5] ={'w','r','i','t','e'};
byte direccion_lectura[5] ={'r','e','a','d'};
RF24 radio(CE_PIN, CSN_PIN);
unsigned long comando = 1997, datos = 2022, last_time = millis();
void setup(){
  Serial.begin(9600);
  printf_begin();
  radio.begin();
  pinMode(2, INPUT_PULLUP);
  pinMode(7, OUTPUT);
  digitalWrite(7, 0);
  attachInterrupt(digitalPinToInterrupt(2), interrupcion, FALLING);
  transmitir_senal = 0;
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setPayloadSize(8);
  radio.printDetails();
  radio.openWritingPipe(direccion_lectura);
  radio.openReadingPipe(1, direccion_escritura);
  radio.startListening();
  
}
 
void loop(){
  if(millis() - last_time > 200){
    radio.stopListening();
    bool x = digitalRead(7);
    radio.write(&x, sizeof(x));
    Serial.print("Salida en: ");
    Serial.println(x);
    transmitir_senal = 0;
    radio.startListening();
    last_time = millis();
    //delay(5);
  }
  else{
    if (radio.available()){
      radio.read(&datos,sizeof(datos));
      //delay(1000);
      Serial.print("RECIBIDO: ");
      Serial.println(datos);
      if(datos == 6088){
        digitalWrite(7, !digitalRead(7));
      }
      //if(datos == 1997)
      transmitir_senal = 1;
      //delay(5);
    }
  }
}
void interrupcion(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 2000) {
    transmitir_senal = 1;
  }
  last_interrupt_time = interrupt_time;
}
