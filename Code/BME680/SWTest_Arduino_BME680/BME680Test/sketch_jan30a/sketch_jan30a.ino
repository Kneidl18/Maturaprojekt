#include <SoftwareSerial.h>
SoftwareSerial UART(7,8); //Rx / Tx

long timeStamp;
const long sendInterval = 1000;

void setup() {
  Serial.begin(9600);
  UART.begin(19200);
  timeStamp = millis();
}

void loop() {
  while(UART.available()){
    Serial.println(UART.read());
  }

  while(Serial.available()){
    Serial.println(Serial.read());
  }

  if(millis() - timeStamp > sendInterval){
    timeStamp = millis();
    UART.println("Test1");
  }
}
