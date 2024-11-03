#include <SoftwareSerial.h>
SoftwareSerial ERIC(7, 8); //RX / TX

const String msg1 = "{0: {'time': 1647166746.8603551, 5: [5, 10, 3], 6: 0b1111111111}, 1: {'time': 1647166746.8603551, 2: [0, 0], 3: {'temp': 27, 'hum': 0, 'press': 0, 'gas': 0}}, 2: {'time': 1647166746.8603551, 1: 0, 2: 5, 4: 7}, 3: {'time': 1647166746.";
const String msg2 = "8603551, 1: 0, 3: {'temp': 0, 'hum': 0, 'press': 0, 'gas': 0}}, 4: [1647166746.8603551, 0], 5: [1647166746.8603551, 0], 6: [1647166746.8603551, 0], 7: [1647166746.8603551, 0], 8: [1647166746.8603551, 0], 9: [1647166746.8603551, 0], 10: ";
const String msg3 = "[1647166746.8603551, 0], 11: [1647166746.8603551, 0], 12: [1647166746.8603551, 1], 13: [1647166746.8603551, 1], 14: [1647166746.8603551, 1], 15: [1647166746.8603551, 1]}";
void setup() {
  // put your setup code here, to run once:
  ERIC.begin(19200);
  
  Serial.begin(19200);

  Serial.println("Done Init"); 
  ERIC.print("ER_CMD#U?");
}

void loop() {
  // put your main code here, to run repeatedly:
  String tempStr = "";
  while(ERIC.available()){
    char temp = ERIC.read();
    tempStr += temp;
  }
  if(tempStr != ""){
    Serial.print(tempStr.length());
    Serial.println(tempStr);
  }

//  Serial.println("transmitting");
//  ERIC.print(msg1);
//  delay(1000);
//  ERIC.print(msg2);
//  delay(1000);
//  ERIC.print(msg3);
//  delay(10000);
}
